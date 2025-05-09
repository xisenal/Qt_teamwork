#include "mainwindow.h"
#include "loginwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "411trial_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    LoginWindow *loginWindow = new LoginWindow();
    MainWindow *mainWindow = new MainWindow();

    QObject::connect(loginWindow, &LoginWindow::loginSuccess, [=]() {
        mainWindow->show();
    });

    loginWindow->show();
    return a.exec();
}
