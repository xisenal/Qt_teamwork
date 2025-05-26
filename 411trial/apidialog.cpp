

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

//         // 提取年份
//         QString year = "N/A";
//         if (item.contains("publication_info")) {
//             QJsonObject pubInfo = item["publication_info"].toObject();
//             QString summary = pubInfo.value("summary").toString();
//             // 尝试从summary中提取年份（通常是4位数字）
//             QRegularExpression yearRegex("\\b(19|20)\\d{2}\\b");
//             QRegularExpressionMatch match = yearRegex.match(summary);
//             if (match.hasMatch()) {
//                 year = match.captured(0);
//             }
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

//         // qDebug() << "Added result" << validResultCount << ":"
//         //          << "Title:" << title
//         //          << "Authors:" << authors
//         //          << "Year:" << year
//         //          << "Citations:" << citations;
//     }

//     if (validResultCount == 0) {
//         QMessageBox::information(this, "信息", "未找到有效的搜索结果");
//     } else {
//         // 调整列宽
//         ui->tableResults->resizeColumnsToContents();
//         // 设置第一列（标题）
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
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>

ApiDialog::ApiDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ApiDialog)
{
    ui->setupUi(this);
    networkManager = new QNetworkAccessManager(this);
    connect(ui->btnsearch, &QPushButton::clicked, this, &ApiDialog::onSearchButtonClicked);

    // 初始化表格
    setupTable();

    // 连接双击信号
    connect(ui->tableResults, &QTableWidget::itemDoubleClicked,
            this, &ApiDialog::onTableItemDoubleClicked);

    // 连接单击信号用于标题链接跳转
    connect(ui->tableResults, &QTableWidget::itemClicked,
            this, &ApiDialog::onTableItemClicked);

    // 连接表头点击信号用于排序
    connect(ui->tableResults->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &ApiDialog::onHeaderClicked);
}

ApiDialog::~ApiDialog()
{
    delete ui;
}

