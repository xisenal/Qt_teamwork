// pluginmanager.cpp
#include "pluginmanager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>

#include <QTimer>


// PluginCard 实现
PluginCard::PluginCard(const PluginData& data, QWidget *parent)
    : QFrame(parent), m_pluginData(data)
{
    PluginCard::setupUI();
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


LaTeXEditor::LaTeXEditor(QWidget *parent)
    : QDialog(parent)
    , m_latexProcess(nullptr)
    , m_previewTimer(nullptr)
    , m_autoPreviewEnabled(false)
    , m_titleLabel(nullptr)
    , m_savePathEdit(nullptr)
    , m_browsePathBtn(nullptr)
    , m_editor(nullptr)
    , m_preview(nullptr)
    , m_outputLog(nullptr)
    , m_rightPanel(nullptr)
    , m_compileBtn(nullptr)
    , m_previewBtn(nullptr)
    , m_autoPreviewBtn(nullptr)
    , m_saveBtn(nullptr)
    , m_clearLogBtn(nullptr)
    , m_statusLabel(nullptr)
{
    // 创建临时目录
    m_tempDir = QDir::tempPath() + "/latex_editor_" + QString::number(QCoreApplication::applicationPid());
    QDir().mkpath(m_tempDir);

    // 初始化预览定时器
    m_previewTimer = new QTimer(this);
    m_previewTimer->setSingleShot(true);
    m_previewTimer->setInterval(1000); // 1秒延迟
    connect(m_previewTimer, &QTimer::timeout, this, &LaTeXEditor::onAutoPreview);

    setupUI();
}

LaTeXEditor::~LaTeXEditor()
{
    if (m_latexProcess) {
        m_latexProcess->kill();
        m_latexProcess->waitForFinished(3000);
    }

    // 清理临时目录
    QDir tempDir(m_tempDir);
    if (tempDir.exists()) {
        tempDir.removeRecursively();
    }
}

void LaTeXEditor::setupUI()
{
    setWindowTitle("XLab LaTeX 编辑器");
    setMinimumSize(1200, 800);

    // Material Design 风格的样式表
    setStyleSheet(R"(
        QDialog {
            background-color: #fafafa;
            font-family: 'Segoe UI', 'Microsoft YaHei', sans-serif;
        }
        QLabel#titleLabel {
            font-size: 24px;
            font-weight: 500;
            color: #3f51b5;
            padding: 16px;
            text-align: center;
        }
        QLineEdit {
            border: none;
            border-bottom: 1px solid #bdbdbd;
            border-radius: 0;
            padding: 8px 4px;
            font-size: 14px;
            background-color: transparent;
            selection-background-color: #e3f2fd;
        }
        QLineEdit:focus {
            border-bottom: 2px solid #3f51b5;
        }
        QTextEdit {
            border: 1px solid #e0e0e0;
            border-radius: 4px;
            padding: 12px;
            font-family: 'Consolas', 'Source Code Pro', 'Microsoft YaHei', monospace;
            font-size: 14px;
            line-height: 1.5;
            background-color: #ffffff;
            selection-background-color: #e3f2fd;
        }
        QPushButton {
            background-color: #3f51b5;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-size: 14px;
            font-weight: 500;
            margin: 4px;
            min-width: 88px;
            min-height: 36px;
        }
        QPushButton:hover {
            background-color: #303f9f;
        }
        QPushButton:pressed {
            background-color: #1a237e;
        }
        QPushButton:disabled {
            background-color: #bdbdbd;
            color: #757575;
        }
        QPushButton:checked {
            background-color: #4caf50;
        }
        QPushButton#browseBtn {
            background-color: #f5f5f5;
            color: #3f51b5;
            border: 1px solid #e0e0e0;
            padding: 8px 12px;
        }
        QPushButton#browseBtn:hover {
            background-color: #e0e0e0;
        }
        QTabWidget::pane {
            border: 1px solid #e0e0e0;
            border-radius: 4px;
            top: -1px;
        }
        QTabBar::tab {
            background-color: #f5f5f5;
            border: 1px solid #e0e0e0;
            padding: 10px 20px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            margin-right: 2px;
            font-size: 14px;
        }
        QTabBar::tab:selected {
            background-color: #ffffff;
            border-bottom-color: #ffffff;
            color: #3f51b5;
        }
        QTabBar::tab:hover:!selected {
            background-color: #e0e0e0;
        }
        QSplitter::handle {
            background-color: #e0e0e0;
            width: 1px;
        }
        QLabel {
            font-size: 14px;
            color: #424242;
        }
        QLabel#statusLabel {
            font-weight: 500;
            padding: 4px 8px;
            border-radius: 4px;
        }
    )");

    // 使用卡片式布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    // 标题栏 - 更现代的设计
    QHBoxLayout* headerLayout = new QHBoxLayout();

    m_titleLabel = new QLabel("XLab LaTeX 编辑器");
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // 状态指示器移到标题栏右侧
    m_statusLabel = new QLabel("就绪");
    m_statusLabel->setObjectName("statusLabel");
    m_statusLabel->setStyleSheet("background-color: #e8f5e9; color: #2e7d32; padding: 4px 8px; border-radius: 4px;");

    headerLayout->addWidget(m_titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(m_statusLabel);

    mainLayout->addLayout(headerLayout);

    // 工具栏 - 重新排列为两行，更清晰的分组
    QFrame* toolbarCard = new QFrame();
    toolbarCard->setStyleSheet("QFrame { background-color: #ffffff; border-radius: 8px; padding: 16px; border: 1px solid #e0e0e0; }");
    QVBoxLayout* toolbarCardLayout = new QVBoxLayout(toolbarCard);
    toolbarCardLayout->setSpacing(12);

    // 保存路径设置栏 - 更清晰的布局
    QHBoxLayout* pathLayout = new QHBoxLayout();
    QLabel* pathLabel = new QLabel("默认保存路径:");
    pathLabel->setStyleSheet("font-weight: 500; margin-right: 8px;");

    m_defaultSavePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LaTeX";
    QDir().mkpath(m_defaultSavePath); // 确保目录存在

    m_savePathEdit = new QLineEdit(m_defaultSavePath);
    m_savePathEdit->setMinimumWidth(400);

    m_browsePathBtn = new QPushButton("浏览");
    m_browsePathBtn->setObjectName("browseBtn");
    m_browsePathBtn->setIcon(QIcon::fromTheme("folder-open"));

    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(m_savePathEdit, 1);
    pathLayout->addWidget(m_browsePathBtn);

    toolbarCardLayout->addLayout(pathLayout);

    // 分隔线
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #e0e0e0;");
    toolbarCardLayout->addWidget(line);

    // 操作按钮 - 分为两组：文档操作和预览操作
    QHBoxLayout* actionLayout = new QHBoxLayout();

    // 文档操作组
    QVBoxLayout* docActionsLayout = new QVBoxLayout();
    QLabel* docActionsLabel = new QLabel("文档操作");
    docActionsLabel->setStyleSheet("font-weight: 500; color: #757575; font-size: 12px;");

    QHBoxLayout* docButtonsLayout = new QHBoxLayout();
    m_compileBtn = new QPushButton("编译 PDF");
    m_compileBtn->setIcon(QIcon::fromTheme("document-export"));
    m_saveBtn = new QPushButton("保存文档");
    m_saveBtn->setIcon(QIcon::fromTheme("document-save"));

    docButtonsLayout->addWidget(m_compileBtn);
    docButtonsLayout->addWidget(m_saveBtn);
    docButtonsLayout->addStretch();

    docActionsLayout->addWidget(docActionsLabel);
    docActionsLayout->addLayout(docButtonsLayout);

    // 预览操作组
    QVBoxLayout* previewActionsLayout = new QVBoxLayout();
    QLabel* previewActionsLabel = new QLabel("预览控制");
    previewActionsLabel->setStyleSheet("font-weight: 500; color: #757575; font-size: 12px;");

    QHBoxLayout* previewButtonsLayout = new QHBoxLayout();
    m_previewBtn = new QPushButton("立即预览");
    m_previewBtn->setIcon(QIcon::fromTheme("view-preview"));
    m_autoPreviewBtn = new QPushButton("自动预览: 关");
    m_autoPreviewBtn->setIcon(QIcon::fromTheme("view-refresh"));
    m_autoPreviewBtn->setCheckable(true);
    m_clearLogBtn = new QPushButton("清除日志");
    m_clearLogBtn->setIcon(QIcon::fromTheme("edit-clear"));
    m_clearLogBtn->setStyleSheet("background-color: #f5f5f5; color: #616161; border: 1px solid #e0e0e0;");

    previewButtonsLayout->addWidget(m_previewBtn);
    previewButtonsLayout->addWidget(m_autoPreviewBtn);
    previewButtonsLayout->addWidget(m_clearLogBtn);
    previewButtonsLayout->addStretch();

    previewActionsLayout->addWidget(previewActionsLabel);
    previewActionsLayout->addLayout(previewButtonsLayout);

    // 将两组操作添加到主操作布局
    actionLayout->addLayout(docActionsLayout);
    actionLayout->addSpacing(24);  // 增加组之间的间距
    actionLayout->addLayout(previewActionsLayout);

    toolbarCardLayout->addLayout(actionLayout);
    mainLayout->addWidget(toolbarCard);

    // 主编辑区域 - 使用卡片式设计
    QFrame* editorCard = new QFrame();
    editorCard->setStyleSheet("QFrame { background-color: #ffffff; border-radius: 8px; padding: 16px; border: 1px solid #e0e0e0; }");
    QVBoxLayout* editorCardLayout = new QVBoxLayout(editorCard);

    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->setHandleWidth(2);
    splitter->setChildrenCollapsible(false);

    // 左侧编辑器 - 添加标题
    QWidget* editorContainer = new QWidget();
    QVBoxLayout* editorLayout = new QVBoxLayout(editorContainer);
    editorLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* editorLabel = new QLabel("LaTeX 源代码");
    editorLabel->setStyleSheet("font-weight: 500; color: #3f51b5; font-size: 14px; margin-bottom: 8px;");

    m_editor = new QTextEdit();
    m_editor->setPlaceholderText(R"(% LaTeX 文档示例
\documentclass[12pt]{article}
\usepackage[utf8]{inputenc}
\usepackage[T1]{fontenc}
\usepackage{amsmath}
\usepackage{geometry}
\geometry{a4paper, margin=1in}

\title{我的 LaTeX 文档}
\author{作者姓名}
\date{\today}

\begin{document}

\maketitle

\section{引言}
这是一个 LaTeX 文档示例。LaTeX 是一个高质量的排版系统，特别适合制作科技和数学文档。

\section{数学公式}
爱因斯坦的质能方程：
\begin{equation}
E = mc^2
\end{equation}

勾股定理：
\begin{equation}
a^2 + b^2 = c^2
\end{equation}

\section{列表}
\begin{itemize}
    \item 第一项
    \item 第二项
    \item 第三项
\end{itemize}

\section{结论}
LaTeX 能够产生高质量的文档输出。

\end{document})");

    editorLayout->addWidget(editorLabel);
    editorLayout->addWidget(m_editor);

    // 右侧面板（选项卡）- 添加标题
    QWidget* previewContainer = new QWidget();
    QVBoxLayout* previewLayout = new QVBoxLayout(previewContainer);
    previewLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* previewLabel = new QLabel("预览与输出");
    previewLabel->setStyleSheet("font-weight: 500; color: #3f51b5; font-size: 14px; margin-bottom: 8px;");

    m_rightPanel = new QTabWidget();
    m_rightPanel->setDocumentMode(true);

    // 预览选项卡
    m_preview = new QTextEdit();
    m_preview->setReadOnly(true);
    m_preview->setPlaceholderText("实时预览将在此显示...\n\n");

    // 输出日志选项卡
    m_outputLog = new QTextEdit();
    m_outputLog->setReadOnly(true);
    m_outputLog->setPlaceholderText("编译输出和错误信息将在此显示...");
    m_outputLog->setStyleSheet(m_outputLog->styleSheet() + "font-family: 'Consolas', 'Source Code Pro', 'Microsoft YaHei', monospace; font-size: 12px;");

    m_rightPanel->addTab(m_preview, "预览");
    m_rightPanel->addTab(m_outputLog, "编译日志");

    previewLayout->addWidget(previewLabel);
    previewLayout->addWidget(m_rightPanel);

    // 将编辑器和预览面板添加到分割器
    splitter->addWidget(editorContainer);
    splitter->addWidget(previewContainer);
    splitter->setSizes({600, 600});

    editorCardLayout->addWidget(splitter);
    mainLayout->addWidget(editorCard, 1);  // 设置拉伸因子，使编辑区域占据更多空间

    // 连接信号槽
    connect(m_compileBtn, &QPushButton::clicked, this, &LaTeXEditor::onCompileClicked);
    connect(m_previewBtn, &QPushButton::clicked, this, &LaTeXEditor::onPreviewClicked);
    connect(m_autoPreviewBtn, &QPushButton::toggled, this, &LaTeXEditor::onAutoPreviewToggled);
    connect(m_saveBtn, &QPushButton::clicked, this, &LaTeXEditor::onSaveClicked);
    connect(m_clearLogBtn, &QPushButton::clicked, [this]() {
        m_outputLog->clear();
    });
    connect(m_browsePathBtn, &QPushButton::clicked, this, &LaTeXEditor::onBrowsePathClicked);
    connect(m_editor, &QTextEdit::textChanged, this, &LaTeXEditor::onTextChanged);
}

