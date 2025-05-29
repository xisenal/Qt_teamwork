#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QMessageBox>
#include <QTimer>
#include <QFrame>
#include "emailsender.h"

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePasswordDialog(const QString &userEmail, QWidget *parent = nullptr);
    ~ChangePasswordDialog();

private slots:
    void onSendCodeClicked();
    void onVerifyCodeClicked();
    void onConfirmPasswordClicked();
    void onCancelClicked();
    void updateCountdown();

private:
    void setupUI();
    void setupStyles();
    void showEmailConfirmPage();
    void showVerificationPage();
    void showPasswordPage();

    void setupEmailConfirmPage();
    void setupVerificationPage();
    void setupPasswordPage();

    // UI组件
    QStackedWidget *stackedWidget;
    QVBoxLayout *mainLayout;

    // 邮箱确认页面
    QWidget *emailConfirmPage;
    QLabel *emailConfirmLabel;
    QPushButton *sendCodeBtn;
    QPushButton *cancelBtn1;

    // 验证码页面
    QWidget *verificationPage;
    QLabel *verificationLabel;
    QLineEdit *codeEdit;
    QPushButton *verifyBtn;
    QPushButton *resendBtn;
    QPushButton *cancelBtn2;
    QTimer *countdownTimer;
    int countdown;

    // 修改密码页面
    QWidget *passwordPage;
    QLabel *passwordLabel;
    QLineEdit *newPasswordEdit;
    QLineEdit *confirmPasswordEdit;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn3;

    // 数据成员
    QString userEmail;
    QString generatedCode;
    EmailSender *emailSender;
};

#endif // CHANGEPASSWORDDIALOG_H
