#include "canvas.h"
#include "ui_canvas.h"

#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QPainter>
#include <QMimeData>
#include <QDrag>
#include <QPixmap>
#include <QLayout>
#include <QApplication> // 新增头文件用于获取鼠标位置
#include <QIcon>
#include <QSize>

// 判断坐标是否位于右侧禁止移动区域（假设右侧200像素为限制区域）
bool canvas::isRightRegion(const QPoint& pos) const {
    return pos.x() > (width() - 200);
}

// 新增删除热区半径
// constexpr int TRASH_ZONE_RADIUS = 30;
constexpr int DELETE_DELAY_MS = 800;

canvas::canvas(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::canvas)
{
    ui->setupUi(this);
    // 主横向布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);        // 消除子控件间距
    mainLayout->setContentsMargins(0, 0, 0, 0); // 消除布局边距

    // 左侧承载背景的区域
    QWidget *leftArea = new QWidget();
    leftArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    leftArea->setStyleSheet(
        "background: url(:/resources/bread.jpg) no-repeat center center;"
        "background-size: cover;"
        "border-right: 2px solid red;"  // 调试：可视化容器边界
        );

    // 右侧占位区域（也可以用实际控件替代）
    QWidget *rightArea = new QWidget();
    rightArea->setFixedWidth(200);    // 固定宽度200px
    rightArea->setStyleSheet("background: transparent;");

    // 将左右区域添加到布局
    mainLayout->addWidget(leftArea,1);
    mainLayout->addWidget(rightArea);

    // 设置布局拉伸系数（左侧自动扩展）
    mainLayout->setStretch(0, 1);     // 左侧区域比例1（自动扩展）
    mainLayout->setStretch(1, 0);     // 右侧区域固定宽度

    setAcceptDrops(true);                      // 设置窗口部件可以接收拖入
    label1 = new QLabel(this);          // 创建标签
    QPixmap pix1(":/resources/Filter.png");
    QPixmap scaled1 = pix1.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label1->setPixmap(scaled1);
    label1->resize(scaled1.size());  // 关键点：同步调整控件尺寸
    label1->setAttribute(Qt::WA_DeleteOnClose); // 当窗口关闭时销毁图片

    label2 = new QLabel(this);          // 创建标签
    QPixmap pix2(":/resources/Swivel.png");
    QPixmap scaled2 = pix2.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label2->setPixmap(scaled2);
    label2->resize(scaled2.size());  // 关键点：同步调整控件尺寸
    label2->setAttribute(Qt::WA_DeleteOnClose); // 当窗口关闭时销毁图片

    label3 = new QLabel(this);          // 创建标签
    QPixmap pix3(":/resources/Wheels.png");
    QPixmap scaled3 = pix3.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label3->setPixmap(scaled3);
    label3->resize(scaled3.size());  // 关键点：同步调整控件尺寸
    label3->setAttribute(Qt::WA_DeleteOnClose); // 当窗口关闭时销毁图片

    label4 = new QLabel(this);          // 创建标签
    QPixmap pix4(":/resources/SLM.png");
    QPixmap scaled4 = pix4.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    label4->setPixmap(scaled4);
    label4->resize(scaled4.size());  // 关键点：同步调整控件尺寸
    label4->setAttribute(Qt::WA_DeleteOnClose); // 当窗口关闭时销毁图片

    // 新增删除热区初始化
    m_trashIcon = new QLabel(this);
    m_trashIcon->setPixmap(QPixmap(":/resources/trash.png").scaled(40, 40));
    m_trashIcon->setFixedSize(60, 60);
    m_trashIcon->setScaledContents(true);  // 关键修改：允许图片自动缩放
    m_trashIcon->setAlignment(Qt::AlignCenter);  // 关键修改：内容居中
    m_trashIcon->setStyleSheet(
        "background: rgba(255, 50, 50, 30);"
        "border-radius: 30px;"
        "border: 2px solid rgba(255, 50, 50, 150);");
    m_trashIcon->hide();

    m_deleteTimer = new QTimer(this);
    m_deleteTimer->setSingleShot(true);
    connect(m_deleteTimer, &QTimer::timeout, this, &canvas::handleDeleteTimeout);

    m_button = new QPushButton(this);
    // m_button->setGeometry(posx, 550, 40, 40);
    m_button->resize(50, 50);
    m_button->setIcon(QIcon(":/resources/EpCirclePlus.png"));
    m_button->setIconSize(QSize(50, 50));
    m_button->setStyleSheet("border: none; background: transparent;");
    // m_button->setText("添加新仪器");
    connect(m_button, &QPushButton::clicked, [=](){
        if (!seaDialog) {
            seaDialog = new SeaDialog(this); // 延迟初始化
        }
        seaDialog->show(); // 显示弹窗
    });
}

