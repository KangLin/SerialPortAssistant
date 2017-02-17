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

#安装前缀
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
    #isEmpty(QMAKE_SH){
        INSTALL_TARGET = $$system_path($${PREFIX}/$(TARGET))
    #} else {
    #    INSTALL_TARGET = $${PREFIX}/$(TARGET)
    #}

    #mingw{  #手机平台不需要
    #    RABBITIM_STRIP.target = RABBITIM_STRIP
    #    RABBITIM_STRIP.commands = "strip $$INSTALL_TARGET"
    #    INSTALLS += RABBITIM_STRIP
    #}
    #安装qt依赖库
    Deployment_qtlib.target = Deployment_qtlib
    Deployment_qtlib.path = $$system_path($${PREFIX})
    Deployment_qtlib.commands = "$$system_path($$[QT_INSTALL_BINS]/windeployqt)" \
                    --compiler-runtime \
                    --verbose 7 \
                    "$${INSTALL_TARGET}"
    INSTALLS += Deployment_qtlib
}
