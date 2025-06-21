#include "Pages/UserMenuPage.hpp"
#include "datamanager.h"
#include <QMessageBox>

UserMenuPage::UserMenuPage(QStackedWidget* stackedWidget, QWidget* parent)
    : MenuPage(stackedWidget, parent)
{
    setupUI();
}

void UserMenuPage::setupUI() {
    m_titleLabel->setText("User Menu");

    // Create menu buttons
    QPushButton* viewTeamBtn = createMenuButton("1. View Team Info");
    QPushButton* viewPlayerBtn = createMenuButton("2. View Player Info");
    QPushButton* viewMatchBtn = createMenuButton("3. View Match Result");
    QPushButton* viewScheduleBtn = createMenuButton("4. View Schedule");
    QPushButton* viewTableBtn = createMenuButton("5. View League Table");
    QPushButton* logoutBtn = createMenuButton("6. Logout");
    QPushButton* exitBtn = createMenuButton("7. Exit");

    // Add buttons to layout
    m_layout->addWidget(viewTeamBtn);
    m_layout->addWidget(viewPlayerBtn);
    m_layout->addWidget(viewMatchBtn);
    m_layout->addWidget(viewScheduleBtn);
    m_layout->addWidget(viewTableBtn);
    m_layout->addWidget(logoutBtn);
    m_layout->addWidget(exitBtn);

    // Add stretch to push buttons to the top
    m_layout->addStretch();

    // Connect signals
    connect(viewTeamBtn, &QPushButton::clicked, this, &UserMenuPage::handleViewTeamInfo);
    connect(viewPlayerBtn, &QPushButton::clicked, this, &UserMenuPage::handleViewPlayerInfo);
    connect(viewMatchBtn, &QPushButton::clicked, this, &UserMenuPage::handleViewMatchResult);
    connect(viewScheduleBtn, &QPushButton::clicked, this, &UserMenuPage::handleViewSchedule);
    connect(viewTableBtn, &QPushButton::clicked, this, &UserMenuPage::handleViewLeagueTable);
    connect(logoutBtn, &QPushButton::clicked, this, &UserMenuPage::handleLogout);
    connect(exitBtn, &QPushButton::clicked, this, &UserMenuPage::handleExit);
}

void UserMenuPage::handleViewTeamInfo() {
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

void UserMenuPage::handleViewPlayerInfo() {
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

void UserMenuPage::handleViewMatchResult() {
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

void UserMenuPage::handleViewSchedule() {
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

void UserMenuPage::handleViewLeagueTable() {
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