#pragma once

#include <QWidget>

namespace Ui {
class SignupPage;
}

class SignupPage : public QWidget {
    Q_OBJECT

public:
    explicit SignupPage(QWidget *parent = nullptr);
    ~SignupPage();

private slots:
    void on_signupButton_clicked();
    void on_backButton_clicked();

private:
    Ui::SignupPage *ui;
    bool validateInput();
}; 