void ApiDialog::setupTable()
{
    // 设置表格列数和表头
    ui->tableResults->setColumnCount(5);
    QStringList headers;
    headers << "标题" << "作者" << "发表年份" << "引用数" << "链接";
    ui->tableResults->setHorizontalHeaderLabels(headers);

    // 设置表格属性
    ui->tableResults->horizontalHeader()->setStretchLastSection(true);
    ui->tableResults->setAlternatingRowColors(true);
    ui->tableResults->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableResults->setWordWrap(true);

    // 启用排序功能
    ui->tableResults->setSortingEnabled(true);

    // 隐藏链接列（用于存储URL，不显示给用户）
    ui->tableResults->setColumnHidden(4, true);

    // 设置鼠标指针样式
    ui->tableResults->viewport()->setCursor(Qt::PointingHandCursor);

    // 设置表头样式，提示可以点击排序
    ui->tableResults->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "background-color: #f0f0f0;"
        "padding: 4px;"
        "border: 1px solid #d0d0d0;"
        "font-weight: bold;"
        "}"
        "QHeaderView::section:hover {"
        "background-color: #e0e0e0;"
        "cursor: pointer;"
        "}"
        );

    // 设置表格样式
    ui->tableResults->setStyleSheet(
        "QTableWidget {"
        "gridline-color: #d0d0d0;"
        "}"
        "QTableWidget::item {"
        "padding: 8px;"
        "border-bottom: 1px solid #e0e0e0;"
        "}"
        "QTableWidget::item:hover {"
        "background-color: #f5f5f5;"
        "}"
        "QTableWidget::item:selected {"
        "background-color: #0078d4;"
        "color: white;"
        "}"
        );
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
        int citationCount = 0;
        if (item.contains("inline_links")) {
            QJsonObject inlineLinks = item["inline_links"].toObject();
            if (inlineLinks.contains("cited_by")) {
                QJsonObject citedBy = inlineLinks["cited_by"].toObject();
                citationCount = citedBy.value("total").toInt();
                if (citationCount > 0) {
                    citations = QString::number(citationCount);
                }
            }
        }

        // 提取链接
        QString link = item.value("link").toString();
        if (link.isEmpty()) {
            // 如果没有直接链接，尝试从其他字段获取
            if (item.contains("inline_links")) {
                QJsonObject inlineLinks = item["inline_links"].toObject();
                if (inlineLinks.contains("serpapi_scholar_link")) {
                    link = inlineLinks["serpapi_scholar_link"].toString();
                }
            }
        }

        // 插入到表格
        int row = ui->tableResults->rowCount();
        ui->tableResults->insertRow(row);

        // 创建表格项并设置内容
        QTableWidgetItem *titleItem = new QTableWidgetItem(title);
        titleItem->setToolTip(title + "\n\n点击打开文献链接");
        titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsEditable);
        // 设置标题为蓝色，表示可点击链接
        titleItem->setForeground(QColor(0, 120, 212));
        titleItem->setFont(QFont(titleItem->font().family(), titleItem->font().pointSize(), QFont::Normal, false));

        QTableWidgetItem *authorsItem = new QTableWidgetItem(authors);
        authorsItem->setToolTip(authors);
        authorsItem->setFlags(authorsItem->flags() & ~Qt::ItemIsEditable);

        // 为年份项设置数字数据，便于排序
        QTableWidgetItem *yearItem = new QTableWidgetItem(year);
        yearItem->setFlags(yearItem->flags() & ~Qt::ItemIsEditable);
        yearItem->setToolTip("点击表头按年份排序");
        if (year != "N/A") {
            yearItem->setData(Qt::UserRole, year.toInt()); // 存储数字用于排序
        } else {
            yearItem->setData(Qt::UserRole, 0);
        }

        // 为引用数项设置数字数据，便于排序
        QTableWidgetItem *citationsItem = new QTableWidgetItem(citations);
        citationsItem->setFlags(citationsItem->flags() & ~Qt::ItemIsEditable);
        citationsItem->setToolTip("点击表头按引用数排序");
        citationsItem->setData(Qt::UserRole, citationCount); // 存储数字用于排序

        // 存储链接（隐藏列）
        QTableWidgetItem *linkItem = new QTableWidgetItem(link);
        linkItem->setFlags(linkItem->flags() & ~Qt::ItemIsEditable);

        ui->tableResults->setItem(row, 0, titleItem);
        ui->tableResults->setItem(row, 1, authorsItem);
        ui->tableResults->setItem(row, 2, yearItem);
        ui->tableResults->setItem(row, 3, citationsItem);
        ui->tableResults->setItem(row, 4, linkItem);

        validResultCount++;

        qDebug() << "Added result" << validResultCount << ":"
                 << "Title:" << title
                 << "Authors:" << authors
                 << "Year:" << year
                 << "Citations:" << citations
                 << "Link:" << link;
    }

    if (validResultCount == 0) {
        QMessageBox::information(this, "信息", "未找到有效的搜索结果");
    } else {
        // 调整列宽
        ui->tableResults->resizeColumnsToContents();
        // 设置第一列（标题）占用更多空间
        ui->tableResults->setColumnWidth(0, 400);

        QMessageBox::information(this, "成功",
                                 QString("成功获取 %1 条文献信息").arg(validResultCount));
    }

    reply->deleteLater();
}

void ApiDialog::onTableItemDoubleClicked(QTableWidgetItem *item)
{
    // 双击任意位置都打开链接
    if (!item) return;

    int row = item->row();
    openLinkForRow(row);
}

void ApiDialog::onTableItemClicked(QTableWidgetItem *item)
{
    // 只有点击标题列（第0列）才打开链接
    if (!item || item->column() != 0) return;

    int row = item->row();
    openLinkForRow(row);
}

void ApiDialog::openLinkForRow(int row)
{
    QTableWidgetItem *linkItem = ui->tableResults->item(row, 4); // 获取链接列的项

    if (linkItem) {
        QString url = linkItem->text();
        if (!url.isEmpty()) {
            // 使用默认浏览器打开链接
            if (QDesktopServices::openUrl(QUrl(url))) {
                qDebug() << "Opening URL:" << url;

                // 视觉反馈：短暂改变标题颜色
                QTableWidgetItem *titleItem = ui->tableResults->item(row, 0);
                if (titleItem) {
                    QColor originalColor = titleItem->foreground().color();
                    titleItem->setForeground(QColor(128, 0, 128)); // 紫色表示已访问

                    // 1秒后恢复原色
                    QTimer::singleShot(1000, [titleItem, originalColor]() {
                        if (titleItem) {
                            titleItem->setForeground(originalColor);
                        }
                    });
                }
            } else {
                QMessageBox::warning(this, "警告", "无法打开链接：" + url);
            }
        } else {
            QMessageBox::information(this, "信息", "该文献没有可用的链接");
        }
    }
}

