#ifndef CANVAS_H
#define CANVAS_H

#include "apidialog.h"

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QPointer>

class SeaDialog : public QDialog
{
    Q_OBJECT

public:
    SeaDialog(QWidget *parent = nullptr);
    ~SeaDialog();

    // private slots:
    void onSearchButtonClicked();
    //     void onResultCardClicked(const QString &url);
    //     void onResultCardDoubleClicked(const QString &url);
    //     void onApiResponseReceived(QNetworkReply *reply);

    // protected:
    //     void paintEvent(QPaintEvent *event) override;

private:
    void setupUI();
    // void addResultCard(const QString &title, const QString &author,
    //                    const QString &year, const QString &citations,
    //                    const QString &url);
    // void clearResults();
    void showError(const QString &message);
    // void showLoading(bool loading);

    // QNetworkAccessManager *networkManager;

    // UI组件
    QVBoxLayout *m_mainLayout;
    QWidget *m_headerWidget;
    QLabel *m_logoLabel;
    ModernSearchBox *m_searchBox;
    ModernSearchButton *m_searchButton;
    QScrollArea *m_scrollArea;
    QWidget *m_resultsContainer;
    QVBoxLayout *m_resultsLayout;
    QLabel *m_statusLabel;
};

namespace Ui {
class canvas;
}

class canvas : public QWidget
{
    Q_OBJECT

public:
    explicit canvas(QWidget *parent = nullptr);
    ~canvas();

private:
    Ui::canvas *ui;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    int posx = 800;

protected:
    void mousePressEvent(QMouseEvent *event) override;      // 鼠标按下事件
    void dragEnterEvent(QDragEnterEvent *event) override;   // 拖动进入事件
    void dragMoveEvent(QDragMoveEvent *event) override;     // 拖动事件
    void dropEvent(QDropEvent *event) override;             // 放下事件
    bool isRightRegion(const QPoint& pos) const;  // 新增右侧区域判断函数
    // 新增拖出事件声明
    void dragLeaveEvent(QDragLeaveEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void handleDeleteTimeout();

private:
    // 新增删除相关成员
    QLabel *m_trashIcon = nullptr;
    QTimer *m_deleteTimer = nullptr;
    QLabel *m_draggedLabel = nullptr;
    QPropertyAnimation *m_trashAnim = nullptr;

    bool isInTrashArea(const QPoint &pos) const;
    void updateTrashPosition();
    void startTrashAnimation(bool expanding);
    void resetTrashState();
    QPushButton *m_button;  // 声明按钮指针
    QPointer<SeaDialog> seaDialog = nullptr; // 初始化为 nullptr
};

#endif // CANVAS_H
