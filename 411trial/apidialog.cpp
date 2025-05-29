

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
// #include <QDesktopServices>
// #include <QUrl>
// #include <QTimer>

// ApiDialog::ApiDialog(QWidget *parent)
//     : QDialog(parent)
//     , ui(new Ui::ApiDialog)
// {
//     ui->setupUi(this);
//     networkManager = new QNetworkAccessManager(this);
//     connect(ui->btnsearch, &QPushButton::clicked, this, &ApiDialog::onSearchButtonClicked);

//     // 初始化表格
//     setupTable();

//     // 连接双击信号
//     connect(ui->tableResults, &QTableWidget::itemDoubleClicked,
//             this, &ApiDialog::onTableItemDoubleClicked);

//     // 连接单击信号用于标题链接跳转
//     connect(ui->tableResults, &QTableWidget::itemClicked,
//             this, &ApiDialog::onTableItemClicked);

//     // 连接表头点击信号用于排序
//     connect(ui->tableResults->horizontalHeader(), &QHeaderView::sectionClicked,
//             this, &ApiDialog::onHeaderClicked);
// }

// ApiDialog::~ApiDialog()
// {
//     delete ui;
// }

// void ApiDialog::setupTable()
// {
//     // 设置表格列数和表头
//     ui->tableResults->setColumnCount(5);
//     QStringList headers;
//     headers << "标题" << "作者" << "发表年份" << "引用数" << "链接";
//     ui->tableResults->setHorizontalHeaderLabels(headers);

//     // 设置表格属性
//     ui->tableResults->horizontalHeader()->setStretchLastSection(true);
//     ui->tableResults->setAlternatingRowColors(true);
//     ui->tableResults->setSelectionBehavior(QAbstractItemView::SelectRows);
//     ui->tableResults->setWordWrap(true);

//     // 启用排序功能
//     ui->tableResults->setSortingEnabled(true);

//     // 隐藏链接列（用于存储URL，不显示给用户）
//     ui->tableResults->setColumnHidden(4, true);

//     // 设置鼠标指针样式
//     ui->tableResults->viewport()->setCursor(Qt::PointingHandCursor);

//     // 设置表头样式，提示可以点击排序
//     ui->tableResults->horizontalHeader()->setStyleSheet(
//         "QHeaderView::section {"
//         "background-color: #f0f0f0;"
//         "padding: 4px;"
//         "border: 1px solid #d0d0d0;"
//         "font-weight: bold;"
//         "}"
//         "QHeaderView::section:hover {"
//         "background-color: #e0e0e0;"
//         "cursor: pointer;"
//         "}"
//         );

//     // 设置表格样式
//     ui->tableResults->setStyleSheet(
//         "QTableWidget {"
//         "gridline-color: #d0d0d0;"
//         "}"
//         "QTableWidget::item {"
//         "padding: 8px;"
//         "border-bottom: 1px solid #e0e0e0;"
//         "}"
//         "QTableWidget::item:hover {"
//         "background-color: #f5f5f5;"
//         "}"
//         "QTableWidget::item:selected {"
//         "background-color: #0078d4;"
//         "color: white;"
//         "}"
//         );
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
//     params.addQueryItem("num", "30");  // 增加结果数量
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
//         int citationCount = 0;
//         if (item.contains("inline_links")) {
//             QJsonObject inlineLinks = item["inline_links"].toObject();
//             if (inlineLinks.contains("cited_by")) {
//                 QJsonObject citedBy = inlineLinks["cited_by"].toObject();
//                 citationCount = citedBy.value("total").toInt();
//                 if (citationCount > 0) {
//                     citations = QString::number(citationCount);
//                 }
//             }
//         }

//         // 提取链接
//         QString link = item.value("link").toString();
//         if (link.isEmpty()) {
//             // 如果没有直接链接，尝试从其他字段获取
//             if (item.contains("inline_links")) {
//                 QJsonObject inlineLinks = item["inline_links"].toObject();
//                 if (inlineLinks.contains("serpapi_scholar_link")) {
//                     link = inlineLinks["serpapi_scholar_link"].toString();
//                 }
//             }
//         }

//         // 插入到表格
//         int row = ui->tableResults->rowCount();
//         ui->tableResults->insertRow(row);

