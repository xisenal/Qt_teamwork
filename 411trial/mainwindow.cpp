// mainwindow.cpp
#include "mainwindow.h"
#include "usermanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>
#include <QFormLayout>
#include <QRandomGenerator>

UserInfoDialog::UserInfoDialog(const QString &email, QWidget *parent)
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

    // 获取用户信息
    QString avatarPath, schoolId, description;
    UserManager userManager;
    userManager.getUserDetailInfo(email, avatarPath, schoolId, description);

    // 头像
    QLabel *avatarLabel = new QLabel(contentWidget);
    QPixmap avatarPixmap(avatarPath);
    if (avatarPixmap.isNull()) {
        avatarPixmap = QPixmap(":/resources/user.png");
    }
    avatarPixmap = avatarPixmap.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatarLabel->setPixmap(avatarPixmap);
    avatarLabel->setFixedSize(60, 60);
    avatarLabel->setAlignment(Qt::AlignCenter);

    // 信息布局
    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(5);
    infoLayout->setAlignment(Qt::AlignVCenter);

    QLabel *nameLabel = new QLabel(description, contentWidget);
    nameLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #333;");
    nameLabel->setAlignment(Qt::AlignCenter);
    QLabel *idLabel = new QLabel(QString("学号：%1").arg(schoolId), contentWidget);
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


//516
void MainWindow::switchToHomePage(bool forceShow) {
    // 如果已经在主页且不需要强制刷新，直接返回
    if(contentStack->currentIndex() == 1 && !forceShow) return;

    // 设置按钮状态
    homeBtn->setChecked(true);
    homeBtn->setStyleSheet(getButtonActiveStyle());

    // 切换页面
    contentStack->setCurrentIndex(1);

    // 取消其他按钮状态
    foreach(QPushButton* btn, findChildren<QPushButton*>()) {
        if(btn != homeBtn) btn->setChecked(false);
    }
}

// 获取激活按钮样式
// QString MainWindow::getButtonActiveStyle() {
//     return
//         "QPushButton {"
//         "   color: #1976D2;"
//         "   background-color: #E3F2FD;"
//         "   border-left: 3px solid #1976D2;"
//         "}";
// }






// 项目卡片组件
QWidget* MainWindow::createProjectCard(const QString &title, const QString &desc)
{
    QWidget *card = new QWidget;
    card->setStyleSheet(R"(
        QWidget {
            background: white;
            border-radius: 8px;
            padding: 16px;
            border: 1px solid #e1e4e8;
        }
        QLabel#title {
            font-size: 16px;
            font-weight: 600;
            color: #24292e;
        }
        QLabel#desc {
            color: #586069;
            font-size: 14px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(card);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setObjectName("title");

    QLabel *descLabel = new QLabel(desc);
    descLabel->setObjectName("desc");
    descLabel->setWordWrap(true);

    layout->addWidget(titleLabel);
    layout->addWidget(descLabel);
    layout->addStretch();

    // 添加悬停效果
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0,0,0,15));
    shadow->setOffset(2, 2);
    card->setGraphicsEffect(shadow);

    return card;
}


QWidget* MainWindow::createProjectsPanel()
{
    QWidget *panel = new QWidget;
    QGridLayout *grid = new QGridLayout(panel);
    grid->setSpacing(16);
    grid->setContentsMargins(8, 8, 8, 8);

    // 项目数据示例
    QList<QPair<QString, QString>> projects = {
        {"魔兽世界编程宝典", "我宣布已经没有人类了"},
        {"QT destructor", "基于Qt的可视化工具，真正的QT CREATOR（doge)"},
        {"AI助手", "(实则人机）"},
        {"xk养成计划","老师菜菜捞捞呜呜"}
    };

    int row = 0, col = 0;
    foreach (const auto &project, projects) {
        QWidget *card = createProjectCard(project.first, project.second);
        grid->addWidget(card, row, col);

        if(++col > 1) {
            col = 0;
            row++;
        }
    }

    return panel;
}


