// // mainwindow.h
// #include <QMainWindow>
// #include <QPushButton>
// #include <QVBoxLayout>
// #include <QPropertyAnimation>

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT
// public:
//     MainWindow(QWidget *parent = nullptr);

// private slots:
//     void toggleSidebar();

// private:
//     QWidget *sidebar;
//     QPushButton *toggleBtn;
//     QPropertyAnimation *animation;
//     bool isCollapsed = false;
//     const int expandedWidth = 200;
//     const int collapsedWidth = 50;
// };


//4.13
// mainwindow.h
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QPropertyAnimation>
#include <QIcon>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void toggleSidebar();

private:
    QWidget *sidebar;
    QPushButton *toggleBtn;
    QPropertyAnimation *animation;
    bool isCollapsed = false;
    const int expandedWidth = 220;  // 加宽以适应图片和文字
    const int collapsedWidth = 70;  // 调整为图片宽度+边距
};