void LaTeXEditor::onBrowsePathClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    "选择默认保存目录",
                                                    m_savePathEdit->text(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        m_savePathEdit->setText(dir);
        m_defaultSavePath = dir;
    }
}

void LaTeXEditor::onTextChanged()
{
    if (m_autoPreviewEnabled) {
        m_previewTimer->stop();
        m_previewTimer->start(); // 重新开始计时
    }
}

void LaTeXEditor::onAutoPreviewToggled(bool enabled)
{
    m_autoPreviewEnabled = enabled;
    if (enabled) {
        m_autoPreviewBtn->setText("自动预览: 开");
        m_autoPreviewBtn->setStyleSheet("background-color: #4caf50; color: white; border: none; border-radius: 4px; padding: 8px 16px; font-size: 14px; font-weight: 500; margin: 4px; min-width: 88px; min-height: 36px;");
        onAutoPreview(); // 立即预览一次
        m_statusLabel->setText("自动预览已启用");
        m_statusLabel->setStyleSheet("background-color: #e8f5e9; color: #2e7d32; padding: 4px 8px; border-radius: 4px;");
    } else {
        m_autoPreviewBtn->setText("自动预览: 关");
        m_autoPreviewBtn->setStyleSheet("background-color: #3f51b5; color: white; border: none; border-radius: 4px; padding: 8px 16px; font-size: 14px; font-weight: 500; margin: 4px; min-width: 88px; min-height: 36px;");
        m_previewTimer->stop();
        m_statusLabel->setText("自动预览已关闭");
        m_statusLabel->setStyleSheet("background-color: #e3f2fd; color: #1565c0; padding: 4px 8px; border-radius: 4px;");
    }
}

