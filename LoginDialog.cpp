#include "LoginDialog.h"
#include <QVBoxLayout>
#include <Qlabel>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    // 注册页面
    QWidget *registerPage = new QWidget;
    QVBoxLayout *registerLayout = new QVBoxLayout;
    nicknameEdit = new QLineEdit;
    emailEdit = new QLineEdit;
    QPushButton *registerBtn = new QPushButton("注册");

    registerLayout->addWidget(new QLabel("昵称:"));
    registerLayout->addWidget(nicknameEdit);
    registerLayout->addWidget(new QLabel("邮箱:"));
    registerLayout->addWidget(emailEdit);
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

    connect(registerBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(loginBtn, &QPushButton::clicked, this, &QDialog::accept);
}

void LoginDialog::switchToLogin() { stack->setCurrentIndex(1); }
void LoginDialog::switchToRegister() { stack->setCurrentIndex(0); }
