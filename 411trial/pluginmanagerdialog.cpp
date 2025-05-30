#include "pluginmanagerdialog.h"


// 对话框版本的实现
PluginManagerDialog::PluginManagerDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    initializePlugins();
    showAllPlugins();
}

PluginManagerDialog::~PluginManagerDialog()
{
}

void PluginManagerDialog::setupUI()
{
    setWindowTitle("插件管理器");
    setMinimumSize(900, 600);
    setAttribute(Qt::WA_DeleteOnClose);

    setStyleSheet(R"(
        QDialog {
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

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 标题和关闭按钮
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("插件管理器");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #24292f; margin-bottom: 10px;");

    QPushButton* closeBtn = new QPushButton("关闭");
    closeBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #dc3545;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 6px 12px;
            font-size: 12px;
        }
        QPushButton:hover {
            background-color: #c82333;
        }
    )");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::close);

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(closeBtn);

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
    m_pluginLayout->setSpacing(8);
    m_pluginLayout->setContentsMargins(0, 0, 0, 0);

    m_pluginArea->setWidget(m_pluginContainer);

    mainLayout->addLayout(titleLayout);
    mainLayout->addWidget(m_searchEdit);
    mainLayout->addLayout(categoryLayout);
    mainLayout->addWidget(m_pluginArea);

    connect(m_searchEdit, &QLineEdit::textChanged, this, &PluginManagerDialog::onSearchTextChanged);
    connect(m_allBtn, &QPushButton::clicked, this, &PluginManagerDialog::onCategoryClicked);
    connect(m_editorBtn, &QPushButton::clicked, this, &PluginManagerDialog::onCategoryClicked);
    connect(m_languageBtn, &QPushButton::clicked, this, &PluginManagerDialog::onCategoryClicked);
    connect(m_themeBtn, &QPushButton::clicked, this, &PluginManagerDialog::onCategoryClicked);
}

// 其他函数实现与PluginManager相同，只需要把类名改为PluginManagerDialog

// // PluginManager 实现
// PluginManager::PluginManager(QWidget *parent)
//     : QMainWindow(parent)
// {
//     setupUI();
//     initializePlugins();
//     showAllPlugins();
// }

// PluginManager::~PluginManager()
// {
// }

// void PluginManager::setupUI()
// {
//     setWindowTitle("插件管理器");
//     setMinimumSize(900, 600);
//     setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除

//     setStyleSheet(R"(
//         QMainWindow {
//             background-color: #f6f8fa;
//         }
//         QLineEdit {
//             border: 1px solid #d0d7de;
//             border-radius: 4px;
//             padding: 8px 12px;
//             font-size: 12px;
//             background-color: white;
//         }
//         QLineEdit:focus {
//             border-color: #0366d6;
//             outline: none;
//         }
//         QPushButton {
//             background-color: #fafbfc;
//             color: #24292f;
//             border: 1px solid #d0d7de;
//             border-radius: 4px;
//             padding: 8px 16px;
//             font-size: 12px;
//             font-weight: 500;
//             margin: 2px;
//         }
//         QPushButton:hover {
//             background-color: #f3f4f6;
//         }
//         QPushButton:checked {
//             background-color: #0366d6;
//             color: white;
//             border-color: #0366d6;
//         }
//     )");

//     m_centralWidget = new QWidget();
//     setCentralWidget(m_centralWidget);

//     QVBoxLayout* mainLayout = new QVBoxLayout(m_centralWidget);
//     mainLayout->setSpacing(16);
//     mainLayout->setContentsMargins(20, 20, 20, 20);

//     // 标题
//     QLabel* titleLabel = new QLabel("插件管理器");
//     titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #24292f; margin-bottom: 10px;");

//     // 搜索栏
//     m_searchEdit = new QLineEdit();
//     m_searchEdit->setPlaceholderText("搜索插件...");
//     m_searchEdit->setFixedHeight(40);

//     // 分类按钮
//     QHBoxLayout* categoryLayout = new QHBoxLayout();

//     m_allBtn = new QPushButton("全部");
//     m_editorBtn = new QPushButton("编辑器");
//     m_languageBtn = new QPushButton("语言支持");
//     m_themeBtn = new QPushButton("主题");

//     m_allBtn->setCheckable(true);
//     m_editorBtn->setCheckable(true);
//     m_languageBtn->setCheckable(true);
//     m_themeBtn->setCheckable(true);

//     m_allBtn->setChecked(true);

//     categoryLayout->addWidget(m_allBtn);
//     categoryLayout->addWidget(m_editorBtn);
//     categoryLayout->addWidget(m_languageBtn);
//     categoryLayout->addWidget(m_themeBtn);
//     categoryLayout->addStretch();

//     // 插件列表区域
//     m_pluginArea = new QScrollArea();
//     m_pluginArea->setWidgetResizable(true);
//     m_pluginArea->setStyleSheet(R"(
//         QScrollArea {
//             border: none;
//             background-color: transparent;
//         }
//     )");

//     m_pluginContainer = new QWidget();
//     m_pluginLayout = new QVBoxLayout(m_pluginContainer);
//     m_pluginLayout->setSpacing(8);
//     m_pluginLayout->setContentsMargins(0, 0, 0, 0);

//     m_pluginArea->setWidget(m_pluginContainer);

//     mainLayout->addWidget(titleLabel);
//     mainLayout->addWidget(m_searchEdit);
//     mainLayout->addLayout(categoryLayout);
//     mainLayout->addWidget(m_pluginArea);

//     connect(m_searchEdit, &QLineEdit::textChanged, this, &PluginManager::onSearchTextChanged);
//     connect(m_allBtn, &QPushButton::clicked, this, &PluginManager::onCategoryClicked);
//     connect(m_editorBtn, &QPushButton::clicked, this, &PluginManager::onCategoryClicked);
//     connect(m_languageBtn, &QPushButton::clicked, this, &PluginManager::onCategoryClicked);
//     connect(m_themeBtn, &QPushButton::clicked, this, &PluginManager::onCategoryClicked);
// }

void PluginManagerDialog::initializePlugins()
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

void PluginManagerDialog::showAllPlugins()
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
        connect(card, &PluginCard::pluginClicked, this, &PluginManagerDialog::onPluginCardClicked);
        m_pluginCards.append(card);
        m_pluginLayout->addWidget(card);
    }

    m_pluginLayout->addStretch();
}

void PluginManagerDialog::showCategoryPlugins(const QString& category)
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
            connect(card, &PluginCard::pluginClicked, this, &PluginManagerDialog::onPluginCardClicked);
            m_pluginCards.append(card);
            m_pluginLayout->addWidget(card);
        }
    }

    m_pluginLayout->addStretch();
}

void PluginManagerDialog::filterPlugins(const QString& searchText)
{
    for (PluginCard* card : m_pluginCards) {
        bool visible = searchText.isEmpty() ||
                       card->property("name").toString().contains(searchText, Qt::CaseInsensitive) ||
                       card->property("description").toString().contains(searchText, Qt::CaseInsensitive);
        card->setVisible(visible);
    }
}

void PluginManagerDialog::onSearchTextChanged(const QString& text)
{
    filterPlugins(text);
}

void PluginManagerDialog::onPluginCardClicked(const PluginData& data)
{
    if (data.name == "LaTeX Workshop") {
        LaTeXEditor* editor = new LaTeXEditor(this);
        editor->show();
    } else {
        qDebug() << "打开插件：" << data.name;
    }
}

void PluginManagerDialog::onCategoryClicked()
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