QWidget* MainWindow::createLabsPanel()
{
    // 主容器
    QWidget *panel = new QWidget;
    panel->setStyleSheet("background: white;");
    //背景改成白色

    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(panel);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(12);

    // 标题栏
    QWidget *titleBar = new QWidget;
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *titleLabel = new QLabel("🧪 实验室项目");
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 18px;
            font-weight: 600;
            color: #24292e;
            padding-left: 8px;
        }
    )");

    QPushButton *expandButton = new QPushButton("全部展开");
    expandButton->setStyleSheet(R"(
        QPushButton {
            color: #2188ff;
            border: 1px solid #e1e4e8;
            border-radius: 6px;
            padding: 4px 12px;
            font-size: 12px;
        }
        QPushButton:hover {
            background: #f6f8fa;
        }
    )");

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(expandButton);

    // 实验室项目网格
    QWidget *gridContainer = new QWidget;
    QGridLayout *gridLayout = new QGridLayout(gridContainer);
    gridLayout->setSpacing(16);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // 实验室项目数据
    QList<QPair<QString, QPair<QString, QString>>> labs = {
        {"量子计算", {":/resources/qpu.png", "量子计算模拟器开发框架"}},
        {"AI助手", {":/resources/ai.png", "基于深度学习的交互式助手"}},
        {"数据沙盒", {":/resources/sandbox.png", "安全数据实验环境"}},
        {"3D视觉可视化", {":/resources/3d.png", "实时三维数据渲染引擎"}}
    };

    // 创建卡片
    int row = 0, col = 0;
    foreach (const auto &lab, labs) {
        QWidget *card = createLabCard(lab.first, lab.second.first, lab.second.second);
        gridLayout->addWidget(card, row, col);

        if(++col > 1) {
            col = 0;
            row++;
        }
    }

    // 滚动区域
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidget(gridContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("QScrollArea { border: none; background: white; }");

    // 组装布局
    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(scrollArea);

    // 连接信号
    connect(expandButton, &QPushButton::clicked, [gridContainer](){
        gridContainer->setMinimumHeight(gridContainer->sizeHint().height());
    });

    return panel;
}

QWidget* MainWindow::createLabCard(const QString &title,
                                   const QString &iconPath,
                                   const QString &description)
{
    QWidget *card = new QWidget;
    card->setStyleSheet(R"(
        QWidget {
            background: white;
            border-radius: 8px;
            border: 1px solid #e1e4e8;
            padding: 16px;
        }
        QLabel#title {
            font-size: 16px;
            font-weight: 600;
            color: #24292e;
        }
        QLabel#desc {
            color: #586069;
            font-size: 14px;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);

    // 图标行
    QWidget *iconRow = new QWidget;
    QHBoxLayout *iconLayout = new QHBoxLayout(iconRow);
    iconLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *iconLabel = new QLabel;
    QPixmap pix(iconPath);
    if(pix.isNull()) {
        pix = QPixmap(":/resources/default_lab.png").scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else {
        pix = pix.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    iconLabel->setPixmap(pix);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setObjectName("title");

    iconLayout->addWidget(iconLabel);
    iconLayout->addWidget(titleLabel);
    iconLayout->addStretch();

    // 描述文本
    QLabel *descLabel = new QLabel(description);
    descLabel->setObjectName("desc");
    descLabel->setWordWrap(true);

    // 状态栏
    QWidget *statusBar = new QWidget;
    QHBoxLayout *statusLayout = new QHBoxLayout(statusBar);

    QLabel *statusLabel = new QLabel("🟢 实验中");
    statusLabel->setStyleSheet("color: #2cbe4e; font-size: 12px;");

    QPushButton *detailButton = new QPushButton("查看详情 →");
    detailButton->setStyleSheet(R"(
        QPushButton {
            color: #2188ff;
            border: none;
            padding: 4px;
            font-size: 12px;
        }
        QPushButton:hover {
            text-decoration: underline;
        }
    )");

    statusLayout->addWidget(statusLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(detailButton);

    // 组装卡片
    layout->addWidget(iconRow);
    layout->addWidget(descLabel);
    layout->addWidget(statusBar);

    // 悬停动画
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0,0,0,15));
    shadow->setOffset(2, 2);
    card->setGraphicsEffect(shadow);

    // 点击事件
    connect(detailButton, &QPushButton::clicked, [this, title](){
        QMessageBox::information(this, "项目详情",
                                 QString("即将展示【%1】的详细信息...").arg(title));
    });

    return card;
}



