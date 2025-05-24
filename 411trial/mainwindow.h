#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "canvas.h"
#include "todolistwindow.h"
#include "apidialog.h"


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
#include <QStackedWidget>
#include <QMessageBox>
#include <QMovie>

class UserInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserInfoDialog(const QString &email, QWidget *parent = nullptr);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void createSubMenu();
    bool eventFilter (QObject *obj, QEvent *event);
    void setCurrentUser(const QString &email);

    void switchToHomePage(bool forceShow);
    QString getButtonActiveStyle();
    QWidget* createProjectsPanel();
    QWidget* createProjectCard(const QString &title, const QString &desc);
    QWidget* createActivityTimeline();
    QWidget* createTimelineItem(const QString &date, const QString &text);
    //QWidget* createLabPage();
    QWidget* createLabCard(const QString &title,
                                   const QString &iconPath,
                                       const QString &description);
    QWidget* createLabsPanel();
    QWidget* createGithubStylePage();

private:
    QString currentUserEmail;
    QString getActiveButtonStyle();
    QString getNormalButtonStyle();
    void resetButtonStates(QPushButton *activeButton);
    void hideHomePage();
    void showHomePage();
    QWidget*  createProfileCard();
    void updateButtonState(QPushButton *activeBtn);

    // static const QString normalButtonStyle;
    // static const QString activeButtonStyle;

    //ApiDialog *apiDialog; // 弹窗实例








protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void toggleLabSubMenu();
    void updateSubMenuPosition();
    void showTodoList();
    void handleLabButtonClick();
    void onProjectButtonClicked(); // 响应按钮点击



private:
    QWidget *sidebar;
    const int collapsedWidth = 100;
    QWidget *subMenu = nullptr;
    QPushButton *labBtn = nullptr;
    QPushButton *todoListBtn = nullptr;

    QPushButton *searchbtn = nullptr;
    bool isSubMenuVisible = false;
    TodoListWindow *todoListWindow = nullptr;
    
    // 用户信息相关
    QLabel *avatar = nullptr;
    UserInfoDialog *userInfoDialog = nullptr;
    void showUserInfoDialog(const QPoint &pos);

    QWidget* subMenuContainer; // 容器用于滚动区域
    QScrollArea* subMenuScrollArea;

    QPushButton *homeBtn = nullptr; // 保存主页按钮指针

    QPointer<ApiDialog> apiDialog = nullptr; // 初始化为 nullptr


    // 窗口拖动相关
    QPoint dragPosition;
    QPushButton *pinButton;
    QPushButton *minButton;
    QPushButton *maxButton;
    QPushButton *closeButton;
    bool isPinned = false;

    // 用于页面切换与新页面
    QStackedWidget *stackedWidget;
    canvas *canvasPage;

    //首页相关
    QStackedWidget *contentStack;  // 新增：页面堆栈
    QWidget *githubHomePage;       // 新增：GitHub风格主页
    QTextEdit *defaultContent;     // 原有内容区域改为成员变量
};

// // 在类定义中添加样式常量
// const QString MainWindow::activeButtonStyle = R"(
//     QPushButton {
//         color: #1976D2;
//         background-color: #E3F2FD;
//         border-left: 3px solid #1976D2;
//     }
// )";

// const QString MainWindow::normalButtonStyle = R"(
//     QPushButton {
//         color: #424242;
//         background-color: white;
//         border: none;
//     }
//     QPushButton:hover {
//         background-color: rgba(0, 0, 0, 0.04);
//     }
// )";


#endif // MAINWINDOW_H
