#-------------------------------------------------
#
# Project created by QtCreator 2017-02-17T10:38:47
#
#-------------------------------------------------

QT       += core gui serialport network xml
android: QT += androidextras

android: versionAtMost(QT_VERSION, 5.10) : error("Qt version must greater 5.10")
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPortAssistant
TEMPLATE = app

isEmpty(DESTDIR): DESTDIR = $$OUT_PWD/../bin

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}

isEmpty(PREFIX) : !isEmpty(INSTALL_ROOT) : PREFIX=$$INSTALL_ROOT
isEmpty(PREFIX) {
    android {
       PREFIX = /.
    } else {
        PREFIX = $$OUT_PWD/../install
    }
}

isEmpty(BUILD_VERSION) {
    isEmpty(GIT) : GIT=$$(GIT)
    isEmpty(GIT) : GIT=git
    isEmpty(GIT_DESCRIBE) {
        GIT_DESCRIBE = $$system(cd $$system_path($$PWD) && $$GIT describe --tags)
        isEmpty(BUILD_VERSION) {
            BUILD_VERSION = $$GIT_DESCRIBE
        }
    }
    isEmpty(BUILD_VERSION) {
        BUILD_VERSION = $$system(cd $$system_path($$PWD) && $$GIT rev-parse --short HEAD)
    }
    
    isEmpty(BUILD_VERSION){
        warning("Built without git, please add BUILD_VERSION to DEFINES or add git path to environment variable GIT or qmake parameter GIT")
    }
}
isEmpty(BUILD_VERSION){
    BUILD_VERSION="v0.4.2"
}
message("BUILD_VERSION:$$BUILD_VERSION")

DEFINES += BUILD_VERSION=\"\\\"$$quote($$BUILD_VERSION)\\\"\"
VERSION=$$replace(BUILD_VERSION, v,)
win32{
    VERSION=$$split(VERSION, -)
    VERSION=$$first(VERSION)
}

icon.files = SerialPortAssistant.ico
icon.path = $$PREFIX
icon.CONFIG += directory no_check_exist 
target.path = $$PREFIX/bin
INSTALLS += target icon

SOURCES +=\
    $$PWD/MainWindow.cpp \
    $$PWD/Main.cpp \
    $$PWD/Global/Log.cpp \
    $$PWD/Global/Global.cpp \
    $$PWD/Common/Tool.cpp 
    
HEADERS += $$PWD/MainWindow.h \
    $$PWD/Global/Log.h \
    $$PWD/Global/Global.h \
    $$PWD/Common/Tool.h 

FORMS += $$PWD/MainWindow.ui

RC_FILE = AppIcon.rc
win32: OTHER_FILES += $$RC_FILE

RESOURCES += \
    Resource/Resource.qrc

win32 : equals(QMAKE_HOST.os, Windows){
    INSTALL_TARGET = $$system_path($${DESTDIR}/$(TARGET))
    Deployment_qtlib.files = $$system_path($${DESTDIR}/)
    Deployment_qtlib.path = $$system_path($${PREFIX})
    Deployment_qtlib.commands = "$$system_path($$[QT_INSTALL_BINS]/windeployqt)" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${INSTALL_TARGET}"
    INSTALLS += Deployment_qtlib
}
win32 {
    msvc {
        QMAKE_CXXFLAGS += /wd"4819"  
        QMAKE_CXXFLAGS += "/utf-8"
        #QMAKE_LFLAGS += -ladvapi32
        CONFIG(debug, debug|release) {
            QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS",5.01" /NODEFAULTLIB:libcmtd
        }else{
            QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS",5.01" /NODEFAULTLIB:libcmt
        }
    } else {
        DEFINES += "_WIN32_WINNT=0x0501" #__USE_MINGW_ANSI_STDIO
    }
}

isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$(RabbitCommon_DIR)
isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$PWD/../../RabbitCommon
!isEmpty(RabbitCommon_DIR): exists("$${RabbitCommon_DIR}/Src/Src.pro"){
    CONFIG(static): DEFINES *= RABBITCOMMON_STATIC_DEFINE LUNARCALENDAR_STATIC_DEFINE
    DEFINES += RABBITCOMMON
    INCLUDEPATH += \
        $${RabbitCommon_DIR}/Src \
        $${RabbitCommon_DIR}/Src/export
    
    LIBS *= -L$$DESTDIR -lRabbitCommon
} else {
    message("Don't find RabbitCommon, in RabbitCommon_DIR:$$RabbitCommon_DIR")
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon ag:")
    message("   git clone https://github.com/KangLin/RabbitCommon.git")
    error  ("2. Then set value RabbitCommon_DIR to download dirctory")
}

include($${RabbitCommon_DIR}/pri/Translations.pri)

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,x86) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
