// //checkinwidget::checkinwidget() {}

// // checkinwidget.cpp
// #include "checkinwidget.h"
// #include <QMessageBox>
// #include <QPalette>
// #include <QGraphicsDropShadowEffect>

// CheckInWidget::CheckInWidget(QWidget *parent)
//     : QWidget(parent)
//     , m_settings(new QSettings("MyApp", "CheckIn", this))
//     , m_checkInStreak(0)
// {
//     setupUI();
//     loadSettings();
//     updateUI();

//     // 初始化签运和名言数组
//     m_luckList << "大吉" << "中吉" << "小吉" << "吉" << "半吉" << "末吉" << "末小吉"
//                << "凶" << "小凶" << "半凶" << "末凶" << "大凶" << "中凶";

//     m_quoteList << "今天是充满passion的一天！"
//                 << "今天会是打败魔兽的一天！！！"
//                 << "每一次努力都不会白费。"
//                 << "保持微笑，好运自然来。"
//                 << "Partido a Partido"
//                 << "机会总是留给有准备的人。"
//                 << "坚持下去，胜利就在前方。"
//                 << "用心感受生活的美好。"
//                 << ""
//                 << "困难只是成功路上的垫脚石。"
//                 << "保持初心，方得始终。"
//                 << "今天的努力是明天的收获。"
//                 << "相信过程，享受当下。"
//                 << "每个挑战都是成长的机会。"
//                 << "C++还是python,难以逃脱的qwq";
// }

// CheckInWidget::~CheckInWidget()
// {
//     saveSettings();
// }

// void CheckInWidget::setupUI()
// {
//     setFixedSize(400, 300);
//     setWindowTitle("每日签到");

//     // 主布局
//     QVBoxLayout *mainLayout = new QVBoxLayout(this);
//     mainLayout->setSpacing(15);
//     mainLayout->setContentsMargins(20, 20, 20, 20);

//     // 标题
//     QLabel *titleLabel = new QLabel("每日签到");
//     titleLabel->setAlignment(Qt::AlignCenter);
//     QFont titleFont = titleLabel->font();
//     titleFont.setPointSize(16);
//     titleFont.setBold(true);
//     titleLabel->setFont(titleFont);
//     titleLabel->setStyleSheet("color: #2c3e50; margin-bottom: 10px;");

//     // 连续签到天数
//     m_streakLabel = new QLabel("连续签到：0 天");
//     m_streakLabel->setAlignment(Qt::AlignCenter);
//     m_streakLabel->setStyleSheet("color: #7f8c8d; font-size: 12px;");

//     // 签到按钮
//     m_checkInBtn = new QPushButton("签到");
//     m_checkInBtn->setFixedSize(120, 40);
//     m_checkInBtn->setStyleSheet(
//         "QPushButton {"
//         "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3498db, stop:1 #2980b9);"
//         "    border: none;"
//         "    color: white;"
//         "    font-size: 14px;"
//         "    font-weight: bold;"
//         "    border-radius: 20px;"
//         "}"
//         "QPushButton:hover {"
//         "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3cb0fd, stop:1 #3498db);"
//         "}"
//         "QPushButton:pressed {"
//         "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2980b9, stop:1 #21618c);"
//         "}"
//         "QPushButton:disabled {"
//         "    background: #bdc3c7;"
//         "    color: #7f8c8d;"
//         "}"
//         );

//     // 状态标签
//     m_statusLabel = new QLabel("");
//     m_statusLabel->setAlignment(Qt::AlignCenter);
//     m_statusLabel->setStyleSheet("color: #27ae60; font-weight: bold;");

//     // 结果框架
//     m_resultFrame = new QFrame();
//     m_resultFrame->setFrameStyle(QFrame::Box);
//     m_resultFrame->setStyleSheet(
//         "QFrame {"
//         "    background-color: #ecf0f1;"
//         "    border: 1px solid #bdc3c7;"
//         "    border-radius: 8px;"
//         "    padding: 10px;"
//         "}"
//         );
//     m_resultFrame->setVisible(false);

//     QVBoxLayout *resultLayout = new QVBoxLayout(m_resultFrame);

//     // 今日签运
//     m_luckLabel = new QLabel("");
//     m_luckLabel->setAlignment(Qt::AlignCenter);
//     m_luckLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #e74c3c; margin: 5px;");

//     // 每日一句
//     m_quoteLabel = new QTextEdit();
//     m_quoteLabel->setFixedHeight(60);
//     m_quoteLabel->setReadOnly(true);
//     m_quoteLabel->setStyleSheet(
//         "QTextEdit {"
//         "    border: none;"
//         "    background: transparent;"
//         "    color: #2c3e50;"
//         "    font-size: 12px;"
//         "    font-style: italic;"
//         "}"
//         );

