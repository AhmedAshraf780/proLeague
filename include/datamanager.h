#pragma once

#include <QString>
#include <QMap>
#include <QVector>
#include <QDate>
#include <QDateTime>

// Forward declarations
class QWidget;

struct Date {
    int day;
    int month;
    int year;
    int hour;
    int minute;

    QDateTime toQDateTime() const {
        return QDateTime(QDate(year, month, day), QTime(hour, minute));
    }
};

struct Result {
    int ID;
    int TeamHomeGoals;
    int TeamAwayGoals;
};

struct Match {
    int ID;
    int TeamHomeID;
    int TeamAwayID;
    Date StartDate;
    Result result;
    QString Stadium;
    bool played;
};

struct Player {
    int ID;
    QString Name;
    Date BirthDate;
    QString Nationality;
};

struct Team {
    int ID;
    QString Name;
    QVector<Player> Players;
    QString Stadium;
};

struct TableRecord {
    int ID;
    int No;
    QString TeamName;
    int Points;
    int GoalsFor;
    int GoalsAgainst;
    int GoalDifference;
    int Wins;
    int Draws;
    int Losses;
    int Played;
};

struct League {
    int ID;
    QString Name;
    QVector<Team> Teams;
    QVector<Match> Matches;
    QVector<TableRecord> Table;
    bool SeasonStarted;
    Date SeasonStartDate;
};

struct User {
    int ID;
    QString Name;
    QString Password;
    bool IsAdmin;
};

class DataManager {
public:
    static DataManager& instance();

    // User management
    bool login(const QString& username, const QString& password);
    bool signup(const QString& username, const QString& password, bool isAdmin = false);
    void logout();
    bool resetPassword(const QString& username, const QString& newPassword);
    bool isLoggedIn() const { return m_isLoggedIn; }
    bool isAdmin() const { return m_currentUser.IsAdmin; }
    const User& currentUser() const { return m_currentUser; }

    // Team management
    bool addTeam(const QString& name, const QString& stadium);
    bool addPlayer(int teamId, const Player& player);
    QVector<Team> getAllTeams() const { return m_league.Teams; }
    Team* getTeam(int id);
    Team* getTeamByName(const QString& name);

    // Season management
    bool setSeasonStartDate(const Date& date);
    bool startSeason();
    bool generateSchedule();
    bool isSeasonStarted() const { return m_league.SeasonStarted; }

    // Match management
    bool addMatchResult(int matchId, int homeGoals, int awayGoals);
    QVector<Match> getAllMatches() const { return m_league.Matches; }
    Match* getMatch(int id);

    // View functions
    Team getTeamInfo(int teamId) const;
    Player getPlayerInfo(int teamId, int playerId) const;
    Match getMatchResult(int matchId) const;
    QVector<Match> getSchedule() const;
    QVector<TableRecord> getLeagueTable() const;

    // Data management
    void saveData();
    void loadData();
    void deleteAllData();
    void generateTestData();

private:
    DataManager(); // Private constructor for singleton
    void updateLeagueTable();
    void sortLeagueTable();
    bool validateDate(const Date& date) const;

    QMap<QString, User> m_users;
    League m_league;
    User m_currentUser;
    bool m_isLoggedIn;
    int m_nextUserId;
    int m_nextTeamId;
    int m_nextPlayerId;
    int m_nextMatchId;
}; 