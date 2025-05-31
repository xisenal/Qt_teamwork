#include "ProfileDialog.h"
#include "changepassworddialog.h"
#include "avatarmanager.h"
#include <QPainter>

QPixmap createRoundPixmap(const QPixmap& source, int size) {
    // 创建一个正方形的透明pixmap
    QPixmap target(size, size);
    target.fill(Qt::transparent);

    // 创建painter
    QPainter painter(&target);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 设置圆形裁剪区域
    painter.setClipRegion(QRegion(QRect(0, 0, size, size), QRegion::Ellipse));

    // 绘制缩放后的图片
    QPixmap scaledSource = source.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    // 居中绘制（如果图片不是正方形，确保居中显示）
    int x = (size - scaledSource.width()) / 2;
    int y = (size - scaledSource.height()) / 2;
    painter.drawPixmap(x, y, scaledSource);

    return target;
}



ProfileDialog::ProfileDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setupStyles();
    loadUserInfo();

    setFixedSize(400, 500);
    setWindowTitle("个人资料");
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

ProfileDialog::~ProfileDialog()
{
}

void ProfileDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 创建主容器
    QFrame *containerFrame = new QFrame();
    containerFrame->setObjectName("containerFrame");

    QVBoxLayout *containerLayout = new QVBoxLayout(containerFrame);
    containerLayout->setSpacing(20);
    containerLayout->setContentsMargins(20, 20, 20, 20);

    // 头部布局
    headerLayout = new QHBoxLayout();
    titleLabel = new QLabel("个人资料");
    titleLabel->setObjectName("titleLabel");
    closeBtn = new QPushButton("×");
    closeBtn->setObjectName("closeBtn");
    closeBtn->setFixedSize(30, 30);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(closeBtn);

    // 头像区域
    QVBoxLayout *avatarLayout = new QVBoxLayout();
    avatarBtn = new QPushButton();
    avatarBtn->setObjectName("avatarBtn");
    avatarBtn->setFixedSize(80, 80);
    avatarBtn->setText("点击更换");

    // 设置图标大小
    avatarBtn->setIconSize(QSize(75, 75));  // 比按钮稍小一点


    QLabel *avatarHint = new QLabel("点击更换头像");
    avatarHint->setObjectName("avatarHint");
    avatarHint->setAlignment(Qt::AlignCenter);

    avatarLayout->addWidget(avatarBtn, 0, Qt::AlignCenter);
    avatarLayout->addWidget(avatarHint, 0, Qt::AlignCenter);

    // 表单布局
    formLayout = new QGridLayout();
    formLayout->setSpacing(15);
    formLayout->setColumnStretch(1, 1);

    // 昵称
    QLabel *nicknameLabel = new QLabel("昵称:");
    nicknameLabel->setObjectName("fieldLabel");
    nicknameEdit = new QLineEdit();
    nicknameEdit->setObjectName("fieldEdit");
    nicknameEdit->setPlaceholderText("请输入昵称");

    // 邮箱
    QLabel *emailLabel = new QLabel("邮箱:");
    emailLabel->setObjectName("fieldLabel");
    emailEdit = new QLineEdit();
    emailEdit->setObjectName("fieldEdit");
    emailEdit->setPlaceholderText("请输入邮箱地址");

    // 手机号
    QLabel *phoneLabel = new QLabel("手机:");
    phoneLabel->setObjectName("fieldLabel");
    phoneEdit = new QLineEdit();
    phoneEdit->setObjectName("fieldEdit");
    phoneEdit->setPlaceholderText("请输入手机号码");

    // 修改密码按钮

    QLabel *pwLabel = new QLabel("密码:");
    pwLabel->setStyleSheet("font-size: 14px");
    pwLabel->setObjectName("passwordLabel");
    changePasswordBtn = new QPushButton("修改密码");
    changePasswordBtn->setObjectName("changePasswordBtn");

    formLayout->addWidget(nicknameLabel, 0, 0);
    formLayout->addWidget(nicknameEdit, 0, 1);
    formLayout->addWidget(emailLabel, 1, 0);
    formLayout->addWidget(emailEdit, 1, 1);
    formLayout->addWidget(phoneLabel, 2, 0);
    formLayout->addWidget(phoneEdit, 2, 1);
    formLayout->addWidget(pwLabel, 3, 0);
    formLayout->addWidget(changePasswordBtn, 3, 1);

    // 底部按钮
    buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    cancelBtn = new QPushButton("取消");
    cancelBtn->setObjectName("cancelBtn");
    saveBtn = new QPushButton("保存");
    saveBtn->setObjectName("saveBtn");

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(saveBtn);

    // 组装布局
    containerLayout->addLayout(headerLayout);
    containerLayout->addLayout(avatarLayout);
    containerLayout->addLayout(formLayout);
    containerLayout->addStretch();
    containerLayout->addLayout(buttonLayout);

    mainLayout->addWidget(containerFrame);

    // 连接信号
    connect(closeBtn, &QPushButton::clicked, this, &ProfileDialog::reject);
    connect(avatarBtn, &QPushButton::clicked, this, &ProfileDialog::onAvatarClicked);
    connect(changePasswordBtn, &QPushButton::clicked, this, &ProfileDialog::onChangePasswordClicked);
    connect(saveBtn, &QPushButton::clicked, this, &ProfileDialog::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &ProfileDialog::onCancelClicked);
}

