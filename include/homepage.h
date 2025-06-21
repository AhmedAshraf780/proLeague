#pragma once

#include <QWidget>

namespace Ui {
class HomePage;
}

class HomePage : public QWidget {
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

private slots:
    void on_loginButton_clicked();
    void on_signupButton_clicked();

private:
    Ui::HomePage *ui;
}; 