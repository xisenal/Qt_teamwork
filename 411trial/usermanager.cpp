#include "usermanager.h"
#include <QDebug>
#include <QCryptographicHash>

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
    return true;
}

bool UserManager::validateLogin(const QString &email, const QString &password)
{
    if (!db.isOpen() && !initDatabase())
        return false;

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