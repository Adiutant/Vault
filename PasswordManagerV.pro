QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    engine/vaultengine.cpp \
    fileprovider/fileprovider.cpp \
    main.cpp \
    mainwindow.cpp \
    widgets/authlineedit.cpp \
    widgets/newaccountdialog.cpp \
    widgets/passwordswidget.cpp \
    widgets/passwordwidget.cpp \
    widgets/requirepassworddialog.cpp \
    widgets/requirepasswordwidget.cpp

HEADERS += \
    widgets/authlineedit.h \
    engine/vaultengine.h \
    fileprovider/fileprovider.h \
    globals.h \
    mainwindow.h \
    widgets/authlineedit.h \
    widgets/newaccountdialog.h \
    widgets/passwordswidget.h \
    widgets/passwordwidget.h \
    widgets/requirepassworddialog.h \
    widgets/requirepasswordwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target





win32:CONFIG(release, debug|release): LIBS += -L$${PWD}/crypt870/Release -lcryptlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$${PWD}/crypt870/Debug -lcryptlib

#LIBS += -L$${PWD}/crypt/Debug -lcryptlib

INCLUDEPATH += $${PWD}/crypt870/Debug/include