//     resultLayout->addWidget(m_luckLabel);
//     resultLayout->addWidget(m_quoteLabel);

//     // 添加阴影效果
//     QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
//     shadowEffect->setBlurRadius(10);
//     shadowEffect->setColor(QColor(0, 0, 0, 80));
//     shadowEffect->setOffset(0, 2);
//     m_checkInBtn->setGraphicsEffect(shadowEffect);

//     // 透明度效果用于动画
//     m_opacityEffect = new QGraphicsOpacityEffect();
//     m_resultFrame->setGraphicsEffect(m_opacityEffect);

//     // 布局组装
//     QHBoxLayout *btnLayout = new QHBoxLayout();
//     btnLayout->addStretch();
//     btnLayout->addWidget(m_checkInBtn);
//     btnLayout->addStretch();

//     mainLayout->addWidget(titleLabel);
//     mainLayout->addWidget(m_streakLabel);
//     mainLayout->addLayout(btnLayout);
//     mainLayout->addWidget(m_statusLabel);
//     mainLayout->addWidget(m_resultFrame);
//     mainLayout->addStretch();

//     // 连接信号
//     connect(m_checkInBtn, &QPushButton::clicked, this, &CheckInWidget::onCheckInClicked);
// }

// void CheckInWidget::loadSettings()
// {
//     m_lastCheckInDate = m_settings->value("lastCheckInDate", QDate()).toDate();
//     m_checkInStreak = m_settings->value("checkInStreak", 0).toInt();
//     m_todayLuck = m_settings->value("todayLuck", "").toString();
//     m_todayQuote = m_settings->value("todayQuote", "").toString();
// }

// void CheckInWidget::saveSettings()
// {
//     m_settings->setValue("lastCheckInDate", m_lastCheckInDate);
//     m_settings->setValue("checkInStreak", m_checkInStreak);
//     m_settings->setValue("todayLuck", m_todayLuck);
//     m_settings->setValue("todayQuote", m_todayQuote);
// }

// bool CheckInWidget::canCheckInToday()
// {
//     QDate today = QDate::currentDate();
//     return m_lastCheckInDate != today;
// }

// QString CheckInWidget::getRandomLuck()
// {
//     int index = QRandomGenerator::global()->bounded(m_luckList.size());
//     return m_luckList[index];
// }

// QString CheckInWidget::getDailyQuote()
// {
//     // 使用日期作为种子，确保同一天得到相同的句子
//     QDate today = QDate::currentDate();
//     uint seed = today.toJulianDay();
//     QRandomGenerator generator(seed);
//     int index = generator.bounded(m_quoteList.size());
//     return m_quoteList[index];
// }

// void CheckInWidget::onCheckInClicked()
// {
//     if (!canCheckInToday()) {
//         QMessageBox::information(this, "提示", "今天已经签到过了！");
//         return;
//     }

//     // 更新签到数据
//     QDate today = QDate::currentDate();

//     // 检查是否连续签到
//     if (m_lastCheckInDate.isValid() && m_lastCheckInDate.addDays(1) == today) {
//         m_checkInStreak++;
//     } else {
//         m_checkInStreak = 1;
//     }

//     m_lastCheckInDate = today;
//     m_todayLuck = getRandomLuck();
//     m_todayQuote = getDailyQuote();

//     saveSettings();
//     playCheckInAnimation();
// }

// void CheckInWidget::playCheckInAnimation()
// {
//     // 禁用按钮
//     m_checkInBtn->setEnabled(false);
//     m_statusLabel->setText("签到中...");

//     // 延迟显示结果，增加仪式感
//     QTimer::singleShot(1000, this, &CheckInWidget::showCheckInResult);
// }

// void CheckInWidget::showCheckInResult()
// {
//     updateUI();

//     // 显示结果框架的淡入动画
//     m_resultFrame->setVisible(true);
//     m_fadeAnimation = new QPropertyAnimation(m_opacityEffect, "opacity");
//     m_fadeAnimation->setDuration(500);
//     m_fadeAnimation->setStartValue(0.0);
//     m_fadeAnimation->setEndValue(1.0);
//     m_fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);

//     m_statusLabel->setText("签到成功！");
// }

// void CheckInWidget::updateUI()
// {
//     // 更新连续签到天数
//     m_streakLabel->setText(QString("连续签到：%1 天").arg(m_checkInStreak));