//         // 创建表格项并设置内容
//         QTableWidgetItem *titleItem = new QTableWidgetItem(title);
//         titleItem->setToolTip(title + "\n\n点击打开文献链接");
//         titleItem->setFlags(titleItem->flags() & ~Qt::ItemIsEditable);
//         // 设置标题为蓝色，表示可点击链接
//         titleItem->setForeground(QColor(0, 120, 212));
//         titleItem->setFont(QFont(titleItem->font().family(), titleItem->font().pointSize(), QFont::Normal, false));

//         QTableWidgetItem *authorsItem = new QTableWidgetItem(authors);
//         authorsItem->setToolTip(authors);
//         authorsItem->setFlags(authorsItem->flags() & ~Qt::ItemIsEditable);

//         // 为年份项设置数字数据，便于排序
//         QTableWidgetItem *yearItem = new QTableWidgetItem(year);
//         yearItem->setFlags(yearItem->flags() & ~Qt::ItemIsEditable);
//         yearItem->setToolTip("点击表头按年份排序");
//         if (year != "N/A") {
//             yearItem->setData(Qt::UserRole, year.toInt()); // 存储数字用于排序
//         } else {
//             yearItem->setData(Qt::UserRole, 0);
//         }

//         // 为引用数项设置数字数据，便于排序
//         QTableWidgetItem *citationsItem = new QTableWidgetItem(citations);
//         citationsItem->setFlags(citationsItem->flags() & ~Qt::ItemIsEditable);
//         citationsItem->setToolTip("点击表头按引用数排序");
//         citationsItem->setData(Qt::UserRole, citationCount); // 存储数字用于排序

//         // 存储链接（隐藏列）
//         QTableWidgetItem *linkItem = new QTableWidgetItem(link);
//         linkItem->setFlags(linkItem->flags() & ~Qt::ItemIsEditable);

//         ui->tableResults->setItem(row, 0, titleItem);
//         ui->tableResults->setItem(row, 1, authorsItem);
//         ui->tableResults->setItem(row, 2, yearItem);
//         ui->tableResults->setItem(row, 3, citationsItem);
//         ui->tableResults->setItem(row, 4, linkItem);

//         validResultCount++;

//         // qDebug() << "Added result" << validResultCount << ":"
//         //          << "Title:" << title
//         //          << "Authors:" << authors
//         //          << "Year:" << year
//         //          << "Citations:" << citations
//         //          << "Link:" << link;
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

// void ApiDialog::onTableItemDoubleClicked(QTableWidgetItem *item)
// {
//     // 双击任意位置都打开链接
//     if (!item) return;

//     int row = item->row();
//     openLinkForRow(row);
// }

// void ApiDialog::onTableItemClicked(QTableWidgetItem *item)
// {
//     // 只有点击标题列（第0列）才打开链接
//     if (!item || item->column() != 0) return;

//     int row = item->row();
//     openLinkForRow(row);
// }

// void ApiDialog::openLinkForRow(int row)
// {
//     QTableWidgetItem *linkItem = ui->tableResults->item(row, 4); // 获取链接列的项

//     if (linkItem) {
//         QString url = linkItem->text();
//         if (!url.isEmpty()) {
//             // 使用默认浏览器打开链接
//             if (QDesktopServices::openUrl(QUrl(url))) {
//                 qDebug() << "Opening URL:" << url;

//                 // 视觉反馈：短暂改变标题颜色
//                 QTableWidgetItem *titleItem = ui->tableResults->item(row, 0);
//                 if (titleItem) {
//                     QColor originalColor = titleItem->foreground().color();
//                     titleItem->setForeground(QColor(128, 0, 128)); // 紫色表示已访问

//                     // 1秒后恢复原色
//                     QTimer::singleShot(1000, [titleItem, originalColor]() {
//                         if (titleItem) {
//                             titleItem->setForeground(originalColor);
//                         }
//                     });
//                 }
//             } else {
//                 QMessageBox::warning(this, "警告", "无法打开链接：" + url);
//             }
//         } else {
//             QMessageBox::information(this, "信息", "该文献没有可用的链接");
//         }
//     }
// }

// void ApiDialog::onHeaderClicked(int logicalIndex)
// {
//     // 临时禁用排序，手动实现自定义排序
//     ui->tableResults->setSortingEnabled(false);

//     static int lastSortedColumn = -1;
//     static Qt::SortOrder lastSortOrder = Qt::AscendingOrder;

//     // 确定排序顺序
//     Qt::SortOrder sortOrder = Qt::AscendingOrder;
//     if (logicalIndex == lastSortedColumn) {
//         // 如果点击的是同一列，切换排序顺序
//         sortOrder = (lastSortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
//     }

