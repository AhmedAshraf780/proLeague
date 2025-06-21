#include "signuppage.h"
#include "ui_signuppage.h"
#include "mainwindow.h"
#include <QMessageBox>

SignupPage::SignupPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignupPage)
{
    ui->setupUi(this);
}

SignupPage::~SignupPage()
{
    delete ui;
}

void SignupPage::on_signupButton_clicked()
{
    if (validateInput()) {
        QString username = ui->usernameLineEdit->text();
        QString password = ui->passwordLineEdit->text();
        QString confirmPassword = ui->confirmPasswordLineEdit->text();

        if (password != confirmPassword) {
            QMessageBox::warning(this, "Error", "Passwords do not match.");
            return;
        }

        // TODO: Implement actual signup logic using the C++ backend
        // For now, just show a success message
        QMessageBox::information(this, "Success", "Account created successfully!");
        
        // Switch back to login page
        if (auto mainWindow = qobject_cast<MainWindow*>(window())) {
            mainWindow->switchToPage(1);
        }
    }
}

void SignupPage::on_backButton_clicked()
{
    if (auto mainWindow = qobject_cast<MainWindow*>(window())) {
        mainWindow->switchToPage(0); // Switch back to home page
    }
}

bool SignupPage::validateInput()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString confirmPassword = ui->confirmPasswordLineEdit->text();

    if (username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please fill in all fields.");
        return false;
    }

    if (username.length() < 3) {
        QMessageBox::warning(this, "Error", "Username must be at least 3 characters long.");
        return false;
    }

    if (password.length() < 6) {
        QMessageBox::warning(this, "Error", "Password must be at least 6 characters long.");
        return false;
    }

    return true;
} 