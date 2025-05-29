#ifndef AVATARMANAGER_H
#define AVATARMANAGER_H

#include <QObject>
#include <QString>
#include <QPixmap>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

class AvatarManager : public QObject
{
    Q_OBJECT

public:
    static AvatarManager* instance();

    // 保存头像到本地
    bool saveAvatar(const QString &avatarPath);

    // 获取当前头像路径
    QString getCurrentAvatarPath() const;

    // 获取头像QPixmap，指定大小
    QPixmap getAvatarPixmap(int width = 50, int height = 50) const;

    // 检查是否有自定义头像
    bool hasCustomAvatar() const;

signals:
    void avatarChanged();

private:
    explicit AvatarManager(QObject *parent = nullptr);
    static AvatarManager* m_instance;

    QString getAvatarStoragePath() const;
    QString getDefaultAvatarPath() const;

    QSettings *settings;
};

#endif // AVATARMANAGER_H
