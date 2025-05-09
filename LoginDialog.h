#pragma once

#include <QDialog>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);

    QString getNickname() const { return nicknameEdit->text(); }
    QString getEmail() const { return emailEdit->text(); }
    QString getPassword() const { return passwordEdit->text(); }

public slots:
    void switchToLogin();
    void switchToRegister();

private:
    QStackedWidget *stack;
    QLineEdit *nicknameEdit;
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
};
