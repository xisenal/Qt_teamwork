QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 4.13 edited
RESOURCES += images.qrc

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    loginwindow.cpp \
    usermanager.cpp \
    emailsender.cpp \
    adminwindow.cpp

HEADERS += \
    mainwindow.h \
    loginwindow.h \
    usermanager.h \
    emailsender.h \
    adminwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    411trial_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
