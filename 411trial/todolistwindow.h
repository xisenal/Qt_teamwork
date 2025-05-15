#ifndef TODOLISTWINDOW_H
#define TODOLISTWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

class TodoListWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TodoListWindow(QWidget *parent = nullptr);

private slots:
    void addTodoItem();
    void removeTodoItem();
    void toggleTodoItem(QListWidgetItem* item);

private:
    void setupUI();
    void loadTodos();
    void saveTodos();

    QListWidget *todoList;
    QLineEdit *todoInput;
    QPushButton *addButton;
    QPushButton *removeButton;
};

#endif // TODOLISTWINDOW_H