#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include "usermanager.h"

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);

private slots:
    void refreshUserList();
    void deleteSelectedUser();
    void editSelectedUser();

private:
    void setupUI();
    void setupUserTable();
    void updateUserTable();

    UserManager *userManager;
    QTableWidget *userTable;
    QPushButton *refreshButton;
    QPushButton *deleteButton;
    QPushButton *editButton;
};

#endif // ADMINWINDOW_H