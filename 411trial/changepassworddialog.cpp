#include "ChangePasswordDialog.h"

ChangePasswordDialog::ChangePasswordDialog(const QString &userEmail, QWidget *parent)
    : QDialog(parent), userEmail(userEmail), countdown(60)
{
    emailSender = new EmailSender(this);
    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, &ChangePasswordDialog::updateCountdown);

    setupUI();
    setupStyles();
    showEmailConfirmPage();

    setFixedSize(400, 300);
    setWindowTitle("修改密码");
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

ChangePasswordDialog::~ChangePasswordDialog()
{
}


void ChangePasswordDialog::setupEmailConfirmPage()
{
    emailConfirmPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(emailConfirmPage);
    layout->setSpacing(20);

    // 标题
    QLabel *titleLabel = new QLabel("验证邮箱");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    // 说明文字
    emailConfirmLabel = new QLabel(QString("我们将向您的邮箱\n%1\n发送验证码以确认身份").arg(userEmail));
    emailConfirmLabel->setObjectName("infoLabel");
    emailConfirmLabel->setAlignment(Qt::AlignCenter);
    emailConfirmLabel->setWordWrap(true);

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    cancelBtn1 = new QPushButton("取消");
    cancelBtn1->setObjectName("cancelBtn");
    sendCodeBtn = new QPushButton("发送验证码");
    sendCodeBtn->setObjectName("primaryBtn");

    buttonLayout->addWidget(cancelBtn1);
    buttonLayout->addWidget(sendCodeBtn);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(emailConfirmLabel);
    layout->addStretch();
    layout->addLayout(buttonLayout);

    stackedWidget->addWidget(emailConfirmPage);

    connect(sendCodeBtn, &QPushButton::clicked, this, &ChangePasswordDialog::onSendCodeClicked);
    connect(cancelBtn1, &QPushButton::clicked, this, &ChangePasswordDialog::onCancelClicked);
}

void ChangePasswordDialog::setupVerificationPage()
{
    verificationPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(verificationPage);
    layout->setSpacing(20);

    // 标题
    QLabel *titleLabel = new QLabel("输入验证码");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    // 说明文字
    verificationLabel = new QLabel(QString("验证码已发送至\n%1\n请查收并输入验证码").arg(userEmail));
    verificationLabel->setObjectName("infoLabel");
    verificationLabel->setAlignment(Qt::AlignCenter);
    verificationLabel->setWordWrap(true);

    // 验证码输入框
    codeEdit = new QLineEdit();
    codeEdit->setObjectName("codeEdit");
    codeEdit->setPlaceholderText("请输入6位验证码");
    codeEdit->setMaxLength(6);
    codeEdit->setAlignment(Qt::AlignCenter);

    // 按钮布局
    QHBoxLayout *buttonLayout1 = new QHBoxLayout();
    resendBtn = new QPushButton("重新发送 (60)");
    resendBtn->setObjectName("secondaryBtn");
    resendBtn->setEnabled(false);

    buttonLayout1->addStretch();
    buttonLayout1->addWidget(resendBtn);
    buttonLayout1->addStretch();

    QHBoxLayout *buttonLayout2 = new QHBoxLayout();
    cancelBtn2 = new QPushButton("取消");
    cancelBtn2->setObjectName("cancelBtn");
    verifyBtn = new QPushButton("验证");
    verifyBtn->setObjectName("primaryBtn");

    buttonLayout2->addWidget(cancelBtn2);
    buttonLayout2->addWidget(verifyBtn);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(verificationLabel);
    layout->addWidget(codeEdit);
    layout->addLayout(buttonLayout1);
    layout->addStretch();
    layout->addLayout(buttonLayout2);

    stackedWidget->addWidget(verificationPage);

    connect(verifyBtn, &QPushButton::clicked, this, &ChangePasswordDialog::onVerifyCodeClicked);
    connect(resendBtn, &QPushButton::clicked, this, &ChangePasswordDialog::onSendCodeClicked);
    connect(cancelBtn2, &QPushButton::clicked, this, &ChangePasswordDialog::onCancelClicked);
}

