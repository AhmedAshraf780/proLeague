#include "Pages/AdminMenuPage.hpp"
#include "datamanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>

AdminMenuPage::AdminMenuPage(QStackedWidget* stackedWidget, QWidget* parent)
    : MenuPage(stackedWidget, parent)
{
    setupUI();
}

void AdminMenuPage::setupUI() {
    m_titleLabel->setText("Admin Menu");

    // Create menu buttons
    QPushButton* generateTestBtn = createMenuButton("0. Generate Test Data");
    QPushButton* addTeamBtn = createMenuButton("1. Add Team");
    QPushButton* addPlayerBtn = createMenuButton("2. Add Player");
    QPushButton* setDateBtn = createMenuButton("3. Set Season Start Date");
    QPushButton* generateScheduleBtn = createMenuButton("4. Generate Match Schedule");
    QPushButton* startSeasonBtn = createMenuButton("5. Start Season");
    QPushButton* addResultBtn = createMenuButton("6. Add Match Result");
    QPushButton* viewTeamBtn = createMenuButton("7. View Team Info");
    QPushButton* viewPlayerBtn = createMenuButton("8. View Player Info");
    QPushButton* viewMatchBtn = createMenuButton("9. View Match Result");
    QPushButton* viewScheduleBtn = createMenuButton("10. View Schedule");
    QPushButton* viewTableBtn = createMenuButton("11. View League Table");
    QPushButton* saveDataBtn = createMenuButton("12. Save Data");
    QPushButton* createAdminBtn = createMenuButton("13. Create Admin Account");
    QPushButton* deleteDataBtn = createMenuButton("14. Delete All League Data");
    QPushButton* logoutBtn = createMenuButton("15. Logout");
    QPushButton* exitBtn = createMenuButton("16. Exit");

    // Add buttons to layout
    m_layout->addWidget(generateTestBtn);
    m_layout->addWidget(addTeamBtn);
    m_layout->addWidget(addPlayerBtn);
    m_layout->addWidget(setDateBtn);
    m_layout->addWidget(generateScheduleBtn);
    m_layout->addWidget(startSeasonBtn);
    m_layout->addWidget(addResultBtn);
    m_layout->addWidget(viewTeamBtn);
    m_layout->addWidget(viewPlayerBtn);
    m_layout->addWidget(viewMatchBtn);
    m_layout->addWidget(viewScheduleBtn);
    m_layout->addWidget(viewTableBtn);
    m_layout->addWidget(saveDataBtn);
    m_layout->addWidget(createAdminBtn);
    m_layout->addWidget(deleteDataBtn);
    m_layout->addWidget(logoutBtn);
    m_layout->addWidget(exitBtn);

    // Add stretch to push buttons to the top
    m_layout->addStretch();

    // Connect signals
    connect(generateTestBtn, &QPushButton::clicked, this, &AdminMenuPage::handleGenerateTestData);
    connect(addTeamBtn, &QPushButton::clicked, this, &AdminMenuPage::handleAddTeam);
    connect(addPlayerBtn, &QPushButton::clicked, this, &AdminMenuPage::handleAddPlayer);
    connect(setDateBtn, &QPushButton::clicked, this, &AdminMenuPage::handleSetSeasonStartDate);
    connect(generateScheduleBtn, &QPushButton::clicked, this, &AdminMenuPage::handleGenerateSchedule);
    connect(startSeasonBtn, &QPushButton::clicked, this, &AdminMenuPage::handleStartSeason);
    connect(addResultBtn, &QPushButton::clicked, this, &AdminMenuPage::handleAddMatchResult);
    connect(viewTeamBtn, &QPushButton::clicked, this, &AdminMenuPage::handleViewTeamInfo);
    connect(viewPlayerBtn, &QPushButton::clicked, this, &AdminMenuPage::handleViewPlayerInfo);
    connect(viewMatchBtn, &QPushButton::clicked, this, &AdminMenuPage::handleViewMatchResult);
    connect(viewScheduleBtn, &QPushButton::clicked, this, &AdminMenuPage::handleViewSchedule);
    connect(viewTableBtn, &QPushButton::clicked, this, &AdminMenuPage::handleViewLeagueTable);
    connect(saveDataBtn, &QPushButton::clicked, this, &AdminMenuPage::handleSaveData);
    connect(createAdminBtn, &QPushButton::clicked, this, &AdminMenuPage::handleCreateAdmin);
    connect(deleteDataBtn, &QPushButton::clicked, this, &AdminMenuPage::handleDeleteData);
    connect(logoutBtn, &QPushButton::clicked, this, &AdminMenuPage::handleLogout);
    connect(exitBtn, &QPushButton::clicked, this, &AdminMenuPage::handleExit);
}