//     // 更新按钮状态
//     if (canCheckInToday()) {
//         m_checkInBtn->setEnabled(true);
//         m_checkInBtn->setText("签到");
//         m_statusLabel->setText("");
//         m_resultFrame->setVisible(false);
//     } else {
//         m_checkInBtn->setEnabled(false);
//         m_checkInBtn->setText("已签到");
//         m_statusLabel->setText("今日已签到");

//         // 显示今日的签运和名言
//         m_luckLabel->setText(QString("今日签运：%1").arg(m_todayLuck));
//         m_quoteLabel->setText(m_todayQuote);
//         m_resultFrame->setVisible(true);
//         m_opacityEffect->setOpacity(1.0);
//     }
// }




#include "checkinwidget.h"
#include <QMessageBox>

CompactCheckInWidget::CompactCheckInWidget(QWidget *parent)
    : QWidget(parent)
    , m_settings(new QSettings("MyApp", "CheckIn", this))
    , m_checkInStreak(0)
{
    // setupUI();
    // loadSettings();
    // updateUI();

    // 初始化数据
    m_luckList << "大吉" << "中吉" << "小吉" << "吉" << "半吉" << "末吉"
               << "凶" << "小凶" << "大凶";

    m_quoteList << "今天是充满passion的一天！"
                << "今天会是打败魔兽的一天！！！"
                << "暂时不能给你明确的答复。"
                << "No pain, No gain."
                << "Partido a Partido"
                << "暂时不能给你明确的答复。"
                << "coding is a process to communicate with yourself."
                << "攻城狮加油捏"
                << "又在肝程设大作业吗，你这家伙"
                << "暂时不能给你明确的答复。"
                << "暂时不能给你明确的答复"
                << "今日状态：WA"
                << "暂时不能给你明确的答复"
                << "今天写指针一定不会报错qwq"
                << "C++还是python,难以逃脱的qwq";

    setupUI();
    loadSettings();

    // 如果今天已经签到过，但签运和名言为空，重新生成
    QDate today = QDate::currentDate();
    if (m_lastCheckInDate == today) {
        if (m_todayLuck.isEmpty()) {
            m_todayLuck = getRandomLuck();
        }
        if (m_todayQuote.isEmpty()) {
            m_todayQuote = getDailyQuote();
        }
        saveSettings(); // 保存生成的内容
    }

    updateUI();

}

CompactCheckInWidget::~CompactCheckInWidget()
{
    saveSettings();
}


void CompactCheckInWidget::setupUI()
{
    // 设置整体样式，与你的界面风格一致
    setStyleSheet(R"(
        QWidget {
            background: white;
            border-radius: 6px;
            border: 1px solid #e1e4e8;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2188ff, stop:1 #0366d6);
            border: none;
            color: white;
            font-size: 12px;
            font-weight: 500;
            border-radius: 4px;
            padding: 6px 12px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0366d6, stop:1 #044289);
        }
        QPushButton:disabled {
            background: #f1f3f4;
            color: #6a737d;
        }
        QLabel {
            color: #24292e;
            font-size: 12px;
        }
    )");

    // 主布局 - 调整边距和间距
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 8, 10, 8);  // 适中的边距
    mainLayout->setSpacing(3);                        // 适中的间距

    // 标题行
    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setSpacing(3);  // 标题行内间距

    QLabel *titleLabel = new QLabel("📅 每日签到");
    titleLabel->setStyleSheet("font-weight: 500; color: #24292e;");
    titleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    m_streakLabel = new QLabel("连续0天");
    m_streakLabel->setStyleSheet("color: #586069; font-size: 11px;");
    m_streakLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_streakLabel->setMinimumWidth(60);  // 确保有足够宽度显示文字

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(m_streakLabel);

    // 签到按钮行
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(8);

    m_checkInBtn = new QPushButton("签到");
    m_checkInBtn->setFixedSize(65, 30);  // 稍微调大一点
    m_checkInBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_statusLabel = new QLabel("");
    m_statusLabel->setStyleSheet("color: #28a745; font-size: 11px;");
    m_statusLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_statusLabel->setMinimumHeight(20);  // 确保有足够高度

    btnLayout->addWidget(m_checkInBtn);
    btnLayout->addWidget(m_statusLabel);
    btnLayout->addStretch();

    // 签运显示
    m_luckLabel = new QLabel("");
    m_luckLabel->setStyleSheet("color: #e36209; font-weight: 500; font-size: 11px;");
    m_luckLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_luckLabel->setMinimumHeight(20);  // 确保显示高度
    m_luckLabel->setWordWrap(true);

    // 每日一句
    m_quoteLabel = new QLabel("");
    m_quoteLabel->setStyleSheet("color: #586069; font-size: 10px; font-style: italic; line-height: 1.2;");
    m_quoteLabel->setWordWrap(true);
    m_quoteLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_quoteLabel->setMinimumHeight(20);  // 确保能显示两行文字
    m_quoteLabel->setMaximumHeight(48);  // 限制最大高度
    m_quoteLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);  // 顶部对齐

    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(m_luckLabel);
    mainLayout->addWidget(m_quoteLabel);

    // 设置整个组件的最小尺寸
    setMinimumSize(280, 130);
    setMaximumHeight(150);

    connect(m_checkInBtn, &QPushButton::clicked, this, &CompactCheckInWidget::onCheckInClicked);

