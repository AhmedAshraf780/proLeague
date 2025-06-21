#include "Pages/MenuPage.hpp"
#include "datamanager.h"
#include <QMessageBox>

MenuPage::MenuPage(QStackedWidget* stackedWidget, QWidget* parent)
    : QWidget(parent)
    , m_stackedWidget(stackedWidget)
{
    setupBaseLayout();
}

void MenuPage::setupBaseLayout() {
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(10);
    m_layout->setContentsMargins(20, 20, 20, 20);

    m_titleLabel = new QLabel(this);
    m_titleLabel->setObjectName("menuTitle");
    m_layout->addWidget(m_titleLabel);

    // Add some spacing after the title
    m_layout->addSpacing(20);
}

QPushButton* MenuPage::createMenuButton(const QString& text) {
    QPushButton* button = new QPushButton(text, this);
    button->setObjectName("menuButton");
    return button;
}

void MenuPage::handleLogout() {
    DataManager::instance().logout();
    emit logoutRequested();
}

void MenuPage::handleExit() {
    DataManager::instance().saveData();
    emit exitRequested();
} 