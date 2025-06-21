#pragma once

#include "Pages/MenuPage.hpp"

class AdminMenuPage : public MenuPage {
    Q_OBJECT

public:
    explicit AdminMenuPage(QStackedWidget* stackedWidget, QWidget* parent = nullptr);

protected slots:
    void handleViewTeamInfo() override;
    void handleViewPlayerInfo() override;
    void handleViewMatchResult() override;
    void handleViewSchedule() override;
    void handleViewLeagueTable() override;

    void handleGenerateTestData();
    void handleAddTeam();
    void handleAddPlayer();
    void handleSetSeasonStartDate();
    void handleGenerateSchedule();
    void handleStartSeason();
    void handleAddMatchResult();
    void handleSaveData();
    void handleCreateAdmin();
    void handleDeleteData();

private:
    void setupUI();
}; 