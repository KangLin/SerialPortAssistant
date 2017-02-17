#-------------------------------------------------
#
# Project created by QtCreator 2017-02-17T10:38:47
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPortAssistant
TEMPLATE = app

#设置目标输出目录  
win32{
    CONFIG(debug, debug|release) {
        TARGET_PATH=$${OUT_PWD}/Debug
    } else {
        TARGET_PATH=$${OUT_PWD}/Release
    }
}else{
    TARGET_PATH=$$OUT_PWD
}

isEmpty(PREFIX) {
    android {
       PREFIX = /.
    } else {
        PREFIX = $$OUT_PWD/install
    }
}
target.path = $$PREFIX
INSTALLS = target

SOURCES +=\
        MainWindow.cpp \
    Main.cpp \
    Log.cpp

HEADERS  += MainWindow.h \
    Log.h

FORMS    += MainWindow.ui

win32 : equals(QMAKE_HOST.os, Windows){
    INSTALL_TARGET = $$system_path($${PREFIX}/$(TARGET))
    Deployment_qtlib.path = $$system_path($${PREFIX})
    Deployment_qtlib.commands = "$$system_path($$[QT_INSTALL_BINS]/windeployqt)" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${INSTALL_TARGET}"
    INSTALLS += Deployment_qtlib
}

DISTFILES += \
    README.md

#应用程序图标  
RC_FILE = AppIcon.rc
