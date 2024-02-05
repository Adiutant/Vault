QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Vault
TEMPLATE = app
unix:LIBS += -lstdc++fs
CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include($${PWD}/qtsingleapplication/src/qtsingleapplication.pri)

SOURCES += \
    api/schemeeventfilter.cpp \
    api/yandexapi.cpp \
    engine/vaultengine.cpp \
    fileprovider/fileprovider.cpp \
    globals.cpp \
    main.cpp \
    mainwindow.cpp \
    crypto_utils_lib/crypto_utils.cpp \
    crypto_utils_lib/utils.cpp \
    settings/storagesettings.cpp \
    widgets/authlineedit.cpp \
    widgets/newaccountdialog.cpp \
    widgets/passwordswidget.cpp \
    widgets/passwordwidget.cpp \
    widgets/requirepassworddialog.cpp \
    widgets/requirepasswordwidget.cpp

HEADERS += \
    api/schemeeventfilter.h \
    api/yandexapi.h \
    settings/storagesettings.h \
    widgets/authlineedit.h \
    engine/vaultengine.h \
    fileprovider/fileprovider.h \
    crypto_utils_lib/crypto_utils.h \
    crypto_utils_lib/utils.h \
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
else: unix:!android: target.path = $${PWD}/install  #/opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$${PWD}/crypt870/Release -lcryptlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$${PWD}/crypt870/Debug -lcryptlib
else:unix: LIBS += -L$$PWD/../cryptopp870/ -lcryptopp

win32: INCLUDEPATH += $${PWD}/crypt870/Debug/include
else:unix: INCLUDEPATH += $${PWD}/../cryptopp870

unix:DEPENDPATH += ${PWD}/../cryptopp870

unix: PRE_TARGETDEPS += $${PWD}/../cryptopp870/libcryptopp.a

RESOURCES += \
    res.qrc

RC_FILE += applogo.rc






