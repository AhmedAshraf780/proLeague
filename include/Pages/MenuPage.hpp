#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>

class MenuPage : public QWidget {
    Q_OBJECT

public:
    explicit MenuPage(QStackedWidget* stackedWidget, QWidget* parent = nullptr);

protected:
    QStackedWidget* m_stackedWidget;
    QVBoxLayout* m_layout;
    QLabel* m_titleLabel;

    QPushButton* createMenuButton(const QString& text);
    void setupBaseLayout();

signals:
    void logoutRequested();
    void exitRequested();

protected slots:
    virtual void handleViewTeamInfo() = 0;
    virtual void handleViewPlayerInfo() = 0;
    virtual void handleViewMatchResult() = 0;
    virtual void handleViewSchedule() = 0;
    virtual void handleViewLeagueTable() = 0;
    virtual void handleLogout();
    virtual void handleExit();
}; 