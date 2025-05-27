#ifndef COMPACTCHECKINWIDGET_H
#define COMPACTCHECKINWIDGET_H


// #include <QWidget>
// #include <QPushButton>
// #include <QLabel>
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QFrame>
// #include <QPropertyAnimation>
// #include <QGraphicsOpacityEffect>
// #include <QTimer>
// #include <QSettings>
// #include <QDate>
// #include <QTime>
// #include <QRandomGenerator>
// #include <QFont>
// #include <QTextEdit>
// #include <QScrollArea>

// class CheckInWidget : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit CheckInWidget(QWidget *parent = nullptr);
//     ~CheckInWidget();

// private slots:
//     void onCheckInClicked();
//     void showCheckInResult();

// private:
//     void setupUI();
//     void loadSettings();
//     void saveSettings();
//     bool canCheckInToday();
//     QString getRandomLuck();
//     QString getDailyQuote();
//     void playCheckInAnimation();
//     void updateUI();

//     // UI组件
//     QPushButton *m_checkInBtn;
//     QLabel *m_statusLabel;
//     QLabel *m_luckLabel;
//     QTextEdit *m_quoteLabel;
//     QLabel *m_streakLabel;
//     QFrame *m_resultFrame;

//     // 动画效果
//     QPropertyAnimation *m_fadeAnimation;
//     QGraphicsOpacityEffect *m_opacityEffect;

//     // 数据
//     QSettings *m_settings;
//     QDate m_lastCheckInDate;
//     int m_checkInStreak;
//     QString m_todayLuck;
//     QString m_todayQuote;

//     // 签运数组
//     QStringList m_luckList;
//     QStringList m_quoteList;
// };


#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QSettings>
#include <QDate>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class CompactCheckInWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompactCheckInWidget(QWidget *parent = nullptr);
    ~CompactCheckInWidget();
    void updateUI();

private slots:
    void onCheckInClicked();

private:
    void setupUI();
    void loadSettings();
    void saveSettings();
    bool canCheckInToday();
    QString getRandomLuck();
    QString getDailyQuote();
    // void updateUI();

    QPushButton *m_checkInBtn;
    QLabel *m_statusLabel;
    QLabel *m_luckLabel;
    QLabel *m_quoteLabel;
    QLabel *m_streakLabel;

    QSettings *m_settings;
    QDate m_lastCheckInDate;
    int m_checkInStreak;
    QString m_todayLuck;
    QString m_todayQuote;

    QStringList m_luckList;
    QStringList m_quoteList;
};



#endif // COMPACTCHECKINWIDGET_H