canvas::~canvas()
{
    delete ui;
}

// 新增辅助函数
bool canvas::isInTrashArea(const QPoint &pos) const
{
    return m_trashIcon->geometry().adjusted(-10, -10, 10, 10).contains(pos);
}

void canvas::updateTrashPosition()
{
    QPoint trashPos(width() - 270, height() - 70);
    m_trashIcon->move(trashPos);
}

void canvas::startTrashAnimation(bool expanding)
{
    if(m_trashAnim && m_trashAnim->state() == QAbstractAnimation::Running)
        return;

    QRect start = m_trashIcon->geometry();
    QRect end = start.adjusted(expanding ? -10 : 10,
                               expanding ? -10 : 10,
                               expanding ? 20 : -20,
                               expanding ? 20 : -20);

    m_trashAnim = new QPropertyAnimation(m_trashIcon, "geometry", this);
    m_trashAnim->setDuration(300);
    m_trashAnim->setStartValue(start);
    m_trashAnim->setEndValue(end);
    m_trashAnim->start();
}

void canvas::resetTrashState()
{
    m_deleteTimer->stop();
    m_trashIcon->hide();
    m_trashIcon->setStyleSheet(// 恢复原始样式
        "background: rgba(255, 50, 50, 30);"
        "border-radius: 30px;"
        "border: 2px solid rgba(255, 50, 50, 150);");
}

void canvas::mousePressEvent(QMouseEvent *event)   //鼠标按下事件
{
    // 第一步：获取图片
    // 将鼠标指针所在位置的部件强制转换为QLabel类型
    QLabel *child = static_cast<QLabel*>(childAt(event->position().toPoint()));
    if (!child || !child->inherits("QLabel")) return; // 如果部件不是QLabel则直接返回
    // QPixmap pixmap = child->pixmap();     // 获取QLabel中的图片
    QPixmap pixmap = QPixmap::fromImage(  // 显式转换QImage到QPixmap
        child->pixmap().toImage().convertToFormat(QImage::Format_ARGB32)
        );
    // 新增：记录拖动起始位置是否在右侧区域
    const bool startInRight = isRightRegion(event->position().toPoint());

    // 第二步：自定义MIME类型
    QByteArray itemData;                                     // 创建字节数组
    QDataStream dataStream(&itemData, QIODevice::WriteOnly); // 创建数据流
    // 将图片信息，位置信息输入到字节数组中
    // dataStream << pixmap << QPoint(event->pos() - child->pos());
    dataStream << pixmap << QPoint(event->pos() - child->pos())
               << reinterpret_cast<quintptr>(child);

    // 第三步：将数据放入QMimeData中
    QMimeData *mimeData = new QMimeData;  // 创建QMimeData用来存放要移动的数据
    // 将字节数组放入QMimeData中，这里的MIME类型是我们自己定义的
    mimeData->setData("myimage/png", itemData);

    // 第四步：将QMimeData数据放入QDrag中
    QDrag *drag = new QDrag(this);      // 创建QDrag，它用来移动数据
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);//在移动过程中显示图片，若不设置则默认显示一个小矩形
    drag->setHotSpot(event->pos() - child->pos()); // 拖动时鼠标指针的位置不变

    // 第五步：给原图片添加阴影
    QPixmap tempPixmap = pixmap; // 使原图片添加阴影
    QPainter painter;            // 创建QPainter，用来绘制QPixmap
    painter.begin(&tempPixmap);
    // 在图片的外接矩形中添加一层透明的淡黑色形成阴影效果
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();
    child->setPixmap(tempPixmap); // 在移动图片过程中，让原图片添加一层黑色阴影

    // 第六步：执行拖放操作
    // if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction)
    //     == Qt::MoveAction){  // 设置拖放可以是移动和复制操作，默认是复制操作
    //     child->close();         // 如果是移动操作，那么拖放完成后关闭原标签
    //     // child->show();            // 如果是复制操作，那么拖放完成后显示标签
    //     // child->setPixmap(pixmap); // 显示原图片，不再使用阴影
    // }
    // else {
    //     child->show();            // 如果是复制操作，那么拖放完成后显示标签
    //     child->setPixmap(pixmap); // 显示原图片，不再使用阴影
    // }

    // 修改：根据起始区域设置允许的操作类型
    Qt::DropActions allowedActions = startInRight ? Qt::CopyAction : Qt::MoveAction;
    Qt::DropAction defaultAction = startInRight ? Qt::CopyAction : Qt::MoveAction;

    if (drag->exec(allowedActions, defaultAction) == Qt::MoveAction) {
        child->close();
    } else {
        child->show();
        child->setPixmap(pixmap);
    }
}

