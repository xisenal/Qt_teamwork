#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H


#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QFrame>

class ProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileDialog(QWidget *parent = nullptr);
    ~ProfileDialog();

private slots:
    void onAvatarClicked();
    void onSaveClicked();
    void onCancelClicked();
    void onChangePasswordClicked();

private:
    void setupUI();
    void setupStyles();
    void loadUserInfo();
    void saveUserInfo();

    // UI组件
    QVBoxLayout *mainLayout;
    QHBoxLayout *headerLayout;
    QGridLayout *formLayout;
    QHBoxLayout *buttonLayout;

    QLabel *titleLabel;
    QPushButton *closeBtn;

    QLabel *avatarLabel;
    QPushButton *avatarBtn;
    QLineEdit *nicknameEdit;
    QLineEdit *emailEdit;
    QLineEdit *phoneEdit;
    QPushButton *changePasswordBtn;

    QPushButton *saveBtn;
    QPushButton *cancelBtn;

    QString currentAvatarPath;
};

#endif // PROFILEDIALOG_H
