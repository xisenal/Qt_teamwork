#include "loginwindow.h"
#include "adminwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTextEdit>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , userManager(new UserManager(this))
    , emailSender(new EmailSender(this))
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setupUI();
    setMaterialStyle();
    userManager->initDatabase();
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void LoginWindow::setupUI()
{
    setFixedSize(400, 500);
    setWindowTitle("登录/注册");

    // 创建一个主容器，用于设置圆角和阴影
    QWidget *container = new QWidget(this);
    container->setObjectName("container");
    container->setStyleSheet("#container { background-color: white; border-radius: 10px; }");

    // 添加阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 80));
    shadowEffect->setOffset(0, 0);
    container->setGraphicsEffect(shadowEffect);

    // 创建标题栏
    QWidget *titleBar = new QWidget(container);
    titleBar->setFixedHeight(40);
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(10, 0, 10, 0);

    // 添加标题
    QLabel *titleLabel = new QLabel("登录/注册", titleBar);
    titleLabel->setStyleSheet("color: #333; font-size: 14px;");

    // 添加控制按钮
    QPushButton *minButton = new QPushButton("-", titleBar);
    QPushButton *closeButton = new QPushButton("×", titleBar);
    minButton->setFixedSize(30, 30);
    closeButton->setFixedSize(30, 30);
    minButton->setStyleSheet(
        "QPushButton { border: none; font-size: 16px; color: #666; }"
        "QPushButton:hover { background-color: #e6e6e6; }");
    closeButton->setStyleSheet(
        "QPushButton { border: none; font-size: 16px; color: #666; }"
        "QPushButton:hover { background-color: #e81123; color: white; }");

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(minButton);
    titleLayout->addWidget(closeButton);

    // 连接按钮信号
    connect(minButton, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);

    // 设置主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(titleBar);

    stackedWidget = new QStackedWidget(container);
    mainLayout->addWidget(stackedWidget);

    // 设置窗口主布局
    QVBoxLayout *windowLayout = new QVBoxLayout(this);
    windowLayout->setContentsMargins(10, 10, 10, 10);
    windowLayout->addWidget(container);

    setupLoginPage();
    setupRegisterPage();
    setupResetPasswordPage();

    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(registerPage);
    stackedWidget->addWidget(resetPasswordPage);
}

void LoginWindow::setupLoginPage()
{
    loginPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(loginPage);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 40, 40, 40);

    QLabel *titleLabel = new QLabel("欢迎登录");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; color: #333; margin-bottom: 20px;");

    loginEmailInput = new QLineEdit;
    loginEmailInput->setPlaceholderText("邮箱");
    loginEmailInput->setValidator(new QRegularExpressionValidator(
        QRegularExpression("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"), this));

    loginPasswordInput = new QLineEdit;
    loginPasswordInput->setPlaceholderText("密码");
    loginPasswordInput->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("登录");

    switchToRegisterLabel = new QLabel("没有账号？<a href=\"#\">立即注册</a>");
    switchToRegisterLabel->setAlignment(Qt::AlignCenter);
    switchToRegisterLabel->setTextFormat(Qt::RichText);
    switchToRegisterLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

    layout->addWidget(titleLabel);
    layout->addWidget(loginEmailInput);
    layout->addWidget(loginPasswordInput);
    QHBoxLayout *loginAgreementLayout = new QHBoxLayout;
    loginAgreementLayout->setAlignment(Qt::AlignCenter);
    loginAgreementCheckBox = new QCheckBox("我已阅读并同意", this);
    QLabel *loginAgreementLink = new QLabel("<a href=\"#\">《用户协议》</a>");
    loginAgreementLink->setTextFormat(Qt::RichText);
    loginAgreementLink->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    loginAgreementLink->setStyleSheet("QLabel { color: #2196F3; }");
    connect(loginAgreementLink, &QLabel::linkActivated, this, &LoginWindow::showAgreementDialog);
    loginAgreementLayout->addWidget(loginAgreementCheckBox);
    loginAgreementLayout->addWidget(loginAgreementLink);
    loginAgreementCheckBox->setStyleSheet("QCheckBox { color: #666; font-size: 13px; }");

    QLabel *forgotPasswordLabel = new QLabel("<a href=\"#\">忘记密码？</a>");
    forgotPasswordLabel->setAlignment(Qt::AlignCenter);
    forgotPasswordLabel->setTextFormat(Qt::RichText);
    forgotPasswordLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    forgotPasswordLabel->setStyleSheet("QLabel { color: #666; font-size: 13px; }");

    layout->addWidget(loginButton);
    layout->addLayout(loginAgreementLayout);
    layout->addWidget(forgotPasswordLabel);
    layout->addWidget(switchToRegisterLabel);
    layout->addStretch();

    connect(switchToRegisterLabel, &QLabel::linkActivated, this, &LoginWindow::switchToRegister);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(forgotPasswordLabel, &QLabel::linkActivated, this, &LoginWindow::switchToResetPassword);
}