void LaTeXEditor::onAutoPreview()
{
    if (!m_autoPreviewEnabled) return;

    QString content = m_editor->toPlainText();
    if (content.trimmed().isEmpty()) return;

    // 生成HTML预览
    generateHTMLPreview(content);
}

void LaTeXEditor::generateHTMLPreview(const QString& latexContent)
{
    QString html = convertLaTeXToHTML(latexContent);
    m_preview->setHtml(html);

    // 切换到预览标签页
    m_rightPanel->setCurrentIndex(0);
}

QString LaTeXEditor::convertLaTeXToHTML(const QString& latex)
{
    QString html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>LaTeX Preview</title>
    <style>
        body {
            font-family: 'Times New Roman', serif;
            max-width: 800px;
            margin: 20px auto;
            padding: 20px;
            line-height: 1.6;
            background-color: white;
        }
        h1 { font-size: 2em; text-align: center; margin-bottom: 0.5em; }
        h2 { font-size: 1.5em; margin-top: 1.5em; margin-bottom: 0.5em; }
        h3 { font-size: 1.2em; margin-top: 1.2em; margin-bottom: 0.4em; }
        .author { text-align: center; font-style: italic; margin-bottom: 1em; }
        .date { text-align: center; margin-bottom: 2em; }
        .equation {
            text-align: center;
            margin: 1em 0;
            font-family: 'Times New Roman', serif;
            font-style: italic;
        }
        ul { margin: 1em 0; padding-left: 2em; }
        li { margin: 0.3em 0; }
        .warning {
            background-color: #fff3cd;
            border: 1px solid #ffeaa7;
            color: #856404;
            padding: 10px;
            border-radius: 4px;
            margin: 10px 0;
        }
    </style>
</head>
<body>
)";

    QString content = latex;

    // 基本转换规则
    QStringList lines = content.split('\n');
    QString body;
    bool inDocument = false;
    bool inEquation = false;
    bool inItemize = false;

    for (const QString& line : lines) {
        QString trimmedLine = line.trimmed();

        if (trimmedLine.contains("\\begin{document}")) {
            inDocument = true;
            continue;
        }
        if (trimmedLine.contains("\\end{document}")) {
            break;
        }
        if (!inDocument) continue;

        // 标题处理
        if (trimmedLine.contains("\\maketitle")) {
            body += "<div class='title-section'></div>\n";
        }
        else if (trimmedLine.startsWith("\\title{")) {
            QString title = extractBraceContent(trimmedLine, "\\title{");
            body += "<h1>" + title + "</h1>\n";
        }
        else if (trimmedLine.startsWith("\\author{")) {
            QString author = extractBraceContent(trimmedLine, "\\author{");
            body += "<div class='author'>" + author + "</div>\n";
        }
        else if (trimmedLine.startsWith("\\date{")) {
            QString date = extractBraceContent(trimmedLine, "\\date{");
            if (date == "\\today") date = QDate::currentDate().toString("yyyy年MM月dd日");
            body += "<div class='date'>" + date + "</div>\n";
        }
        // 章节处理
        else if (trimmedLine.startsWith("\\section{")) {
            QString section = extractBraceContent(trimmedLine, "\\section{");
            body += "<h2>" + section + "</h2>\n";
        }
        else if (trimmedLine.startsWith("\\subsection{")) {
            QString subsection = extractBraceContent(trimmedLine, "\\subsection{");
            body += "<h3>" + subsection + "</h3>\n";
        }
        // 公式处理
        else if (trimmedLine.contains("\\begin{equation}")) {
            inEquation = true;
            body += "<div class='equation'>\n";
        }
        else if (trimmedLine.contains("\\end{equation}")) {
            inEquation = false;
            body += "</div>\n";
        }
        // 列表处理
        else if (trimmedLine.contains("\\begin{itemize}")) {
            inItemize = true;
            body += "<ul>\n";
        }
        else if (trimmedLine.contains("\\end{itemize}")) {
            inItemize = false;
            body += "</ul>\n";
        }
        else if (trimmedLine.startsWith("\\item")) {
            QString item = trimmedLine.mid(5).trimmed();
            body += "<li>" + item + "</li>\n";
        }
        // 普通文本
        else if (!trimmedLine.isEmpty() && !trimmedLine.startsWith("%") && !trimmedLine.startsWith("\\")) {
            if (inEquation) {
                body += "<em>" + trimmedLine + "</em><br>\n";
            } else {
                body += "<p>" + trimmedLine + "</p>\n";
            }
        }
    }

    //html += "<div class='warning'>⚠️ 这是简化的HTML预览，实际LaTeX输出可能有所不同。点击"编译PDF"查看准确结果。</div>\n";
    html += body;
    html += "</body></html>";

    return html;
}

