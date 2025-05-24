#pragma once
#include <QDialog>
#include <QNetworkAccessManager>

QT_BEGIN_NAMESPACE
namespace Ui {
class ApiDialog;
}
QT_END_NAMESPACE

class ApiDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ApiDialog(QWidget *parent = nullptr);
    ~ApiDialog();
private slots:
    void onSearchButtonClicked();                     // 触发API请求
    void onApiResponseReceived(QNetworkReply *reply); // 处理API响应
private:
    Ui::ApiDialog *ui;
    QNetworkAccessManager *networkManager;
};
