// // //5.24
// // // apidialog.cpp
// // #include "apidialog.h"
// // #include <QNetworkRequest>
// // #include <QUrlQuery>
// // #include "ui_apidialog.h"
// // #include <QNetworkReply>
// // #include <QJsonArray>
// // #include <QJsonDocument>
// // #include <QJsonObject>
// // #include <QMessageBox>

// // ApiDialog::ApiDialog(QWidget *parent)
// //     : QDialog(parent)
// //     , ui(new Ui::ApiDialog)
// // {
// //     ui->setupUi(this);
// //     networkManager = new QNetworkAccessManager(this);
// //     connect(ui->btnsearch, &QPushButton::clicked, this, &ApiDialog::onSearchButtonClicked);
// // }

// // ApiDialog::~ApiDialog()
// // {
// //     delete ui;
// // }

// // void ApiDialog::onSearchButtonClicked()
// // {
// //     QString keyword = ui->txtKeyword->text(); // 获取输入的关键词
// //     QUrl url("https://serpapi.com/search");
// //     QUrlQuery params;
// //     params.addQueryItem("q", keyword);
// //     // params.addQueryItem("api_key", " can't tell you(doge");
// //     params.addQueryItem("api_key", "d0e75c248bd4209b25e41b7961260175699842f4d4730dacc3196ddb3893d83f");
// //     params.addQueryItem("engine", "google_scholar");
// //     params.addQueryItem("hl", "en");  // 确保语言参数正确
// //     url.setQuery(params);

// //     QNetworkRequest request(url);
// //     request.setRawHeader("Accept-Charset", "utf-8"); // 强制使用UTF-8编码
// //     networkManager->get(request); // 发送GET请求
// //     connect(networkManager,
// //             &QNetworkAccessManager::finished,
// //             this,
// //             &ApiDialog::onApiResponseReceived); // 连接响应信号
// // }

// // void ApiDialog::onApiResponseReceived(QNetworkReply *reply)
// // {
// //     if (reply->error() == QNetworkReply::NoError) {
// //         QByteArray data = reply->readAll();
// //         qDebug() << "Raw API Response:" << data; // 打印原始数据
// //         //QByteArray data = reply->readAll();
// //         QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
// //         QJsonArray results = jsonDoc.object()["organic_results"].toArray();

// //         // 清空原有结果
// //         ui->tableResults->setRowCount(0);

// //         // 填充表格
// //         // for (const auto &result : results) {
// //         //     QJsonObject item = result.toObject();
// //         //     int row = ui->tableResults->rowCount();
// //         //     ui->tableResults->insertRow(row);
// //         //     ui->tableResults->setItem(row, 0, new QTableWidgetItem(item["title"].toString()));
// //         //     ui->tableResults->setItem(row, 1, new QTableWidgetItem(item["authors"].toString()));
// //         // }

// //         // foreach (const QJsonValue &result, results) {
// //         //     QJsonObject item = result.toObject();
// //         //     int row = ui->tableResults->rowCount();
// //         //     ui->tableResults->insertRow(row);

// //         //     // 提取标题
// //         //     QString title = item["title"].toString();
// //         //     QTableWidgetItem *titleItem = new QTableWidgetItem(title);
// //         //     ui->tableResults->setItem(row, 0, titleItem);

// //         //     // 提取作者（注意字段名可能为"author"或"authors"）
// //         //     QString authors = item.contains("authors") ? item["authors"].toString() : "N/A";
// //         //     QTableWidgetItem *authorsItem = new QTableWidgetItem(authors);
// //         //     ui->tableResults->setItem(row, 1, authorsItem);
// //         // }


// //         foreach (const QJsonValue &result, results) {
// //             if (!result.isObject()) continue; // 跳过非对象元素
// //             QJsonObject item = result.toObject();

// //             // 提取标题（容错处理）
// //             QString title = item.value("title").toString();
// //             if (title.isEmpty()) {
// //                 qDebug() << "Skipping result with empty title";
// //                 continue;
// //             }

// //             // 提取作者（处理字段缺失）
// //             QString authors = item.value("authors").toString("N/A");

