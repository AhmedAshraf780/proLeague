#include "homepage.h"
#include "ui_homepage.h"
#include "mainwindow.h"

HomePage::HomePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomePage)
{
    ui->setupUi(this);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::on_loginButton_clicked()
{
    if (auto mainWindow = qobject_cast<MainWindow*>(window())) {
        mainWindow->switchToPage(1); // Switch to login page
    }
}

void HomePage::on_signupButton_clicked()
{
    if (auto mainWindow = qobject_cast<MainWindow*>(window())) {
        mainWindow->switchToPage(2); // Switch to signup page
    }
} 