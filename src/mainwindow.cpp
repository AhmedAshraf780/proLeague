#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <QFile>
#include <QApplication>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Pages/HomePage.hpp"
#include "Pages/LoginPage.hpp"
#include "Pages/UserMenuPage.hpp"
#include "Pages/AdminMenuPage.hpp"
#include "datamanager.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_stackedWidget(new QStackedWidget(this))
    , m_homePage(nullptr)
    , m_loginPage(nullptr)
    , m_userMenu(nullptr)
    , m_adminMenu(nullptr)
{
    ui->setupUi(this);
    setupUI();
    loadStyleSheet();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupUI() {
    // Set window properties
    setWindowTitle("Pro League Management System");
    setMinimumSize(800, 600);

    // Create stacked widget
    ui->centralwidget->setLayout(new QVBoxLayout);
    ui->centralwidget->layout()->addWidget(m_stackedWidget);

    // Create pages
    m_homePage = new HomePage(m_stackedWidget);
    m_loginPage = new LoginPage(m_stackedWidget);
    m_userMenu = new UserMenuPage(m_stackedWidget);
    m_adminMenu = new AdminMenuPage(m_stackedWidget);

    // Add pages to stacked widget
    m_stackedWidget->addWidget(m_homePage);
    m_stackedWidget->addWidget(m_loginPage);
    m_stackedWidget->addWidget(m_userMenu);
    m_stackedWidget->addWidget(m_adminMenu);

    // Connect signals
    connect(m_homePage, &HomePage::loginRequested, this, &MainWindow::handleLoginRequest);
    connect(m_homePage, &HomePage::signupRequested, this, &MainWindow::handleSignupRequest);
    
    connect(m_loginPage, &LoginPage::loginSuccessful, this, &MainWindow::handleLoginSuccess);
    connect(m_loginPage, &LoginPage::backRequested, this, &MainWindow::handleBack);
    
    connect(m_userMenu, &UserMenuPage::logoutRequested, this, &MainWindow::handleLogout);
    connect(m_userMenu, &UserMenuPage::exitRequested, this, &MainWindow::handleExit);
    
    connect(m_adminMenu, &AdminMenuPage::logoutRequested, this, &MainWindow::handleLogout);
    connect(m_adminMenu, &AdminMenuPage::exitRequested, this, &MainWindow::handleExit);

    // Show home page by default
    showHomePage();
}

void MainWindow::loadStyleSheet() {
    QFile styleFile(":/styles/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = styleFile.readAll();
        setStyleSheet(style);
        styleFile.close();
    }
}

void MainWindow::handleLoginRequest() {
    showLoginPage();
}

void MainWindow::handleSignupRequest() {
    // TODO: Implement signup page
    // For now, just show login page
    showLoginPage();
}

void MainWindow::handleLoginSuccess() {
    if (DataManager::instance().isAdmin()) {
        showAdminMenu();
    } else {
        showUserMenu();
    }
}

void MainWindow::handleLogout() {
    DataManager::instance().logout();
    showHomePage();
}

void MainWindow::handleExit() {
    close();
}

void MainWindow::handleBack() {
    showHomePage();
}

void MainWindow::showHomePage() {
    m_stackedWidget->setCurrentWidget(m_homePage);
}

void MainWindow::showLoginPage() {
    m_stackedWidget->setCurrentWidget(m_loginPage);
}

void MainWindow::showUserMenu() {
    m_stackedWidget->setCurrentWidget(m_userMenu);
}

void MainWindow::showAdminMenu() {
    m_stackedWidget->setCurrentWidget(m_adminMenu);
} 