#pragma once

#include "Pages/MenuPage.hpp"

class UserMenuPage : public MenuPage {
    Q_OBJECT

public:
    explicit UserMenuPage(QStackedWidget* stackedWidget, QWidget* parent = nullptr);

protected slots:
    void handleViewTeamInfo() override;
    void handleViewPlayerInfo() override;
    void handleViewMatchResult() override;
    void handleViewSchedule() override;
    void handleViewLeagueTable() override;

private:
    void setupUI();
}; 