void ApiDialog::onHeaderClicked(int logicalIndex)
{
    // 临时禁用排序，手动实现自定义排序
    ui->tableResults->setSortingEnabled(false);

    static int lastSortedColumn = -1;
    static Qt::SortOrder lastSortOrder = Qt::AscendingOrder;

    // 确定排序顺序
    Qt::SortOrder sortOrder = Qt::AscendingOrder;
    if (logicalIndex == lastSortedColumn) {
        // 如果点击的是同一列，切换排序顺序
        sortOrder = (lastSortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    }

    // 根据列索引进行不同的排序
    switch (logicalIndex) {
    case 0: // 标题 - 按字母顺序
    case 1: // 作者 - 按字母顺序
        ui->tableResults->sortItems(logicalIndex, sortOrder);
        break;

    case 2: // 年份 - 按数字排序
        sortByNumericData(logicalIndex, sortOrder);
        break;

    case 3: // 引用数 - 按数字排序
        //sortByNumericData(logicalIndex, sortOrder);

        mynewsort(logicalIndex, sortOrder);

        break;
    }

    lastSortedColumn = logicalIndex;
    lastSortOrder = sortOrder;

    // 重新启用排序
    ui->tableResults->setSortingEnabled(true);

    // 显示排序提示
    QString columnName;
    switch (logicalIndex) {
    case 0: columnName = "标题"; break;
    case 1: columnName = "作者"; break;
    case 2: columnName = "年份"; break;
    case 3: columnName = "引用数"; break;
    }

    QString orderText = (sortOrder == Qt::AscendingOrder) ? "升序" : "降序";
    ui->tableResults->setToolTip(QString("已按 %1 %2 排序").arg(columnName, orderText));
}

void ApiDialog::sortByNumericData(int column, Qt::SortOrder order)
{
    int rowCount = ui->tableResults->rowCount();
    if (rowCount <= 1) return;

    // 创建排序数据结构
    QList<QPair<int, int>> sortData; // <数值, 行号>

    for (int i = 0; i < rowCount; ++i) {
        QTableWidgetItem *item = ui->tableResults->item(i, column);
        if (item) {
            int value = item->data(Qt::UserRole).toInt();
            sortData.append(qMakePair(value, i));
        }
    }

    // 排序
    std::sort(sortData.begin(), sortData.end(), [order](const QPair<int, int> &a, const QPair<int, int> &b) {
        if (order == Qt::AscendingOrder) {
            return a.first < b.first;
        } else {
            return a.first > b.first;
        }
    });

    // 重新排列表格行
    QList<QList<QTableWidgetItem*>> allRows;
    for (int i = 0; i < rowCount; ++i) {
        QList<QTableWidgetItem*> row;
        for (int j = 0; j < ui->tableResults->columnCount(); ++j) {
            row.append(ui->tableResults->takeItem(i, j));
        }
        allRows.append(row);
    }

    // 按排序顺序重新插入
    for (int i = 0; i < sortData.size(); ++i) {
        int originalRow = sortData[i].second;
        for (int j = 0; j < ui->tableResults->columnCount(); ++j) {
            ui->tableResults->setItem(i, j, allRows[originalRow][j]);
        }
    }
}




void ApiDialog::mynewsort(int column, Qt::SortOrder order)
{
    int rowCount = ui->tableResults->rowCount();
    if (rowCount <= 1) return;

    // 创建排序数据结构
    QList<QPair<int, int>> sortData; // <数值, 行号>

    for (int i = 0; i < rowCount; ++i) {
        QTableWidgetItem *item = ui->tableResults->item(i, column);
        if (item) {
            int value = item->data(Qt::UserRole).toInt();
            sortData.append(qMakePair(value, i));
        }
    }

    // 排序
    std::sort(sortData.begin(), sortData.end(), [order](const QPair<int, int> &a, const QPair<int, int> &b) {
        if (order == Qt::AscendingOrder) {
            return a < b;
        } else {
            return a > b;
        }
    });

    // 重新排列表格行
    QList<QList<QTableWidgetItem*>> allRows;
    for (int i = 0; i < rowCount; ++i) {
        QList<QTableWidgetItem*> row;
        for (int j = 0; j < ui->tableResults->columnCount(); ++j) {
            row.append(ui->tableResults->takeItem(i, j));
        }
        allRows.append(row);
    }

    // 按排序顺序重新插入
    for (int i = 0; i < sortData.size(); ++i) {
        int originalRow = sortData[i].second;
        for (int j = 0; j < ui->tableResults->columnCount(); ++j) {
            ui->tableResults->setItem(i, j, allRows[originalRow][j]);
        }
    }
}