// //             // 插入表格
// //             int row = ui->tableResults->rowCount();
// //             ui->tableResults->insertRow(row);
// //             ui->tableResults->setItem(row, 0, new QTableWidgetItem(title));
// //             ui->tableResults->setItem(row, 1, new QTableWidgetItem(authors));
// //         }

// //         if (ui->tableResults->rowCount() == 0) {
// //             QMessageBox::information(this, "Info", "No valid results found.");
// //         }

// //         ui->tableResults->resizeColumnsToContents(); // 调整列宽
// //     } else {
// //         QMessageBox::critical(this, "Error", "API请求失败：" + reply->errorString());
// //     }
// //     reply->deleteLater();
// // }





// //5.24
// // apidialog.cpp
// #include "apidialog.h"
// #include <QNetworkRequest>
// #include <QUrlQuery>
// #include "ui_apidialog.h"
// #include <QNetworkReply>
// #include <QJsonArray>
// #include <QJsonDocument>
// #include <QJsonObject>
// #include <QMessageBox>
// #include <QDebug>
// #include <QHeaderView>
// #include <QRegularExpression>
// // #include <QRegExp>

// ApiDialog::ApiDialog(QWidget *parent)
//     : QDialog(parent)
//     , ui(new Ui::ApiDialog)
// {
//     ui->setupUi(this);
//     networkManager = new QNetworkAccessManager(this);
//     connect(ui->btnsearch, &QPushButton::clicked, this, &ApiDialog::onSearchButtonClicked);

//     // 初始化表格
//     setupTable();
// }

// ApiDialog::~ApiDialog()
// {
//     delete ui;
// }

// void ApiDialog::setupTable()
// {
//     // 设置表格列数和表头
//     ui->tableResults->setColumnCount(4);
//     QStringList headers;
//     headers << "标题" << "作者" << "发表年份" << "引用数";
//     ui->tableResults->setHorizontalHeaderLabels(headers);

//     // 设置表格属性
//     ui->tableResults->horizontalHeader()->setStretchLastSection(true);
//     ui->tableResults->setAlternatingRowColors(true);
//     ui->tableResults->setSelectionBehavior(QAbstractItemView::SelectRows);
//     ui->tableResults->setWordWrap(true);
// }

// void ApiDialog::onSearchButtonClicked()
// {
//     QString keyword = ui->txtKeyword->text().trimmed();
//     if (keyword.isEmpty()) {
//         QMessageBox::warning(this, "警告", "请输入搜索关键词");
//         return;
//     }

//     QUrl url("https://serpapi.com/search");
//     QUrlQuery params;
//     params.addQueryItem("q", keyword);
//     params.addQueryItem("api_key", "d0e75c248bd4209b25e41b7961260175699842f4d4730dacc3196ddb3893d83f");
//     params.addQueryItem("engine", "google_scholar");
//     params.addQueryItem("hl", "en");
//     params.addQueryItem("num", "20");  // 增加结果数量
//     url.setQuery(params);

//     QNetworkRequest request(url);
//     request.setRawHeader("Accept-Charset", "utf-8");
//     request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");

//     QNetworkReply *reply = networkManager->get(request);
//     connect(reply, &QNetworkReply::finished, [this, reply]() {
//         onApiResponseReceived(reply);
//     });
// }

// void ApiDialog::onApiResponseReceived(QNetworkReply *reply)
// {
//     if (reply->error() != QNetworkReply::NoError) {
//         QMessageBox::critical(this, "错误", "API请求失败：" + reply->errorString());
//         reply->deleteLater();
//         return;
//     }

//     QByteArray data = reply->readAll();
//     qDebug() << "Raw API Response:" << data;

//     // 解析JSON响应
//     QJsonParseError parseError;
//     QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);

//     if (parseError.error != QJsonParseError::NoError) {
//         QMessageBox::critical(this, "错误", "JSON解析失败：" + parseError.errorString());
//         reply->deleteLater();
//         return;
//     }

//     QJsonObject rootObject = jsonDoc.object();

