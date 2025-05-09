#include "adminwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsEffect>
#include <QGraphicsDropShadowEffect>

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , userManager(new UserManager(this))
{
    setupUI();
    refreshUserList();
}

void AdminWindow::setupUI()
{
    setWindowTitle("管理员界面");
    setMinimumSize(800, 600);

    // 设置窗口样式
    setStyleSheet(
        "QMainWindow { background-color: #f5f5f5; }"
        "QPushButton { "
            "background-color: #2196F3; "
            "color: white; "
            "border: none; "
            "padding: 8px 16px;"
            "border-radius: 4px;"
            "font-size: 14px; "
        "}"
        "QPushButton:hover { background-color: #1976D2; }"
        "QPushButton:pressed { background-color: #0D47A1; }"
        "QTableWidget { "
            "background-color: white;"
            "border: 1px solid #E0E0E0; "
            "border-radius: 4px; "
            "gridline-color: #EEEEEE; "
        "}"
        "QTableWidget::item { padding: 8px; }"
        "QHeaderView::section {"
            "background-color: #FAFAFA; "
            "padding: 8px;"
            "border: none; "
            "border-bottom: 2px solid #E0E0E0; "
            "font-weight: bold;"
        "}"
    );

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);

    // 创建按钮工具栏
    QHBoxLayout *toolbarLayout = new QHBoxLayout;
    toolbarLayout->setSpacing(8);

    refreshButton = new QPushButton("刷新列表", this);
    deleteButton = new QPushButton("删除用户", this);
    editButton = new QPushButton("编辑用户", this);

    // 设置按钮图标和大小策略
    refreshButton->setMinimumWidth(100);
    deleteButton->setMinimumWidth(100);
    editButton->setMinimumWidth(100);

    toolbarLayout->addWidget(refreshButton);
    toolbarLayout->addWidget(deleteButton);
    toolbarLayout->addWidget(editButton);
    toolbarLayout->addStretch();

    mainLayout->addLayout(toolbarLayout);

    // 设置用户表格
    setupUserTable();
    mainLayout->addWidget(userTable);

    // 连接信号和槽
    connect(refreshButton, &QPushButton::clicked, this, &AdminWindow::refreshUserList);
    connect(deleteButton, &QPushButton::clicked, this, &AdminWindow::deleteSelectedUser);
    connect(editButton, &QPushButton::clicked, this, &AdminWindow::editSelectedUser);
}

void AdminWindow::setupUserTable()
{
    userTable = new QTableWidget(this);
    userTable->setColumnCount(3);
    userTable->setHorizontalHeaderLabels({"邮箱", "学号", "姓名"});
    userTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    userTable->setSelectionBehavior(QTableWidget::SelectRows);
    userTable->setSelectionMode(QTableWidget::SingleSelection);
    userTable->setEditTriggers(QTableWidget::NoEditTriggers);
    
    // Material Design风格的表格设置
    userTable->setShowGrid(true);
    userTable->setAlternatingRowColors(true);
    userTable->setStyleSheet(
        "QTableWidget { selection-background-color: #E3F2FD; }"
        "QTableWidget::item { border-bottom: 1px solid #E0E0E0; }"
        "QTableWidget::item:selected { background-color: #BBDEFB; color: black; }"
        "QTableWidget::item:hover { background-color: #F5F5F5; }"
    );
    
    // 添加表格动画效果
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 50));
    shadow->setOffset(0, 2);
    userTable->setGraphicsEffect(shadow);
}

void AdminWindow::refreshUserList()
{
    updateUserTable();
}

void AdminWindow::updateUserTable()
{
    QList<QMap<QString, QString>> users = userManager->getAllUsers();
    userTable->setRowCount(users.count());

    for (int i = 0; i < users.count(); ++i) {
        userTable->setItem(i, 0, new QTableWidgetItem(users[i]["email"]));
        userTable->setItem(i, 1, new QTableWidgetItem(users[i]["student_id"]));
        userTable->setItem(i, 2, new QTableWidgetItem(users[i]["name"]));
    }
}

void AdminWindow::deleteSelectedUser()
{
    int currentRow = userTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择要删除的用户");
        return;
    }

    QString email = userTable->item(currentRow, 0)->text();
    
    QMessageBox::StandardButton reply = QMessageBox::question(this, "确认",
        QString("确定要删除用户 %1 吗？").arg(email),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (userManager->deleteUser(email)) {
            QMessageBox::information(this, "成功", "用户已删除");
            refreshUserList();
        } else {
            QMessageBox::warning(this, "错误", "删除用户失败");
        }
    }
}

void AdminWindow::editSelectedUser()
{
    int currentRow = userTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "提示", "请先选择要编辑的用户");
        return;
    }

    QString email = userTable->item(currentRow, 0)->text();
    QString currentStudentId = userTable->item(currentRow, 1)->text();
    QString currentName = userTable->item(currentRow, 2)->text();

    bool ok;
    QString newStudentId = QInputDialog::getText(this, "编辑用户",
        "学号:", QLineEdit::Normal, currentStudentId, &ok);
    if (!ok) return;

    QString newName = QInputDialog::getText(this, "编辑用户",
        "姓名:", QLineEdit::Normal, currentName, &ok);
    if (!ok) return;

    if (userManager->updateUserInfo(email, newStudentId, newName)) {
        QMessageBox::information(this, "成功", "用户信息已更新");
        refreshUserList();
    } else {
        QMessageBox::warning(this, "错误", "更新用户信息失败");
    }
}
