#include "adminwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>

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

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 创建按钮工具栏
    QHBoxLayout *toolbarLayout = new QHBoxLayout;
    refreshButton = new QPushButton("刷新列表", this);
    deleteButton = new QPushButton("删除用户", this);
    editButton = new QPushButton("编辑用户", this);

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