//     // 检查是否存在错误信息
//     if (rootObject.contains("error")) {
//         QString errorMsg = rootObject["error"].toString();
//         QMessageBox::critical(this, "API错误", errorMsg);
//         reply->deleteLater();
//         return;
//     }

//     // 获取搜索结果
//     QJsonArray results = rootObject["organic_results"].toArray();

//     qDebug() << "Found" << results.size() << "results";

//     // 清空表格
//     ui->tableResults->setRowCount(0);

//     if (results.isEmpty()) {
//         QMessageBox::information(this, "信息", "未找到相关文献");
//         reply->deleteLater();
//         return;
//     }

//     // 填充表格数据
//     int validResultCount = 0;
//     for (int i = 0; i < results.size(); ++i) {
//         QJsonObject item = results[i].toObject();

//         // 调试：打印每个结果的结构
//         qDebug() << "Result" << i << ":" << QJsonDocument(item).toJson(QJsonDocument::Compact);

//         // 提取标题
//         QString title = item.value("title").toString().trimmed();
//         if (title.isEmpty()) {
//             qDebug() << "Skipping result" << i << "- empty title";
//             continue;
//         }

//         // 提取作者信息
//         QString authors = "N/A";
//         if (item.contains("publication_info")) {
//             QJsonObject pubInfo = item["publication_info"].toObject();
//             if (pubInfo.contains("authors")) {
//                 QJsonArray authorsArray = pubInfo["authors"].toArray();
//                 QStringList authorsList;
//                 for (const auto &author : authorsArray) {
//                     QJsonObject authorObj = author.toObject();
//                     QString authorName = authorObj["name"].toString();
//                     if (!authorName.isEmpty()) {
//                         authorsList << authorName;
//                     }
//                 }
//                 if (!authorsList.isEmpty()) {
//                     authors = authorsList.join(", ");
//                 }
//             } else {
//                 // 尝试从summary中提取作者信息
//                 QString summary = pubInfo["summary"].toString();
//                 if (!summary.isEmpty()) {
//                     // 通常作者信息在summary的开头，用" - "分隔
//                     int dashIndex = summary.indexOf(" - ");
//                     if (dashIndex > 0) {
//                         authors = summary.left(dashIndex);
//                     }
//                 }
//             }
//         }

//         // // 提取年份
//         // QString year = "N/A";
//         // if (item.contains("publication_info")) {
//         //     QJsonObject pubInfo = item["publication_info"].toObject();
//         //     year = pubInfo.value("summary").toString();
//         //     // 尝试从summary中提取年份（通常是4位数字）
//         //     QRegularExpressio yearRegex("\\b(19|20)\\d{2}\\b");
//         //     if (yearRegex.indexIn(year) != -1) {
//         //         year = yearRegex.cap(0);
//         //     } else {
//         //         year = "N/A";
//         //     }
//         // }

//         QRegularExpression yearRegex("\\b(19|20)\\d{2}\\b");
//         QRegularExpressionMatch match = yearRegex.match(summary);
//         if (match.hasMatch()) {
//             year = match.captured(0);
//         }

//         // 提取引用数
//         QString citations = "N/A";
//         if (item.contains("inline_links")) {
//             QJsonObject inlineLinks = item["inline_links"].toObject();
//             if (inlineLinks.contains("cited_by")) {
//                 QJsonObject citedBy = inlineLinks["cited_by"].toObject();
//                 int citationCount = citedBy.value("total").toInt();
//                 if (citationCount > 0) {
//                     citations = QString::number(citationCount);
//                 }
//             }
//         }

//         // 插入到表格
//         int row = ui->tableResults->rowCount();
//         ui->tableResults->insertRow(row);

//         // 创建表格项并设置内容
//         QTableWidgetItem *titleItem = new QTableWidgetItem(title);
//         titleItem->setToolTip(title); // 设置工具提示显示完整标题
//         titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsEditable); // 设置为只读

//         QTableWidgetItem *authorsItem = new QTableWidgetItem(authors);
//         authorsItem->setToolTip(authors);
//         authorsItem->setFlags(authorsItem->flags() & ~Qt::ItemIsEditable);

