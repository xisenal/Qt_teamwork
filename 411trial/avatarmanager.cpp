#include "avatarmanager.h"

#include <QDebug>
#include <QCryptographicHash>
#include <QDateTime>

AvatarManager* AvatarManager::m_instance = nullptr;

AvatarManager* AvatarManager::instance()
{
    if (!m_instance) {
        m_instance = new AvatarManager();
    }
    return m_instance;
}

AvatarManager::AvatarManager(QObject *parent)
    : QObject(parent)
{
    settings = new QSettings(this);

    // 确保头像存储目录存在
    QDir dir;
    QString avatarDir = getAvatarStoragePath();
    if (!dir.exists(avatarDir)) {
        dir.mkpath(avatarDir);
    }
}

bool AvatarManager::saveAvatar(const QString &avatarPath)
{
    if (avatarPath.isEmpty()) {
        return false;
    }

    // 生成唯一的文件名
    QString timestamp = QString::number(QDateTime::currentSecsSinceEpoch());
    QFileInfo fileInfo(avatarPath);
    QString extension = fileInfo.suffix();
    QString newFileName = QString("avatar_%1.%2").arg(timestamp, extension);
    QString targetPath = getAvatarStoragePath() + "/" + newFileName;

    // 复制文件到应用数据目录
    if (QFile::copy(avatarPath, targetPath)) {
        // 删除旧的头像文件（如果存在）
        QString oldPath = getCurrentAvatarPath();
        if (!oldPath.isEmpty() && oldPath != getDefaultAvatarPath()) {
            QFile::remove(oldPath);
        }

        // 保存新头像路径到配置
        settings->setValue("avatar/path", targetPath);
        settings->sync();

        emit avatarChanged();
        return true;
    }

    return false;
}

QString AvatarManager::getCurrentAvatarPath() const
{
    QString customPath = settings->value("avatar/path", "").toString();

    // 检查自定义头像文件是否存在
    if (!customPath.isEmpty() && QFile::exists(customPath)) {
        return customPath;
    }

    // 返回默认头像
    return getDefaultAvatarPath();
}

QPixmap AvatarManager::getAvatarPixmap(int width, int height) const
{
    QString avatarPath = getCurrentAvatarPath();
    QPixmap pixmap(avatarPath);

    if (pixmap.isNull()) {
        // 如果加载失败，返回默认头像
        pixmap = QPixmap(getDefaultAvatarPath());
    }

    if (!pixmap.isNull()) {
        return pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // 如果都失败了，创建一个简单的默认图像
    QPixmap defaultPixmap(width, height);
    defaultPixmap.fill(Qt::lightGray);
    return defaultPixmap;
}

bool AvatarManager::hasCustomAvatar() const
{
    QString customPath = settings->value("avatar/path", "").toString();
    return !customPath.isEmpty() && QFile::exists(customPath);
}

QString AvatarManager::getAvatarStoragePath() const
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return dataPath + "/avatars";
}

QString AvatarManager::getDefaultAvatarPath() const
{
    return ":/resources/avatar.png";
}
