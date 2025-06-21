#pragma once

#include "Pages/BasePage.hpp"
#include <QLineEdit>

class LoginPage : public BasePage {
    Q_OBJECT

public:
    explicit LoginPage(QStackedWidget* stackedWidget, QWidget* parent = nullptr);

signals:
    void loginSuccessful();
    void backRequested();

protected slots:
    void handleLogin();
    void handleBack();

private:
    void setupUI();
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
}; 