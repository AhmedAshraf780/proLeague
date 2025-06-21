#include "Pages/HomePage.hpp"
#include <QApplication>
#include <QPushButton>
#include <QMessageBox>

HomePage::HomePage(QStackedWidget* stackedWidget, QWidget* parent)
    : BasePage(stackedWidget, parent)
{
    setupUI();
}

void HomePage::setupUI() {
    m_titleLabel->setText("Pro League Management System");

    // Create menu buttons
    QPushButton* loginBtn = createMenuButton("1. Login");
    QPushButton* signupBtn = createMenuButton("2. Sign Up");
    QPushButton* exitBtn = createMenuButton("3. Exit");

    // Add buttons to layout
    m_layout->addWidget(loginBtn);
    m_layout->addWidget(signupBtn);
    m_layout->addWidget(exitBtn);

    // Add stretch to push buttons to the top
    m_layout->addStretch();

    // Connect signals
    connect(loginBtn, &QPushButton::clicked, this, &HomePage::handleLogin);
    connect(signupBtn, &QPushButton::clicked, this, &HomePage::handleSignup);
    connect(exitBtn, &QPushButton::clicked, this, &HomePage::handleExit);
}

void HomePage::handleLogin() {
    emit loginRequested();
}

void HomePage::handleSignup() {
    emit signupRequested();
}

void HomePage::handleExit() {
    QApplication::quit();
} 