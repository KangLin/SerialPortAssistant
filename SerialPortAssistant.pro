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

include(Resource/translations/translations.pri)

other.files = LICENSE.md Authors.txt ChangeLog.md
other.path = $$PREFIX
other.CONFIG += directory no_check_exist 
target.path = $$PREFIX
install.files = Install/Install.nsi
install.path = $$OUT_PWD
install.CONFIG += directory no_check_exist 
INSTALLS += target other install

SOURCES +=\
    MainWindow.cpp \
    Main.cpp \
    Global/Log.cpp \
    Global/GlobalDir.cpp \
    Global/Global.cpp \
    Common/Tool.cpp \
    Widgets/DlgAbout/DlgAbout.cpp
    
HEADERS += MainWindow.h \
    Global/Log.h \
    Global/GlobalDir.h \
    Global/Global.h \
    Common/Tool.h \
    Widgets/DlgAbout/DlgAbout.h

FORMS += MainWindow.ui \
    Widgets/DlgAbout/DlgAbout.ui

win32 : equals(QMAKE_HOST.os, Windows){
    INSTALL_TARGET = $$system_path($${PREFIX}/$$(TARGET))

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

DISTFILES += \
    README*.md \
    Authors.txt \
    ChangeLog.md \
    LICENSE.md \
    appveyor.yml \
    ci/* \
    Install/* \
    .travis.yml \
    tag.sh

RC_FILE = AppIcon.rc

RESOURCES += \
    Resource/Resource.qrc
