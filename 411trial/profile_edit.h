#ifndef PROFILE_EDIT_H
#define PROFILE_EDIT_H

#include <QObject>
#include <QLabel>
#include <QString>
#include <QWidget>
#include <QInputDialog>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QFileDialog>

// 定义可编辑标签基类（复用状态编辑的逻辑）
// class EditableLabel : public QLabel {
//     Q_OBJECT
// public:
//     explicit EditableLabel(const QString& text, QWidget* parent = nullptr)
//         : QLabel(text, parent) {
//         setCursor(Qt::PointingHandCursor);
//         setStyleSheet("color: #333; padding: 2px;");
//     }

//     void setTextChangedCallback(std::function<void(const QString&)> callback) {
//         m_callback = callback;
//     }

// signals:
//     void textChanged(const QString& newText);

// protected:
//     void mousePressEvent(QMouseEvent* event) override {
//         Q_UNUSED(event)
//         startEditing();
//     }

//     virtual void startEditing() {
//         bool ok;
//         QString newText = QInputDialog::getText(this, tr("编辑"), tr("输入新内容:"),
//                                                 QLineEdit::Normal, text(), &ok);
//         if (ok && !newText.isEmpty() && newText != text()) {
//             setText(newText);
//             emit textChanged(newText);
//             if (m_callback) m_callback(newText);
//         }
//     }

// private:
//     std::function<void(const QString&)> m_callback;
// };

// // 多行编辑的Bio标签
// class EditableBioLabel : public EditableLabel {
// public:
//     using EditableLabel::EditableLabel;

// protected:
//     void startEditing() override {
//         QDialog dialog(this);
//         dialog.setWindowTitle(tr("编辑个人简介"));

//         QTextEdit *textEdit = new QTextEdit(&dialog);
//         textEdit->setPlainText(text());
//         textEdit->setMinimumSize(300, 150);

//         QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);

//         QVBoxLayout *layout = new QVBoxLayout(&dialog);
//         layout->addWidget(textEdit);
//         layout->addWidget(btnBox);

//         connect(btnBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
//         connect(btnBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

//         if (dialog.exec() == QDialog::Accepted) {
//             QString newBio = textEdit->toPlainText();
//             if (!newBio.isEmpty() && newBio != text()) {
//                 setText(newBio);
//                 emit textChanged(newBio);
//             }
//         }
//     }
// };

// // 可编辑头像组件
// class ClickableAvatarLabel : public QLabel {
//     Q_OBJECT
// public:
//     explicit ClickableAvatarLabel(QWidget* parent = nullptr)
//         : QLabel(parent) {
//         setCursor(Qt::PointingHandCursor);
//         setAlignment(Qt::AlignCenter);
//         setFixedSize(80, 80);
//     }

// signals:
//     void avatarChanged(const QString& newPath);

// protected:
//     void mousePressEvent(QMouseEvent* event) override {
//         Q_UNUSED(event)
//         changeAvatar();
//     }

// private:
//     void changeAvatar() {
//         QString filePath = QFileDialog::getOpenFileName(this, tr("选择头像"), "",
//                                                         tr("图片文件 (*.png *.jpg *.jpeg *.bmp)"));
//         if (!filePath.isEmpty()) {
//             QPixmap pixmap(filePath);
//             if (!pixmap.isNull()) {
//                 setPixmap(pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//                 emit avatarChanged(filePath);
//             }
//         }
//     }
// };



// 定义可编辑标签基类（使用回调函数）
class EditableLabel : public QLabel {
public:
    explicit EditableLabel(const QString& text, QWidget* parent = nullptr)
        : QLabel(text, parent) {
        setCursor(Qt::PointingHandCursor);
        setStyleSheet("color: #333; padding: 2px;");
    }

    void setTextChangedCallback(std::function<void(const QString&)> callback) {
        m_callback = callback;
    }

protected:
    void mousePressEvent(QMouseEvent* event) override {
        Q_UNUSED(event)
        startEditing();
    }

    virtual void startEditing() {
        bool ok;
        QString newText = QInputDialog::getText(this, tr("编辑"), tr("输入新内容:"),
                                                QLineEdit::Normal, text(), &ok);
        if (ok && !newText.isEmpty() && newText != text()) {
            setText(newText);
            if (m_callback) m_callback(newText);
        }
    }


    //edit
public:
    std::function<void(const QString&)> m_callback;
};

// 多行编辑的Bio标签
class EditableBioLabel : public EditableLabel {
public:
    using EditableLabel::EditableLabel;

protected:
    void startEditing() override {
        QDialog dialog(this);
        dialog.setWindowTitle(tr("编辑个人简介"));

        QTextEdit *textEdit = new QTextEdit(&dialog);
        textEdit->setPlainText(text());
        textEdit->setMinimumSize(300, 150);

        QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);

        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        layout->addWidget(textEdit);
        layout->addWidget(btnBox);

        connect(btnBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(btnBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted) {
            QString newBio = textEdit->toPlainText();
            if (!newBio.isEmpty() && newBio != text()) {
                setText(newBio);
                if (m_callback) m_callback(newBio);
            }
        }
    }
};

// 可编辑头像组件（使用回调函数）
class ClickableAvatarLabel : public QLabel {
public:
    explicit ClickableAvatarLabel(QWidget* parent = nullptr)
        : QLabel(parent) {
        setCursor(Qt::PointingHandCursor);
        setAlignment(Qt::AlignCenter);
        setFixedSize(80, 80);
    }

    void setAvatarChangedCallback(std::function<void(const QString&)> callback) {
        m_callback = callback;
    }

protected:
    void mousePressEvent(QMouseEvent* event) override {
        Q_UNUSED(event)
        changeAvatar();
    }

private:
    void changeAvatar() {
        QString filePath = QFileDialog::getOpenFileName(this, tr("选择头像"), "",
                                                        tr("图片文件 (*.png *.jpg *.jpeg *.bmp)"));
        if (!filePath.isEmpty()) {
            QPixmap pixmap(filePath);
            if (!pixmap.isNull()) {
                setPixmap(pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                if (m_callback) m_callback(filePath);
            }
        }
    }

    std::function<void(const QString&)> m_callback;
};


#endif // PROFILE_EDIT_H
