#include "loginpage.h"
#include "ui_loginpage.h"
#include "mainwindow.h"
#include <QMessageBox>

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::on_loginButton_clicked()
{
    if (validateInput()) {
        QString username = ui->usernameLineEdit->text();
        QString password = ui->passwordLineEdit->text();

        // TODO: Implement actual login logic using the C++ backend
        // For now, just show a success message
        QMessageBox::information(this, "Success", "Login successful!");
    }
}

void LoginPage::on_backButton_clicked()
{
    if (auto mainWindow = qobject_cast<MainWindow*>(window())) {
        mainWindow->switchToPage(0); // Switch back to home page
    }
}

void LoginPage::on_resetPasswordButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your username first.");
        return;
    }

    // TODO: Implement actual password reset logic using the C++ backend
    QMessageBox::information(this, "Success", "Password reset instructions sent!");
}

bool LoginPage::validateInput()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill in all fields.");
        return false;
    }

    return true;
} 