//         QTableWidgetItem *yearItem = new QTableWidgetItem(year);
//         yearItem->setFlags(yearItem->flags() & ~Qt::ItemIsEditable);

//         QTableWidgetItem *citationsItem = new QTableWidgetItem(citations);
//         citationsItem->setFlags(citationsItem->flags() & ~Qt::ItemIsEditable);

//         ui->tableResults->setItem(row, 0, titleItem);
//         ui->tableResults->setItem(row, 1, authorsItem);
//         ui->tableResults->setItem(row, 2, yearItem);
//         ui->tableResults->setItem(row, 3, citationsItem);

//         validResultCount++;

//         qDebug() << "Added result" << validResultCount << ":"
//                  << "Title:" << title
//                  << "Authors:" << authors
//                  << "Year:" << year
//                  << "Citations:" << citations;
//     }

//     if (validResultCount == 0) {
//         QMessageBox::information(this, "信息", "未找到有效的搜索结果");
//     } else {
//         // 调整列宽
//         ui->tableResults->resizeColumnsToContents();
//         // 设置第一列（标题）占用更多空间
//         ui->tableResults->setColumnWidth(0, 400);

//         QMessageBox::information(this, "成功",
//                                  QString("成功获取 %1 条文献信息").arg(validResultCount));
//     }

//     reply->deleteLater();
// }




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
#include <QDebug>
#include <QHeaderView>
#include <QRegularExpression>

ApiDialog::ApiDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ApiDialog)
{
    ui->setupUi(this);
    networkManager = new QNetworkAccessManager(this);
    connect(ui->btnsearch, &QPushButton::clicked, this, &ApiDialog::onSearchButtonClicked);

    // 初始化表格
    setupTable();
}

ApiDialog::~ApiDialog()
{
    delete ui;
}

void ApiDialog::setupTable()
{
    // 设置表格列数和表头
    ui->tableResults->setColumnCount(4);
    QStringList headers;
    headers << "标题" << "作者" << "发表年份" << "引用数";
    ui->tableResults->setHorizontalHeaderLabels(headers);

    // 设置表格属性
    ui->tableResults->horizontalHeader()->setStretchLastSection(true);
    ui->tableResults->setAlternatingRowColors(true);
    ui->tableResults->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableResults->setWordWrap(true);
}

void ApiDialog::onSearchButtonClicked()
{
    QString keyword = ui->txtKeyword->text().trimmed();
    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入搜索关键词");
        return;
    }

    QUrl url("https://serpapi.com/search");
    QUrlQuery params;
    params.addQueryItem("q", keyword);
    params.addQueryItem("api_key", "d0e75c248bd4209b25e41b7961260175699842f4d4730dacc3196ddb3893d83f");
    params.addQueryItem("engine", "google_scholar");
    params.addQueryItem("hl", "en");
    params.addQueryItem("num", "20");  // 增加结果数量
    url.setQuery(params);

    QNetworkRequest request(url);
    request.setRawHeader("Accept-Charset", "utf-8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        onApiResponseReceived(reply);
    });
}

