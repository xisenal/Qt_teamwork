#ifndef PLUGINMANAGERDIALOG_H
#define PLUGINMANAGERDIALOG_H
// 如果你想让插件管理器继承QDialog而不是QMainWindow，可以这样修改：

#include "pluginmanager.h"


#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QFrame>
#include <QLineEdit>
#include <QTextEdit>
#include <QDialog>
#include <QSplitter>
#include <QGridLayout>
#include <QApplication>
#include <QStyleOption>
#include <QPainter>
// 插件管理器对话框版本
class PluginManagerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PluginManagerDialog(QWidget *parent = nullptr);
    ~PluginManagerDialog();

private slots:
    void onSearchTextChanged(const QString& text);
    void onPluginCardClicked(const PluginData& data);
    void onCategoryClicked();

private:
    void setupUI();
    void initializePlugins();
    void filterPlugins(const QString& searchText);
    void showAllPlugins();
    void showCategoryPlugins(const QString& category);

    QLineEdit* m_searchEdit;
    QScrollArea* m_pluginArea;
    QWidget* m_pluginContainer;
    QVBoxLayout* m_pluginLayout;
    QList<PluginData> m_plugins;
    QList<PluginCard*> m_pluginCards;

    // 分类按钮
    QPushButton* m_allBtn;
    QPushButton* m_editorBtn;
    QPushButton* m_languageBtn;
    QPushButton* m_themeBtn;
};

#endif // PLUGINMANAGERDIALOG_H