//     // 根据列索引进行不同的排序
//     switch (logicalIndex) {
//     case 0: // 标题 - 按字母顺序
//     case 1: // 作者 - 按字母顺序
//         ui->tableResults->sortItems(logicalIndex, sortOrder);
//         break;

//     case 2: // 年份 - 按数字排序
//         sortByNumericData(logicalIndex, sortOrder);
//         break;

//     case 3: // 引用数 - 按数字排序
//         //sortByNumericData(logicalIndex, sortOrder);

//         mynewsort(logicalIndex, sortOrder);

//         break;
//     }

//     lastSortedColumn = logicalIndex;
//     lastSortOrder = sortOrder;

//     // 重新启用排序
//     ui->tableResults->setSortingEnabled(true);

//     // 显示排序提示
//     QString columnName;
//     switch (logicalIndex) {
//     case 0: columnName = "标题"; break;
//     case 1: columnName = "作者"; break;
//     case 2: columnName = "年份"; break;
//     case 3: columnName = "引用数"; break;
//     }

//     QString orderText = (sortOrder == Qt::AscendingOrder) ? "升序" : "降序";
//     ui->tableResults->setToolTip(QString("已按 %1 %2 排序").arg(columnName, orderText));
// }

// void ApiDialog::sortByNumericData(int column, Qt::SortOrder order)
// {
//     int rowCount = ui->tableResults->rowCount();
//     if (rowCount <= 1) return;

//     // 创建排序数据结构
//     QList<QPair<int, int>> sortData; // <数值, 行号>

//     for (int i = 0; i < rowCount; ++i) {
//         QTableWidgetItem *item = ui->tableResults->item(i, column);
//         if (item) {
//             int value = item->data(Qt::UserRole).toInt();
//             sortData.append(qMakePair(value, i));
//         }
//     }

//     // 排序
//     std::sort(sortData.begin(), sortData.end(), [order](const QPair<int, int> &a, const QPair<int, int> &b) {
//         if (order == Qt::AscendingOrder) {
//             return a.first < b.first;
//         } else {
//             return a.first > b.first;
//         }
//     });

//     // 重新排列表格行
//     QList<QList<QTableWidgetItem*>> allRows;
//     for (int i = 0; i < rowCount; ++i) {
//         QList<QTableWidgetItem*> row;
//         for (int j = 0; j < ui->tableResults->columnCount(); ++j) {
//             row.append(ui->tableResults->takeItem(i, j));
//         }
//         allRows.append(row);
//     }

//     // 按排序顺序重新插入
//     for (int i = 0; i < sortData.size(); ++i) {
//         int originalRow = sortData[i].second;
//         for (int j = 0; j < ui->tableResults->columnCount(); ++j) {
//             ui->tableResults->setItem(i, j, allRows[originalRow][j]);
//         }
//     }
// }




// void ApiDialog::mynewsort(int column, Qt::SortOrder order)
// {
//     // int rowCount = ui->tableResults->rowCount();
//     // if (rowCount <= 1) return;

//     // // 创建排序数据结构
//     // QList<QPair<int, int>> sortData; // <数值, 行号>

//     // for (int i = 0; i < rowCount; ++i) {
//     //     QTableWidgetItem *item = ui->tableResults->item(i, column);
//     //     if (item) {
//     //         int value = item->data(Qt::UserRole).toInt();
//     //         sortData.append(qMakePair(value, i));
//     //     }
//     // }

//     // // 排序
//     // std::sort(sortData.begin(), sortData.end(), [order](const QPair<int, int> &a, const QPair<int, int> &b) {
//     //     if (order == Qt::AscendingOrder) {
//     //         return a < b;
//     //     } else {
//     //         return a > b;
//     //     }
//     // });

//     // // 重新排列表格行
//     // QList<QList<QTableWidgetItem*>> allRows;
//     // for (int i = 0; i < rowCount; ++i) {
//     //     QList<QTableWidgetItem*> row;
//     //     for (int j = 0; j < ui->tableResults->columnCount(); ++j) {
//     //         row.append(ui->tableResults->takeItem(i, j));
//     //     }
//     //     allRows.append(row);
//     // }

