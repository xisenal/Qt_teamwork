#include "emailsender.h"
#include <QDateTime>
#include <QDebug>
#include <QRandomGenerator>

EmailSender::EmailSender(QObject *parent)
    : QObject(parent)
{
    connect(&socket, &QSslSocket::encrypted, this, [this]() {
        qDebug() << "Connection encrypted";
    });

    connect(&socket, &QSslSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
        qDebug() << "Socket error:" << error;
        lastError = socket.errorString();
    });

    connect(&socket, &QSslSocket::sslErrors, this, [this](const QList<QSslError> &errors) {
        for (const QSslError &error : errors) {
            qDebug() << "SSL error:" << error.errorString();
        }
        socket.ignoreSslErrors();
    });
}

bool EmailSender::sendVerificationCode(const QString &recipientEmail, QString &verificationCode)
{
    verificationCode = generateVerificationCode();
    QString subject = "验证码 - XLab邮箱验证";
    QString body = QString("欢迎加入X-Lab!您的验证码是: %1\n\n此验证码将在10分钟内有效。拒绝成为牛马，领先世界一亿年！导师再也不用担心你摸鱼了qwq（\n如果这不是您的操作，请忽略此邮件。").arg(verificationCode);

    return sendEmail(recipientEmail, subject, body);
}


bool  EmailSender::SendpasswordCode(const QString &recipientEmail, QString &verificationCode)
{
    verificationCode = generateVerificationCode();
    QString subject = "验证码 - XLab邮箱验证";
    QString body = QString("尊敬的X-Lab用户：您好！您的验证码是: %1\n\n此验证码将在10分钟内有效。请注意：您的账户正在X-Lab平台尝试更改密码，请确认这是您本人的操作。如否，请尽快登录X-Lab更换密码来确保安全。").arg(verificationCode);

    return sendEmail(recipientEmail, subject, body);
}

QString EmailSender::generateVerificationCode()
{
    QString code;
    for (int i = 0; i < 6; ++i) {
        code.append(QString::number(QRandomGenerator::global()->bounded(10)));
    }
    return code;
}

bool EmailSender::connectToHost()
{
    socket.connectToHostEncrypted(smtpServer, smtpPort);
    if (!socket.waitForConnected(30000)) {
        lastError = "Connection timeout";
        return false;
    }
    return waitForResponse();
}

bool EmailSender::login()
{
    sendCommand("EHLO localhost");
    if (!waitForResponse()) return false;

    sendCommand("AUTH LOGIN");
    if (!waitForResponse()) return false;

    sendCommand(QByteArray().append(senderEmail.toUtf8().toBase64()));
    if (!waitForResponse()) return false;

    sendCommand(QByteArray().append(senderPassword.toUtf8().toBase64()));
    return waitForResponse();
}

bool EmailSender::sendEmail(const QString &to, const QString &subject, const QString &body)
{
    if (!connectToHost()) return false;
    if (!login()) return false;

    sendCommand(QString("MAIL FROM:<%1>").arg(senderEmail));
    if (!waitForResponse()) return false;

    sendCommand(QString("RCPT TO:<%1>").arg(to));
    if (!waitForResponse()) return false;

    sendCommand("DATA");
    if (!waitForResponse()) return false;

    QString message = QString(
        "From: XLab Mail System <%1>\r\n"
        "To: <%2>\r\n"
        "Subject: %3\r\n"
        "Content-Type: text/plain; charset=UTF-8\r\n"
        "\r\n"
        "%4\r\n"
        "."
    ).arg(senderEmail, to, subject, body);

    sendCommand(message);
    if (!waitForResponse()) return false;

    sendCommand("QUIT");
    socket.disconnectFromHost();

    return true;
}

bool EmailSender::waitForResponse()
{
    if (!socket.waitForReadyRead(30000)) {
        lastError = "Response timeout";
        return false;
    }


    QString response = QString::fromUtf8(socket.readAll());
    qDebug() << "Server response:" << response;

    return response.startsWith('2') || response.startsWith('3');
}

void EmailSender::sendCommand(const QString &cmd)
{
    qDebug() << "Sending command:" << cmd;
    socket.write((cmd + "\r\n").toUtf8());
    socket.flush();
}
