#include "datamanager.h"
#include "streamoperators.h"
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <ctime>
#include <random>

DataManager& DataManager::instance() {
    static DataManager instance;
    return instance;
}

DataManager::DataManager()
    : m_isLoggedIn(false)
    , m_nextUserId(1)
    , m_nextTeamId(1)
    , m_nextPlayerId(1)
    , m_nextMatchId(1)
{
    loadData();
    
    // Create default admin if no users exist
    if (m_users.isEmpty()) {
        User admin;
        admin.ID = m_nextUserId++;
        admin.Name = "admin";
        admin.Password = "admin";
        admin.IsAdmin = true;
        m_users["admin"] = admin;
    }
}

bool DataManager::login(const QString& username, const QString& password) {
    auto it = m_users.find(username);
    if (it != m_users.end() && it.value().Password == password) {
        m_currentUser = it.value();
        m_isLoggedIn = true;
        return true;
    }
    return false;
}

bool DataManager::signup(const QString& username, const QString& password, bool isAdmin) {
    if (m_users.contains(username)) {
        return false;
    }

    User newUser;
    newUser.ID = m_nextUserId++;
    newUser.Name = username;
    newUser.Password = password;
    newUser.IsAdmin = isAdmin;
    m_users[username] = newUser;
    return true;
}

void DataManager::logout() {
    m_isLoggedIn = false;
}

bool DataManager::resetPassword(const QString& username, const QString& newPassword) {
    auto it = m_users.find(username);
    if (it != m_users.end()) {
        it.value().Password = newPassword;
        return true;
    }
    return false;
}

bool DataManager::addTeam(const QString& name, const QString& stadium) {
    if (!m_currentUser.IsAdmin || m_league.SeasonStarted) {
        return false;
    }

    // Check if team already exists
    for (const Team& team : m_league.Teams) {
        if (team.Name == name) {
            return false;
        }
    }

    Team newTeam;
    newTeam.ID = m_nextTeamId++;
    newTeam.Name = name;
    newTeam.Stadium = stadium;
    m_league.Teams.append(newTeam);
    return true;
}

bool DataManager::addPlayer(int teamId, const Player& player) {
    if (!m_currentUser.IsAdmin || m_league.SeasonStarted) {
        return false;
    }

    for (Team& team : m_league.Teams) {
        if (team.ID == teamId) {
            if (team.Players.size() >= 34) { // MAX_PLAYERS_PER_TEAM
                return false;
            }
            team.Players.append(player);
            return true;
        }
    }
    return false;
}

Team* DataManager::getTeam(int id) {
    for (Team& team : m_league.Teams) {
        if (team.ID == id) {
            return &team;
        }
    }
    return nullptr;
}

Team* DataManager::getTeamByName(const QString& name) {
    for (Team& team : m_league.Teams) {
        if (team.Name == name) {
            return &team;
        }
    }
    return nullptr;
}

bool DataManager::validateDate(const Date& date) const {
    if (date.year < 1900 || date.year > 2100) return false;
    if (date.month < 1 || date.month > 12) return false;
    if (date.hour < 0 || date.hour > 23) return false;
    if (date.minute < 0 || date.minute > 59) return false;

    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (date.year % 400 == 0 || (date.year % 4 == 0 && date.year % 100 != 0)) {
        daysInMonth[2] = 29;
    }

    return date.day >= 1 && date.day <= daysInMonth[date.month];
}

bool DataManager::setSeasonStartDate(const Date& date) {
    if (!m_currentUser.IsAdmin || m_league.SeasonStarted) {
        return false;
    }

    if (!validateDate(date)) {
        return false;
    }

    // Check if date is in the future
    QDateTime startDate = date.toQDateTime();
    if (startDate <= QDateTime::currentDateTime()) {
        return false;
    }

    m_league.SeasonStartDate = date;
    return true;
}

bool DataManager::startSeason() {
    if (!m_currentUser.IsAdmin || m_league.SeasonStarted) {
        return false;
    }

    if (m_league.Teams.size() < 2) {
        return false;
    }

    if (m_league.Matches.isEmpty()) {
        return false;
    }

    // Check if all teams have required number of players
    for (const Team& team : m_league.Teams) {
        if (team.Players.size() < 34) { // MAX_PLAYERS_PER_TEAM
            return false;
        }
    }

    m_league.SeasonStarted = true;
    return true;
}

