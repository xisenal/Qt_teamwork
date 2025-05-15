// mainwindow.cpp
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

UserInfoDialog::UserInfoDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    setFixedSize(300, 100);

    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);
    mainLayout->setAlignment(Qt::AlignCenter);

    // 创建水平布局容器
    QWidget *contentWidget = new QWidget(this);
    QHBoxLayout *contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setSpacing(15);
    contentLayout->setAlignment(Qt::AlignCenter);

    // 头像
    QLabel *avatarLabel = new QLabel(contentWidget);
    QPixmap avatar(":/resources/avatar.png");
    avatar = avatar.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatarLabel->setPixmap(avatar);
    avatarLabel->setFixedSize(60, 60);
    avatarLabel->setAlignment(Qt::AlignCenter);

    // 信息布局
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(5);
    infoLayout->setAlignment(Qt::AlignVCenter);

    QLabel *nameLabel = new QLabel("Name_of_this", contentWidget);
    nameLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #333;");
    nameLabel->setAlignment(Qt::AlignCenter);
    QLabel *idLabel = new QLabel("学号：2200011302", contentWidget);
    idLabel->setStyleSheet("font-size: 12px; color: #666;");
    idLabel->setAlignment(Qt::AlignCenter);

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(idLabel);

    // 添加到水平布局
    contentLayout->addWidget(avatarLabel);
    contentLayout->addLayout(infoLayout);

    // 将内容容器添加到主布局
    mainLayout->addWidget(contentWidget);

    // 设置窗口样式
    setStyleSheet(
        "QDialog {"
        "    background: white;"
        "    border: 1px solid #ddd;"
        "    border-radius: 5px;"
        "}");
}

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
    sidebar->setStyleSheet("background-color: #FFFFFF;"); // Material Design风格的白色背景
    sidebar->setFixedWidth(collapsedWidth);



    // 导航按钮配置
    QVBoxLayout *navLayout = new QVBoxLayout(sidebar);
    navLayout->setAlignment(Qt::AlignTop);
    navLayout->setSpacing(8);
    navLayout->setContentsMargins(10, 10, 10, 10);

    const QList<QPair<QString, QString>> navItems = {
        {"         主页", ":/resources/home.png"},
        {"        实验室", ":/resources/lab.png"},
        {"         项目", ":/resources/tasks.png"},
        {"      个人资料", ":/resources/profile.png"},
        {"     To Do List", ":/resources/to-do.png"}
    };

    // 用户信息容器
    QWidget *userInfoBox = new QWidget(sidebar);
    userInfoBox->setObjectName("userInfoBox");
    userInfoBox->setFixedHeight(70);
    userInfoBox->setStyleSheet(
        "QWidget#userInfoBox {"
        "   background-color: transparent;"
        "   border: none;"
        "}");

    // 用户信息布局
    QVBoxLayout *userLayout = new QVBoxLayout(userInfoBox);
    userLayout->setContentsMargins(10, 10, 10, 10);
    userLayout->setSpacing(0);

    // 头像显示
    avatar = new QLabel(userInfoBox);
    QPixmap pix(":/resources/avatar.png");
    pix = pix.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatar->setPixmap(pix);
    avatar->setAlignment(Qt::AlignCenter);
    avatar->setCursor(Qt::PointingHandCursor);
    avatar->installEventFilter(this);

    // 组装用户信息
    userLayout->addWidget(avatar);
    navLayout->addWidget(userInfoBox);

    // 导航按钮
    foreach (const auto &item, navItems) {
        QPushButton *btn = new QPushButton(sidebar);
        btn->setIcon(QIcon(item.second));
        btn->setText("");
        btn->setIconSize(QSize(32, 32));
        btn->setToolTip(item.first.trimmed());
        btn->setStyleSheet(
            "QPushButton {"
            "   color: #424242;"
            "   text-align: center;"
            "   font-size: 14px;"
            "   font-weight: 500;"
            "   padding: 12px 8px;"
            "   margin: 2px;"
            "   border: none;"
            "   border-radius: 4px;"
            "   background-color: transparent;"
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
        } else if (item.first.contains("To Do List")) {
            todoListBtn = btn;  // 保存To Do List按钮指针
            connect(btn, &QPushButton::clicked, this, &MainWindow::showTodoList);
        }
    }

    navLayout->addStretch();

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




    navLayout->addStretch();


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




void MainWindow::showTodoList()
{
    if (!todoListWindow) {
        todoListWindow = new TodoListWindow(this);
    }
    todoListWindow->show();
}

void MainWindow::showUserInfoDialog(const QPoint &pos)
{
    if (!userInfoDialog) {
        userInfoDialog = new UserInfoDialog(this);
    }

    // 直接使用鼠标点击位置
    QPoint globalPos = avatar->mapToGlobal(pos);
    userInfoDialog->move(globalPos.x(), globalPos.y());
    userInfoDialog->show();
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

        // 处理头像点击事件
        if (obj == avatar) {
            showUserInfoDialog(mouseEvent->pos());
            return true;
        }

        // 处理点击窗口外部关闭弹窗
        if (userInfoDialog && userInfoDialog->isVisible()) {
            QPoint dialogPos = userInfoDialog->mapFromGlobal(globalPos);
            if (!userInfoDialog->rect().contains(dialogPos)) {
                userInfoDialog->hide();
            }
        }

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