//     // // 按排序顺序重新插入
//     // for (int i = 0; i < sortData.size(); ++i) {
//     //     int originalRow = sortData[i].second;
//     //     for (int j = 0; j < ui->tableResults->columnCount(); ++j) {
//     //         ui->tableResults->setItem(i, j, allRows[originalRow][j]);
//     //     }
//     // }


//     int rowCount = ui->tableResults->rowCount();
//     if (rowCount <= 1) return;

//     // 创建排序数据结构
//     QList<QPair<int, int>> sortData; // <数值, 行号>

//     for (int i = 0; i < rowCount; ++i) {
//         QTableWidgetItem *item = ui->tableResults->item(i, column);
//         if (item) {
//             // 获取存储在UserRole中的数值
//             int value = item->data(Qt::UserRole).toInt();

//             // 如果UserRole中没有数据，尝试从显示文本解析
//             if (value == 0 && item->text() != "0") {
//                 QString text = item->text().trimmed();
//                 if (text == "N/A" || text.isEmpty()) {
//                     value = -1; // 将N/A设为-1，方便排序处理
//                 } else {
//                     bool ok;
//                     value = text.toInt(&ok);
//                     if (!ok) {
//                         value = -1; // 解析失败也设为-1
//                     }
//                 }
//             }

//             sortData.append(qMakePair(value, i));
//         }
//     }

//     // 排序 - 修复了比较逻辑
//     std::sort(sortData.begin(), sortData.end(), [order](const QPair<int, int> &a, const QPair<int, int> &b) {
//         // 处理N/A值（-1）
//         if (a.first == -1 && b.first == -1) {
//             return false; // 两个N/A值相等
//         }
//         if (a.first == -1) {
//             return order == Qt::DescendingOrder; // N/A在降序时排前面，升序时排后面
//         }
//         if (b.first == -1) {
//             return order == Qt::AscendingOrder; // N/A在升序时排后面，降序时排前面
//         }

//         // 正常数值比较 - 这里是关键修复
//         if (order == Qt::AscendingOrder) {
//             return a.first < b.first; // 比较数值部分，不是整个pair
//         } else {
//             return a.first > b.first; // 比较数值部分，不是整个pair
//         }
//     });

//     // 重新排列表格行
//     QList<QList<QTableWidgetItem*>> allRows;
//     for (int i = 0; i < rowCount; ++i) {
//         QList<QTableWidgetItem*> row;
//         for (int j = 0; j < ui->tableResults->columnCount(); ++j) {
//             row.append(ui->tableResults->takeItem(i, j));
//         }
//         allRows.append(row);
//     }

//     // 按排序顺序重新插入
//     for (int i = 0; i < sortData.size(); ++i) {
//         int originalRow = sortData[i].second;
//         for (int j = 0; j < ui->tableResults->columnCount(); ++j) {
//             ui->tableResults->setItem(i, j, allRows[originalRow][j]);
//         }
//     }
// }



// 5.29 整体风格优化
#include "apidialog.h"
#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QDebug>

// ModernSearchBox 实现
ModernSearchBox::ModernSearchBox(QWidget *parent)
    : QLineEdit(parent), m_focused(false), m_hovered(false)
{
    setFixedHeight(48);
    setStyleSheet(
        "ModernSearchBox {"
        "border: 2px solid #e0e0e0;"
        "border-radius: 24px;"
        "padding: 0 20px;"
        "font-size: 16px;"
        "background-color: white;"
        "}"
        "ModernSearchBox:focus {"
        "border: 2px solid #4285f4;"
        "outline: none;"
        "}"
        );

    // // 添加阴影效果
    // m_shadowEffect = new QGraphicsDropShadowEffect(this);
    // m_shadowEffect->setBlurRadius(8);
    // m_shadowEffect->setColor(QColor(0, 0, 0, 30));
    // m_shadowEffect->setOffset(0, 2);
    // setGraphicsEffect(m_shadowEffect);

    // // 阴影动画
    // m_shadowAnimation = new QPropertyAnimation(m_shadowEffect, "blurRadius");
    // m_shadowAnimation->setDuration(200);
    // 添加阴影效果
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(8);
    m_shadowEffect->setColor(QColor(0, 0, 0, 30));
    m_shadowEffect->setOffset(0, 2);
    setGraphicsEffect(m_shadowEffect);

    // 阴影动画 - 添加缓动曲线
    m_shadowAnimation = new QPropertyAnimation(m_shadowEffect, "blurRadius");
    m_shadowAnimation->setDuration(200);
    m_shadowAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

void ModernSearchBox::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
}

