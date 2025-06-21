#pragma once

#include "Pages/BasePage.hpp"

class HomePage : public BasePage {
    Q_OBJECT

public:
    explicit HomePage(QStackedWidget* stackedWidget, QWidget* parent = nullptr);

signals:
    void loginRequested();
    void signupRequested();

protected slots:
    void handleLogin();
    void handleSignup();
    void handleExit();

private:
    void setupUI();
}; 