#-------------------------------------------------
#
# Project created by QtCreator 2017-02-17T10:38:47
#
#-------------------------------------------------

QT       += core gui serialport

!greaterThan(QT_VERSION, 5.1) : error("Qt version must greater 5.1")
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPortAssistant
TEMPLATE = app

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}

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
    BUILD_VERSION="v0.3.2"
}
message("BUILD_VERSION:$$BUILD_VERSION")

DEFINES += BUILD_VERSION=\"\\\"$$quote($$BUILD_VERSION)\\\"\"

include(pri/Translations.pri)

other.files = License.md Authors.md ChangeLog.md AppIcon.ico
other.path = $$PREFIX
other.CONFIG += directory no_check_exist 
target.path = $$PREFIX/bin
install.files = Install/Install.nsi
install.path = $$OUT_PWD
install.CONFIG += directory no_check_exist 
INSTALLS += target other install

!android : unix {
    DESKTOP_FILE.target = DESKTOP_FILE
    DESKTOP_FILE.files = $$PWD/debian/SerialPortAssistant.desktop
    DESKTOP_FILE.path = $$system_path($${PREFIX})/share/applications
    DESKTOP_FILE.CONFIG += directory no_check_exist
    INSTALLS += DESKTOP_FILE
}

SOURCES +=\
    $$PWD/MainWindow.cpp \
    $$PWD/Main.cpp \
    $$PWD/Global/Log.cpp \
    $$PWD/Global/GlobalDir.cpp \
    $$PWD/Global/Global.cpp \
    $$PWD/Common/Tool.cpp 
    
HEADERS += $$PWD/MainWindow.h \
    $$PWD/Global/Log.h \
    $$PWD/Global/GlobalDir.h \
    $$PWD/Global/Global.h \
    $$PWD/Common/Tool.h 

FORMS += $$PWD/MainWindow.ui

OTHER_FILES += \
    Authors.md \
    License.md \
    README*.md \
    ChangeLog.md \
    appveyor.yml \
    ci/* \
    Install/* \
    .travis.yml \
    tag.sh \
    debian/* \
    build_debpackage.sh

RC_FILE = AppIcon.rc

RESOURCES += \
    Resource/Resource.qrc

win32 : equals(QMAKE_HOST.os, Windows){
    INSTALL_TARGET = $$system_path($${PREFIX}/bin/$$(TARGET))

    Deployment_qtlib.path = $$system_path($${PREFIX}/bin)
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
isEmpty(RabbitCommon_DIR): RabbitCommon_DIR=$$PWD/../RabbitCommon
!isEmpty(RabbitCommon_DIR): exists("$${RabbitCommon_DIR}/Src/RabbitCommon.pri"){
    DEFINES += RABBITCOMMON
    include("$${RabbitCommon_DIR}/Src/RabbitCommon.pri")
} else{
    message("RabbitCommon_DIR:$$RabbitCommon_DIR")
    message("1. Please download RabbitCommon source code from https://github.com/KangLin/RabbitCommon ag:")
    message("   git clone https://github.com/KangLin/RabbitCommon.git")
    error  ("2. Then set value RabbitCommon_DIR to download root dirctory")
}

