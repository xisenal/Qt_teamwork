// mainwindow.cpp
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

void MainWindow::createSubMenu() {
    subMenu = new QWidget(this);
    subMenu->setStyleSheet(
        "QWidget {"
        "   background: #FFFFFF;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 4px;"
        "   box-shadow: 0 2px 4px rgba(0,0,0,0.1);"
        "}"
        "QLabel {"
        "   color: #424242;"
        "}"
        "QToolButton {"
        "   color: #424242;"
        "   border: none;"
        "   padding: 4px;"
        "}"
        "QToolButton:hover {"
        "   background: #F5F5F5;"
        "}");
    subMenu->setFixedWidth(150);
    subMenu->setFixedHeight(730);
    subMenu->hide();

    QVBoxLayout *mainLayout = new QVBoxLayout(subMenu);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(4);

    const QStringList items = {"Owner", "Contributor", "Visitor"};
    foreach (const QString &text, items) {
        // 创建可折叠面板
        QWidget *panel = new QWidget(subMenu);
        QVBoxLayout *panelLayout = new QVBoxLayout(panel);
        panelLayout->setContentsMargins(0, 0, 0, 0);
        panelLayout->setSpacing(0);

        // 标题栏
        QWidget *header = new QWidget(panel);
        QHBoxLayout *headerLayout = new QHBoxLayout(header);
        headerLayout->setContentsMargins(4, 4, 4, 4);
        headerLayout->setSpacing(4);

        // 折叠按钮
        QToolButton *toggleBtn = new QToolButton(header);
        toggleBtn->setArrowType(Qt::RightArrow);
        toggleBtn->setObjectName("toggleBtn");
        toggleBtn->setFixedSize(16, 16);

        // 标题文本
        QLabel *titleLabel = new QLabel(text, header);

        // 添加按钮
        QToolButton *addBtn = new QToolButton(header);
        addBtn->setText("+");
        addBtn->setFixedSize(16, 16);

        headerLayout->addWidget(toggleBtn);
        headerLayout->addWidget(titleLabel);
        headerLayout->addStretch();
        headerLayout->addWidget(addBtn);

        // 内容区域
        QWidget *content = new QWidget(panel);
        QVBoxLayout *contentLayout = new QVBoxLayout(content);
        contentLayout->setContentsMargins(24, 2, 2, 2); // 添加左侧缩进
        content->hide();

        panelLayout->addWidget(header);
        panelLayout->addWidget(content);
        mainLayout->addWidget(panel);

        // 连接信号
        connect(toggleBtn, &QToolButton::clicked, [=](){
            content->setVisible(!content->isVisible());
            toggleBtn->setArrowType(content->isVisible() ?
                                        Qt::DownArrow : Qt::RightArrow);
        });

        connect(addBtn, &QToolButton::clicked, [=](){
            // 创建可编辑项
            QLineEdit *newItem = new QLineEdit(content);
            newItem->setPlaceholderText("New lab");
            newItem->setStyleSheet(
                "QLineEdit {"
                "   border: 1px solid #E0E0E0;"
                "   border-radius: 2px;"
                "   padding: 2px 4px;"
                "}"
                "QLineEdit:focus {"
                "   border-color: #0078D4;"
                "}");
            newItem->setMinimumHeight(24);
            contentLayout->addWidget(newItem);
        });
    }

    mainLayout->addStretch(); // 添加弹簧使内容置顶

    // 事件过滤器保持原有逻辑
    sidebar->installEventFilter(this);

    // 创建完成后立即执行布局计算
    subMenu->show();  // 必须先show才能正确计算尺寸
    subMenu->hide();
    //subMenu->setAttribute(Qt::WA_LayoutUpsideDown);  // 适应不同布局方向
}


// 修改后的位置更新函数
void MainWindow::updateSubMenuPosition() {
    if (subMenu && labBtn) {
        // 转换为当前窗口坐标系
        QPoint pos = labBtn->mapToGlobal(QPoint(0, 0));
        pos = this->mapFromGlobal(pos);

        // 调整显示位置为按钮右下方
        int y = 38;//手动调的
        subMenu->move(100, y); // 垂直居中

        // 确保最小高度
        //subMenu->setMinimumHeight(items.count() * 30); // 根据项目数动态调整
        subMenu->adjustSize(); // 关键！让布局重新计算
    }
}

