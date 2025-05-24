//5.24
// apidialog.cpp
#include "apidialog.h"
#include <QNetworkRequest>
#include <QUrlQuery>
#include "ui_apidialog.h"
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

ApiDialog::ApiDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ApiDialog)
{
    ui->setupUi(this);
    networkManager = new QNetworkAccessManager(this);
    connect(ui->btnsearch, &QPushButton::clicked, this, &ApiDialog::onSearchButtonClicked);
}

ApiDialog::~ApiDialog()
{
    delete ui;
}

void ApiDialog::onSearchButtonClicked()
{
    QString keyword = ui->txtKeyword->text(); // 获取输入的关键词
    QUrl url("https://serpapi.com/search");
    QUrlQuery params;
    params.addQueryItem("q", keyword);
    params.addQueryItem("api_key", " can't tell you(doge");
    params.addQueryItem("engine", "google_scholar");
    params.addQueryItem("hl", "en");  // 确保语言参数正确
    url.setQuery(params);

    QNetworkRequest request(url);
    request.setRawHeader("Accept-Charset", "utf-8"); // 强制使用UTF-8编码
    networkManager->get(request); // 发送GET请求
    connect(networkManager,
            &QNetworkAccessManager::finished,
            this,
            &ApiDialog::onApiResponseReceived); // 连接响应信号
}

void ApiDialog::onApiResponseReceived(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        qDebug() << "Raw API Response:" << data; // 打印原始数据
        //QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonArray results = jsonDoc.object()["organic_results"].toArray();

        // 清空原有结果
        ui->tableResults->setRowCount(0);

        // 填充表格
        // for (const auto &result : results) {
        //     QJsonObject item = result.toObject();
        //     int row = ui->tableResults->rowCount();
        //     ui->tableResults->insertRow(row);
        //     ui->tableResults->setItem(row, 0, new QTableWidgetItem(item["title"].toString()));
        //     ui->tableResults->setItem(row, 1, new QTableWidgetItem(item["authors"].toString()));
        // }

        // foreach (const QJsonValue &result, results) {
        //     QJsonObject item = result.toObject();
        //     int row = ui->tableResults->rowCount();
        //     ui->tableResults->insertRow(row);

        //     // 提取标题
        //     QString title = item["title"].toString();
        //     QTableWidgetItem *titleItem = new QTableWidgetItem(title);
        //     ui->tableResults->setItem(row, 0, titleItem);

        //     // 提取作者（注意字段名可能为"author"或"authors"）
        //     QString authors = item.contains("authors") ? item["authors"].toString() : "N/A";
        //     QTableWidgetItem *authorsItem = new QTableWidgetItem(authors);
        //     ui->tableResults->setItem(row, 1, authorsItem);
        // }


        foreach (const QJsonValue &result, results) {
            if (!result.isObject()) continue; // 跳过非对象元素
            QJsonObject item = result.toObject();

            // 提取标题（容错处理）
            QString title = item.value("title").toString();
            if (title.isEmpty()) {
                qDebug() << "Skipping result with empty title";
                continue;
            }

            // 提取作者（处理字段缺失）
            QString authors = item.value("authors").toString("N/A");

            // 插入表格
            int row = ui->tableResults->rowCount();
            ui->tableResults->insertRow(row);
            ui->tableResults->setItem(row, 0, new QTableWidgetItem(title));
            ui->tableResults->setItem(row, 1, new QTableWidgetItem(authors));
        }

        if (ui->tableResults->rowCount() == 0) {
            QMessageBox::information(this, "Info", "No valid results found.");
        }

        ui->tableResults->resizeColumnsToContents(); // 调整列宽
    } else {
        QMessageBox::critical(this, "Error", "API请求失败：" + reply->errorString());
    }
    reply->deleteLater();
}