bool DataManager::generateSchedule() {
    if (!m_currentUser.IsAdmin || m_league.SeasonStarted) {
        return false;
    }

    if (m_league.Teams.size() < 2) {
        return false;
    }

    m_league.Matches.clear();

    // Generate matches
    for (int i = 0; i < m_league.Teams.size(); i++) {
        for (int j = i + 1; j < m_league.Teams.size(); j++) {
            Match match;
            match.ID = m_nextMatchId++;
            match.TeamHomeID = m_league.Teams[i].ID;
            match.TeamAwayID = m_league.Teams[j].ID;
            match.Stadium = m_league.Teams[i].Stadium;
            match.played = false;
            m_league.Matches.append(match);
        }
    }

    // Shuffle matches
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(m_league.Matches.begin(), m_league.Matches.end(), gen);

    // Assign dates (every Friday)
    if (m_league.SeasonStartDate.day == 0) {
        return false;
    }

    QDateTime currentDate = m_league.SeasonStartDate.toQDateTime();
    
    // Find first Friday
    while (currentDate.date().dayOfWeek() != 5) {
        currentDate = currentDate.addDays(1);
    }

    int matchIndex = 0;
    while (matchIndex < m_league.Matches.size()) {
        // 10 matches per Friday: 3 at 4PM, 3 at 6PM, 4 at 8PM
        for (int slot = 0; slot < 3 && matchIndex < m_league.Matches.size(); slot++) {
            int matchesInThisSlot = (slot == 2) ? 4 : 3;

            for (int m = 0; m < matchesInThisSlot && matchIndex < m_league.Matches.size(); m++) {
                QDateTime matchTime = currentDate;
                matchTime = matchTime.addSecs(3600 * (16 + slot * 2)); // 16 = 4PM, 18 = 6PM, 20 = 8PM

                Date& matchDate = m_league.Matches[matchIndex].StartDate;
                matchDate.day = matchTime.date().day();
                matchDate.month = matchTime.date().month();
                matchDate.year = matchTime.date().year();
                matchDate.hour = matchTime.time().hour();
                matchDate.minute = 0;

                matchIndex++;
            }
        }

        // Move to next Friday
        currentDate = currentDate.addDays(7);
    }

    return true;
}

bool DataManager::addMatchResult(int matchId, int homeGoals, int awayGoals) {
    if (!m_currentUser.IsAdmin || !m_league.SeasonStarted) {
        return false;
    }

    for (Match& match : m_league.Matches) {
        if (match.ID == matchId && !match.played) {
            match.result.TeamHomeGoals = homeGoals;
            match.result.TeamAwayGoals = awayGoals;
            match.played = true;
            updateLeagueTable();
            return true;
        }
    }
    return false;
}

void DataManager::updateLeagueTable() {
    // Reset table
    m_league.Table.clear();
    for (const Team& team : m_league.Teams) {
        TableRecord record;
        record.ID = team.ID;
        record.TeamName = team.Name;
        record.Points = 0;
        record.GoalsFor = 0;
        record.GoalsAgainst = 0;
        record.GoalDifference = 0;
        record.Wins = 0;
        record.Draws = 0;
        record.Losses = 0;
        record.Played = 0;
        m_league.Table.append(record);
    }

    // Update statistics
    for (const Match& match : m_league.Matches) {
        if (match.played) {
            for (TableRecord& record : m_league.Table) {
                if (record.ID == match.TeamHomeID) {
                    record.Played++;
                    record.GoalsFor += match.result.TeamHomeGoals;
                    record.GoalsAgainst += match.result.TeamAwayGoals;
                    if (match.result.TeamHomeGoals > match.result.TeamAwayGoals) {
                        record.Wins++;
                        record.Points += 3;
                    }
                    else if (match.result.TeamHomeGoals == match.result.TeamAwayGoals) {
                        record.Draws++;
                        record.Points += 1;
                    }
                    else {
                        record.Losses++;
                    }
                }
                else if (record.ID == match.TeamAwayID) {
                    record.Played++;
                    record.GoalsFor += match.result.TeamAwayGoals;
                    record.GoalsAgainst += match.result.TeamHomeGoals;
                    if (match.result.TeamAwayGoals > match.result.TeamHomeGoals) {
                        record.Wins++;
                        record.Points += 3;
                    }
                    else if (match.result.TeamAwayGoals == match.result.TeamHomeGoals) {
                        record.Draws++;
                        record.Points += 1;
                    }
                    else {
                        record.Losses++;
                    }
                }
            }
        }
    }

    // Update goal differences
    for (TableRecord& record : m_league.Table) {
        record.GoalDifference = record.GoalsFor - record.GoalsAgainst;
    }

    sortLeagueTable();
}

void DataManager::sortLeagueTable() {
    std::sort(m_league.Table.begin(), m_league.Table.end(),
        [](const TableRecord& a, const TableRecord& b) {
            if (a.Points != b.Points)
                return a.Points > b.Points;
            if (a.GoalDifference != b.GoalDifference)
                return a.GoalDifference > b.GoalDifference;
            return a.GoalsFor > b.GoalsFor;
        });

    // Update positions
    for (int i = 0; i < m_league.Table.size(); i++) {
        m_league.Table[i].No = i + 1;
    }
}

