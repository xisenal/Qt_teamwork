QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 4.13 edited
RESOURCES += images.qrc

SOURCES += \
    apidialog.cpp \
    avatarmanager.cpp \
    canvas.cpp \
    changepassworddialog.cpp \
    checkinwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    loginwindow.cpp \
    pluginmanager.cpp \
    pluginmanagerdialog.cpp \
    profiledialog.cpp \
    usermanager.cpp \
    emailsender.cpp \
    adminwindow.cpp \
    todolistwindow.cpp

HEADERS += \
    apidialog.h \
    avatarmanager.h \
    canvas.h \
    changepassworddialog.h \
    checkinwidget.h \
    mainwindow.h \
    loginwindow.h \
    plugin_interface.h \
    pluginmanager.h \
    pluginmanagerdialog.h \
    profile_edit.h \
    profiledialog.h \
    usermanager.h \
    emailsender.h \
    adminwindow.h \
    todolistwindow.h

FORMS += \
    apidialog.ui \
    canvas.ui \
    apidialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    411trial_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Create and install userdata directory
userdatadir.path = $$OUT_PWD/userdata
userdatadir.files = userdata
INSTALLS += userdatadir

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
