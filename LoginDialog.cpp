#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QRegularExpression>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    // 注册页面
    QWidget *registerPage = new QWidget;
    QVBoxLayout *registerLayout = new QVBoxLayout;
    nicknameEdit = new QLineEdit;
    emailEdit = new QLineEdit;
    studentIdEdit = new QLineEdit;
    QPushButton *registerBtn = new QPushButton("注册");
    
    // 设置验证器
    QRegularExpression emailRegex("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}");
    emailEdit->setPlaceholderText("请输入有效的邮箱地址");
    
    QRegularExpression studentIdRegex("^\\d{10}$");
    studentIdEdit->setPlaceholderText("请输入10位学号");

    registerLayout->addWidget(new QLabel("昵称:"));
    registerLayout->addWidget(nicknameEdit);
    registerLayout->addWidget(new QLabel("邮箱:"));
    registerLayout->addWidget(emailEdit);
    registerLayout->addWidget(new QLabel("学号:"));
    registerLayout->addWidget(studentIdEdit);
    registerLayout->addWidget(registerBtn);
    registerPage->setLayout(registerLayout);

    // 登录页面
    QWidget *loginPage = new QWidget;
    QVBoxLayout *loginLayout = new QVBoxLayout;
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    QPushButton *loginBtn = new QPushButton("登录");

    loginLayout->addWidget(new QLabel("邮箱:"));
    loginLayout->addWidget(emailEdit);
    loginLayout->addWidget(new QLabel("密码:"));
    loginLayout->addWidget(passwordEdit);
    loginLayout->addWidget(loginBtn);
    loginPage->setLayout(loginLayout);

    stack = new QStackedWidget;
    stack->addWidget(registerPage);
    stack->addWidget(loginPage);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(stack);
    setLayout(mainLayout);

    connect(registerBtn, &QPushButton::clicked, this, &LoginDialog::validateAndAccept);
    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::validateAndAccept);
}

void LoginDialog::validateAndAccept() {
    QRegularExpression emailRegex("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}");
    QRegularExpression studentIdRegex("^\\d{10}$");
    
    if (!emailRegex.match(emailEdit->text()).hasMatch()) {
        QMessageBox::warning(this, "验证错误", "请输入有效的邮箱地址");
        return;
    }
    
    if (stack->currentIndex() == 0) { // 注册页面
        if (!studentIdRegex.match(studentIdEdit->text()).hasMatch()) {
            QMessageBox::warning(this, "验证错误", "请输入10位数字学号");
            return;
        }
    }
    
    accept();
}

void LoginDialog::switchToLogin() { stack->setCurrentIndex(1); }
void LoginDialog::switchToRegister() { stack->setCurrentIndex(0); }