QString LaTeXEditor::extractBraceContent(const QString& line, const QString& command)
{
    int start = line.indexOf(command);
    if (start == -1) return "";

    start += command.length();
    int braceCount = 0;
    int end = start;

    for (int i = start; i < line.length(); ++i) {
        if (line[i] == '{') braceCount++;
        else if (line[i] == '}') {
            braceCount--;
            if (braceCount == 0) {
                end = i;
                break;
            }
        }
    }

    return line.mid(start, end - start);
}

void LaTeXEditor::onPreviewClicked()
{
    QString content = m_editor->toPlainText();
    if (content.trimmed().isEmpty()) {
        m_statusLabel->setText("无内容可预览");
        m_statusLabel->setStyleSheet("background-color: #fff8e1; color: #f57f17; padding: 4px 8px; border-radius: 4px;");
        return;
    }

    generateHTMLPreview(content);
    m_statusLabel->setText("预览已更新");
    m_statusLabel->setStyleSheet("background-color: #e3f2fd; color: #1565c0; padding: 4px 8px; border-radius: 4px;");

    // 切换到预览标签页
    m_rightPanel->setCurrentIndex(0);
}

// 保持原有的编译相关方法不变
bool LaTeXEditor::compileLaTeX(const QString& content)
{
    // 创建临时 .tex 文件
    m_currentFile = m_tempDir + "/document.tex";
    QFile file(m_currentFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_outputLog->append("错误：无法创建临时文件");
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << content;
    file.close();

    // 设置状态
    m_statusLabel->setText("编译中...");
    m_statusLabel->setStyleSheet("background-color: #fff8e1; color: #f57f17; padding: 4px 8px; border-radius: 4px;");
    m_compileBtn->setEnabled(false);

    // 创建进程
    if (m_latexProcess) {
        m_latexProcess->kill();
        m_latexProcess->deleteLater();
    }

    m_latexProcess = new QProcess(this);
    connect(m_latexProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &LaTeXEditor::onProcessFinished);
    connect(m_latexProcess, &QProcess::errorOccurred, this, &LaTeXEditor::onProcessError);

    // 设置工作目录
    m_latexProcess->setWorkingDirectory(m_tempDir);

    // 执行 pdflatex
    QStringList arguments;
    arguments << "-interaction=nonstopmode" << "-output-directory=" + m_tempDir << m_currentFile;

    m_outputLog->append("开始编译...");
    m_outputLog->append("命令: pdflatex " + arguments.join(" "));
    m_outputLog->append("工作目录: " + m_tempDir);
    m_outputLog->append("========================================");

    m_latexProcess->start("pdflatex", arguments);

    return true;
}

void LaTeXEditor::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    m_compileBtn->setEnabled(true);

    if (exitStatus == QProcess::CrashExit) {
        m_statusLabel->setText("编译崩溃");
        m_statusLabel->setStyleSheet("background-color: #ffebee; color: #c62828; padding: 4px 8px; border-radius: 4px;");
        m_outputLog->append("编译进程崩溃！");
        return;
    }

    // 获取输出
    QString output = QString::fromLocal8Bit(m_latexProcess->readAllStandardOutput());
    QString errorOutput = QString::fromLocal8Bit(m_latexProcess->readAllStandardError());

    m_outputLog->append("标准输出:");
    m_outputLog->append(output);
    if (!errorOutput.isEmpty()) {
        m_outputLog->append("错误输出:");
        m_outputLog->append(errorOutput);
    }
    m_outputLog->append("========================================");

    if (exitCode == 0) {
        m_statusLabel->setText("编译成功");
        m_statusLabel->setStyleSheet("background-color: #e8f5e9; color: #2e7d32; padding: 4px 8px; border-radius: 4px;");

        // 检查是否生成了 PDF 文件
        QString pdfFile = m_tempDir + "/document.pdf";
        if (QFile::exists(pdfFile)) {
            m_outputLog->append("PDF 文件已生成: " + pdfFile);
            m_preview->setText("编译成功！\n\nPDF 文件已生成：\n" + pdfFile +
                               "\n\n请使用系统默认的 PDF 阅读器打开查看。");
        } else {
            m_outputLog->append("警告：编译成功但未找到 PDF 文件");
        }
    } else {
        m_statusLabel->setText("编译失败");
        m_statusLabel->setStyleSheet("background-color: #ffebee; color: #c62828; padding: 4px 8px; border-radius: 4px;");
        m_preview->setText("编译失败，请查看编译日志获取详细错误信息。");
    }

    m_latexProcess->deleteLater();
    m_latexProcess = nullptr;
}