void ModernSearchBox::focusInEvent(QFocusEvent *event)
{
    m_focused = true;
    m_shadowAnimation->setStartValue(8);
    m_shadowAnimation->setEndValue(16);
    m_shadowAnimation->start();
    QLineEdit::focusInEvent(event);
}

void ModernSearchBox::focusOutEvent(QFocusEvent *event)
{
    m_focused = false;
    m_shadowAnimation->setStartValue(16);
    m_shadowAnimation->setEndValue(8);
    m_shadowAnimation->start();
    QLineEdit::focusOutEvent(event);
}

void ModernSearchBox::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
    if (!m_focused) {
        m_shadowAnimation->setStartValue(8);
        m_shadowAnimation->setEndValue(12);
        m_shadowAnimation->start();
    }
    QLineEdit::enterEvent(event);
}

void ModernSearchBox::leaveEvent(QEvent *event)
{
    m_hovered = false;
    if (!m_focused) {
        m_shadowAnimation->setStartValue(12);
        m_shadowAnimation->setEndValue(8);
        m_shadowAnimation->start();
    }
    QLineEdit::leaveEvent(event);
}

// ModernSearchButton 实现
ModernSearchButton::ModernSearchButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent), m_hovered(false), m_pressed(false)
{
    setFixedSize(120, 40);
    m_currentColor = QColor(248, 249, 250);

    setStyleSheet(
        "ModernSearchButton {"
        "border: 1px solid #f0f0f0;"
        "border-radius: 4px;"
        "padding: 8px 16px;"
        "font-size: 14px;"
        "background-color: #f8f9fa;"
        "color: #3c4043;"
        "}"
        );

    // // 颜色动画
    // m_colorAnimation = new QPropertyAnimation(this, "color");
    // m_colorAnimation->setDuration(150);
    // connect(m_colorAnimation, &QPropertyAnimation::valueChanged, this, &ModernSearchButton::onColorChanged);
    // 修改动画属性名
    m_colorAnimation = new QPropertyAnimation(this, "buttonColor");
    m_colorAnimation->setDuration(150);
    m_colorAnimation->setEasingCurve(QEasingCurve::OutCubic); // 添加缓动曲线
}

// 添加 setter 方法
void ModernSearchButton::setButtonColor(const QColor &color)
{
    if (m_currentColor != color) {
        m_currentColor = color;
        update(); // 触发重绘
    }
}

void ModernSearchButton::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
    m_colorAnimation->stop(); // 停止当前动画
    m_colorAnimation->setStartValue(m_currentColor);
    m_colorAnimation->setEndValue(QColor(241, 243, 244));
    m_colorAnimation->start();
    QPushButton::enterEvent(event);
}

void ModernSearchButton::leaveEvent(QEvent *event)
{
    m_hovered = false;
    m_colorAnimation->stop();
    m_colorAnimation->setStartValue(m_currentColor);
    m_colorAnimation->setEndValue(QColor(248, 249, 250));
    m_colorAnimation->start();
    QPushButton::leaveEvent(event);
}

void ModernSearchButton::mousePressEvent(QMouseEvent *event)
{
    m_pressed = true;
    m_colorAnimation->stop();
    m_colorAnimation->setStartValue(m_currentColor);
    m_colorAnimation->setEndValue(QColor(232, 234, 237));
    m_colorAnimation->start();
    QPushButton::mousePressEvent(event);
}

void ModernSearchButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressed = false;
    QColor targetColor = m_hovered ? QColor(241, 243, 244) : QColor(248, 249, 250);
    m_colorAnimation->stop();
    m_colorAnimation->setStartValue(m_currentColor);
    m_colorAnimation->setEndValue(targetColor);
    m_colorAnimation->start();
    QPushButton::mouseReleaseEvent(event);
}


void ModernSearchButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景
    QRect rect = this->rect();
    painter.setBrush(m_currentColor);
    painter.setPen(QPen(QColor(240, 240, 240), 1));
    painter.drawRoundedRect(rect, 4, 4);

    // 绘制文字
    painter.setPen(QColor(60, 64, 67));
    painter.setFont(font());
    painter.drawText(rect, Qt::AlignCenter, text());
}

// void ModernSearchButton::enterEvent(QEnterEvent *event)
// {
//     m_hovered = true;
//     m_colorAnimation->setStartValue(m_currentColor);
//     m_colorAnimation->setEndValue(QColor(241, 243, 244));
//     m_colorAnimation->start();
//     QPushButton::enterEvent(event);
// }

