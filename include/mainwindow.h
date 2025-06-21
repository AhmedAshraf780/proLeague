#pragma once

#include <QMainWindow>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class HomePage;
class LoginPage;
class UserMenuPage;
class AdminMenuPage;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void handleLoginRequest();
    void handleSignupRequest();
    void handleLoginSuccess();
    void handleLogout();
    void handleExit();
    void handleBack();

private:
    void setupUI();
    void loadStyleSheet();
    void showHomePage();
    void showLoginPage();
    void showUserMenu();
    void showAdminMenu();

    Ui::MainWindow* ui;
    QStackedWidget* m_stackedWidget;
    HomePage* m_homePage;
    LoginPage* m_loginPage;
    UserMenuPage* m_userMenu;
    AdminMenuPage* m_adminMenu;
}; 