void LaTeXEditor::onProcessError(QProcess::ProcessError error)
{
    m_compileBtn->setEnabled(true);
    m_statusLabel->setText("编译错误");
    m_statusLabel->setStyleSheet("background-color: #ffebee; color: #c62828; padding: 4px 8px; border-radius: 4px;");

    QString errorString;
    switch (error) {
    case QProcess::FailedToStart:
        errorString = "无法启动 pdflatex。请确保已安装 LaTeX 发行版。";
        break;
    case QProcess::Crashed:
        errorString = "pdflatex 进程崩溃";
        break;
    case QProcess::Timedout:
        errorString = "pdflatex 进程超时";
        break;
    case QProcess::ReadError:
        errorString = "读取 pdflatex 输出时出错";
        break;
    case QProcess::WriteError:
        errorString = "写入 pdflatex 输入时出错";
        break;
    default:
        errorString = "未知的 pdflatex 错误";
        break;
    }

    m_outputLog->append("进程错误: " + errorString);
    m_preview->setText("编译失败：" + errorString);
}

void LaTeXEditor::onCompileClicked()
{
    QString content = m_editor->toPlainText();
    if (content.trimmed().isEmpty()) {
        m_outputLog->append("错误：编辑器内容为空");
        return;
    }

    compileLaTeX(content);
}