// void ModernSearchButton::leaveEvent(QEvent *event)
// {
//     m_hovered = false;
//     m_colorAnimation->setStartValue(m_currentColor);
//     m_colorAnimation->setEndValue(QColor(248, 249, 250));
//     m_colorAnimation->start();
//     QPushButton::leaveEvent(event);
// }

// void ModernSearchButton::mousePressEvent(QMouseEvent *event)
// {
//     m_pressed = true;
//     m_colorAnimation->setStartValue(m_currentColor);
//     m_colorAnimation->setEndValue(QColor(232, 234, 237));
//     m_colorAnimation->start();
//     QPushButton::mousePressEvent(event);
// }

// void ModernSearchButton::mouseReleaseEvent(QMouseEvent *event)
// {
//     m_pressed = false;
//     QColor targetColor = m_hovered ? QColor(241, 243, 244) : QColor(248, 249, 250);
//     m_colorAnimation->setStartValue(m_currentColor);
//     m_colorAnimation->setEndValue(targetColor);
//     m_colorAnimation->start();
//     QPushButton::mouseReleaseEvent(event);
// }

// void ModernSearchButton::onColorChanged()
// {
//     m_currentColor = m_colorAnimation->currentValue().value<QColor>();
//     update();
// }

// ResultCard 实现
ResultCard::ResultCard(const QString &title, const QString &author,
                       const QString &year, const QString &citations,
                       const QString &url, QWidget *parent)
    : QFrame(parent), m_url(url), m_hovered(false)
{
    setFixedHeight(120);
    setStyleSheet(
        "ResultCard {"
        "background-color: white;"
        "border: 1px solid #e0e0e0;"
        "border-radius: 8px;"
        "margin: 4px;"
        "}"
        "ResultCard:hover {"
        "border: 1px solid #dadce0;"
        "}"
        );

    // 添加阴影
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(4);
    m_shadowEffect->setColor(QColor(0, 0, 0, 20));
    m_shadowEffect->setOffset(0, 1);
    setGraphicsEffect(m_shadowEffect);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 12, 16, 12);
    layout->setSpacing(4);

    // 标题
    m_titleLabel = new QLabel(title);
    m_titleLabel->setStyleSheet(
        "font-size: 18px;"
        "color: #1a0dab;"
        "font-weight: normal;"
        "text-decoration: underline;"
        );
    m_titleLabel->setWordWrap(true);
    m_titleLabel->setCursor(Qt::PointingHandCursor);

    // 作者和年份
    QString authorYear = QString("%1 - %2").arg(author, year);
    m_authorLabel = new QLabel(authorYear);
    m_authorLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #006621;"
        );

    // 引用数
    m_citationsLabel = new QLabel(QString("被引用 %1 次").arg(citations));
    m_citationsLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #5f6368;"
        );

    layout->addWidget(m_titleLabel);
    layout->addWidget(m_authorLabel);
    layout->addWidget(m_citationsLabel);
    layout->addStretch();

    setCursor(Qt::PointingHandCursor);
}

void ResultCard::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
}

void ResultCard::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
    m_shadowEffect->setBlurRadius(8);
    m_shadowEffect->setOffset(0, 2);
    QFrame::enterEvent(event);
}

void ResultCard::leaveEvent(QEvent *event)
{
    m_hovered = false;
    m_shadowEffect->setBlurRadius(4);
    m_shadowEffect->setOffset(0, 1);
    QFrame::leaveEvent(event);
}

void ResultCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit cardClicked(m_url);
    }
    QFrame::mousePressEvent(event);
}

void ResultCard::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit cardDoubleClicked(m_url);
    }
    QFrame::mouseDoubleClickEvent(event);
}

// ApiDialog 实现
ApiDialog::ApiDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("X-Lab Search");
    setFixedSize(900, 700);
    //setStyleSheet("background-color: white;");

    networkManager = new QNetworkAccessManager(this);
    setupUI();
}

ApiDialog::~ApiDialog()
{
}

void ApiDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

    // 顶部header区域
    m_headerWidget = new QWidget;
    m_headerWidget->setFixedHeight(200);
    m_headerWidget->setStyleSheet("background-color: white;");

    QVBoxLayout *headerLayout = new QVBoxLayout(m_headerWidget);
    headerLayout->setContentsMargins(20, 40, 20, 30);
    headerLayout->setSpacing(30);

    // Logo
    m_logoLabel = new QLabel("X-Lab Search");
    m_logoLabel->setAlignment(Qt::AlignCenter);
    m_logoLabel->setStyleSheet(
        "font-size: 48px;"
        "font-weight: 300;"
        "color: #4285f4;"
        "font-family: 'Arial', sans-serif;"
        );

    // Logo - 修改为Google配色
    m_logoLabel = new QLabel("X-Lab Search");
    m_logoLabel->setAlignment(Qt::AlignCenter);

    // 方案1: 使用HTML富文本实现Google配色
    m_logoLabel->setText(
        "<span style='font-size: 48px; font-weight: 300; font-family: Arial, sans-serif;'>"
        "<span style='color: #4285f4;'>X</span>"
        "<span style='color: #ea4335;'>-</span>"
        "<span style='color: #fbbc05;'>L</span>"
        "<span style='color: #4285f4;'>a</span>"
        "<span style='color: #34a853;'>b</span>"
        "<span style='color: #ea4335;'> </span>"
        "<span style='color: #4285f4;'>S</span>"
        "<span style='color: #ea4335;'>e</span>"
        "<span style='color: #fbbc05;'>a</span>"
        "<span style='color: #34a853;'>r</span>"
        "<span style='color: #ea4335;'>c</span>"
        "<span style='color: #4285f4;'>h</span>"
        "</span>"
        );

    // 移除原来的纯色样式，因为现在使用HTML
    m_logoLabel->setStyleSheet("background: transparent;");

    // 搜索区域
    QWidget *searchWidget = new QWidget;
    QHBoxLayout *searchLayout = new QHBoxLayout(searchWidget);
    searchLayout->setContentsMargins(50, 0, 50, 0);
    searchLayout->setSpacing(16);

    m_searchBox = new ModernSearchBox;
    m_searchBox->setPlaceholderText("输入搜索关键词...");

    m_searchButton = new ModernSearchButton("搜索");
    connect(m_searchButton, &QPushButton::clicked, this, &ApiDialog::onSearchButtonClicked);

    searchLayout->addWidget(m_searchBox, 1);
    searchLayout->addWidget(m_searchButton);

    headerLayout->addWidget(m_logoLabel);
    headerLayout->addWidget(searchWidget);

    // 结果区域
    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setStyleSheet(
        "QScrollArea {"
        "border: none;"
        "background-color: #f8f9fa;"
        "}"
        "QScrollBar:vertical {"
        "background-color: transparent;"
        "width: 12px;"
        "}"
        "QScrollBar::handle:vertical {"
        "background-color: #dadce0;"
        "border-radius: 6px;"
        "min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "background-color: #bdc1c6;"
        "}"
        );

    m_resultsContainer = new QWidget;
    m_resultsLayout = new QVBoxLayout(m_resultsContainer);
    m_resultsLayout->setContentsMargins(50, 20, 50, 20);
    m_resultsLayout->setSpacing(8);

    // 状态标签
    m_statusLabel = new QLabel("X-Lab: 在这里，探索学术前沿，发掘科技宝藏");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #5f6368;"
        "padding: 40px;"
        );
    m_resultsLayout->addWidget(m_statusLabel);
    m_resultsLayout->addStretch();

    m_scrollArea->setWidget(m_resultsContainer);

    m_mainLayout->addWidget(m_headerWidget);
    m_mainLayout->addWidget(m_scrollArea, 1);

    // 连接回车键搜索
    connect(m_searchBox, &QLineEdit::returnPressed, this, &ApiDialog::onSearchButtonClicked);
}

void ApiDialog::onSearchButtonClicked()
{
    QString keyword = m_searchBox->text().trimmed();
    if (keyword.isEmpty()) {
        showError("请输入搜索关键词");
        return;
    }

    clearResults();
    showLoading(true);

    // 构建API请求
    QUrl url("https://serpapi.com/search");
    QUrlQuery params;
    params.addQueryItem("q", keyword);
    params.addQueryItem("api_key", "d0e75c248bd4209b25e41b7961260175699842f4d4730dacc3196ddb3893d83f");
    params.addQueryItem("engine", "google_scholar");
    params.addQueryItem("hl", "en");
    params.addQueryItem("num", "30");
    url.setQuery(params);

    QNetworkRequest request(url);
    request.setRawHeader("Accept-Charset", "utf-8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        onApiResponseReceived(reply);
    });
}

