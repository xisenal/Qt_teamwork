#ifndef EMAILSENDER_H
#define EMAILSENDER_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QSslSocket>
#include <QTextStream>
#include <QRandomGenerator>

class EmailSender : public QObject
{
    Q_OBJECT

public:
    explicit EmailSender(QObject *parent = nullptr);
    bool sendVerificationCode(const QString &recipientEmail, QString &verificationCode);
    bool SendpasswordCode(const QString &recipientEmail, QString &verificationCode);

private:
    QString generateVerificationCode();
    bool connectToHost();
    bool login();
    bool sendEmail(const QString &to, const QString &subject, const QString &body);

    QSslSocket socket;
    QString smtpServer = "smtp.163.com";
    int smtpPort = 465;
    QString senderEmail = "xlabmailsystem@163.com";
    QString senderPassword = "TTq7YiegBbVth3tv";
    QString lastError;

    bool waitForResponse();
    void sendCommand(const QString &cmd);
};

#endif // EMAILSENDER_H