void LaTeXEditor::onSaveClicked()
{
    QString defaultFileName = m_savePathEdit->text() + "/document.tex";
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "保存 LaTeX 文件",
                                                    defaultFileName,
                                                    "LaTeX Files (*.tex);;All Files (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setEncoding(QStringConverter::Utf8);
            out << m_editor->toPlainText();
            file.close();
            m_outputLog->append("文件已保存: " + fileName);
            m_statusLabel->setText("已保存");
            m_statusLabel->setStyleSheet("background-color: #e8f5e9; color: #2e7d32; padding: 4px 8px; border-radius: 4px;");
        } else {
            m_outputLog->append("错误：无法保存文件 " + fileName);
            m_statusLabel->setText("保存失败");
            m_statusLabel->setStyleSheet("background-color: #ffebee; color: #c62828; padding: 4px 8px; border-radius: 4px;");
        }
    }
}






// bool LaTeXEditor::compileLaTeX(const QString& content)
// {
//     // 创建临时 .tex 文件
//     m_currentFile = m_tempDir + "/document.tex";
//     QFile file(m_currentFile);
//     if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//         m_outputLog->append("错误：无法创建临时文件");
//         return false;
//     }

//     QTextStream out(&file);
//     //out.setCodec("UTF-8");
//     out.setEncoding(QStringConverter::Utf8);
//     out << content;
//     file.close();

