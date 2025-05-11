#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QPropertyAnimation>
#include <QIcon>
#include <QPoint>
#include <QMouseEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void toggleSidebar();

private:
    QWidget *sidebar;
    QPushButton *toggleBtn;
    QPropertyAnimation *animation;
    bool isCollapsed = false;
    const int expandedWidth = 220;
    const int collapsedWidth = 70;

    // 窗口拖动相关
    QPoint dragPosition;
    QPushButton *pinButton;
    QPushButton *minButton;
    QPushButton *maxButton;
    QPushButton *closeButton;
    bool isPinned = false;
};

#endif // MAINWINDOW_H