// 修改后的切换函数
void MainWindow::toggleLabSubMenu() {
    if (!subMenu) {
        createSubMenu();
        subMenu->setParent(this);  // 确保父子关系正确
        subMenu->setWindowFlags(Qt::SubWindow);
    }

    isSubMenuVisible = !isSubMenuVisible;

    if (isSubMenuVisible) {
        updateSubMenuPosition();
        // 展开第一个分类（可选）
        if(auto firstPanel = subMenu->findChild<QWidget*>())
            firstPanel->findChild<QToolButton*>()->click();
    }

    subMenu->setVisible(isSubMenuVisible);
    subMenu->raise();
}




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置无边框窗口
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // 主窗口设置
    setMinimumSize(1024, 768);

    // 创建标题栏
    QWidget *titleBar = new QWidget(this);
    titleBar->setFixedHeight(40);
    titleBar->setStyleSheet("background-color: #FFFFFF; border-bottom: 1px solid #E0E0E0;");

    // 创建标题栏按钮
    pinButton = new QPushButton("📌", titleBar);
    minButton = new QPushButton("-", titleBar);
    maxButton = new QPushButton("□", titleBar);
    closeButton = new QPushButton("×", titleBar);



    // labBtn = new QLabel(this);     // 确保已创建
    // subMenu = new QMenu(this);     // 确保已创建
    // labBtn->installEventFilter(this); // 安装事件过滤器

    // 设置按钮样式
    QString buttonStyle = 
        "QPushButton { "
        "    border: none; "
        "    font-size: 16px; "
        "    color: #666; "
        "    padding: 8px; "
        "    width: 30px; "
        "    height: 30px; "
        "} "
        "QPushButton:hover { background-color: #e6e6e6; } ";

    pinButton->setStyleSheet(buttonStyle);
    minButton->setStyleSheet(buttonStyle);
    maxButton->setStyleSheet(buttonStyle);
    closeButton->setStyleSheet(buttonStyle + "QPushButton:hover { background-color: #e81123; color: white; }");

    // 连接按钮信号
    connect(pinButton, &QPushButton::clicked, [this, buttonStyle]() {
        isPinned = !isPinned;
        if (isPinned) {
            setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
            pinButton->setStyleSheet(buttonStyle + "QPushButton { color: #2196F3; }");
        } else {
            setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
            pinButton->setStyleSheet(buttonStyle);
        }
        show();
    });
    connect(minButton, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(maxButton, &QPushButton::clicked, [this]() {
        if (isMaximized()) {
            showNormal();
            maxButton->setText("□");
        } else {
            showMaximized();
            maxButton->setText("❐");
        }
    });
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);

    // 创建标题栏布局
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(10, 0, 10, 0);
    titleLayout->setSpacing(8);
    titleLayout->addStretch();
    titleLayout->addWidget(pinButton);
    titleLayout->addWidget(minButton);
    titleLayout->addWidget(maxButton);
    titleLayout->addWidget(closeButton);

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

        if (item.first.contains("实验室")) {
            labBtn = btn;  // 保存实验室按钮的指针
        }

        if (item.first.contains("主页")) {
            homeBtn = btn;  // 保存主页按钮指针
        }
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

    // 创建中央部件和主布局
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainVLayout = new QVBoxLayout(centralWidget);
    mainVLayout->setContentsMargins(0, 0, 0, 0);
    mainVLayout->setSpacing(0);
    mainVLayout->addWidget(titleBar);
    
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(sidebar);
    contentLayout->addWidget(contentArea, 1);
    
    mainVLayout->addLayout(contentLayout, 1);

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


    createSubMenu();  // 初始化 subMenu
    // 在构造函数末尾添加连接
    connect(labBtn, &QPushButton::clicked, this, &MainWindow::toggleLabSubMenu);

    connect(sidebar, &QWidget::customContextMenuRequested,
            this, &MainWindow::updateSubMenuPosition);

    // 添加事件过滤器处理外部点击（在MainWindow构造函数末尾）
    //subMenu->installEventFilter(this);
    qApp->installEventFilter(this);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && !isMaximized()) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
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


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if (!mouseEvent) return QMainWindow::eventFilter(obj, event);

        // 获取点击的全局坐标
        QPoint globalPos;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        globalPos = mouseEvent->globalPos();
#else
        globalPos = mouseEvent->globalPosition().toPoint();
#endif

        // 如果子菜单可见且点击的是主页按钮
        if (subMenu && subMenu->isVisible())
        {
            // 转换坐标到主页按钮坐标系
            QPoint homeBtnPos = homeBtn->mapFromGlobal(globalPos);

            // 判断是否点击在主页按钮区域内
            if (homeBtn->rect().contains(homeBtnPos))
            {
                subMenu->hide();
                isSubMenuVisible = false;
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