//     // 设置状态
//     m_statusLabel->setText("编译中...");
//     m_statusLabel->setStyleSheet("color: #ffc107; font-weight: bold; margin-left: 10px;");
//     m_compileBtn->setEnabled(false);

//     // 创建进程
//     if (m_latexProcess) {
//         m_latexProcess->kill();
//         m_latexProcess->deleteLater();
//     }

//     m_latexProcess = new QProcess(this);
//     connect(m_latexProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
//             this, &LaTeXEditor::onProcessFinished);
//     connect(m_latexProcess, &QProcess::errorOccurred, this, &LaTeXEditor::onProcessError);

//     // 设置工作目录
//     m_latexProcess->setWorkingDirectory(m_tempDir);

//     // 执行 pdflatex
//     QStringList arguments;
//     arguments << "-interaction=nonstopmode" << "-output-directory=" + m_tempDir << m_currentFile;

//     m_outputLog->append("开始编译...");
//     m_outputLog->append("命令: pdflatex " + arguments.join(" "));
//     m_outputLog->append("工作目录: " + m_tempDir);
//     m_outputLog->append("========================================");

//     m_latexProcess->start("pdflatex", arguments);

//     return true;
// }

// void LaTeXEditor::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
// {
//     m_compileBtn->setEnabled(true);

//     if (exitStatus == QProcess::CrashExit) {
//         m_statusLabel->setText("编译崩溃");
//         m_statusLabel->setStyleSheet("color: #dc3545; font-weight: bold; margin-left: 10px;");
//         m_outputLog->append("编译进程崩溃！");
//         return;
//     }

//     // 获取输出
//     QString output = QString::fromLocal8Bit(m_latexProcess->readAllStandardOutput());
//     QString errorOutput = QString::fromLocal8Bit(m_latexProcess->readAllStandardError());

//     m_outputLog->append("标准输出:");
//     m_outputLog->append(output);
//     if (!errorOutput.isEmpty()) {
//         m_outputLog->append("错误输出:");
//         m_outputLog->append(errorOutput);
//     }
//     m_outputLog->append("========================================");

//     if (exitCode == 0) {
//         m_statusLabel->setText("编译成功");
//         m_statusLabel->setStyleSheet("color: #28a745; font-weight: bold; margin-left: 10px;");

