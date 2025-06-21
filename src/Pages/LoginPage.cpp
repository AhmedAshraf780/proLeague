#include "Pages/LoginPage.hpp"
#include "datamanager.h"
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QFormLayout>

LoginPage::LoginPage(QStackedWidget* stackedWidget, QWidget* parent)
    : BasePage(stackedWidget, parent)
    , m_usernameEdit(new QLineEdit(this))
    , m_passwordEdit(new QLineEdit(this))
{
    setupUI();
}

void LoginPage::setupUI() {
    m_titleLabel->setText("Login");

    // Create form layout for input fields
    QWidget* formWidget = new QWidget(this);
    QFormLayout* formLayout = new QFormLayout(formWidget);

    // Setup username field
    m_usernameEdit->setPlaceholderText("Enter username");
    formLayout->addRow("Username:", m_usernameEdit);

    // Setup password field
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Enter password");
    formLayout->addRow("Password:", m_passwordEdit);

    // Create buttons
    QPushButton* loginBtn = createMenuButton("Login");
    QPushButton* backBtn = createMenuButton("Back");

    // Add widgets to main layout
    m_layout->addWidget(formWidget);
    m_layout->addWidget(loginBtn);
    m_layout->addWidget(backBtn);

    // Add stretch to push form to the top
    m_layout->addStretch();

    // Connect signals
    connect(loginBtn, &QPushButton::clicked, this, &LoginPage::handleLogin);
    connect(backBtn, &QPushButton::clicked, this, &LoginPage::handleBack);

    // Allow pressing Enter to login
    connect(m_usernameEdit, &QLineEdit::returnPressed, this, &LoginPage::handleLogin);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginPage::handleLogin);
}

void LoginPage::handleLogin() {
    QString username = m_usernameEdit->text();
    QString password = m_passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", "Please enter both username and password.");
        return;
    }

    if (DataManager::instance().login(username, password)) {
        m_usernameEdit->clear();
        m_passwordEdit->clear();
        emit loginSuccessful();
    } else {
        QMessageBox::warning(this, "Login Error", "Invalid username or password.");
    }
}

void LoginPage::handleBack() {
    m_usernameEdit->clear();
    m_passwordEdit->clear();
    emit backRequested();
} 