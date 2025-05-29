// #pragma once
// #include <QDialog>
// #include <QNetworkAccessManager>
// #include <QTableWidgetItem>

// QT_BEGIN_NAMESPACE
// namespace Ui {
// class ApiDialog;
// }
// QT_END_NAMESPACE

// class ApiDialog : public QDialog
// {
//     Q_OBJECT
// public:
//     explicit ApiDialog(QWidget *parent = nullptr);
//     ~ApiDialog();
//     void setupTable();

//     void onTableItemDoubleClicked(QTableWidgetItem *item);
//     void onTableItemClicked(QTableWidgetItem *item);
//     void openLinkForRow(int row);
//     void onHeaderClicked(int logicalIndex);
//     void sortByNumericData(int column, Qt::SortOrder order);

//     void mynewsort(int column, Qt::SortOrder order);




// private slots:
//     void onSearchButtonClicked();                     // 触发API请求
//     void onApiResponseReceived(QNetworkReply *reply); // 处理API响应
// private:
//     Ui::ApiDialog *ui;
//     QNetworkAccessManager *networkManager;
// };













#ifndef APIDIALOG_H
#define APIDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>

// 自定义搜索框类
class ModernSearchBox : public QLineEdit
{
    Q_OBJECT
public:
    explicit ModernSearchBox(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    bool m_focused;
    bool m_hovered;
    QPropertyAnimation *m_shadowAnimation;
    QGraphicsDropShadowEffect *m_shadowEffect;
};

// 自定义搜索按钮类
class ModernSearchButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor buttonColor READ buttonColor WRITE setButtonColor)
public:
    explicit ModernSearchButton(const QString &text, QWidget *parent = nullptr);

    // 添加属性的 getter 和 setter
    QColor buttonColor() const { return m_currentColor; }
    void setButtonColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool m_hovered;
    bool m_pressed;
    QPropertyAnimation *m_colorAnimation;
    QColor m_currentColor;

private slots:
    //void onColorChanged();
};

// 自定义结果卡片类
class ResultCard : public QFrame
{
    Q_OBJECT
public:
    explicit ResultCard(const QString &title, const QString &author,
                        const QString &year, const QString &citations,
                        const QString &url, QWidget *parent = nullptr);

signals:
    void cardClicked(const QString &url);
    void cardDoubleClicked(const QString &url);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QString m_url;
    bool m_hovered;
    QLabel *m_titleLabel;
    QLabel *m_authorLabel;
    QLabel *m_yearLabel;
    QLabel *m_citationsLabel;
    QGraphicsDropShadowEffect *m_shadowEffect;
};

class ApiDialog : public QDialog
{
    Q_OBJECT

public:
    ApiDialog(QWidget *parent = nullptr);
    ~ApiDialog();

private slots:
    void onSearchButtonClicked();
    void onResultCardClicked(const QString &url);
    void onResultCardDoubleClicked(const QString &url);
    void onApiResponseReceived(QNetworkReply *reply);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUI();
    void addResultCard(const QString &title, const QString &author,
                       const QString &year, const QString &citations,
                       const QString &url);
    void clearResults();
    void showError(const QString &message);
    void showLoading(bool loading);

    QNetworkAccessManager *networkManager;

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

#endif // APIDIALOG_H
