// pluginmanager.cpp
#include "pluginmanager.h"
#include <QDebug>

// PluginCard 实现
PluginCard::PluginCard(const PluginData& data, QWidget *parent)
    : QFrame(parent), m_pluginData(data)
{
    setupUI();
}

void PluginCard::setupUI()
{
    setFrameStyle(QFrame::Box);
    setStyleSheet(R"(
        QFrame {
            background-color: #ffffff;
            border: 1px solid #e1e4e8;
            border-radius: 8px;
            margin: 4px;
        }
        QFrame:hover {
            border-color: #0366d6;
        }
    )");

    setFixedHeight(220);
    setMinimumWidth(450);
    setMaximumWidth(800);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(12, 12, 12, 12);

    // 头部：图标和标题
    QHBoxLayout* headerLayout = new QHBoxLayout();

    // 图标
    QLabel* iconLabel = new QLabel(m_pluginData.icon);
    iconLabel->setStyleSheet(R"(
        QLabel {
            background-color: #f6f8fa;
            border: 1px solid #d0d7de;
            border-radius: 4px;
            padding: 8px;
            font-size: 16px;
            font-weight: bold;
            color: #24292f;
        }
    )");
    iconLabel->setFixedSize(56, 56);
    iconLabel->setAlignment(Qt::AlignCenter);

    // 名称和版本
    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(2);

    QLabel* nameLabel = new QLabel(m_pluginData.name);
    nameLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #24292f;");

    QLabel* versionLabel = new QLabel("v" + m_pluginData.version);
    versionLabel->setStyleSheet("font-size: 11px; color: #656d76;");

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(versionLabel);

    headerLayout->addWidget(iconLabel);
    headerLayout->addLayout(infoLayout);
    headerLayout->addStretch();

    // 描述
    QLabel* descLabel = new QLabel(m_pluginData.description);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("font-size: 11px; color: #656d76; line-height: 1.4;");
    descLabel->setMaximumHeight(60);
    descLabel->setMinimumHeight(40);

    // 按钮区域
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    m_installBtn = new QPushButton(m_pluginData.installed ? "已安装" : "安装");
    m_installBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #0366d6;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 6px 12px;
            font-size: 11px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #0256cc;
        }
        QPushButton:pressed {
            background-color: #024ea4;
        }
    )");

    if (m_pluginData.installed) {
        m_installBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #28a745;
                color: white;
                border: none;
                border-radius: 4px;
                padding: 6px 12px;
                font-size: 11px;
                font-weight: 500;
            }
        )");
    }

    m_openBtn = new QPushButton("打开");
    m_openBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #fafbfc;
            color: #24292f;
            border: 1px solid #d0d7de;
            border-radius: 4px;
            padding: 6px 12px;
            font-size: 11px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #f3f4f6;
            border-color: #d0d7de;
        }
    )");
    m_openBtn->setVisible(m_pluginData.installed);

    buttonLayout->addWidget(m_installBtn);
    buttonLayout->addWidget(m_openBtn);
    buttonLayout->addStretch();

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(descLabel);
    mainLayout->addLayout(buttonLayout);

    connect(m_installBtn, &QPushButton::clicked, this, &PluginCard::onInstallClicked);
    connect(m_openBtn, &QPushButton::clicked, this, &PluginCard::onOpenClicked);
}

void PluginCard::onInstallClicked()
{
    if (!m_pluginData.installed) {
        m_pluginData.installed = true;
        m_installBtn->setText("已安装");
        m_installBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #28a745;
                color: white;
                border: none;
                border-radius: 4px;
                padding: 6px 12px;
                font-size: 11px;
                font-weight: 500;
            }
        )");
        m_openBtn->setVisible(true);
    }
}

void PluginCard::onOpenClicked()
{
    emit pluginClicked(m_pluginData);
}

// LaTeXEditor 实现
LaTeXEditor::LaTeXEditor(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
}

