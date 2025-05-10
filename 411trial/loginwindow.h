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
    void switchToResetPassword();
    void handleLogin();
    void handleRegister();
    void handleResetPassword();
    void showAgreementDialog();

private:
    void setupUI();
    void openAdminWindow();
    void openMainWindow();
    void setupLoginPage();
    void setupRegisterPage();
    void setupResetPasswordPage();
    void setMaterialStyle();

    QStackedWidget *stackedWidget;
    QWidget *loginPage;
    QWidget *registerPage;
    QWidget *resetPasswordPage;

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

    // Reset password page widgets
    QLineEdit *resetEmailInput;
    QLineEdit *resetVerificationCodeInput;
    QLineEdit *resetPasswordInput;
    QLineEdit *resetConfirmPasswordInput;
    QPushButton *resetSendVerificationButton;
    QPushButton *resetPasswordButton;
    QLabel *resetSwitchToLoginLabel;
    QString resetVerificationCode;

    UserManager *userManager;
    EmailSender *emailSender;
};

#endif // LOGINWINDOW_H