void ApiDialog::onApiResponseReceived(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "错误", "API请求失败：" + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    qDebug() << "Raw API Response:" << data;

    // 解析JSON响应
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "错误", "JSON解析失败：" + parseError.errorString());
        reply->deleteLater();
        return;
    }

    QJsonObject rootObject = jsonDoc.object();

    // 检查是否存在错误信息
    if (rootObject.contains("error")) {
        QString errorMsg = rootObject["error"].toString();
        QMessageBox::critical(this, "API错误", errorMsg);
        reply->deleteLater();
        return;
    }

    // 获取搜索结果
    QJsonArray results = rootObject["organic_results"].toArray();

    qDebug() << "Found" << results.size() << "results";

    // 清空表格
    ui->tableResults->setRowCount(0);

    if (results.isEmpty()) {
        QMessageBox::information(this, "信息", "未找到相关文献");
        reply->deleteLater();
        return;
    }

    // 填充表格数据
    int validResultCount = 0;
    for (int i = 0; i < results.size(); ++i) {
        QJsonObject item = results[i].toObject();

        // 调试：打印每个结果的结构
        qDebug() << "Result" << i << ":" << QJsonDocument(item).toJson(QJsonDocument::Compact);

        // 提取标题
        QString title = item.value("title").toString().trimmed();
        if (title.isEmpty()) {
            qDebug() << "Skipping result" << i << "- empty title";
            continue;
        }

        // 提取作者信息
        QString authors = "N/A";
        if (item.contains("publication_info")) {
            QJsonObject pubInfo = item["publication_info"].toObject();
            if (pubInfo.contains("authors")) {
                QJsonArray authorsArray = pubInfo["authors"].toArray();
                QStringList authorsList;
                for (const auto &author : authorsArray) {
                    QJsonObject authorObj = author.toObject();
                    QString authorName = authorObj["name"].toString();
                    if (!authorName.isEmpty()) {
                        authorsList << authorName;
                    }
                }
                if (!authorsList.isEmpty()) {
                    authors = authorsList.join(", ");
                }
            } else {
                // 尝试从summary中提取作者信息
                QString summary = pubInfo["summary"].toString();
                if (!summary.isEmpty()) {
                    // 通常作者信息在summary的开头，用" - "分隔
                    int dashIndex = summary.indexOf(" - ");
                    if (dashIndex > 0) {
                        authors = summary.left(dashIndex);
                    }
                }
            }
        }

        // 提取年份
        QString year = "N/A";
        if (item.contains("publication_info")) {
            QJsonObject pubInfo = item["publication_info"].toObject();
            QString summary = pubInfo.value("summary").toString();
            // 尝试从summary中提取年份（通常是4位数字）
            QRegularExpression yearRegex("\\b(19|20)\\d{2}\\b");
            QRegularExpressionMatch match = yearRegex.match(summary);
            if (match.hasMatch()) {
                year = match.captured(0);
            }
        }

        // 提取引用数
        QString citations = "N/A";
        if (item.contains("inline_links")) {
            QJsonObject inlineLinks = item["inline_links"].toObject();
            if (inlineLinks.contains("cited_by")) {
                QJsonObject citedBy = inlineLinks["cited_by"].toObject();
                int citationCount = citedBy.value("total").toInt();
                if (citationCount > 0) {
                    citations = QString::number(citationCount);
                }
            }
        }

        // 插入到表格
        int row = ui->tableResults->rowCount();
        ui->tableResults->insertRow(row);

        // 创建表格项并设置内容
        QTableWidgetItem *titleItem = new QTableWidgetItem(title);
        titleItem->setToolTip(title); // 设置工具提示显示完整标题
        titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsEditable); // 设置为只读

        QTableWidgetItem *authorsItem = new QTableWidgetItem(authors);
        authorsItem->setToolTip(authors);
        authorsItem->setFlags(authorsItem->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *yearItem = new QTableWidgetItem(year);
        yearItem->setFlags(yearItem->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *citationsItem = new QTableWidgetItem(citations);
        citationsItem->setFlags(citationsItem->flags() & ~Qt::ItemIsEditable);

        ui->tableResults->setItem(row, 0, titleItem);
        ui->tableResults->setItem(row, 1, authorsItem);
        ui->tableResults->setItem(row, 2, yearItem);
        ui->tableResults->setItem(row, 3, citationsItem);

        validResultCount++;

        // qDebug() << "Added result" << validResultCount << ":"
        //          << "Title:" << title
        //          << "Authors:" << authors
        //          << "Year:" << year
        //          << "Citations:" << citations;
    }

    if (validResultCount == 0) {
        QMessageBox::information(this, "信息", "未找到有效的搜索结果");
    } else {
        // 调整列宽
        ui->tableResults->resizeColumnsToContents();
        // 设置第一列（标题）
        ui->tableResults->setColumnWidth(0, 400);

        QMessageBox::information(this, "成功",
                                 QString("成功获取 %1 条文献信息").arg(validResultCount));
    }

    reply->deleteLater();
}