// 调试信息 - 可以临时启用来检查布局
#ifdef QT_DEBUG
// setStyleSheet(styleSheet() + "QWidget { border: 1px solid red; }");  // 调试时显示边界
#endif
}

void CompactCheckInWidget::loadSettings()
{
    m_lastCheckInDate = m_settings->value("lastCheckInDate", QDate()).toDate();
    m_checkInStreak = m_settings->value("checkInStreak", 0).toInt();
    m_todayLuck = m_settings->value("todayLuck", "").toString();
    m_todayQuote = m_settings->value("todayQuote", "").toString();
}

void CompactCheckInWidget::saveSettings()
{
    m_settings->setValue("lastCheckInDate", m_lastCheckInDate);
    m_settings->setValue("checkInStreak", m_checkInStreak);
    m_settings->setValue("todayLuck", m_todayLuck);
    m_settings->setValue("todayQuote", m_todayQuote);
}

bool CompactCheckInWidget::canCheckInToday()
{
    QDate today = QDate::currentDate();
    return m_lastCheckInDate != today;
}

QString CompactCheckInWidget::getRandomLuck()
{
    int index = QRandomGenerator::global()->bounded(m_luckList.size());
    return m_luckList[index];
}

QString CompactCheckInWidget::getDailyQuote()
{
    QDate today = QDate::currentDate();
    uint seed = today.toJulianDay();
    QRandomGenerator generator(seed);
    int index = generator.bounded(m_quoteList.size());
    return m_quoteList[index];
}

void CompactCheckInWidget::onCheckInClicked()
{
    if (!canCheckInToday()) {
        return;
    }

    QDate today = QDate::currentDate();

    if (m_lastCheckInDate.isValid() && m_lastCheckInDate.addDays(1) == today) {
        m_checkInStreak++;
    } else {
        m_checkInStreak = 1;
    }

    m_lastCheckInDate = today;
    m_todayLuck = getRandomLuck();
    m_todayQuote = getDailyQuote();

    saveSettings();
    updateUI();
}

void CompactCheckInWidget::updateUI()
{
    m_streakLabel->setText(QString("连续%1天").arg(m_checkInStreak));

    if (canCheckInToday()) {
        m_checkInBtn->setEnabled(true);
        m_checkInBtn->setText("签到");
        m_statusLabel->setText("");
        m_luckLabel->setText("");
        m_quoteLabel->setText("");
    } else {
        m_checkInBtn->setEnabled(false);
        m_checkInBtn->setText("已签");
        m_statusLabel->setText("✓ 已完成");

        // 确保今日的签运和名言存在
        if (m_todayLuck.isEmpty()) {
            m_todayLuck = getRandomLuck();
        }
        if (m_todayQuote.isEmpty()) {
            m_todayQuote = getDailyQuote();
        }

        m_luckLabel->setText(QString("🎯 今日签运：%1").arg(m_todayLuck));
        m_quoteLabel->setText(QString("💭 %1").arg(m_todayQuote));
    }
}






// void CompactCheckInWidget::setupUI()
// {
//     // 设置整体样式，与你的界面风格一致
//     setStyleSheet(R"(
//         QWidget {
//             background: white;
//             border-radius: 6px;
//             border: 1px solid #e1e4e8;
//         }
//         QPushButton {
//             background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2188ff, stop:1 #0366d6);
//             border: none;
//             color: white;
//             font-size: 12px;
//             font-weight: 500;
//             border-radius: 4px;
//             padding: 6px 12px;
//         }
//         QPushButton:hover {
//             background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0366d6, stop:1 #044289);
//         }
//         QPushButton:disabled {
//             background: #f1f3f4;
//             color: #6a737d;
//         }
//         QLabel {
//             color: #24292e;
//             font-size: 12px;
//         }
//     )");

//     QVBoxLayout *mainLayout = new QVBoxLayout(this);
//     mainLayout->setContentsMargins(12, 8, 12, 8);
//     mainLayout->setSpacing(6);