void DataManager::saveData() {
    QFile file("league.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << m_league;
        file.close();
    }

    QFile usersFile("users.dat");
    if (usersFile.open(QIODevice::WriteOnly)) {
        QDataStream out(&usersFile);
        out << m_users;
        usersFile.close();
    }
}

void DataManager::loadData() {
    QFile file("league.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in >> m_league;
        file.close();
    }
    else {
        // Initialize new league
        m_league.ID = 1;
        m_league.Name = "Pro League";
        m_league.SeasonStarted = false;
        m_league.SeasonStartDate = Date{0, 0, 0, 0, 0};
    }

    QFile usersFile("users.dat");
    if (usersFile.open(QIODevice::ReadOnly)) {
        QDataStream in(&usersFile);
        in >> m_users;
        usersFile.close();
    }
}

void DataManager::deleteAllData() {
    if (!m_currentUser.IsAdmin) {
        return;
    }

    // Keep users but reset league
    m_league.Teams.clear();
    m_league.Matches.clear();
    m_league.Table.clear();
    m_league.SeasonStarted = false;
    m_league.SeasonStartDate = Date{0, 0, 0, 0, 0};

    m_nextTeamId = 1;
    m_nextPlayerId = 1;
    m_nextMatchId = 1;

    saveData();
}

void DataManager::generateTestData() {
    if (!m_currentUser.IsAdmin) {
        return;
    }

    // Sample data
    QStringList teamNames = {
        "Alpha FC", "Bravo United", "Charlie Town", "Delta City", "Echo Rovers",
        "Foxtrot FC", "Golf Athletic", "Hotel Wanderers", "India United", "Juliet City"
    };

    QStringList stadiums = {
        "Alpha Arena", "Bravo Park", "Charlie Stadium", "Delta Dome", "Echo Field",
        "Foxtrot Gardens", "Golf Stadium", "Hotel Arena", "India Park", "Juliet Ground"
    };

    QStringList firstNames = {
        "James", "Robert", "John", "Michael", "David", "William", "Richard", "Thomas", "Christopher", "Daniel"
    };

    QStringList lastNames = {
        "Smith", "Johnson", "Williams", "Brown", "Jones", "Miller", "Davis", "Garcia", "Rodriguez", "Wilson"
    };

    QStringList nationalities = {
        "England", "Spain", "Germany", "France", "Brazil", "Argentina", "Italy", "Netherlands", "Portugal", "USA"
    };

    // Clear existing data
    deleteAllData();

    // Add teams and players
    for (int t = 0; t < teamNames.size(); t++) {
        addTeam(teamNames[t], stadiums[t]);

        // Add 34 players to each team
        for (int p = 0; p < 34; p++) {
            Player player;
            player.ID = m_nextPlayerId++;
            player.Name = firstNames[rand() % firstNames.size()] + " " + lastNames[rand() % lastNames.size()];
            player.Nationality = nationalities[rand() % nationalities.size()];
            
            // Random birth date between 1985 and 2005
            player.BirthDate.year = 1985 + rand() % 21;
            player.BirthDate.month = 1 + rand() % 12;
            player.BirthDate.day = 1 + rand() % 28;
            player.BirthDate.hour = 0;
            player.BirthDate.minute = 0;

            addPlayer(m_nextTeamId - 1, player);
        }
    }

    // Set season start date to next month
    QDateTime startDate = QDateTime::currentDateTime().addMonths(1);
    Date seasonStart;
    seasonStart.day = startDate.date().day();
    seasonStart.month = startDate.date().month();
    seasonStart.year = startDate.date().year();
    seasonStart.hour = 0;
    seasonStart.minute = 0;
    setSeasonStartDate(seasonStart);

    // Generate schedule and start season
    generateSchedule();
    startSeason();

    // Generate some random match results
    for (Match& match : m_league.Matches) {
        match.result.TeamHomeGoals = rand() % 5;
        match.result.TeamAwayGoals = rand() % 5;
        match.played = true;
    }

    updateLeagueTable();
    saveData();
}

QVector<Match> DataManager::getSchedule() const {
    // Return all matches sorted by date
    QVector<Match> sortedMatches = m_league.Matches;
    std::sort(sortedMatches.begin(), sortedMatches.end(), 
        [](const Match& a, const Match& b) {
            QDateTime aTime = a.StartDate.toQDateTime();
            QDateTime bTime = b.StartDate.toQDateTime();
            return aTime < bTime;
        });
    return sortedMatches;
}

QVector<TableRecord> DataManager::getLeagueTable() const {
    if (!m_league.SeasonStarted) {
        return QVector<TableRecord>();
    }
    return m_league.Table;
} 