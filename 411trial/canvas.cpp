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

// 判断坐标是否位于右侧禁止移动区域（假设右侧200像素为限制区域）
bool canvas::isRightRegion(const QPoint& pos) const {
    return pos.x() > (width() - 200);
}

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

}

canvas::~canvas()
{
    delete ui;
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
    dataStream << pixmap << QPoint(event->pos() - child->pos());

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

    } else {
        event->ignore();
    }
}
void canvas::dragMoveEvent(QDragMoveEvent *event)   // 拖动事件
{
    if (event->mimeData()->hasFormat("myimage/png")) {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void canvas::dropEvent(QDropEvent *event) // 放下事件
{
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
        // 使用数据流将字节数组中的数据读入到QPixmap和QPoint变量中
        dataStream >> pixmap >> offset;
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
    QApplication::restoreOverrideCursor();
    QWidget::dragLeaveEvent(event);
}

void canvas::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    qDebug() << "Resize Event Triggered. New Size:" << event->size();
    // 计算右侧坐标（示例：距离右侧 150px）
    posx = this->width() - 150;
    qDebug() << this->width() << posx ;
    //随页面大小移动位置
    label1->move(posx, 100);
    label2->move(posx, 300);
    label3->move(posx, 500);
}
