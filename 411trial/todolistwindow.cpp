#include "todolistwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QMessageBox>

TodoListWindow::TodoListWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    loadTodos();
}

void TodoListWindow::setupUI()
{
    // 设置窗口属性
    setWindowTitle("To Do List");
    setMinimumSize(400, 600);

    // 创建中央部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    // 创建输入区域
    QHBoxLayout *inputLayout = new QHBoxLayout();
    todoInput = new QLineEdit(this);
    todoInput->setPlaceholderText("输入新的待办事项...");
    todoInput->setStyleSheet(
        "QLineEdit {"
        "    padding: 8px;"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 4px;"
        "    background: white;"
        "    font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "    border-color: #2196F3;"
        "}");

    addButton = new QPushButton("添加", this);
    addButton->setStyleSheet(
        "QPushButton {"
        "    padding: 8px 16px;"
        "    background-color: #2196F3;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #0D47A1;"
        "}");

    inputLayout->addWidget(todoInput);
    inputLayout->addWidget(addButton);

    // 创建列表
    todoList = new QListWidget(this);
    todoList->setStyleSheet(
        "QListWidget {"
        "    border: 1px solid #E0E0E0;"
        "    border-radius: 4px;"
        "    background: white;"
        "    padding: 5px;"
        "}"
        "QListWidget::item {"
        "    padding: 8px;"
        "    border-bottom: 1px solid #F5F5F5;"
        "}"
        "QListWidget::item:hover {"
        "    background: #F5F5F5;"
        "}"
        "QListWidget::item:selected {"
        "    background: #E3F2FD;"
        "    color: #2196F3;"
        "}");

    // 创建删除按钮
    removeButton = new QPushButton("删除选中项", this);
    removeButton->setStyleSheet(
        "QPushButton {"
        "    padding: 8px 16px;"
        "    background-color: #F44336;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #D32F2F;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #B71C1C;"
        "}");

    // 添加组件到主布局
    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(todoList);
    mainLayout->addWidget(removeButton);

    // 连接信号和槽
    connect(addButton, &QPushButton::clicked, this, &TodoListWindow::addTodoItem);
    connect(removeButton, &QPushButton::clicked, this, &TodoListWindow::removeTodoItem);
    connect(todoInput, &QLineEdit::returnPressed, this, &TodoListWindow::addTodoItem);
    connect(todoList, &QListWidget::itemClicked, this, &TodoListWindow::toggleTodoItem);
}

void TodoListWindow::addTodoItem()
{
    QString text = todoInput->text().trimmed();
    if (!text.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem(text);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        todoList->addItem(item);
        todoInput->clear();
        saveTodos();
    }
}

void TodoListWindow::removeTodoItem()
{
    QList<QListWidgetItem*> items = todoList->selectedItems();
    for (QListWidgetItem *item : items) {
        delete todoList->takeItem(todoList->row(item));
    }
    saveTodos();
}

void TodoListWindow::toggleTodoItem(QListWidgetItem* item)
{
    if (item) {
        item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        if (item->checkState() == Qt::Checked) {
            item->setForeground(Qt::gray);
            QFont font = item->font();
            font.setStrikeOut(true);
            item->setFont(font);
        } else {
            item->setForeground(Qt::black);
            QFont font = item->font();
            font.setStrikeOut(false);
            item->setFont(font);
        }
        saveTodos();
    }
}

void TodoListWindow::loadTodos()
{
    QSettings settings("TodoApp", "TodoList");
    int size = settings.beginReadArray("todos");
    
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString text = settings.value("text").toString();
        bool checked = settings.value("checked").toBool();
        
        QListWidgetItem *item = new QListWidgetItem(text);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
        
        if (checked) {
            item->setForeground(Qt::gray);
            QFont font = item->font();
            font.setStrikeOut(true);
            item->setFont(font);
        }
        
        todoList->addItem(item);
    }
    settings.endArray();
}

void TodoListWindow::saveTodos()
{
    QSettings settings("TodoApp", "TodoList");
    settings.beginWriteArray("todos");
    
    for (int i = 0; i < todoList->count(); ++i) {
        settings.setArrayIndex(i);
        QListWidgetItem *item = todoList->item(i);
        settings.setValue("text", item->text());
        settings.setValue("checked", item->checkState() == Qt::Checked);
    }
    settings.endArray();
}