void ChangePasswordDialog::setupPasswordPage()
{
    passwordPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(passwordPage);
    layout->setSpacing(20);

    // 标题
    QLabel *titleLabel = new QLabel("设置新密码");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    // 说明文字
    passwordLabel = new QLabel("请输入新密码，密码长度应为6-20位");
    passwordLabel->setObjectName("infoLabel");
    passwordLabel->setAlignment(Qt::AlignCenter);

    // 密码输入框
    QVBoxLayout *inputLayout = new QVBoxLayout();

    newPasswordEdit = new QLineEdit();
    newPasswordEdit->setObjectName("passwordEdit");
    newPasswordEdit->setPlaceholderText("请输入新密码");
    newPasswordEdit->setEchoMode(QLineEdit::Password);

    confirmPasswordEdit = new QLineEdit();
    confirmPasswordEdit->setObjectName("passwordEdit");
    confirmPasswordEdit->setPlaceholderText("请确认新密码");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    inputLayout->addWidget(newPasswordEdit);
    inputLayout->addWidget(confirmPasswordEdit);

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    cancelBtn3 = new QPushButton("取消");
    cancelBtn3->setObjectName("cancelBtn");
    confirmBtn = new QPushButton("确认修改");
    confirmBtn->setObjectName("primaryBtn");

    buttonLayout->addWidget(cancelBtn3);
    buttonLayout->addWidget(confirmBtn);

    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addWidget(passwordLabel);
    layout->addLayout(inputLayout);
    layout->addStretch();
    layout->addLayout(buttonLayout);

    stackedWidget->addWidget(passwordPage);

    connect(confirmBtn, &QPushButton::clicked, this, &ChangePasswordDialog::onConfirmPasswordClicked);
    connect(cancelBtn3, &QPushButton::clicked, this, &ChangePasswordDialog::onCancelClicked);
}


void ChangePasswordDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建主容器
    QFrame *containerFrame = new QFrame();
    containerFrame->setObjectName("containerFrame");

    QVBoxLayout *containerLayout = new QVBoxLayout(containerFrame);
    containerLayout->setContentsMargins(20, 20, 20, 20);

    stackedWidget = new QStackedWidget();

    // 创建三个页面
    setupEmailConfirmPage();
    setupVerificationPage();
    setupPasswordPage();

    containerLayout->addWidget(stackedWidget);
    mainLayout->addWidget(containerFrame);
}



void ChangePasswordDialog::setupStyles()
{
    setStyleSheet(R"(
        QFrame#containerFrame {
            background-color: white;
            border-radius: 12px;
            border: 1px solid #e0e0e0;
        }

        QLabel#titleLabel {
            font-size: 18px;
            font-weight: bold;
            color: #2c2c2c;
            padding: 10px 0px;
        }

        QLabel#infoLabel {
            font-size: 14px;
            color: #666666;
            line-height: 1.5;
            padding: 10px;
        }

        QLineEdit#codeEdit {
            padding: 12px;
            border: 2px solid #d0d7de;
            border-radius: 8px;
            font-size: 18px;
            font-weight: bold;
            letter-spacing: 2px;
            background-color: #f8f9fa;
        }

        QLineEdit#codeEdit:focus {
            border-color: #1976D2;
            background-color: white;
        }

        QLineEdit#passwordEdit {
            padding: 12px;
            border: 1px solid #d0d7de;
            border-radius: 6px;
            font-size: 14px;
            background-color: white;
            margin: 2px 0px;
        }

        QLineEdit#passwordEdit:focus {
            border-color: #1976D2;
        }

        QPushButton#primaryBtn {
            background-color: #1976D2;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 12px 24px;
            font-size: 14px;
            font-weight: 500;
            min-width: 100px;
        }

        QPushButton#primaryBtn:hover {
            background-color: #1565C0;
        }

        QPushButton#primaryBtn:pressed {
            background-color: #0D47A1;
        }

        QPushButton#primaryBtn:disabled {
            background-color: #cccccc;
            color: #888888;
        }

        QPushButton#cancelBtn {
            background-color: transparent;
            color: #666666;
            border: 1px solid #d0d7de;
            border-radius: 6px;
            padding: 12px 24px;
            font-size: 14px;
            min-width: 100px;
        }

        QPushButton#cancelBtn:hover {
            background-color: #f8f9fa;
            border-color: #999999;
        }

        QPushButton#secondaryBtn {
            background-color: transparent;
            color: #1976D2;
            border: 1px solid #1976D2;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 12px;
        }

        QPushButton#secondaryBtn:hover {
            background-color: #f0f7ff;
        }

        QPushButton#secondaryBtn:disabled {
            color: #cccccc;
            border-color: #cccccc;
            background-color: transparent;
        }
    )");
}

