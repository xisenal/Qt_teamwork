// pluginmanager.h
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

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

// 插件数据结构
struct PluginData {
    QString name;
    QString version;
    QString description;
    QString icon;
    bool installed;
    QString category;
};

// 插件卡片组件
class PluginCard : public QFrame
{
    Q_OBJECT

public:
    explicit PluginCard(const PluginData& data, QWidget *parent = nullptr);

private slots:
    void onInstallClicked();
    void onOpenClicked();

signals:
    void pluginClicked(const PluginData& data);

private:
    void setupUI();
    PluginData m_pluginData;
    QPushButton* m_installBtn;
    QPushButton* m_openBtn;
};

// LaTeX编辑器窗口
class LaTeXEditor : public QDialog
{
    Q_OBJECT

public:
    explicit LaTeXEditor(QWidget *parent = nullptr);

private slots:
    void onPreviewClicked();
    void onCompileClicked();
    void onSaveClicked();

private:
    void setupUI();
    QTextEdit* m_editor;
    QTextEdit* m_preview;
    QPushButton* m_previewBtn;
    QPushButton* m_compileBtn;
    QPushButton* m_saveBtn;
};

// 主窗口
class PluginManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit PluginManager(QWidget *parent = nullptr);
    ~PluginManager();

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

    QWidget* m_centralWidget;
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

#endif // PLUGINMANAGER_H