void ProfileDialog::setupStyles()
{
    setStyleSheet(R"(
        QFrame#containerFrame {
            background-color: white;
            border-radius: 12px;
            border: 1px solid #e0e0e0;
        }

        QLabel#titleLabel {
            font-size: 18px;
            font-weight: bold;
            color: #2c2c2c;
            padding: 5px 0px;
        }

        QPushButton#closeBtn {
            background-color: transparent;
            border: none;
            font-size: 20px;
            font-weight: bold;
            color: #999999;
            border-radius: 15px;
        }

        QPushButton#closeBtn:hover {
            background-color: #f5f5f5;
            color: #666666;
        }

        QPushButton#avatarBtn {
            background-color: #f8f9fa;
            border: 2px dashed #d0d7de;
            border-radius: 40px;
            font-size: 12px;
            color: #656d76;
        }

        QPushButton#avatarBtn:hover {
            background-color: #f1f3f4;
            border-color: #1976D2;
            color: #1976D2;
        }

        QLabel#avatarHint {
            font-size: 12px;
            color: #999999;
            margin-top: 5px;
        }

        QLabel#fieldLabel {
            font-size: 14px;
            color: #2c2c2c;
            font-weight: 500;
            padding-right: 10px;
        }
        QLabel#passwordLabel {
            font-size: 14px;
            color: #2c2c2c;
            font-weight: 500;
            padding-right: 10px;
        }

        QLineEdit#fieldEdit {
            padding: 10px 12px;
            border: 1px solid #d0d7de;
            border-radius: 6px;
            font-size: 14px;
            background-color: white;
        }

        QLineEdit#fieldEdit:focus {
            border-color: #1976D2;
            outline: none;
        }

        QPushButton#changePasswordBtn {
            background-color: #f8f9fa;
            border: 1px solid #d0d7de;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 14px;
            color: #24292f;
        }

        QPushButton#changePasswordBtn:hover {
            background-color: #f1f3f4;
            border-color: #1976D2;
        }

        QPushButton#saveBtn {
            background-color: #1976D2;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: 500;
            min-width: 80px;
        }

        QPushButton#saveBtn:hover {
            background-color: #1565C0;
        }

        QPushButton#saveBtn:pressed {
            background-color: #0D47A1;
        }

        QPushButton#cancelBtn {
            background-color: transparent;
            color: #666666;
            border: 1px solid #d0d7de;
            border-radius: 6px;
            padding: 10px 20px;
            font-size: 14px;
            min-width: 80px;
        }

        QPushButton#cancelBtn:hover {
            background-color: #f8f9fa;
            border-color: #999999;
        }
    )");
}

void ProfileDialog::loadUserInfo()
{
    // 这里可以从配置文件或数据库加载用户信息
    nicknameEdit->setText("jamming");
    emailEdit->setText("2400012908@stu.pku.edu.cn");
    phoneEdit->setText("188********");

    // 加载头像
    QPixmap avatar = AvatarManager::instance()->getAvatarPixmap(70, 70);
    QPixmap roundPixmap = createRoundPixmap(avatar, 70);
    avatarBtn->setIcon(QIcon(roundPixmap));
    //avatarBtn->setIcon(QIcon(avatar));
    avatarBtn->setText("");
}

void ProfileDialog::saveUserInfo()
{
    // 保存用户信息到配置文件或数据库
    QString nickname = nicknameEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();

    if (nickname.isEmpty()) {
        QMessageBox::warning(this, "提示", "昵称不能为空！");
        return;
    }

    if (email.isEmpty()) {
        QMessageBox::warning(this, "提示", "邮箱不能为空！");
        return;
    }

    // 简单的邮箱格式验证
    if (!email.contains("@") || !email.contains(".")) {
        QMessageBox::warning(this, "提示", "邮箱格式不正确！");
        return;
    }

    // 这里添加实际的保存逻辑
    QMessageBox::information(this, "成功", "个人资料保存成功！");
    accept();
}

void ProfileDialog::onAvatarClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择头像",
        "",
        "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)"
        );

    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            // 使用头像管理器保存头像
            if (AvatarManager::instance()->saveAvatar(fileName)) {
                // 更新按钮显示
                QPixmap scaledPixmap = pixmap.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation);

                QPixmap s_roundPixmap = createRoundPixmap(scaledPixmap, 70);
                avatarBtn->setIcon(QIcon(s_roundPixmap));
                //avatarBtn->setIcon(QIcon(scaledPixmap));
                avatarBtn->setText("");
                QMessageBox::information(this, "成功", "头像更新成功！");
            } else {
                QMessageBox::warning(this, "错误", "头像保存失败！");
            }
        } else {
            QMessageBox::warning(this, "错误", "无法加载选择的图片！");
        }
    }
}

void ProfileDialog::onSaveClicked()
{
    saveUserInfo();
}

void ProfileDialog::onCancelClicked()
{
    reject();
}

void ProfileDialog::onChangePasswordClicked()
{
    // 获取当前用户邮箱
    QString userEmail = emailEdit->text().trimmed();

    if (userEmail.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先填写邮箱地址！");
        return;
    }

    // 打开修改密码对话框
    ChangePasswordDialog *dialog = new ChangePasswordDialog(userEmail, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->exec();
}