// 时间线条目组件
QWidget* MainWindow::createTimelineItem(const QString &date, const QString &text)
{
    QWidget *item = new QWidget;
    item->setStyleSheet("margin: 8px 0;");

    QHBoxLayout *layout = new QHBoxLayout(item);

    // 时间轴
    QWidget *timelineAxis = new QWidget;
    timelineAxis->setFixedWidth(30);
    timelineAxis->setStyleSheet(R"(
        QWidget {
            border-right: 2px solid #e1e4e8;
            position: relative;
        }
    )");

    // 时间点标记
    QLabel *dot = new QLabel(timelineAxis);
    dot->setPixmap(QPixmap(":/resources/circle.png").scaled(12,12));
    dot->move(9, 5);

    // 内容区域
    QLabel *content = new QLabel(
        QString("<b style='color:#24292e;'>%1</b>"
                "<div style='color:#586069; margin-top:4px;'>%2</div>")
            .arg(date, text)
        );
    content->setWordWrap(true);

    layout->addWidget(timelineAxis);
    layout->addWidget(content);

    return item;
}

// 活动时间线组件
QWidget* MainWindow::createActivityTimeline()
{
    QWidget *timeline = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(timeline);

    // 示例活动数据
    QList<QPair<QString, QString>> activities = {
        {"2025-04-11", "创建项目仓库 QtLab"},
        {"2025-05-16", "完成首页UI原型设计"},
        {"2024-05-17", "X-lab v 0.0.0震撼发布"}
    };

    foreach (const auto &activity, activities) {
        layout->addWidget(createTimelineItem(activity.first, activity.second));
    }

    return timeline;
}