void canvas::dragEnterEvent(QDragEnterEvent *event) // 拖动进入事件
{
    // 如果有我们定义的MIME类型数据，则进行移动操作
    if (event->mimeData()->hasFormat("myimage/png")) {
        // 新增：实时检测鼠标位置
        const bool inRight = isRightRegion(event->position().toPoint());

        // event->setDropAction(Qt::CopyAction);
        // event->accept();
        // if (inRight) {
        //     // 在右侧区域：显示禁止图标，不接受拖放
        //     QApplication::setOverrideCursor(Qt::ForbiddenCursor);
        //     event->ignore();
        // } else {
        //     // 在可操作区域：设置移动动作
        //     QApplication::restoreOverrideCursor();
        //     event->setDropAction(Qt::MoveAction);
        //     event->accept();
        // }

        if (inRight) {
            // 在可操作区域：设置复制动作
            event->setDropAction(Qt::CopyAction);
            event->accept();
        } else {
            // 在可操作区域：设置移动动作
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }

        updateTrashPosition();
        m_trashIcon->show();
        startTrashAnimation(true);
        event->acceptProposedAction();

    } else {
        event->ignore();
    }
}
void canvas::dragMoveEvent(QDragMoveEvent *event)   // 拖动事件
{
    if (event->mimeData()->hasFormat("myimage/png")) {
        const bool inTrash = isInTrashArea(event->position().toPoint());

        // 更新热区样式
        m_trashIcon->setStyleSheet(QString(
                                       "background: rgba(255, 50, 50, %1);"
                                       "border-radius: 30px;"
                                       "border: 2px solid rgba(255, 50, 50, %2);")
                                       .arg(inTrash ? "120" : "30")
                                       .arg(inTrash ? "200" : "150"));

        // 管理删除计时器
        if(inTrash && !m_deleteTimer->isActive()) {
            m_deleteTimer->start(DELETE_DELAY_MS);
        } else if(!inTrash) {
            m_deleteTimer->stop();
        }

        event->setDropAction(Qt::CopyAction);
        event->accept();

    } else {
        event->ignore();
    }
}

