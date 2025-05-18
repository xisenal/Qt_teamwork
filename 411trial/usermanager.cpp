#include "usermanager.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>

UserManager::UserManager(QObject *parent)
    : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");
}

UserManager::~UserManager()
{
    if (db.isOpen())
        db.close();
}

bool UserManager::initDatabase()
{
    if (!db.open()) {
        qDebug() << "Error: Failed to connect database" << db.lastError();
        return false;
    }

    return createTables();
}

bool UserManager::createTables()
{
    QSqlQuery query;
    QString sql = "CREATE TABLE IF NOT EXISTS users ("
                  "email TEXT PRIMARY KEY,"
                  "student_id TEXT NOT NULL,"
                  "name TEXT NOT NULL,"
                  "password TEXT NOT NULL)";

    if (!query.exec(sql)) {
        qDebug() << "Error: Failed to create table" << query.lastError();
        return false;
    }
    return true;
}

bool UserManager::createUserDirectory(const QString &email)
{
    qDebug() << "开始创建用户目录，用户邮箱:" << email;

    // 首先确保应用程序根目录下的userdata目录存在
    QDir appDir(QCoreApplication::applicationDirPath());
    qDebug() << "应用程序目录路径:" << appDir.absolutePath();

    if (!appDir.exists("userdata")) {
        qDebug() << "userdata目录不存在，尝试创建...";
        if (!appDir.mkdir("userdata")) {
            qDebug() << "Error: Failed to create userdata directory at" << appDir.absolutePath();
            return false;
        }
        qDebug() << "userdata目录创建成功";
    } else {
        qDebug() << "userdata目录已存在";
    }

    QString userDataPath = appDir.filePath("userdata/" + email);
    QDir userDir(userDataPath);
    qDebug() << "用户目录路径:" << userDataPath;

    // 创建用户主目录
    if (!userDir.exists()) {
        qDebug() << "用户目录不存在，尝试创建...";
        if (!userDir.mkpath(".")) {
            qDebug() << "Error: Failed to create user directory at" << userDataPath;
            return false;
        }
        qDebug() << "用户目录创建成功";
    } else {
        qDebug() << "用户目录已存在";
    }

    // 创建子目录
    QStringList subDirs = {"pic"};
    for (const QString &subDir : subDirs) {
        qDebug() << "尝试创建子目录:" << subDir;
        if (!userDir.mkpath(subDir)) {
            qDebug() << "Error: Failed to create subdirectory:" << subDir;
            return false;
        }
        qDebug() << "子目录" << subDir << "创建成功";
    }

    // 复制默认头像到用户的pic目录
    QString defaultAvatarPath = ":/resources/user.png";
    QString userAvatarPath = userDataPath + "/pic/user.png";
    qDebug() << "尝试复制默认头像到:" << userAvatarPath;
    if (!QFile::copy(defaultAvatarPath, userAvatarPath)) {
        qDebug() << "Error: Failed to copy default avatar to user directory";
        return false;
    }
    qDebug() << "默认头像复制成功";

    // 创建默认的用户信息JSON文件
    QString userInfoPath = userDataPath + "/user_info.json";
    qDebug() << "尝试创建用户信息文件:" << userInfoPath;
    QFile userInfoFile(userInfoPath);
    if (userInfoFile.open(QIODevice::WriteOnly)) {
        QJsonObject userInfo;
        userInfo["email"] = email;
        userInfo["avatar"] = "pic/user.png";
        userInfo["description"] = "用户9527";
        userInfo["schoolId"] = "xxxxxxxxxx";

        QJsonDocument doc(userInfo);
        userInfoFile.write(doc.toJson());
        userInfoFile.close();
        qDebug() << "用户信息文件创建成功";
    } else {
        qDebug() << "Error: Failed to create user info file at" << userInfoPath;
        return false;
    }

    qDebug() << "用户目录结构创建完成";
    return true;
}

bool UserManager::registerUser(const QString &email, const QString &studentId,
                               const QString &name, const QString &password)
{
    if (!db.isOpen() && !initDatabase())
        return false;

    QSqlQuery query;
    query.prepare("INSERT INTO users (email, student_id, name, password) "
                  "VALUES (?, ?, ?, ?)");

    // Hash the password before storing
    QString hashedPassword = QString(QCryptographicHash::hash(
                                         password.toUtf8(), QCryptographicHash::Sha256).toHex());

    query.addBindValue(email);
    query.addBindValue(studentId);
    query.addBindValue(name);
    query.addBindValue(hashedPassword);

    if (!query.exec()) {
        qDebug() << "Error: Failed to register user" << query.lastError();
        return false;
    }

    // 创建用户目录结构
    if (!createUserDirectory(email)) {
        // 如果创建目录失败，回滚用户注册
        query.prepare("DELETE FROM users WHERE email = ?");
        query.addBindValue(email);
        query.exec();
        return false;
    }

    return true;
}