QWidget*   MainWindow::createProfileCard()
{
    QWidget *profileCard = new QWidget;
    profileCard->setStyleSheet(R"(
        QWidget {
            background: white;
            border-radius: 8px;
            padding: 16px;
            border: 1px solid #e1e4e8;
        }
        QLabel#name {
            font-size: 20px;
            font-weight: 600;
            color: #24292e;
        }
        QLabel#title {
            color: #586069;
            font-size: 14px;
            margin-top: 4px;
        }
        QLabel#bio {
            color: #586069;
            font-size: 14px;
            margin-top: 8px;
        }
    )");

    // 主布局（水平排列）
    QHBoxLayout *mainLayout = new QHBoxLayout(profileCard);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(16);

    // ========== 左侧区域（头像+简介） ==========
    QWidget *leftPanel = new QWidget;
    QHBoxLayout *leftLayout = new QHBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(16);



    // 外层容器
    QWidget *avatarContainer = new QWidget;
    avatarContainer->setFixedSize(150, 240); // 整体尺寸
    avatarContainer->setStyleSheet(R"(
    QWidget {
        background: white;
        border-radius: 8px;
        border: 1px solid #e1e4e8;
    }
)");

    // 使用垂直布局管理头像和状态信息
    QVBoxLayout *promainLayout = new QVBoxLayout(avatarContainer);
    promainLayout->setContentsMargins(5, 5, 5, 5);
    promainLayout->setSpacing(6);
    //edited

    // 上半部分 - 放大头像
    QLabel *avatar = new QLabel;
    QPixmap pix(":/resources/myavatar.png");
    avatar->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    avatar->setStyleSheet(R"(
    QLabel {
        border-radius: 60px;
        border: 2px solid #e1e4e8;
        background-color: transparent;  // 新增
        overflow: hidden;        // 新增
    }
)");
    avatar->setAlignment(Qt::AlignCenter);
    // 调整容器内边距（原10,10,10,10）

    promainLayout->addWidget(avatar);
    promainLayout->setContentsMargins(12, 16, 12, 16); // 增加上下边距
    promainLayout->setSpacing(16);  // 原12 → 增加垂直间距


    // 下半部分 - 信息区域
    QVBoxLayout *proinfoLayout = new QVBoxLayout;
    proinfoLayout->setSpacing(2);
    //8

    // 地区显示
    QLabel *region = new QLabel("中国 · 北京");
    region->setAlignment(Qt::AlignCenter);
    region->setStyleSheet("font-size: 12px; color: #666;");
    proinfoLayout->addWidget(region);

    // 状态显示（仿微信样式）
    QHBoxLayout *statusLayout = new QHBoxLayout;
    statusLayout->setContentsMargins(0, 0, 0, 0);
    statusLayout->setSpacing(2);
    //6

    QLabel *statusIcon = new QLabel;
    statusIcon->setFixedSize(10, 8);
    statusIcon->setStyleSheet(R"(
    background-color: #7fff00;
    border-radius: 4px;
)");

    QLabel *statusText = new QLabel("摸鱼中");
    statusText->setStyleSheet("font-size: 12px; color: #666;");

    statusLayout->addStretch();
    statusLayout->addWidget(statusIcon);
    statusLayout->addWidget(statusText);
    statusLayout->addStretch();

    proinfoLayout->addLayout(statusLayout);
    promainLayout->addLayout(proinfoLayout);

    // // 头像
    // QLabel *avatar = new QLabel;
    // QPixmap pix(":/resources/myavatar.png"); // 替换为实际路径
    // avatar->setPixmap(pix.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // avatar->setStyleSheet(R"(
    //     border-radius: 40px;
    //     border: 2px solid #e1e4e8;
    // )");

    // 个人信息
    QWidget *info = new QWidget;
    QVBoxLayout *infoLayout = new QVBoxLayout(info);

    QLabel *name = new QLabel("Jamming Y");
    name->setObjectName("name");

    QLabel *title = new QLabel("FW Qt Developer");
    title->setObjectName("title");

    QLabel *bio = new QLabel("Open source contributor | Working on AI projects  (really？");
    bio->setObjectName("bio");
    bio->setWordWrap(true);

    infoLayout->addWidget(name);
    infoLayout->addWidget(title);
    infoLayout->addWidget(bio);
    infoLayout->addStretch();

    leftLayout->addWidget(avatarContainer);
    leftLayout->addWidget(info);

    // ========== 右侧日历区域 ==========
    QWidget *calendarWidget = new QWidget;
    calendarWidget->setStyleSheet(R"(
        QWidget {
            background: #f6f8fa;
            border-radius: 6px;
            padding: 12px;
        }
        QLabel#month {
            font-size: 16px;
            font-weight: 500;
            color: #24292e;
        }
        QLabel.day {
            font-size: 12px;
            padding: 4px;
            min-width: 24px;
            text-align: center;
        }
        QLabel.active {
            background: #2188ff;
            color: white;
            border-radius: 12px;
        }
    )");

    QVBoxLayout *calendarLayout = new QVBoxLayout(calendarWidget);

    // 日历头部
    QWidget *calendarHeader = new QWidget;
    QHBoxLayout *headerLayout = new QHBoxLayout(calendarHeader);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *prevBtn = new QPushButton("←");
    QLabel *monthLabel = new QLabel("May 2025");
    monthLabel->setObjectName("month");
    QPushButton *nextBtn = new QPushButton("→");

    // 设置按钮样式
    QString btnStyle = "QPushButton { border: none; padding: 4px; color: #586069; }"
                       "QPushButton:hover { background: #e1e4e8; }";
    prevBtn->setStyleSheet(btnStyle);
    nextBtn->setStyleSheet(btnStyle);

    headerLayout->addWidget(prevBtn);
    headerLayout->addWidget(monthLabel);
    headerLayout->addWidget(nextBtn);

    // 星期标题
    QWidget *weekdays = new QWidget;
    QHBoxLayout *weekLayout = new QHBoxLayout(weekdays);
    weekLayout->setContentsMargins(0, 0, 0, 0);
    weekLayout->setSpacing(0);

    QStringList days = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    foreach (const QString &day, days) {
        QLabel *label = new QLabel(day);
        label->setStyleSheet("color: #586069; font-size: 11px;");
        weekLayout->addWidget(label);
    }

    // 日期网格
    QWidget *gridWidget = new QWidget;
    QGridLayout *gridLayout = new QGridLayout(gridWidget);
    gridLayout->setSpacing(4);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // 生成日期（示例数据）
    int dayCount = 1;

    for (int row = 0; row < 5; ++row) {
        if(row == 0){
            for (int col = 4; col < 7; ++col) {
                QLabel *dayLabel = new QLabel(QString::number(dayCount));
                dayLabel->setAlignment(Qt::AlignCenter);

                // 随机生成活跃日期示例
                bool isActive = QRandomGenerator::global()->bounded(5) == 0;
                if (isActive) {
                    dayLabel->setProperty("class", "active");
                }

                dayLabel->setStyleSheet("QLabel { color: #24292e; }"
                                        "QLabel.active { background: #2188ff; color: white; }");

                gridLayout->addWidget(dayLabel, row, col);
                dayCount++;
            }
        }
        else{
        for (int col = 0; col < 7; ++col) {
            QLabel *dayLabel = new QLabel(QString::number(dayCount));
            dayLabel->setAlignment(Qt::AlignCenter);

            // 随机生成活跃日期示例
            bool isActive = QRandomGenerator::global()->bounded(5) == 0;
            if (isActive) {
                dayLabel->setProperty("class", "active");
            }

            dayLabel->setStyleSheet("QLabel { color: #24292e; }"
                                    "QLabel.active { background: #2188ff; color: white; }");

            gridLayout->addWidget(dayLabel, row, col);
            dayCount++;
            if (dayCount > 31) break;
        }
        }
    }

    // 组装日历
    calendarLayout->addWidget(calendarHeader);
    calendarLayout->addWidget(weekdays);
    calendarLayout->addWidget(gridWidget);

    // ========== 组合左右布局 ==========
    mainLayout->addWidget(leftPanel, 1); // 左侧自适应
    mainLayout->addWidget(calendarWidget); // 右侧固定宽度

    return profileCard;
}


