#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include "usermanager.h"
#include "emailsender.h"
#include "adminwindow.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void loginSuccess();
    void adminLoginSuccess();

private slots:
    void switchToRegister();
    void switchToLogin();
    void handleLogin();
    void handleRegister();

private:
    void setupUI();
    void openAdminWindow();
    void openMainWindow();
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
    QCheckBox *loginAgreementCheckBox;

    // Register page widgets
    QLineEdit *registerEmailInput;
    QLineEdit *registerPasswordInput;
    QLineEdit *registerConfirmPasswordInput;
    QLineEdit *verificationCodeInput;
    QPushButton *registerButton;
    QPushButton *sendVerificationButton;
    QLabel *switchToLoginLabel;
    QCheckBox *registerAgreementCheckBox;
    QString currentVerificationCode;

    UserManager *userManager;
    EmailSender *emailSender;
};

#endif // LOGINWINDOW_H