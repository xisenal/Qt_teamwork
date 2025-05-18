#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = nullptr);
    ~UserManager();

    bool initDatabase();
    bool registerUser(const QString &email, const QString &studentId,
                      const QString &name, const QString &password);
    bool createUserDirectory(const QString &email);
    bool validateLogin(const QString &email, const QString &password, bool &isAdmin);
    bool getUserInfo(const QString &email, QString &studentId, QString &name);
    bool getUserDetailInfo(const QString &email, QString &avatarPath, QString &schoolId, QString &description);
    QList<QMap<QString, QString>> getAllUsers();
    bool deleteUser(const QString &email);
    bool updateUserInfo(const QString &email, const QString &studentId, const QString &name);
    bool resetPassword(const QString &email, const QString &newPassword);

private:
    QSqlDatabase db;
    bool createTables();
};

#endif // USERMANAGER_H