QString MainWindow::getButtonActiveStyle() {
    QWidget *page = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(page);

    // 顶部个人信息卡片
    // QWidget *profileCard = new QWidget;
    // profileCard->setStyleSheet("background: white; border-radius: 8px; padding: 16px;");



    // // ...添加具体个人信息内容...
    QWidget *profileCard = createProfileCard();

    // 使用QScrollArea确保内容可滚动
    QScrollArea *scrollArea = new QScrollArea;
    QWidget *scrollContent = new QWidget;
    QVBoxLayout *contentLayout = new QVBoxLayout(scrollContent);

    // 添加项目面板
    contentLayout->addWidget(createProjectsPanel());

    // 添加实验室面板
    contentLayout->addWidget(createLabsPanel());

    // 添加活动时间线
    contentLayout->addWidget(createActivityTimeline());

    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(profileCard);
    mainLayout->addWidget(scrollArea);

    //return page;
    return
        "QPushButton {"
        "   color: #1976D2;"
        "   background-color: #E3F2FD;"
        "   border-left: 3px solid #1976D2;"
        "}";
}



QWidget* MainWindow::createGithubStylePage() {
    QWidget *page = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(page);

    // 顶部个人信息卡片
    // QWidget *profileCard = new QWidget;
    // profileCard->setStyleSheet("background: white; border-radius: 8px; padding: 16px;");
    // // ...添加具体个人信息内容...
    QWidget *profileCard = createProfileCard();

    // 使用QScrollArea确保内容可滚动
    QScrollArea *scrollArea = new QScrollArea;
    QWidget *scrollContent = new QWidget;
    QVBoxLayout *contentLayout = new QVBoxLayout(scrollContent);

    // 添加项目面板
    contentLayout->addWidget(createProjectsPanel());

    // 添加实验室面板
    contentLayout->addWidget(createLabsPanel());

    // 添加活动时间线
    contentLayout->addWidget(createActivityTimeline());

    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(profileCard);
    mainLayout->addWidget(scrollArea);

    return page;
}

// 新增两个功能函数
void MainWindow::showHomePage()
{
    githubHomePage->show();
    resetButtonStates(homeBtn);
}

void MainWindow::hideHomePage()
{
    githubHomePage->hide();
    resetButtonStates(nullptr); // 取消所有选中
}





// void MainWindow::updateButtonState(QPushButton *activeBtn)
// {
//     // 遍历所有导航按钮
//     QList<QPushButton*> navButtons = {
//         homeBtn,
//         labBtn,
//         // 添加其他按钮...
//     };

