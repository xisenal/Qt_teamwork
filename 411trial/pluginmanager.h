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
#include <QProcess>

//#include <QWebEngineView>


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

// // LaTeX编辑器窗口
// class LaTeXEditor : public QDialog
// {
//     Q_OBJECT

// public:
//     explicit LaTeXEditor(QWidget *parent = nullptr);

// private slots:
//     void onPreviewClicked();
//     void onCompileClicked();
//     void onSaveClicked();

// private:
//     void setupUI();
//     QTextEdit* m_editor;
//     QTextEdit* m_preview;
//     QPushButton* m_previewBtn;
//     QPushButton* m_compileBtn;
//     QPushButton* m_saveBtn;
// };



// // LaTeX编辑器窗口
// class LaTeXEditor : public QDialog
// {
//     Q_OBJECT

// public:
//     explicit LaTeXEditor(QWidget *parent = nullptr);

// private slots:
//     void onPreviewClicked();
//     void onCompileClicked();
//     void onSaveClicked();
//     void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
//     void onProcessError(QProcess::ProcessError error);

//     // 在private slots部分添加
//     void onTextChanged();
//     void onAutoPreviewToggled(bool enabled);
//     void onAutoPreview();
//     void onBrowsePathClicked();

// private:
//     void setupUI();
//     bool compileLaTeX(const QString& content);
//     void showCompileOutput();


//     QTextEdit* m_editor;
//     QTextEdit* m_preview;
//     QTextEdit* m_outputLog;
//     QPushButton* m_previewBtn;
//     QPushButton* m_compileBtn;
//     QPushButton* m_saveBtn;
//     QPushButton* m_clearLogBtn;
//     QTabWidget* m_rightPanel;
//     QProcess* m_latexProcess;
//     QString m_tempDir;
//     QString m_currentFile;
//     QLabel* m_statusLabel;




// private:
//     QLabel* m_titleLabel;
//     QLineEdit* m_savePathEdit;
//     QPushButton* m_browsePathBtn;
//     QString m_defaultSavePath;




// private:

//     QWebEngineView* m_pdfViewer;  // 需要包含 QWebEngineView 或使用其他PDF查看组件


//     QTimer* m_previewTimer;
//     QPushButton* m_autoPreviewBtn;
//     bool m_autoPreviewEnabled;


//     void generateHTMLPreview(const QString& latexContent);
//     QString convertLaTeXToHTML(const QString& latex);
//     QString extractBraceContent(const QString& line, const QString& command);

// };



#ifndef LATEXEDITOR_H
#define LATEXEDITOR_H

#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QProcess>
#include <QTimer>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringConverter>
#include <QDate>
#include <QCoreApplication>

class LaTeXEditor : public QDialog
{
    Q_OBJECT

public:
    explicit LaTeXEditor(QWidget *parent = nullptr);
    ~LaTeXEditor();

private slots:
    void onCompileClicked();
    void onPreviewClicked();
    void onSaveClicked();
    void onBrowsePathClicked();
    void onTextChanged();
    void onAutoPreviewToggled(bool enabled);
    void onAutoPreview();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);

private:
    void setupUI();
    bool compileLaTeX(const QString& content);
    void generateHTMLPreview(const QString& latexContent);
    QString convertLaTeXToHTML(const QString& latex);
    QString extractBraceContent(const QString& line, const QString& command);

    // UI组件
    QLabel* m_titleLabel;
    QLineEdit* m_savePathEdit;
    QPushButton* m_browsePathBtn;
    QTextEdit* m_editor;
    QTextEdit* m_preview;
    QTextEdit* m_outputLog;
    QTabWidget* m_rightPanel;
    QPushButton* m_compileBtn;
    QPushButton* m_previewBtn;
    QPushButton* m_autoPreviewBtn;
    QPushButton* m_saveBtn;
    QPushButton* m_clearLogBtn;
    QLabel* m_statusLabel;

    // 功能组件
    QProcess* m_latexProcess;
    QTimer* m_previewTimer;

    // 数据成员
    QString m_tempDir;
    QString m_currentFile;
    QString m_defaultSavePath;
    bool m_autoPreviewEnabled;
};

#endif // LATEXEDITOR_H




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
