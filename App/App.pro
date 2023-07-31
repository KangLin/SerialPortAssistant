#-------------------------------------------------
#
# Project created by QtCreator 2017-02-17T10:38:47
#
#-------------------------------------------------

QT       += core gui serialport network xml
CONFIG *= c++11 link_pkgconfig link_prl
android: versionAtMost(QT_VERSION, 6): QT += androidextras

android: versionAtMost(QT_VERSION, 5.10) : error("Qt version must greater 5.10")
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT *= statemachine

TARGET = SerialPortAssistant
TEMPLATE = app

isEmpty(DESTDIR): DESTDIR = $$OUT_PWD/../bin
DEFINES *= HAVE_RABBITCOMMON_GUI
CONFIG(debug, debug|release) {
    DEFINES *= _DEBUG
}

isEmpty(PREFIX) : !isEmpty(INSTALL_ROOT) : PREFIX=$$INSTALL_ROOT
isEmpty(PREFIX) {
    qnx : PREFIX = /tmp
    else : android : PREFIX = /.
    else : PREFIX = $$OUT_PWD/../install
}

isEmpty(SerialPortAssistant_VERSION) {
    isEmpty(GIT) : GIT=$$(GIT)
    isEmpty(GIT) : GIT=git
    isEmpty(GIT_DESCRIBE) {
        GIT_DESCRIBE = $$system(cd $$system_path($$PWD) && $$GIT describe --tags)
        isEmpty(SerialPortAssistant_VERSION) {
            SerialPortAssistant_VERSION = $$GIT_DESCRIBE
        }
    }
    SerialPortAssistant_Revision = $$system(cd $$system_path($$PWD) && $$GIT rev-parse --short HEAD)
    isEmpty(SerialPortAssistant_VERSION) {
        SerialPortAssistant_VERSION = $$SerialPortAssistant_Revision
    }
    
    isEmpty(SerialPortAssistant_VERSION){
        warning("Built without git, please add SerialPortAssistant_VERSION to DEFINES or add git path to environment variable GIT or qmake parameter GIT")
    }
}
isEmpty(SerialPortAssistant_VERSION){
    SerialPortAssistant_VERSION="v0.5.15"
}
message("SerialPortAssistant_VERSION:$$SerialPortAssistant_VERSION")
message("SerialPortAssistant_Revision:$$SerialPortAssistant_Revision")

DEFINES += SerialPortAssistant_VERSION=\"\\\"$$quote($$SerialPortAssistant_VERSION)\\\"\"
!isEmpty(SerialPortAssistant_Revision) {
    DEFINES *= SerialPortAssistant_Revision=\"\\\"$$quote($$SerialPortAssistant_Revision)\\\"\"
}
first_version = $$$$str_member($$SerialPortAssistant_VERSION, 0, 0)
equals(first_version, "v") {
    VERSION=$$replace(SerialPortAssistant_VERSION, v,)
    win32{
        VERSION=$$split(VERSION, -)
        VERSION=$$first(VERSION)
    }
}

equals(BUILD_QUIWidget, "ON"): DEFINES *= BUILD_QUIWidget

icon.files = SerialPortAssistant.ico
icon.path = $$PREFIX
icon.CONFIG += directory no_check_exist 
!android: target.path = $$PREFIX/bin
INSTALLS += target icon

SOURCES +=\
    $$PWD/MainWindow.cpp \
    $$PWD/Main.cpp \
    $$PWD/Global/Global.cpp \
    $$PWD/Common/Tool.cpp  \
    $$PWD/SendFile.cpp \
    $$PWD/../3th_lib/Control/lightbutton.cpp
    
HEADERS += $$PWD/MainWindow.h \
    $$PWD/Global/Global.h \
    $$PWD/Common/Tool.h  \
    $$PWD/SendFile.h \
    $$PWD/../3th_lib/Control/lightbutton.h

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
    DEFINES += RABBITCOMMON HAVE_RABBITCOMMON_GUI HAVE_UPDATE HAVE_ABOUT
    INCLUDEPATH += \
        $${RabbitCommon_DIR}/Src \
        $${RabbitCommon_DIR}/Src/export \
        $${RabbitCommon_DIR}/Src/DlgAbout \
        $${RabbitCommon_DIR}/Src/FrmUpdater \
        $${RabbitCommon_DIR}/Src/Style \
        $$PWD/../3th_lib/Control

    LIBS *= -L$$DESTDIR -lRabbitCommon
} else {
    message("Don't find RabbitCommon, in environment variable RabbitCommon_DIR:$$RabbitCommon_DIR")
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon ag:")
    message("   git clone https://github.com/KangLin/RabbitCommon.git")
    error  ("2. Then set environment variable RabbitCommon_DIR to download directory")
}

configure.files = $$PWD/../etc/SerialPortAssistant_logqt.ini
configure.path = $$PREFIX/etc
configure.CONFIG += directory no_check_exist
!android: configure.path = $$PREFIX/etc
INSTALLS += configure

include($${RabbitCommon_DIR}/pri/Translations.pri)

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,x86) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