//     foreach(QPushButton* btn, navButtons) {
//         if(btn && btn != activeBtn) {
//             btn->setChecked(false);    // 取消其他按钮选中
//             btn->setStyleSheet(normalButtonStyle);
//         }
//     }

//     // 强制激活当前按钮
//     activeBtn->setChecked(true);
//     activeBtn->setStyleSheet(activeButtonStyle);

//     // 确保页面可见性
//     contentStack->currentWidget()->show();
// }


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置无边框窗口
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("background: white;");

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



    // 修改内容区域为堆叠布局
    contentStack = new QStackedWidget(this);

    // 创建GitHub风格主页
    githubHomePage = createGithubStylePage();

//     // 保留原有内容区域作为默认页
//     defaultContent = new QTextEdit();
//     defaultContent->setStyleSheet(R"(
//     QTextEdit {
//         background: white;
//         border: none;
//         font-size: 14px;
//         padding: 24px;
//         color: #212121;
//     }
// )");

//     //可能可以设置背景（
//     //defaultContent->setStyleSheet(/* 原有样式保持不变 */);
//     // 方式2：设置中央部件样式


    QLabel *welcomeLabel = new QLabel();
    QMovie *welcomeMovie = new QMovie(":/animations/welcome_ed.gif");  // 资源需添加到.qrc文件
    welcomeLabel->setMovie(welcomeMovie);
    welcomeMovie->start();

    // 设置控件样式（替换原有QTextEdit样式）
    welcomeLabel->setStyleSheet(R"(
    QLabel {
        background: #1a1a1a;  /* 深色背景 */
        qproperty-alignment: AlignCenter;  /* 内容居中 */
    }
    )");


//     // 将页面添加到堆栈
//     contentStack->addWidget(defaultContent);  // 索引0
    contentStack->addWidget(welcomeLabel);  // 索引0
    contentStack->addWidget(githubHomePage);   // 索引1





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



    // // 导航按钮配置
    QVBoxLayout *navLayout = new QVBoxLayout(sidebar);

    // 修改布局连接
    // 替换原有contentArea
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
        "   background-color: white;"
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
    //contentLayout->addWidget(userInfoBox);

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
            "   background-color: white;"
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
        //contentLayout->addWidget(btn);

        btn->setCheckable(true);  // 新增：启用可检查状态

        if (item.first.contains("实验室")) {
            labBtn = btn;  // 保存实验室按钮的指针
            // hideHomePage();
            connect(labBtn, &QPushButton::clicked, [this](){
                canvasPage = new canvas();
                contentStack->addWidget(canvasPage);
                contentStack->setCurrentWidget(canvasPage);
                // contentStack->setCurrentWidget(defaultContent);
            //     //resetButtonStates(labBtn); // 更新按钮状态
            //     //updateButtonState(labBtn);
            });
            //contentStack->setCurrentWidget(defaultContent);

        }

        if (item.first.contains("主页")) {
            homeBtn = btn;  // 保存主页按钮指针
            connect(homeBtn, &QPushButton::clicked, [this](){
                //switchToHomePage(true);  // 强制刷新
                // showHomePage();
                contentStack->setCurrentWidget(githubHomePage);
                //resetButtonStates(homeBtn);
                //updateButtonState(homeBtn);
            });

            //
            //connect(homeBtn, &QPushButton::clicked, this, &MainWindow::createGithubStylePage());

        }

        else if (item.first.contains("To Do List")) {
            todoListBtn = btn;  // 保存To Do List按钮指针
            connect(btn, &QPushButton::clicked, this, &MainWindow::showTodoList);
        }
    }

    navLayout->addStretch();
    //contentLayout->addStretch();

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

    // 创建StackedWidget作为中央部件
    stackedWidget = new QStackedWidget(this);
    // setCentralWidget(stackedWidget);

    // // 创建页面容器
    QWidget *mainPage = new QWidget();
    stackedWidget->addWidget(mainPage); // 必须添加为页面

    // // 在页面上创建布局
    // QVBoxLayout *mainVLayout = new QVBoxLayout(mainPage); // ✅ 正确绑定到页面
    mainVLayout->setContentsMargins(0, 0, 0, 0);
    mainVLayout->setSpacing(0);
    mainVLayout->addWidget(titleBar);
    
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    contentLayout->addWidget(sidebar);
    //contentLayout->addWidget(contentArea, 1);

    contentLayout->addWidget(contentStack, 1);
    
    mainVLayout->addLayout(contentLayout, 1);




    navLayout->addStretch();


    createSubMenu();  // 初始化 subMenu
    // 在构造函数末尾添加连接
    connect(labBtn, &QPushButton::clicked, this, &MainWindow::toggleLabSubMenu);

    // connect(labBtn, &QPushButton::clicked, this, &MainWindow::handleLabButtonClick);

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