bool UserManager::validateLogin(const QString &email, const QString &password, bool &isAdmin)
{
    if (!db.isOpen() && !initDatabase())
        return false;

    // 检查是否是管理员账户
    if (email == "xlab@xlab.com" && password == "XLab") {
        isAdmin = true;
        return true;
    }

    isAdmin = false;
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE email = ?");
    query.addBindValue(email);

    if (!query.exec() || !query.next()) {
        return false;
    }

    QString storedHash = query.value(0).toString();
    QString inputHash = QString(QCryptographicHash::hash(
                                    password.toUtf8(), QCryptographicHash::Sha256).toHex());

    return storedHash == inputHash;
}

QList<QMap<QString, QString>> UserManager::getAllUsers()
{
    QList<QMap<QString, QString>> userList;
    if (!db.isOpen() && !initDatabase())
        return userList;

    QSqlQuery query;
    query.prepare("SELECT email, student_id, name FROM users");

    if (!query.exec()) {
        qDebug() << "Error: Failed to get users" << query.lastError();
        return userList;
    }

    while (query.next()) {
        QMap<QString, QString> user;
        user["email"] = query.value(0).toString();
        user["student_id"] = query.value(1).toString();
        user["name"] = query.value(2).toString();
        userList.append(user);
    }

    return userList;
}

bool UserManager::deleteUser(const QString &email)
{
    if (!db.isOpen() && !initDatabase())
        return false;

    // 首先删除用户目录
    QDir appDir(QCoreApplication::applicationDirPath());
    QString userDataPath = appDir.filePath("userdata/" + email);
    QDir userDir(userDataPath);
    
    if (userDir.exists()) {
        qDebug() << "正在删除用户目录:" << userDataPath;
        if (!userDir.removeRecursively()) {
            qDebug() << "Error: Failed to delete user directory at" << userDataPath;
            return false;
        }
        qDebug() << "用户目录删除成功";
    }

    // 然后删除数据库记录
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE email = ?");
    query.addBindValue(email);

    if (!query.exec()) {
        qDebug() << "Error: Failed to delete user" << query.lastError();
        return false;
    }

    return true;
}

bool UserManager::updateUserInfo(const QString &email, const QString &studentId, const QString &name)
{
    if (!db.isOpen() && !initDatabase())
        return false;

    QSqlQuery query;
    query.prepare("UPDATE users SET student_id = ?, name = ? WHERE email = ?");
    query.addBindValue(studentId);
    query.addBindValue(name);
    query.addBindValue(email);

    if (!query.exec()) {
        qDebug() << "Error: Failed to update user info" << query.lastError();
        return false;
    }

    return true;
}

bool UserManager::getUserInfo(const QString &email, QString &studentId, QString &name)
{
    if (!db.isOpen() && !initDatabase())
        return false;

    QSqlQuery query;
    query.prepare("SELECT student_id, name FROM users WHERE email = ?");
    query.addBindValue(email);

    if (!query.exec() || !query.next()) {
        return false;
    }

    studentId = query.value(0).toString();
    name = query.value(1).toString();
    return true;
}

bool UserManager::getUserDetailInfo(const QString &email, QString &avatarPath, QString &schoolId, QString &description)
{
    // 获取应用程序目录
    QDir appDir(QCoreApplication::applicationDirPath());
    QString userDataPath = appDir.filePath("userdata/" + email);
    QString userInfoPath = userDataPath + "/user_info.json";

    // 读取用户信息JSON文件
    QFile userInfoFile(userInfoPath);
    if (!userInfoFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Error: Failed to open user info file at" << userInfoPath;
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(userInfoFile.readAll());
    userInfoFile.close();

    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Error: Invalid JSON format in user info file";
        return false;
    }

    QJsonObject userInfo = doc.object();
    
    // 获取头像完整路径
    QString relativeAvatarPath = userInfo["avatar"].toString();
    avatarPath = userDataPath + "/" + relativeAvatarPath;
    
    // 获取学号和昵称
    schoolId = userInfo["schoolId"].toString();
    description = userInfo["description"].toString();

    return true;
}

bool UserManager::resetPassword(const QString &email, const QString &newPassword)
{
    if (!db.isOpen() && !initDatabase())
        return false;

    QSqlQuery query;
    query.prepare("UPDATE users SET password = ? WHERE email = ?");

    // Hash the new password before storing
    QString hashedPassword = QString(QCryptographicHash::hash(
                                         newPassword.toUtf8(), QCryptographicHash::Sha256).toHex());

    query.addBindValue(hashedPassword);
    query.addBindValue(email);

    if (!query.exec()) {
        qDebug() << "Error: Failed to reset password" << query.lastError();
        return false;
    }

    return true;
}
