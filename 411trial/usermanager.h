#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = nullptr);
    ~UserManager();

    bool initDatabase();
    bool registerUser(const QString &email, const QString &studentId, 
                     const QString &name, const QString &password);
    bool validateLogin(const QString &email, const QString &password, bool &isAdmin);
    bool getUserInfo(const QString &email, QString &studentId, QString &name);
    QList<QMap<QString, QString>> getAllUsers();
    bool deleteUser(const QString &email);
    bool updateUserInfo(const QString &email, const QString &studentId, const QString &name);
    bool resetPassword(const QString &email, const QString &newPassword);

private:
    QSqlDatabase db;
    bool createTables();
};

#endif // USERMANAGER_H