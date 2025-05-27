#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QLabel>


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
};

#endif // CANVAS_H