void LaTeXEditor::setupUI()
{
    setWindowTitle("LaTeX 编辑器");
    setMinimumSize(1000, 700);

    setStyleSheet(R"(
        QDialog {
            background-color: #ffffff;
        }
        QTextEdit {
            border: 1px solid #d0d7de;
            border-radius: 4px;
            padding: 8px;
            font-family: 'Consolas', 'Monaco', monospace;
            font-size: 12px;
            line-height: 1.4;
        }
        QPushButton {
            background-color: #0366d6;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-size: 12px;
            font-weight: 500;
            margin: 2px;
        }
        QPushButton:hover {
            background-color: #0256cc;
        }
    )");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 工具栏
    QHBoxLayout* toolbarLayout = new QHBoxLayout();

    m_compileBtn = new QPushButton("编译");
    m_previewBtn = new QPushButton("预览");
    m_saveBtn = new QPushButton("保存");

    toolbarLayout->addWidget(m_compileBtn);
    toolbarLayout->addWidget(m_previewBtn);
    toolbarLayout->addWidget(m_saveBtn);
    toolbarLayout->addStretch();

    // 编辑区域
    QSplitter* splitter = new QSplitter(Qt::Horizontal);

    m_editor = new QTextEdit();
    m_editor->setPlaceholderText("在此输入LaTeX代码...\n\n例如:\n\\documentclass{article}\n\\begin{document}\n\\title{我的文档}\n\\author{作者}\n\\maketitle\n\n\\section{简介}\n这是一个LaTeX文档示例。\n\n\\subsection{数学公式}\n爱因斯坦质能方程：$E = mc^2$\n\n\\end{document}");

    m_preview = new QTextEdit();
    m_preview->setReadOnly(true);
    m_preview->setPlaceholderText("编译预览将在此显示...");

    splitter->addWidget(m_editor);
    splitter->addWidget(m_preview);
    splitter->setSizes({500, 500});

    mainLayout->addLayout(toolbarLayout);
    mainLayout->addWidget(splitter);

    connect(m_compileBtn, &QPushButton::clicked, this, &LaTeXEditor::onCompileClicked);
    connect(m_previewBtn, &QPushButton::clicked, this, &LaTeXEditor::onPreviewClicked);
    connect(m_saveBtn, &QPushButton::clicked, this, &LaTeXEditor::onSaveClicked);
}

void LaTeXEditor::onCompileClicked()
{
    QString content = m_editor->toPlainText();
    // 这里可以调用实际的LaTeX编译器
    m_preview->setText("编译结果：\n" + content + "\n\n[注：这里应该是编译后的输出结果]");
}

void LaTeXEditor::onPreviewClicked()
{
    QString content = m_editor->toPlainText();
    // 简单的预览功能
    m_preview->setText("预览：\n" + content);
}

void LaTeXEditor::onSaveClicked()
{
    // 这里可以添加保存文件的逻辑
    qDebug() << "保存LaTeX文档";
}

// PluginManager 实现
PluginManager::PluginManager(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    initializePlugins();
    showAllPlugins();
}

PluginManager::~PluginManager()
{
}

