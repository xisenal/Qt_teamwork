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
#include <QDialog>
#include <QLabel>
#include <QPixmap>
#include "todolistwindow.h"

class UserInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserInfoDialog(QWidget *parent = nullptr);
};

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
    void toggleLabSubMenu();
    void updateSubMenuPosition();
    void showTodoList();



private:
    QWidget *sidebar;
    const int collapsedWidth = 100;
    QWidget *subMenu = nullptr;
    QPushButton *labBtn = nullptr;
    QPushButton *todoListBtn = nullptr;
    bool isSubMenuVisible = false;
    TodoListWindow *todoListWindow = nullptr;
    
    // 用户信息相关
    QLabel *avatar = nullptr;
    UserInfoDialog *userInfoDialog = nullptr;
    void showUserInfoDialog(const QPoint &pos);

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