//     // 标题行
//     QHBoxLayout *titleLayout = new QHBoxLayout();
//     QLabel *titleLabel = new QLabel("📅 每日签到");
//     titleLabel->setStyleSheet("font-weight: 500; color: #24292e;");

//     m_streakLabel = new QLabel("连续0天");
//     m_streakLabel->setStyleSheet("color: #586069; font-size: 6px;");

//     titleLayout->addWidget(titleLabel);
//     titleLayout->addStretch();
//     titleLayout->addWidget(m_streakLabel);

//     // 签到按钮行
//     QHBoxLayout *btnLayout = new QHBoxLayout();
//     m_checkInBtn = new QPushButton("签到");
//     m_checkInBtn->setFixedSize(60, 28);

//     m_statusLabel = new QLabel("");
//     m_statusLabel->setStyleSheet("color: #28a745; font-size: 11px;");

//     btnLayout->addWidget(m_checkInBtn);
//     btnLayout->addWidget(m_statusLabel);
//     btnLayout->addStretch();

//     // 签运显示
//     m_luckLabel = new QLabel("");
//     m_luckLabel->setStyleSheet("color: #e36209; font-weight: 500;");

//     // 每日一句
//     m_quoteLabel = new QLabel("");
//     m_quoteLabel->setStyleSheet("color: #586069; font-size: 11px; font-style: italic;");
//     m_quoteLabel->setWordWrap(true);
//     m_quoteLabel->setMaximumHeight(30);

//     mainLayout->addLayout(titleLayout);
//     mainLayout->addLayout(btnLayout);
//     mainLayout->addWidget(m_luckLabel);
//     mainLayout->addWidget(m_quoteLabel);

//     connect(m_checkInBtn, &QPushButton::clicked, this, &CompactCheckInWidget::onCheckInClicked);
// }

// void CompactCheckInWidget::loadSettings()
// {
//     m_lastCheckInDate = m_settings->value("lastCheckInDate", QDate()).toDate();
//     m_checkInStreak = m_settings->value("checkInStreak", 0).toInt();
//     m_todayLuck = m_settings->value("todayLuck", "").toString();
//     m_todayQuote = m_settings->value("todayQuote", "").toString();
// }

// void CompactCheckInWidget::saveSettings()
// {
//     m_settings->setValue("lastCheckInDate", m_lastCheckInDate);
//     m_settings->setValue("checkInStreak", m_checkInStreak);
//     m_settings->setValue("todayLuck", m_todayLuck);
//     m_settings->setValue("todayQuote", m_todayQuote);
// }

// bool CompactCheckInWidget::canCheckInToday()
// {
//     QDate today = QDate::currentDate();
//     return m_lastCheckInDate != today;
// }

// QString CompactCheckInWidget::getRandomLuck()
// {
//     int index = QRandomGenerator::global()->bounded(m_luckList.size());
//     return m_luckList[index];
// }

// QString CompactCheckInWidget::getDailyQuote()
// {
//     QDate today = QDate::currentDate();
//     uint seed = today.toJulianDay();
//     QRandomGenerator generator(seed);
//     int index = generator.bounded(m_quoteList.size());
//     return m_quoteList[index];
// }

// void CompactCheckInWidget::onCheckInClicked()
// {
//     if (!canCheckInToday()) {
//         return;
//     }

//     QDate today = QDate::currentDate();

//     if (m_lastCheckInDate.isValid() && m_lastCheckInDate.addDays(1) == today) {
//         m_checkInStreak++;
//     } else {
//         m_checkInStreak = 1;
//     }

//     m_lastCheckInDate = today;
//     m_todayLuck = getRandomLuck();
//     m_todayQuote = getDailyQuote();

//     saveSettings();
//     updateUI();
// }

// void CompactCheckInWidget::updateUI()
// {
//     m_streakLabel->setText(QString("连续%1天").arg(m_checkInStreak));

//     if (canCheckInToday()) {
//         m_checkInBtn->setEnabled(true);
//         m_checkInBtn->setText("签到");
//         m_statusLabel->setText("");
//         m_luckLabel->setText("");
//         m_quoteLabel->setText("");
//     } else {
//         m_checkInBtn->setEnabled(false);
//         m_checkInBtn->setText("已签");
//         m_statusLabel->setText("✓ 已完成");
//         m_luckLabel->setText(QString("🎯 今日签运：%1").arg(m_todayLuck));
//         m_quoteLabel->setText(QString("💭 %1").arg(m_todayQuote));
//     }
// }