// Inherit view functions from UserMenuPage
void AdminMenuPage::handleViewTeamInfo() {
    auto teams = DataManager::instance().getAllTeams();
    if (teams.isEmpty()) {
        QMessageBox::information(this, "View Team Info", "No teams available.");
        return;
    }

    QString info = "Available Teams:\n\n";
    for (const Team& team : teams) {
        info += QString("%1. %2\n").arg(team.ID).arg(team.Name);
    }

    QMessageBox::information(this, "View Team Info", info);
}

void AdminMenuPage::handleViewPlayerInfo() {
    auto teams = DataManager::instance().getAllTeams();
    if (teams.isEmpty()) {
        QMessageBox::information(this, "View Player Info", "No teams available.");
        return;
    }

    QString info = "Players by Team:\n\n";
    for (const Team& team : teams) {
        info += QString("Team: %1\n").arg(team.Name);
        if (team.Players.isEmpty()) {
            info += "  No players in this team.\n";
        } else {
            for (const Player& player : team.Players) {
                info += QString("  %1. %2 (%3)\n")
                    .arg(player.ID)
                    .arg(player.Name)
                    .arg(player.Nationality);
            }
        }
        info += "\n";
    }

    QMessageBox::information(this, "View Player Info", info);
}

void AdminMenuPage::handleViewMatchResult() {
    auto matches = DataManager::instance().getAllMatches();
    if (matches.isEmpty()) {
        QMessageBox::information(this, "View Match Result", "No matches scheduled.");
        return;
    }

    QString info = "Match Results:\n\n";
    for (const Match& match : matches) {
        Team* homeTeam = DataManager::instance().getTeam(match.TeamHomeID);
        Team* awayTeam = DataManager::instance().getTeam(match.TeamAwayID);
        
        if (homeTeam && awayTeam) {
            info += QString("%1 vs %2 (").arg(homeTeam->Name).arg(awayTeam->Name);
            
            QDateTime matchDate = match.StartDate.toQDateTime();
            info += matchDate.toString("dd/MM/yyyy hh:mm") + ")\n";
            
            if (match.played) {
                info += QString("Result: %1 - %2\n")
                    .arg(match.result.TeamHomeGoals)
                    .arg(match.result.TeamAwayGoals);
            } else {
                info += "Not played yet\n";
            }
            info += "\n";
        }
    }

    QMessageBox::information(this, "View Match Result", info);
}

void AdminMenuPage::handleViewSchedule() {
    auto matches = DataManager::instance().getSchedule();
    if (matches.isEmpty()) {
        QMessageBox::information(this, "View Schedule", "No matches scheduled.");
        return;
    }

    QString info = "Match Schedule:\n\n";
    QDate currentDate;

    for (const Match& match : matches) {
        QDateTime matchDateTime = match.StartDate.toQDateTime();
        
        if (matchDateTime.date() != currentDate) {
            currentDate = matchDateTime.date();
            info += "\nDate: " + currentDate.toString("dd/MM/yyyy") + "\n";
        }

        Team* homeTeam = DataManager::instance().getTeam(match.TeamHomeID);
        Team* awayTeam = DataManager::instance().getTeam(match.TeamAwayID);
        
        if (homeTeam && awayTeam) {
            info += QString("%1 - %2 vs %3 at %4\n")
                .arg(matchDateTime.toString("hh:mm"))
                .arg(homeTeam->Name)
                .arg(awayTeam->Name)
                .arg(match.Stadium);
        }
    }

    QMessageBox::information(this, "View Schedule", info);
}

void AdminMenuPage::handleViewLeagueTable() {
    if (!DataManager::instance().isSeasonStarted()) {
        QMessageBox::information(this, "View League Table", "Season has not started yet.");
        return;
    }

    auto table = DataManager::instance().getLeagueTable();
    if (table.isEmpty()) {
        QMessageBox::information(this, "View League Table", "No league table available.");
        return;
    }

    QString info = "League Table:\n\n";
    info += QString("%-4s%-20s%-5s%-5s%-5s%-5s%-5s%-5s%-5s%-5s\n")
        .arg("Pos")
        .arg("Team")
        .arg("P")
        .arg("W")
        .arg("D")
        .arg("L")
        .arg("GF")
        .arg("GA")
        .arg("GD")
        .arg("Pts");
    info += QString().fill('-', 70) + "\n";

    for (const TableRecord& record : table) {
        info += QString("%-4d%-20s%-5d%-5d%-5d%-5d%-5d%-5d%-5d%-5d\n")
            .arg(record.No)
            .arg(record.TeamName)
            .arg(record.Played)
            .arg(record.Wins)
            .arg(record.Draws)
            .arg(record.Losses)
            .arg(record.GoalsFor)
            .arg(record.GoalsAgainst)
            .arg(record.GoalDifference)
            .arg(record.Points);
    }

    info += "\nP: Played, W: Won, D: Drawn, L: Lost\n";
    info += "GF: Goals For, GA: Goals Against, GD: Goal Difference, Pts: Points";

    QMessageBox::information(this, "View League Table", info);
}