void PluginManager::setupUI()
{
    setWindowTitle("插件管理器");
    setMinimumSize(1000, 700);
    setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除

    setStyleSheet(R"(
        QMainWindow {
            background-color: #f6f8fa;
        }
        QLineEdit {
            border: 1px solid #d0d7de;
            border-radius: 4px;
            padding: 8px 12px;
            font-size: 12px;
            background-color: white;
        }
        QLineEdit:focus {
            border-color: #0366d6;
            outline: none;
        }
        QPushButton {
            background-color: #fafbfc;
            color: #24292f;
            border: 1px solid #d0d7de;
            border-radius: 4px;
            padding: 8px 16px;
            font-size: 12px;
            font-weight: 500;
            margin: 2px;
        }
        QPushButton:hover {
            background-color: #f3f4f6;
        }
        QPushButton:checked {
            background-color: #0366d6;
            color: white;
            border-color: #0366d6;
        }
    )");

    m_centralWidget = new QWidget();
    setCentralWidget(m_centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(30, 20, 30, 20);

    // 标题
    QLabel* titleLabel = new QLabel("插件管理器");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #24292f; margin-bottom: 10px;");

    // 搜索栏
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("搜索插件...");
    m_searchEdit->setFixedHeight(40);

    // 分类按钮
    QHBoxLayout* categoryLayout = new QHBoxLayout();

    m_allBtn = new QPushButton("全部");
    m_editorBtn = new QPushButton("编辑器");
    m_languageBtn = new QPushButton("语言支持");
    m_themeBtn = new QPushButton("主题");

    m_allBtn->setCheckable(true);
    m_editorBtn->setCheckable(true);
    m_languageBtn->setCheckable(true);
    m_themeBtn->setCheckable(true);

    m_allBtn->setChecked(true);

    categoryLayout->addWidget(m_allBtn);
    categoryLayout->addWidget(m_editorBtn);
    categoryLayout->addWidget(m_languageBtn);
    categoryLayout->addWidget(m_themeBtn);
    categoryLayout->addStretch();

    // 插件列表区域
    m_pluginArea = new QScrollArea();
    m_pluginArea->setWidgetResizable(true);
    m_pluginArea->setStyleSheet(R"(
        QScrollArea {
            border: none;
            background-color: transparent;
        }
    )");

    m_pluginContainer = new QWidget();
    m_pluginLayout = new QVBoxLayout(m_pluginContainer);
    m_pluginLayout->setSpacing(12);
    m_pluginLayout->setContentsMargins(10, 10, 10, 10);

    m_pluginArea->setWidget(m_pluginContainer);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(m_searchEdit);
    mainLayout->addLayout(categoryLayout);
    mainLayout->addWidget(m_pluginArea);

    connect(m_searchEdit, &QLineEdit::textChanged, this, &PluginManager::onSearchTextChanged);
    connect(m_allBtn, &QPushButton::clicked, this, &PluginManager::onCategoryClicked);
    connect(m_editorBtn, &QPushButton::clicked, this, &PluginManager::onCategoryClicked);
    connect(m_languageBtn, &QPushButton::clicked, this, &PluginManager::onCategoryClicked);
    connect(m_themeBtn, &QPushButton::clicked, this, &PluginManager::onCategoryClicked);
}

void PluginManager::initializePlugins()
{
    m_plugins = {
        {"LaTeX Workshop", "3.51.0", "LaTeX编辑和编译工具，支持语法高亮、自动补全和实时预览", "TeX", true, "语言支持"},
        {"Python", "2023.8.0", "Python语言支持，包括智能感知、调试和代码格式化", "Py", false, "语言支持"},
        {"C/C++", "1.15.4", "C和C++语言支持，提供IntelliSense和调试功能", "C++", true, "语言支持"},
        {"One Dark Pro", "3.15.8", "流行的深色主题，护眼且美观", "🎨", false, "主题"},
        {"Material Theme", "34.2.1", "Material Design风格的主题包", "🎯", false, "主题"},
        {"Prettier", "9.13.0", "代码格式化工具，支持多种编程语言", "✨", true, "编辑器"},
        {"Auto Rename Tag", "0.1.10", "自动重命名HTML/XML标签对", "🏷️", false, "编辑器"},
        {"GitLens", "14.1.1", "增强Git功能，显示代码作者和历史", "Git", true, "编辑器"}
    };
}

void PluginManager::showAllPlugins()
{
    // 清除现有卡片
    for (PluginCard* card : m_pluginCards) {
        m_pluginLayout->removeWidget(card);
        card->deleteLater();
    }
    m_pluginCards.clear();

    // 添加所有插件卡片
    for (const PluginData& plugin : m_plugins) {
        PluginCard* card = new PluginCard(plugin);
        connect(card, &PluginCard::pluginClicked, this, &PluginManager::onPluginCardClicked);
        m_pluginCards.append(card);
        m_pluginLayout->addWidget(card);
    }

    m_pluginLayout->addStretch();
}

void PluginManager::showCategoryPlugins(const QString& category)
{
    // 清除现有卡片
    for (PluginCard* card : m_pluginCards) {
        m_pluginLayout->removeWidget(card);
        card->deleteLater();
    }
    m_pluginCards.clear();

    // 添加分类插件卡片
    for (const PluginData& plugin : m_plugins) {
        if (plugin.category == category) {
            PluginCard* card = new PluginCard(plugin);
            connect(card, &PluginCard::pluginClicked, this, &PluginManager::onPluginCardClicked);
            m_pluginCards.append(card);
            m_pluginLayout->addWidget(card);
        }
    }

    m_pluginLayout->addStretch();
}

void PluginManager::filterPlugins(const QString& searchText)
{
    for (PluginCard* card : m_pluginCards) {
        bool visible = searchText.isEmpty() ||
                       card->property("name").toString().contains(searchText, Qt::CaseInsensitive) ||
                       card->property("description").toString().contains(searchText, Qt::CaseInsensitive);
        card->setVisible(visible);
    }
}

void PluginManager::onSearchTextChanged(const QString& text)
{
    filterPlugins(text);
}

void PluginManager::onPluginCardClicked(const PluginData& data)
{
    if (data.name == "LaTeX Workshop") {
        LaTeXEditor* editor = new LaTeXEditor(this);
        editor->show();
    } else {
        qDebug() << "打开插件：" << data.name;
    }
}

void PluginManager::onCategoryClicked()
{
    QPushButton* sender = qobject_cast<QPushButton*>(QObject::sender());

    // 取消其他按钮的选中状态
    m_allBtn->setChecked(false);
    m_editorBtn->setChecked(false);
    m_languageBtn->setChecked(false);
    m_themeBtn->setChecked(false);

    // 设置当前按钮为选中状态
    sender->setChecked(true);

    if (sender == m_allBtn) {
        showAllPlugins();
    } else if (sender == m_editorBtn) {
        showCategoryPlugins("编辑器");
    } else if (sender == m_languageBtn) {
        showCategoryPlugins("语言支持");
    } else if (sender == m_themeBtn) {
        showCategoryPlugins("主题");
    }
}




