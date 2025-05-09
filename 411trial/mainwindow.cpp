// mainwindow.cpp
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 主窗口设置
    setMinimumSize(1024, 768);

    // 侧边栏初始化
    sidebar = new QWidget(this);
    sidebar->setStyleSheet("background-color: #FFFFFF; border-right: 1px solid #E0E0E0;"); // Material Design风格的白色背景和边框
    sidebar->setFixedWidth(expandedWidth);

    // 带图标的切换按钮
    toggleBtn = new QPushButton(sidebar);
    toggleBtn->setIcon(QIcon(":/resources/menu.png"));
    toggleBtn->setIconSize(QSize(32, 32));
    toggleBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "   color: #757575;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(0, 0, 0, 0.04);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(0, 0, 0, 0.08);"
        "}");

    // 导航按钮配置
    QVBoxLayout *navLayout = new QVBoxLayout(sidebar);
    navLayout->setAlignment(Qt::AlignTop);
    navLayout->setSpacing(8);
    navLayout->setContentsMargins(10, 20, 10, 20);

    const QList<QPair<QString, QString>> navItems = {
        {"         主页", ":/resources/home.png"},
        {"        实验室", ":/resources/lab.png"},
        {"         项目", ":/resources/tasks.png"},
        {"      个人资料", ":/resources/profile.png"}


    };

    foreach (const auto &item, navItems) {
        QPushButton *btn = new QPushButton(sidebar);
        btn->setIcon(QIcon(item.second));
        btn->setText(item.first);
        btn->setIconSize(QSize(32, 32));
        btn->setStyleSheet(
            "QPushButton {"
            "   color: #424242;"
            "   text-align: left;"
            "   font-size: 14px;"
            "   font-weight: 500;"
            "   padding: 12px 16px;"
            "   border-radius: 4px;"
            "   background-color: transparent;"
            "   spacing: 12px;"
            "}"
            "QPushButton:hover {"
            "   background-color: rgba(0, 0, 0, 0.04);"
            "   color: #1976D2;"
            "}"
            "QPushButton:pressed {"
            "   background-color: rgba(0, 0, 0, 0.08);"
            "}"
            "QPushButton:checked {"
            "   color: #1976D2;"
            "   background-color: #E3F2FD;"
            "}");
        btn->setFixedHeight(50);
        navLayout->addWidget(btn);
    }

    navLayout->addStretch();
    navLayout->addWidget(toggleBtn);

    // 内容区域
    QTextEdit *contentArea = new QTextEdit(this);
    contentArea->setStyleSheet(
        "QTextEdit {"
        "   background-color: #FAFAFA;"
        "   border: none;"
        "   font-size: 14px;"
        "   padding: 24px;"
        "   color: #212121;"
        "}");

    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(contentArea, 1);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 动画配置
    animation = new QPropertyAnimation(sidebar, "minimumWidth");
    animation->setDuration(350);
    animation->setEasingCurve(QEasingCurve::InOutQuad);

    // 信号连接
    connect(toggleBtn, &QPushButton::clicked, this, &MainWindow::toggleSidebar);


    // 用户信息容器（带顶部边框）
    QWidget *userInfoBox = new QWidget(sidebar);
    userInfoBox->setObjectName("userInfoBox");
    userInfoBox->setFixedHeight(100);  // 固定高度防止挤压

    // 用户信息布局
    QVBoxLayout *userLayout = new QVBoxLayout(userInfoBox);
    userLayout->setContentsMargins(10, 10, 10, 10);
    userLayout->setSpacing(8);

    // 头像显示
    QLabel *avatar = new QLabel(userInfoBox);
    QPixmap pix(":/resources/avatar.png");
    pix = pix.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatar->setPixmap(pix);
    avatar->setAlignment(Qt::AlignCenter);
    //avatar->setStyleSheet("border-radius:24px; border:5px solid #0d0920;");

    // 昵称标签
    QLabel *username = new QLabel("Jam", userInfoBox);
    username->setAlignment(Qt::AlignCenter);
    username->setStyleSheet("color: #424242; font-size: 16px; font-weight: 500;");

    // 组装用户信息
    userLayout->addWidget(avatar);
    userLayout->addWidget(username);


    navLayout->addStretch();  // 将用户信息推到最底部
    navLayout->addWidget(userInfoBox);
    navLayout->addWidget(toggleBtn);  // 原toggleBtn位置调整
}

void MainWindow::toggleSidebar()
{
    animation->stop();
    animation->setStartValue(sidebar->width());
    animation->setEndValue(isCollapsed ? expandedWidth : collapsedWidth);
    animation->start();

    // 更新按钮状态
    if (isCollapsed) {
        toggleBtn->setIcon(QIcon(":/resources/menu.png"));
        foreach (QPushButton *btn, sidebar->findChildren<QPushButton*>()) {
            if (btn != toggleBtn) {
                btn->setText(btn->property("originalText").toString());
                btn->setStyleSheet(btn->styleSheet() + "spacing: 15px;");  // 恢复间距
            }
        }
    } else {
        toggleBtn->setIcon(QIcon(":/icons/menu_close.png"));
        foreach (QPushButton *btn, sidebar->findChildren<QPushButton*>()) {
            if (btn != toggleBtn) {
                btn->setProperty("originalText", btn->text());
                btn->setText("");
                btn->setStyleSheet(btn->styleSheet() + "spacing: 0px;");  // 移除文字间距
            }
        }
    }

    // 新增折叠状态下的用户信息处理
    foreach (QLabel *label, sidebar->findChildren<QLabel*>()) {
        if (label->objectName() == "usernameLabel") {  // 给username设置objectName
            label->setVisible(!isCollapsed);  // 折叠时隐藏用户名
        }
    }
    // avatar->setPixmap(isCollapsed ?
    //                       pix.scaled(32,32, Qt::KeepAspectRatio) :  // 折叠时小头像
    //                       pix.scaled(48,48, Qt::KeepAspectRatio));  // 展开时正常大小

    isCollapsed = !isCollapsed;
}