void MainWindow::setCurrentUser(const QString &email)
{
    currentUserEmail = email;
    
    // 获取用户信息
    QString avatarPath, schoolId, description;
    UserManager userManager;
    if (userManager.getUserDetailInfo(email, avatarPath, schoolId, description)) {
        // 更新侧边栏头像
        QPixmap avatarPixmap(avatarPath);
        if (avatarPixmap.isNull()) {
            avatarPixmap = QPixmap(":/resources/user.png");
        }
        avatarPixmap = avatarPixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        avatar->setPixmap(avatarPixmap);
    }
}

void MainWindow::showUserInfoDialog(const QPoint &pos)
{
    if (!userInfoDialog) {
        userInfoDialog = new UserInfoDialog(currentUserEmail, this);
    }

    // 直接使用鼠标点击位置
    QPoint globalPos = avatar->mapToGlobal(pos);
    userInfoDialog->move(globalPos.x(), globalPos.y());
    userInfoDialog->show();
}

// bool MainWindow::eventFilter(QObject *obj, QEvent *event)
// {
//     if (event->type() == QEvent::MouseButtonPress)
//     {
//         QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
//         if (!mouseEvent) return QMainWindow::eventFilter(obj, event);

//         // 获取点击的全局坐标
//         QPoint globalPos;
// #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//         globalPos = mouseEvent->globalPos();
// #else
//         globalPos = mouseEvent->globalPosition().toPoint();
// #endif

//         // 处理头像点击事件
//         if (obj == avatar) {
//             showUserInfoDialog(mouseEvent->pos());
//             return true;
//         }

//         // 处理点击窗口外部关闭弹窗
//         if (userInfoDialog && userInfoDialog->isVisible()) {
//             QPoint dialogPos = userInfoDialog->mapFromGlobal(globalPos);
//             if (!userInfoDialog->rect().contains(dialogPos)) {
//                 userInfoDialog->hide();
//             }
//         }

//         // 如果子菜单可见且点击的是主页按钮
//         if (subMenu && subMenu->isVisible())
//         {
//             // 转换坐标到主页按钮坐标系
//             QPoint homeBtnPos = homeBtn->mapFromGlobal(globalPos);

//             // 判断是否点击在主页按钮区域内
//             if (homeBtn->rect().contains(homeBtnPos))
//             {
//                 subMenu->hide();
//                 isSubMenuVisible = false;
//             }
//         }
//     }
//     return QMainWindow::eventFilter(obj, event);
// }

// QString MainWindow::getActiveButtonStyle()
// {
//     return R"(
//         QPushButton {
//             color: #1976D2;
//             background-color: #E3F2FD;
//             border-left: 3px solid #1976D2;
//         }
//     )";
// }

// QString MainWindow::getNormalButtonStyle()
// {
//     return R"(
//         QPushButton {
//             color: #424242;
//             background-color: transparent;
//             border: none;
//         }
//         QPushButton:hover {
//             background-color: rgba(0, 0, 0, 0.04);
//         }
//     )";
// }

// void MainWindow::resetButtonStates(QPushButton *activeButton)
// {
//     // 遍历侧边栏所有按钮
//     foreach(QPushButton* btn, sidebar->findChildren<QPushButton*>()) {
//         if(btn == activeButton) {
//             btn->setChecked(true);
//             btn->setStyleSheet(getActiveButtonStyle());
//         } else {
//             btn->setChecked(false);
//             btn->setStyleSheet(getNormalButtonStyle());
//         }
//     }
// }

// void MainWindow::resetButtonStates(QPushButton *activeBtn)
// {
//     QList<QPushButton*> buttons = {
//         homeBtn, labBtn, /* 其他按钮... */
//     };

