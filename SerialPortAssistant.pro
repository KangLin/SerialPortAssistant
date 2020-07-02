TEMPLATE = subdirs
DESTDIR = $$OUT_PWD/bin

isEmpty(PREFIX) : !isEmpty(INSTALL_ROOT) : PREFIX=$$INSTALL_ROOT
isEmpty(PREFIX) {
    qnx : PREFIX = /tmp
    else : android : PREFIX = /.
    else : PREFIX = $$OUT_PWD/install
}
RabbitCommon.file = 3th_lib/RabbitCommon.pro
App.depends = RabbitCommon
CONFIG *= ordered
SUBDIRS = RabbitCommon App

other.files = License.md Authors.md Authors_zh_CN.md \
    ChangeLog.md ChangeLog_zh_CN.md
win32: other.files *= AppIcon.ico
android: other.path = $$PREFIX/assets
else: other.path = $$PREFIX
other.CONFIG += directory no_check_exist
INSTALLS += other 

install_win.files = Install/Install.nsi
install_win.path = $$OUT_PWD
install_win.CONFIG += directory no_check_exist 
win32: INSTALLS += install_win

install_unix.files = Install/install.sh
install_unix.path = $$PREFIX
install_unix.CONFIG += directory no_check_exist 
unix: !android: INSTALLS += install_unix

!android : !macx : unix {
    # install icons
    icon128.target = icon128
    icon128.files = App/Resource/png/SerialPortAssistant.png
    icon128.path = $${PREFIX}/share/pixmaps
    icon128.CONFIG = directory no_check_exist

    # install desktop file
    DESKTOP_FILE.target = DESKTOP_FILE
    DESKTOP_FILE.files = $$PWD/debian/SerialPortAssistant.desktop
    DESKTOP_FILE.path = $$system_path($${PREFIX})/share/applications
    DESKTOP_FILE.CONFIG += directory no_check_exist
    INSTALLS += DESKTOP_FILE icon128
}

OTHER_FILES += \
    Authors.md \
    Authors_zh_CN.md \
    License.md \
    README*.md \
    ChangeLog.md \
    ChangeLog_zh_CN.md \
    appveyor.yml \
    ci/* \
    test/* \
    Install/* \
    .travis.yml \
    tag.sh \
    debian/* \
    Update/* \
    build_debpackage.sh
