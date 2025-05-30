#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <QtPlugin>
#include <QString>
#include <QWidget>
#include <QJsonObject>

class PluginInterface
{
public:
    virtual ~PluginInterface() = default;

    // 插件基本信息
    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual QString description() const = 0;
    virtual QString author() const = 0;

    // 插件功能
    virtual bool initialize() = 0;
    virtual void cleanup() = 0;

    // 获取插件的主要功能组件
    virtual QWidget* createWidget(QWidget* parent = nullptr) = 0;

    // 插件配置
    virtual QJsonObject defaultSettings() const = 0;
    virtual void applySettings(const QJsonObject& settings) = 0;

    // 插件能力查询
    virtual QStringList supportedFeatures() const = 0;

    // 与主程序交互的接口
    virtual void setHostInterface(QObject* hostInterface) = 0;
};

// 定义插件接口的 IID (Interface ID)
#define PluginInterface_IID "com.yourcompany.yourapp.PluginInterface/1.0"
Q_DECLARE_INTERFACE(PluginInterface, PluginInterface_IID)



//5.30

//需要实现所有纯虚函数来构建具体的插件

#endif // PLUGIN_INTERFACE_H