void ApiDialog::addResultCard(const QString &title, const QString &author,
                              const QString &year, const QString &citations,
                              const QString &url)
{
    ResultCard *card = new ResultCard(title, author, year, citations, url);
    connect(card, &ResultCard::cardClicked, this, &ApiDialog::onResultCardClicked);
    connect(card, &ResultCard::cardDoubleClicked, this, &ApiDialog::onResultCardDoubleClicked);

    m_resultsLayout->insertWidget(m_resultsLayout->count() - 1, card);
}

void ApiDialog::clearResults()
{
    // 清除所有结果卡片
    QLayoutItem *child;
    while ((child = m_resultsLayout->takeAt(0)) != nullptr) {
        if (child->widget() && child->widget() != m_statusLabel) {
            delete child->widget();
        }
        delete child;
    }

    // 重新添加状态标签和弹簧
    m_resultsLayout->addWidget(m_statusLabel);
    m_resultsLayout->addStretch();
    m_statusLabel->show();
}

void ApiDialog::onResultCardClicked(const QString &url)
{
    // 处理单击事件
    QDesktopServices::openUrl(QUrl(url));
}

void ApiDialog::onResultCardDoubleClicked(const QString &url)
{
    // 处理双击事件
    QDesktopServices::openUrl(QUrl(url));
}

void ApiDialog::onApiResponseReceived(QNetworkReply *reply)
{
    showLoading(false);

    if (reply->error() != QNetworkReply::NoError) {
        showError("API请求失败：" + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    qDebug() << "Raw API Response:" << data;

    // 解析JSON响应
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        showError("JSON解析失败：" + parseError.errorString());
        reply->deleteLater();
        return;
    }

    QJsonObject rootObject = jsonDoc.object();

    // 检查是否存在错误信息
    if (rootObject.contains("error")) {
        QString errorMsg = rootObject["error"].toString();
        showError("API错误：" + errorMsg);
        reply->deleteLater();
        return;
    }

    // 获取搜索结果
    QJsonArray results = rootObject["organic_results"].toArray();
    qDebug() << "Found" << results.size() << "results";

    if (results.isEmpty()) {
        m_statusLabel->setText("未找到相关文献");
        m_statusLabel->show();
        reply->deleteLater();
        return;
    }

    // 处理搜索结果
    int validResultCount = 0;
    for (int i = 0; i < results.size(); ++i) {
        QJsonObject item = results[i].toObject();

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

        // 提取链接
        QString link = item.value("link").toString();
        if (link.isEmpty()) {
            if (item.contains("inline_links")) {
                QJsonObject inlineLinks = item["inline_links"].toObject();
                if (inlineLinks.contains("serpapi_scholar_link")) {
                    link = inlineLinks["serpapi_scholar_link"].toString();
                }
            }
        }

        // 添加结果卡片
        addResultCard(title, authors, year, citations, link);
        validResultCount++;
    }

    if (validResultCount == 0) {
        m_statusLabel->setText("未找到有效的搜索结果");
        m_statusLabel->show();
    } else {
        // 显示结果统计
        m_statusLabel->setText(QString("找到 %1 条相关文献").arg(validResultCount));
        m_statusLabel->setStyleSheet(
            "font-size: 14px;"
            "color: #5f6368;"
            "padding: 10px 0;"
            );
        m_statusLabel->show();
        m_resultsLayout->removeWidget(m_statusLabel);
        m_resultsLayout->insertWidget(0, m_statusLabel);
    }

    reply->deleteLater();
}

void ApiDialog::showError(const QString &message)
{
    m_statusLabel->setText("❌ " + message);
    m_statusLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #d93025;"
        "padding: 20px;"
        );
    m_statusLabel->show();
}

void ApiDialog::showLoading(bool loading)
{
    if (loading) {
        m_statusLabel->setText("🔍 X-lab正全力为您检索中，请稍候...");
        m_statusLabel->setStyleSheet(
            "font-size: 16px;"
            "color: #1a73e8;"
            "padding: 20px;"
            );
        m_statusLabel->show();
        m_searchButton->setEnabled(false);
        m_searchButton->setText("搜索中");
    } else {
        m_searchButton->setEnabled(true);
        m_searchButton->setText("搜索");
    }
}

void ApiDialog::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
}


//#include "apidialog.moc"
