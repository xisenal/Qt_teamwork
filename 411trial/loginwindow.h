#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "usermanager.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccess();

private slots:
    void switchToRegister();
    void switchToLogin();
    void handleLogin();
    void handleRegister();

private:
    void setupUI();
    void setupLoginPage();
    void setupRegisterPage();
    void setMaterialStyle();

    QStackedWidget *stackedWidget;
    QWidget *loginPage;
    QWidget *registerPage;

    // Login page widgets
    QLineEdit *loginEmailInput;
    QLineEdit *loginPasswordInput;
    QPushButton *loginButton;
    QLabel *switchToRegisterLabel;

    // Register page widgets
    QLineEdit *registerEmailInput;
    QLineEdit *registerStudentIdInput;
    QLineEdit *registerNameInput;
    QLineEdit *registerPasswordInput;
    QPushButton *registerButton;
    QLabel *switchToLoginLabel;

    UserManager *userManager;
};

#endif // LOGINWINDOW_H