void LoginWindow::setupRegisterPage()
{
    registerPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(registerPage);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 1, 40, 40);

    QLabel *titleLabel = new QLabel("新用户注册");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; color: #333; margin-bottom: 20px;");

    registerEmailInput = new QLineEdit;
    registerEmailInput->setPlaceholderText("邮箱");
    registerEmailInput->setValidator(new QRegularExpressionValidator(
        QRegularExpression("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"), this));

    registerPasswordInput = new QLineEdit;
    registerPasswordInput->setPlaceholderText("密码");
    registerPasswordInput->setEchoMode(QLineEdit::Password);

    registerConfirmPasswordInput = new QLineEdit;
    registerConfirmPasswordInput->setPlaceholderText("确认密码");
    registerConfirmPasswordInput->setEchoMode(QLineEdit::Password);

    QHBoxLayout *verificationLayout = new QHBoxLayout;
    verificationCodeInput = new QLineEdit;
    verificationCodeInput->setPlaceholderText("验证码");
    verificationCodeInput->setMaxLength(6);
    sendVerificationButton = new QPushButton("发送验证码");
    verificationLayout->addWidget(verificationCodeInput);
    verificationLayout->addWidget(sendVerificationButton);

    registerButton = new QPushButton("注册");

    switchToLoginLabel = new QLabel("已有账号？<a href=\"#\">返回登录</a>");
    switchToLoginLabel->setAlignment(Qt::AlignCenter);
    switchToLoginLabel->setTextFormat(Qt::RichText);
    switchToLoginLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

    layout->addWidget(titleLabel);
    layout->addWidget(registerEmailInput);
    layout->addWidget(registerPasswordInput);
    layout->addWidget(registerConfirmPasswordInput);
    layout->addLayout(verificationLayout);
    QHBoxLayout *registerAgreementLayout = new QHBoxLayout;
    registerAgreementLayout->setAlignment(Qt::AlignCenter);
    registerAgreementCheckBox = new QCheckBox("我已阅读并同意", this);
    QLabel *registerAgreementLink = new QLabel("<a href=\"#\">《用户协议》</a>");
    registerAgreementLink->setTextFormat(Qt::RichText);
    registerAgreementLink->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    registerAgreementLink->setStyleSheet("QLabel { color: #2196F3; }");
    connect(registerAgreementLink, &QLabel::linkActivated, this, &LoginWindow::showAgreementDialog);
    registerAgreementLayout->addWidget(registerAgreementCheckBox);
    registerAgreementLayout->addWidget(registerAgreementLink);
    registerAgreementCheckBox->setStyleSheet("QCheckBox { color: #666; font-size: 13px; }");

    layout->addWidget(registerButton);
    layout->addLayout(registerAgreementLayout);
    layout->addWidget(switchToLoginLabel);
    layout->addStretch();

    connect(switchToLoginLabel, &QLabel::linkActivated, this, &LoginWindow::switchToLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::handleRegister);
    connect(sendVerificationButton, &QPushButton::clicked, this, [this]() {
        QString email = registerEmailInput->text();
        if (email.isEmpty()) {
            QMessageBox::warning(this, "提示", "请输入邮箱地址");
            return;
        }
        if (emailSender->sendVerificationCode(email, currentVerificationCode)) {
            QMessageBox::information(this, "成功", "验证码已发送到您的邮箱");
            sendVerificationButton->setEnabled(false);
            QTimer::singleShot(60000, this, [this]() {
                sendVerificationButton->setEnabled(true);
            });
        } else {
            QMessageBox::warning(this, "错误", "验证码发送失败，请稍后重试");
        }
    });
}