void AdminMenuPage::handleGenerateTestData() {
    if (QMessageBox::question(this, "Generate Test Data", 
        "This will delete all existing data and generate test data. Continue?",
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        DataManager::instance().generateTestData();
        QMessageBox::information(this, "Generate Test Data", "Test data generated successfully.");
    }
}

void AdminMenuPage::handleAddTeam() {
    bool ok;
    QString teamName = QInputDialog::getText(this, "Add Team",
        "Team Name:", QLineEdit::Normal, "", &ok);
    if (!ok || teamName.isEmpty()) return;

    QString stadium = QInputDialog::getText(this, "Add Team",
        "Stadium Name:", QLineEdit::Normal, "", &ok);
    if (!ok || stadium.isEmpty()) return;

    if (DataManager::instance().addTeam(teamName, stadium)) {
        QMessageBox::information(this, "Add Team", "Team added successfully.");
    } else {
        QMessageBox::warning(this, "Add Team", "Failed to add team.");
    }
}

void AdminMenuPage::handleAddPlayer() {
    auto teams = DataManager::instance().getAllTeams();
    if (teams.isEmpty()) {
        QMessageBox::warning(this, "Add Player", "No teams available. Please add a team first.");
        return;
    }

    QStringList teamNames;
    for (const Team& team : teams) {
        teamNames << team.Name;
    }

    bool ok;
    QString teamName = QInputDialog::getItem(this, "Add Player",
        "Select Team:", teamNames, 0, false, &ok);
    if (!ok || teamName.isEmpty()) return;

    Team* team = DataManager::instance().getTeamByName(teamName);
    if (!team) return;

    QString playerName = QInputDialog::getText(this, "Add Player",
        "Player Name:", QLineEdit::Normal, "", &ok);
    if (!ok || playerName.isEmpty()) return;

    QString nationality = QInputDialog::getText(this, "Add Player",
        "Nationality:", QLineEdit::Normal, "", &ok);
    if (!ok || nationality.isEmpty()) return;

    // Create a custom dialog for date selection
    QDialog dateDialog(this);
    dateDialog.setWindowTitle("Select Birth Date");
    auto layout = new QVBoxLayout(&dateDialog);

    auto dateEdit = new QDateTimeEdit(&dateDialog);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate().addYears(-20));
    dateEdit->setMaximumDate(QDate::currentDate());
    dateEdit->setMinimumDate(QDate(1950, 1, 1));

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dateDialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dateDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dateDialog, &QDialog::reject);

    layout->addWidget(dateEdit);
    layout->addWidget(buttonBox);

    if (dateDialog.exec() != QDialog::Accepted) return;

    QDate birthDate = dateEdit->date();

    Player player;
    player.Name = playerName;
    player.Nationality = nationality;
    player.BirthDate.day = birthDate.day();
    player.BirthDate.month = birthDate.month();
    player.BirthDate.year = birthDate.year();
    player.BirthDate.hour = 0;
    player.BirthDate.minute = 0;

    if (DataManager::instance().addPlayer(team->ID, player)) {
        QMessageBox::information(this, "Add Player", "Player added successfully.");
    } else {
        QMessageBox::warning(this, "Add Player", "Failed to add player.");
    }
}