void canvas::dropEvent(QDropEvent *event) // 放下事件
{
    resetTrashState();

    // 新增：最终坐标校验
    if (isRightRegion(event->position().toPoint())) {
        event->ignore();
        return;
    }

    if (event->mimeData()->hasFormat("myimage/png")) {
        QByteArray itemData = event->mimeData()->data("myimage/png");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QPoint offset;
        quintptr labelPtr;
        // 使用数据流将字节数组中的数据读入到QPixmap和QPoint变量中
        dataStream >> pixmap >> offset >> labelPtr;
        m_draggedLabel = reinterpret_cast<QLabel*>(labelPtr);
        if (isInTrashArea(event->position().toPoint()) && m_draggedLabel) {
            // 执行删除动画
            QPropertyAnimation *anim = new QPropertyAnimation(m_draggedLabel, "geometry");
            anim->setDuration(500);
            anim->setStartValue(m_draggedLabel->geometry());
            anim->setEndValue(QRect(m_trashIcon->pos(), QSize(0, 0)));

            // 直接关闭原标签（关键修正）
            connect(anim, &QPropertyAnimation::finished, [this](){
                if(m_draggedLabel) {
                    m_draggedLabel->close();  // 触发WA_DeleteOnClose
                    m_draggedLabel->deleteLater();
                    m_draggedLabel = nullptr;
                }
            });
            anim->start(QAbstractAnimation::DeleteWhenStopped);
            // 阻止原有drop逻辑执行
            event->setDropAction(Qt::MoveAction);
            event->accept();
            return;
        }
        else
        {
            // 新建标签，为其添加图片，并根据图片大小设置标签的大小
            QLabel *newLabel = new QLabel(this);
            newLabel->setAttribute(Qt::WA_TranslucentBackground); // 关键设置
            newLabel->setStyleSheet("background: transparent;"); // 双重保障
            newLabel->setPixmap(pixmap);
            newLabel->resize(pixmap.size());
            // 让图片移动到放下的位置，不设置的话，图片会默认显示在(0,0)点即窗口左上角
            newLabel->move(event->position().toPoint() - offset);
            newLabel->show();
            newLabel->setAttribute(Qt::WA_DeleteOnClose);
        }
        // event->setDropAction(Qt::CopyAction);
        // event->accept();
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

// 新增：恢复光标状态
void canvas::dragLeaveEvent(QDragLeaveEvent* event)
{
    resetTrashState();
    QApplication::restoreOverrideCursor();
    QWidget::dragLeaveEvent(event);
}

void canvas::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    updateTrashPosition();
    qDebug() << "Resize Event Triggered. New Size:" << event->size();
    // 计算右侧坐标（示例：距离右侧 150px）
    posx = this->width() - 150;
    qDebug() << this->width() << posx ;
    //随页面大小移动位置
    label1->move(posx, 25);
    label2->move(posx, 150);
    label3->move(posx, 275);
    label4->move(posx, 400);
    m_button->move(posx+25, 550);
}

void canvas::handleDeleteTimeout()
{
    if(m_draggedLabel) {
        startTrashAnimation(false);
        QApplication::beep(); // 提示音效
    }
}

// SeaDialog 实现
SeaDialog::SeaDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Lab Search");
    setFixedSize(500, 500);
    //setStyleSheet("background-color: white;");

    // networkManager = new QNetworkAccessManager(this);
    setupUI();
}

SeaDialog::~SeaDialog()
{
}

void SeaDialog::setupUI()
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
    connect(m_searchButton, &QPushButton::clicked, this, &SeaDialog::onSearchButtonClicked);

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
    connect(m_searchBox, &QLineEdit::returnPressed, this, &SeaDialog::onSearchButtonClicked);
}

void SeaDialog::onSearchButtonClicked()
{
    QString keyword = m_searchBox->text().trimmed();
    if (keyword.isEmpty()) {
        showError("请输入搜索关键词");
        return;
    }

    // clearResults();
    // showLoading(true);

    // // 构建API请求
    // QUrl url("https://serpapi.com/search");
    // QUrlQuery params;
    // params.addQueryItem("q", keyword);
    // params.addQueryItem("api_key", "d0e75c248bd4209b25e41b7961260175699842f4d4730dacc3196ddb3893d83f");
    // params.addQueryItem("engine", "google_scholar");
    // params.addQueryItem("hl", "en");
    // params.addQueryItem("num", "30");
    // url.setQuery(params);

    // QNetworkRequest request(url);
    // request.setRawHeader("Accept-Charset", "utf-8");
    // request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");

    // QNetworkReply *reply = networkManager->get(request);
    // connect(reply, &QNetworkReply::finished, [this, reply]() {
    //     onApiResponseReceived(reply);
    // });
}

void SeaDialog::showError(const QString &message)
{
    m_statusLabel->setText("❌ " + message);
    m_statusLabel->setStyleSheet(
        "font-size: 16px;"
        "color: #d93025;"
        "padding: 20px;"
        );
    m_statusLabel->show();
}