void LoginWindow::setupResetPasswordPage()
{
    resetPasswordPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(resetPasswordPage);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 40, 40, 40);

    QLabel *titleLabel = new QLabel("重置密码");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; color: #333; margin-bottom: 20px;");

    resetEmailInput = new QLineEdit;
    resetEmailInput->setPlaceholderText("邮箱");
    resetEmailInput->setValidator(new QRegularExpressionValidator(
        QRegularExpression("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"), this));

    QHBoxLayout *verificationLayout = new QHBoxLayout;
    resetVerificationCodeInput = new QLineEdit;
    resetVerificationCodeInput->setPlaceholderText("验证码");
    resetVerificationCodeInput->setMaxLength(6);
    resetSendVerificationButton = new QPushButton("发送验证码");
    verificationLayout->addWidget(resetVerificationCodeInput);
    verificationLayout->addWidget(resetSendVerificationButton);

    resetPasswordInput = new QLineEdit;
    resetPasswordInput->setPlaceholderText("新密码");
    resetPasswordInput->setEchoMode(QLineEdit::Password);

    resetConfirmPasswordInput = new QLineEdit;
    resetConfirmPasswordInput->setPlaceholderText("确认新密码");
    resetConfirmPasswordInput->setEchoMode(QLineEdit::Password);

    resetPasswordButton = new QPushButton("重置密码");

    resetSwitchToLoginLabel = new QLabel("<a href=\"#\">返回登录</a>");
    resetSwitchToLoginLabel->setAlignment(Qt::AlignCenter);
    resetSwitchToLoginLabel->setTextFormat(Qt::RichText);
    resetSwitchToLoginLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

    layout->addWidget(titleLabel);
    layout->addWidget(resetEmailInput);
    layout->addLayout(verificationLayout);
    layout->addWidget(resetPasswordInput);
    layout->addWidget(resetConfirmPasswordInput);
    layout->addWidget(resetPasswordButton);
    layout->addWidget(resetSwitchToLoginLabel);
    layout->addStretch();

    connect(resetSwitchToLoginLabel, &QLabel::linkActivated, this, &LoginWindow::switchToLogin);
    connect(resetPasswordButton, &QPushButton::clicked, this, &LoginWindow::handleResetPassword);
    connect(resetSendVerificationButton, &QPushButton::clicked, this, [this]() {
        QString email = resetEmailInput->text();
        if (email.isEmpty()) {
            QMessageBox::warning(this, "提示", "请输入邮箱地址");
            return;
        }
        if (emailSender->sendVerificationCode(email, resetVerificationCode)) {
            QMessageBox::information(this, "成功", "验证码已发送到您的邮箱");
            resetSendVerificationButton->setEnabled(false);
            QTimer::singleShot(60000, this, [this]() {
                resetSendVerificationButton->setEnabled(true);
            });
        } else {
            QMessageBox::warning(this, "错误", "验证码发送失败，请稍后重试");
        }
    });
}

void LoginWindow::switchToResetPassword()
{
    stackedWidget->setCurrentWidget(resetPasswordPage);
}

void LoginWindow::handleResetPassword()
{
    QString email = resetEmailInput->text();
    QString verificationCode = resetVerificationCodeInput->text();
    QString newPassword = resetPasswordInput->text();
    QString confirmPassword = resetConfirmPasswordInput->text();

    if (email.isEmpty() || verificationCode.isEmpty() || newPassword.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写完整信息");
        return;
    }

    if (verificationCode != resetVerificationCode) {
        QMessageBox::warning(this, "提示", "验证码错误");
        return;
    }

    if (newPassword != confirmPassword) {
        QMessageBox::warning(this, "提示", "两次输入的密码不一致");
        return;
    }

    if (userManager->resetPassword(email, newPassword)) {
        QMessageBox::information(this, "成功", "密码重置成功，请使用新密码登录");
        switchToLogin();
        resetEmailInput->clear();
        resetVerificationCodeInput->clear();
        resetPasswordInput->clear();
        resetConfirmPasswordInput->clear();
    } else {
        QMessageBox::warning(this, "错误", "密码重置失败，请稍后重试");
    }
}