//         // 检查是否生成了 PDF 文件
//         QString pdfFile = m_tempDir + "/document.pdf";
//         if (QFile::exists(pdfFile)) {
//             m_outputLog->append("PDF 文件已生成: " + pdfFile);
//             m_preview->setText("编译成功！\n\nPDF 文件已生成：\n" + pdfFile +
//                                "\n\n请使用系统默认的 PDF 阅读器打开查看。\n\n" +
//                                "注意：当前版本暂不支持在界面内直接预览 PDF，" +
//                                "建议使用外部 PDF 阅读器查看编译结果。");
//         } else {
//             m_outputLog->append("警告：编译成功但未找到 PDF 文件");
//         }
//     } else {
//         m_statusLabel->setText("编译失败");
//         m_statusLabel->setStyleSheet("color: #dc3545; font-weight: bold; margin-left: 10px;");
//         m_preview->setText("编译失败，请查看编译日志获取详细错误信息。\n\n常见问题：\n1. 检查 LaTeX 语法是否正确\n2. 确保安装了 pdflatex\n3. 检查包依赖是否满足");
//     }

//     m_latexProcess->deleteLater();
//     m_latexProcess = nullptr;
// }

// void LaTeXEditor::onProcessError(QProcess::ProcessError error)
// {
//     m_compileBtn->setEnabled(true);
//     m_statusLabel->setText("编译错误");
//     m_statusLabel->setStyleSheet("color: #dc3545; font-weight: bold; margin-left: 10px;");

//     QString errorString;
//     switch (error) {
//     case QProcess::FailedToStart:
//         errorString = "无法启动 pdflatex。请确保已安装 LaTeX 发行版（如 TeX Live 或 MiKTeX）并且 pdflatex 在系统 PATH 中。";
//         break;
//     case QProcess::Crashed:
//         errorString = "pdflatex 进程崩溃";
//         break;
//     case QProcess::Timedout:
//         errorString = "pdflatex 进程超时";
//         break;
//     case QProcess::ReadError:
//         errorString = "读取 pdflatex 输出时出错";
//         break;
//     case QProcess::WriteError:
//         errorString = "写入 pdflatex 输入时出错";
//         break;
//     default:
//         errorString = "未知的 pdflatex 错误";
//         break;
//     }

//     m_outputLog->append("进程错误: " + errorString);
//     m_outputLog->append("========================================");
//     m_preview->setText("编译失败：" + errorString + "\n\n解决方案：\n1. 安装 LaTeX 发行版（推荐 TeX Live）\n2. 确保 pdflatex 命令可在命令行中运行\n3. 重启应用程序");
// }

// void LaTeXEditor::onCompileClicked()
// {
//     QString content = m_editor->toPlainText();
//     if (content.trimmed().isEmpty()) {
//         m_outputLog->append("错误：编辑器内容为空");
//         return;
//     }

//     compileLaTeX(content);
// }

// void LaTeXEditor::onPreviewClicked()
// {
//     QString content = m_editor->toPlainText();
//     if (content.trimmed().isEmpty()) {
//         return;
//     }

//     // 实时预览（简化版本，显示源码结构）
//     m_preview->setText("LaTeX 源码预览：\n\n" + content +
//                        "\n\n提示：点击'编译 PDF'按钮进行完整编译。");
//     m_rightPanel->setCurrentIndex(0); // 切换到预览标签页
// }

// // void LaTeXEditor::onSaveClicked()
// // {
// //     QString fileName = QFileDialog::getSaveFileName(this,
// //                                                     "保存 LaTeX 文件",
// //                                                     "document.tex",
// //                                                     "LaTeX Files (*.tex);;All Files (*)");
// //     if (!fileName.isEmpty()) {
// //         QFile file(fileName);
// //         if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
// //             QTextStream out(&file);
// //             //out.setCodec("UTF-8");
// //             out.setEncoding(QStringConverter::Utf8);
// //             out << m_editor->toPlainText();
// //             file.close();
// //             m_outputLog->append("文件已保存: " + fileName);
// //             m_statusLabel->setText("已保存");
// //             m_statusLabel->setStyleSheet("color: #28a745; font-weight: bold; margin-left: 10px;");
// //         } else {
// //             m_outputLog->append("错误：无法保存文件 " + fileName);
// //         }
// //     }
// // }








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