//     foreach(QPushButton* btn, buttons) {
//         if(btn && btn != activeBtn) {
//             btn->setChecked(false);
//             btn->setStyleSheet(getNormalButtonStyle());
//         }
//     }

//     if(activeBtn) {
//         activeBtn->setChecked(true);
//         activeBtn->setStyleSheet(getActiveButtonStyle());
//     }
// }

void MainWindow::resetButtonStates(QPushButton *activeBtn)
{
    QList<QPushButton*> buttons = {homeBtn, labBtn};
    foreach(QPushButton* btn, buttons) {
        if(btn == activeBtn) {
            btn->setChecked(true);
            btn->setStyleSheet(
                "QPushButton {"
                "   color: #1976D2;"
                "   background-color: #E3F2FD;"
                "   border-left: 3px solid #1976D2;"
                "}"
                );
        } else {
            btn->setChecked(false);
            btn->setStyleSheet(
                "QPushButton {"
                "   color: #424242;"
                "   background-color: white;"
                "   border: none;"
                "}"
                );
        }
    }
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        // 仅处理头像点击
        if (obj == avatar) {
            showUserInfoDialog(mouseEvent->pos());
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}


// bool MainWindow::eventFilter(QObject *obj, QEvent *event)
// {
//     if (event->type() == QEvent::MouseButtonPress)
//     {
//         QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
//         if (!mouseEvent) return QMainWindow::eventFilter(obj, event);

//         // 获取点击的全局坐标
//         QPoint globalPos;
// #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
//         globalPos = mouseEvent->globalPos();
// #else
//         globalPos = mouseEvent->globalPosition().toPoint();
// #endif

//         // 新增：处理首页按钮点击
//         if (obj == homeBtn) {
//             // 切换到GitHub风格主页
//             contentStack->setCurrentWidget(githubHomePage);

//             // 更新按钮状态
//             //resetButtonStates(homeBtn);


//             if (subMenu && subMenu->isVisible()) {
//                 //githubHomePage->hide();
//                 contentStack->setCurrentIndex(0);
//                 contentStack->setCurrentWidget(defaultContent);
//                 isSubMenuVisible = true;
//                 subMenu->show();
//             }
//             QPoint labBtnPos = labBtn->mapFromGlobal(globalPos);
//             if (homeBtn->rect().contains(labBtnPos)) {
//                 subMenu->hide();
//                 isSubMenuVisible = false;
//             }

//             return true; // 事件已处理
//         }

//         // --- 保留原有逻辑 ---
//         // 处理头像点击事件
//         if (obj == avatar) {
//             showUserInfoDialog(mouseEvent->pos());
//             return true;
//         }

//         // 处理外部点击关闭弹窗
//         if (userInfoDialog && userInfoDialog->isVisible()) {
//             QPoint dialogPos = userInfoDialog->mapFromGlobal(globalPos);
//             if (!userInfoDialog->rect().contains(dialogPos)) {
//                 userInfoDialog->hide();
//             }
//         }

//         // 处理子菜单点击
//         if (subMenu && subMenu->isVisible()) {
//             QPoint homeBtnPos = homeBtn->mapFromGlobal(globalPos);
//             if (homeBtn->rect().contains(homeBtnPos)) {
//                 subMenu->hide();
//                 isSubMenuVisible = false;
//                 contentStack->setCurrentWidget(githubHomePage);
//             }
//         }
//     }
//     return QMainWindow::eventFilter(obj, event);
// }


void MainWindow::handleLabButtonClick()
{
    // stackedWidget = new QStackedWidget(this);
    qDebug()<<"AAA";
    qDebug() << "stackedWidget地址:" << stackedWidget;
    if (!stackedWidget->widget(1)) { // 首次访问时创建
        qDebug()<<"BBB";
        canvasPage = new canvas();
        stackedWidget->addWidget(canvasPage);
        stackedWidget->setCurrentWidget(canvasPage);
        // int index = stackedWidget->addWidget(canvasPage);
        // qDebug() << "New widget added at index:" << index;
    }
    qDebug()<<"CCC";
    // stackedWidget->setCurrentIndex(1);
}