void LoginWindow::showAgreementDialog()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("用户协议");
    dialog->setFixedSize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QTextEdit *textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet("QTextEdit { background-color: white; padding: 10px; }");

    QString agreementText = 
        "用户协议\n\n"
        "1. 协议的接受\n"
        "   通过注册或使用本软件，即表示您同意遵守本协议的所有条款和条件。\n\n"
        "2. 用户责任\n"
        "   2.1 您应当遵守中华人民共和国相关法律法规。\n"
        "   2.2 您应当妥善保管账号和密码，对账号下的所有活动负责。\n"
        "   2.3 您不得利用本软件从事任何违法或不当行为。\n\n"
        "3. 隐私保护\n"
        "   3.1 我们重视用户的隐私保护，将依法保护用户的个人信息。\n"
        "   3.2 未经您的同意，我们不会向第三方披露您的个人信息。\n\n"
        "4. 服务内容\n"
        "   4.1 本软件提供的服务内容包括但不限于：用户注册、登录等基本功能。\n"
        "   4.2 我们保留随时修改或中断服务的权利。\n\n"
        "5. 知识产权\n"
        "   5.1 本软件的所有权利均归开发者所有。\n"
        "   5.2 未经授权，用户不得复制、修改、传播或使用本软件的任何内容。\n\n"
        "6. 免责声明\n"
        "   6.1 对于因不可抗力导致的服务中断或损失，我们不承担责任。\n"
        "   6.2 用户因违反本协议造成的任何损失由用户自行承担。\n\n"
        "7. 协议修改\n"
        "   我们保留随时修改本协议的权利，修改后的协议将在软件中公布。";

    textEdit->setText(agreementText);
    layout->addWidget(textEdit);

    QPushButton *closeButton = new QPushButton("关闭", dialog);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    padding: 8px 16px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}");
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    layout->addLayout(buttonLayout);

    dialog->exec();
    dialog->deleteLater();
}

void LoginWindow::setMaterialStyle()
{
    QString inputStyle = 
        "QLineEdit {"
        "    border: none;"
        "    border-bottom: 2px solid #ddd;"
        "    padding: 8px 4px;"
        "    background: transparent;"
        "    font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "    border-bottom: 2px solid #2196F3;"
        "}";

    QString buttonStyle =
        "QPushButton {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    padding: 10px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #0D47A1;"
        "}";

    QString labelStyle =
        "QLabel {"
        "    color: #666;"
        "    font-size: 13px;"
        "}"
        "QLabel a {"
        "    color: #2196F3;"
        "    text-decoration: none;"
        "}"
        "QLabel a:hover {"
        "    color: #1976D2;"
        "}";

    loginEmailInput->setStyleSheet(inputStyle);
    loginPasswordInput->setStyleSheet(inputStyle);
    registerEmailInput->setStyleSheet(inputStyle);
    registerPasswordInput->setStyleSheet(inputStyle);
    registerConfirmPasswordInput->setStyleSheet(inputStyle);

    loginButton->setStyleSheet(buttonStyle);
    registerButton->setStyleSheet(buttonStyle);

    switchToRegisterLabel->setStyleSheet(labelStyle);
    switchToLoginLabel->setStyleSheet(labelStyle);
}

void LoginWindow::openAdminWindow()
{
    AdminWindow *adminWindow = new AdminWindow;
    adminWindow->show();
    emit adminLoginSuccess();
}

void LoginWindow::openMainWindow()
{
    emit loginSuccess();
}

void LoginWindow::switchToRegister()
{
    stackedWidget->setCurrentWidget(registerPage);
}

void LoginWindow::switchToLogin()
{
    stackedWidget->setCurrentWidget(loginPage);
}

void LoginWindow::handleLogin()
{
    QString email = loginEmailInput->text();
    QString password = loginPasswordInput->text();

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入邮箱和密码");
        return;
    }

    if (!loginAgreementCheckBox->isChecked()) {
        QMessageBox::warning(this, "提示", "请阅读并同意用户协议");
        return;
    }

    bool isAdmin = false;
    if (userManager->validateLogin(email, password, isAdmin)) {
        if (isAdmin) {
            openAdminWindow();
        } else {
            openMainWindow();
        }
        close();
    } else {
        QMessageBox::warning(this, "错误", "邮箱或密码错误");
    }
}

void LoginWindow::handleRegister()
{
    QString email = registerEmailInput->text();
    QString password = registerPasswordInput->text();
    QString confirmPassword = registerConfirmPasswordInput->text();

    if (email.isEmpty() || password.isEmpty() || confirmPassword.isEmpty() || verificationCodeInput->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写完整的注册信息");
        return;
    }

    if (!registerAgreementCheckBox->isChecked()) {
        QMessageBox::warning(this, "提示", "请阅读并同意用户协议");
        return;
    }

    if (verificationCodeInput->text() != currentVerificationCode) {
        QMessageBox::warning(this, "提示", "验证码错误");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "提示", "两次输入的密码不一致");
        return;
    }

    // 使用默认值作为学号和姓名
    QString defaultStudentId = "未设置";
    QString defaultName = "未设置";

    if (userManager->registerUser(email, defaultStudentId, defaultName, password)) {
        QMessageBox::information(this, "成功", "注册成功，请登录");
        switchToLogin();
        registerEmailInput->clear();
        registerPasswordInput->clear();
        registerConfirmPasswordInput->clear();
    } else {
        QMessageBox::warning(this, "错误", "注册失败，该邮箱可能已被注册");
    }
}