void AdminMenuPage::handleSetSeasonStartDate() {
    // Create a custom dialog for date and time selection
    QDialog dateDialog(this);
    dateDialog.setWindowTitle("Set Season Start Date");
    auto layout = new QVBoxLayout(&dateDialog);

    auto dateTimeEdit = new QDateTimeEdit(&dateDialog);
    dateTimeEdit->setDisplayFormat("dd/MM/yyyy hh:mm");
    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(1));
    dateTimeEdit->setMinimumDateTime(QDateTime::currentDateTime().addDays(1));
    dateTimeEdit->setMaximumDateTime(QDateTime::currentDateTime().addYears(1));

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dateDialog);
    connect(buttonBox, &QDialogButtonBox::accepted, &dateDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dateDialog, &QDialog::reject);

    layout->addWidget(dateTimeEdit);
    layout->addWidget(buttonBox);

    if (dateDialog.exec() != QDialog::Accepted) return;

    QDateTime startDate = dateTimeEdit->dateTime();

    Date date;
    date.day = startDate.date().day();
    date.month = startDate.date().month();
    date.year = startDate.date().year();
    date.hour = startDate.time().hour();
    date.minute = startDate.time().minute();

    if (DataManager::instance().setSeasonStartDate(date)) {
        QMessageBox::information(this, "Set Season Start Date", "Season start date set successfully.");
    } else {
        QMessageBox::warning(this, "Set Season Start Date", "Failed to set season start date.");
    }
}

void AdminMenuPage::handleGenerateSchedule() {
    if (QMessageBox::question(this, "Generate Schedule", 
        "This will generate a new match schedule. Continue?",
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        if (DataManager::instance().generateSchedule()) {
            QMessageBox::information(this, "Generate Schedule", "Match schedule generated successfully.");
        } else {
            QMessageBox::warning(this, "Generate Schedule", "Failed to generate match schedule.");
        }
    }
}

void AdminMenuPage::handleStartSeason() {
    if (QMessageBox::question(this, "Start Season", 
        "This will start the season. No more modifications to teams or players will be allowed. Continue?",
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        if (DataManager::instance().startSeason()) {
            QMessageBox::information(this, "Start Season", "Season started successfully.");
        } else {
            QMessageBox::warning(this, "Start Season", "Failed to start season.");
        }
    }
}

void AdminMenuPage::handleAddMatchResult() {
    auto matches = DataManager::instance().getAllMatches();
    QStringList matchList;
    QVector<int> matchIds;

    for (const Match& match : matches) {
        if (!match.played) {
            Team* homeTeam = DataManager::instance().getTeam(match.TeamHomeID);
            Team* awayTeam = DataManager::instance().getTeam(match.TeamAwayID);
            if (homeTeam && awayTeam) {
                matchList << QString("%1 vs %2").arg(homeTeam->Name).arg(awayTeam->Name);
                matchIds << match.ID;
            }
        }
    }

    if (matchList.isEmpty()) {
        QMessageBox::information(this, "Add Match Result", "No unplayed matches available.");
        return;
    }

    bool ok;
    QString selectedMatch = QInputDialog::getItem(this, "Add Match Result",
        "Select Match:", matchList, 0, false, &ok);
    if (!ok || selectedMatch.isEmpty()) return;

    int matchIndex = matchList.indexOf(selectedMatch);
    int matchId = matchIds[matchIndex];

    int homeGoals = QInputDialog::getInt(this, "Add Match Result",
        "Home Team Goals:", 0, 0, 99, 1, &ok);
    if (!ok) return;

    int awayGoals = QInputDialog::getInt(this, "Add Match Result",
        "Away Team Goals:", 0, 0, 99, 1, &ok);
    if (!ok) return;

    if (DataManager::instance().addMatchResult(matchId, homeGoals, awayGoals)) {
        QMessageBox::information(this, "Add Match Result", "Match result added successfully.");
    } else {
        QMessageBox::warning(this, "Add Match Result", "Failed to add match result.");
    }
}

void AdminMenuPage::handleSaveData() {
    DataManager::instance().saveData();
    QMessageBox::information(this, "Save Data", "Data saved successfully.");
}

void AdminMenuPage::handleCreateAdmin() {
    bool ok;
    QString username = QInputDialog::getText(this, "Create Admin Account",
        "Username:", QLineEdit::Normal, "", &ok);
    if (!ok || username.isEmpty()) return;

    QString password = QInputDialog::getText(this, "Create Admin Account",
        "Password:", QLineEdit::Password, "", &ok);
    if (!ok || password.isEmpty()) return;

    if (DataManager::instance().signup(username, password, true)) {
        QMessageBox::information(this, "Create Admin Account", "Admin account created successfully.");
    } else {
        QMessageBox::warning(this, "Create Admin Account", "Failed to create admin account.");
    }
}

void AdminMenuPage::handleDeleteData() {
    if (QMessageBox::question(this, "Delete All League Data", 
        "This will delete ALL league and match data! User accounts will be preserved. Continue?",
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        DataManager::instance().deleteAllData();
        QMessageBox::information(this, "Delete All League Data", "All league data deleted successfully.");
    }
} 