void ChangePasswordDialog::showEmailConfirmPage()
{
    stackedWidget->setCurrentWidget(emailConfirmPage);
}

void ChangePasswordDialog::showVerificationPage()
{
    stackedWidget->setCurrentWidget(verificationPage);
    codeEdit->setFocus();

    // 开始倒计时
    countdown = 60;
    countdownTimer->start(1000);
    resendBtn->setEnabled(false);
    updateCountdown();
}

void ChangePasswordDialog::showPasswordPage()
{
    stackedWidget->setCurrentWidget(passwordPage);
    newPasswordEdit->setFocus();
}

void ChangePasswordDialog::onSendCodeClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    btn->setEnabled(false);
    btn->setText("发送中...");

    // 发送验证码
    if (emailSender->SendpasswordCode(userEmail, generatedCode)) {
        QMessageBox::information(this, "成功", "验证码已发送，请查收邮件！");
        showVerificationPage();
    } else {
        QMessageBox::warning(this, "发送失败", "验证码发送失败，请检查网络连接后重试。");
        btn->setEnabled(true);
        btn->setText("发送验证码");
    }
}

void ChangePasswordDialog::onVerifyCodeClicked()
{
    QString inputCode = codeEdit->text().trimmed();

    if (inputCode.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入验证码！");
        return;
    }

    if (inputCode.length() != 6) {
        QMessageBox::warning(this, "提示", "验证码应为6位数字！");
        return;
    }

    if (inputCode == generatedCode) {
        countdownTimer->stop();
        showPasswordPage();
    } else {
        QMessageBox::warning(this, "验证失败", "验证码错误，请重新输入！");
        codeEdit->clear();
        codeEdit->setFocus();
    }
}

void ChangePasswordDialog::onConfirmPasswordClicked()
{
    QString newPassword = newPasswordEdit->text().trimmed();
    QString confirmPassword = confirmPasswordEdit->text().trimmed();

    if (newPassword.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写完整的密码信息！");
        return;
    }

    if (newPassword.length() < 6 || newPassword.length() > 20) {
        QMessageBox::warning(this, "提示", "密码长度应为6-20位！");
        return;
    }

    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "提示", "两次输入的密码不一致！");
        confirmPasswordEdit->clear();
        confirmPasswordEdit->setFocus();
        return;
    }

    // 这里可以添加密码修改的实际逻辑
    QMessageBox::information(this, "成功", "密码修改成功！");
    accept();
}

void ChangePasswordDialog::onCancelClicked()
{
    reject();
}

void ChangePasswordDialog::updateCountdown()
{
    countdown--;
    if (countdown <= 0) {
        countdownTimer->stop();
        resendBtn->setText("重新发送");
        resendBtn->setEnabled(true);
    } else {
        resendBtn->setText(QString("重新发送 (%1)").arg(countdown));
    }
}
