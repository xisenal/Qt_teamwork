#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QPropertyAnimation>
#include <QIcon>
#include <QPoint>
#include <QMouseEvent>
#include <QApplication>
#include <QToolButton>
#include <QScrollArea>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void createSubMenu();
    bool eventFilter (QObject *obj, QEvent *event);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void toggleSidebar();
    void toggleLabSubMenu();
    void updateSubMenuPosition();



private:
    QWidget *sidebar;
    QPushButton *toggleBtn;
    QPropertyAnimation *animation;
    bool isCollapsed = false;
    const int expandedWidth = 220;
    const int collapsedWidth = 70;
    QWidget *subMenu = nullptr;
    QPushButton *labBtn = nullptr;
    bool isSubMenuVisible = false;

    QWidget* subMenuContainer; // 容器用于滚动区域
    QScrollArea* subMenuScrollArea;

    QPushButton *homeBtn = nullptr; // 保存主页按钮指针


    // 窗口拖动相关
    QPoint dragPosition;
    QPushButton *pinButton;
    QPushButton *minButton;
    QPushButton *maxButton;
    QPushButton *closeButton;
    bool isPinned = false;
};

#endif // MAINWINDOW_H
