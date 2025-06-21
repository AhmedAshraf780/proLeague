#pragma once

#include <QWidget>

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget {
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

private slots:
    void on_loginButton_clicked();
    void on_backButton_clicked();
    void on_resetPasswordButton_clicked();

private:
    Ui::LoginPage *ui;
    bool validateInput();
}; 