#include "loginwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , userManager(new UserManager(this))
    , emailSender(new EmailSender(this))
{
    setupUI();
    setMaterialStyle();
    userManager->initDatabase();
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::setupUI()
{
    setFixedSize(400, 500);
    setWindowTitle("登录/注册");

    stackedWidget = new QStackedWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stackedWidget);

    setupLoginPage();
    setupRegisterPage();

    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(registerPage);
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
    layout->addWidget(loginButton);
    layout->addWidget(switchToRegisterLabel);
    layout->addStretch();

    connect(switchToRegisterLabel, &QLabel::linkActivated, this, &LoginWindow::switchToRegister);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
}

void LoginWindow::setupRegisterPage()
{
    registerPage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(registerPage);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 40, 40, 40);

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
    layout->addWidget(registerButton);
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
        QMessageBox::warning(this, "提示", "请填写完整的登录信息");
        return;
    }

    if (userManager->validateLogin(email, password)) {
        emit loginSuccess();
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