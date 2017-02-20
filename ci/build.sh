#!/bin/bash
set -ev

if [ "$BUILD_TARGERT" = "windows_mingw" \
    -a -n "$APPVEYOR" ]; then
    export PATH=/C/Qt/Tools/mingw${TOOLCHAIN_VERSION}_32/bin:$PATH
fi

SOURCE_DIR=`pwd`
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi

cd ${SOURCE_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${SOURCE_DIR}/Tools/android-sdk
    export ANDROID_NDK_ROOT=${SOURCE_DIR}/Tools/android-ndk
    if [ -z "$APPVEYOR" ]; then
        export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    fi
    export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/${QT_VERSION_DIR}/android_armv7
    if [ "${QT_VERSION}" = "5.2.1" ]; then
        export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/android_armv7
    fi
    export PATH=${SOURCE_DIR}/Tools/apache-ant/bin:$JAVA_HOME:$PATH
fi

if [ "${BUILD_TARGERT}" = "unix" ]; then
    QT_DIR=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}
    export QT_ROOT=${QT_DIR}/${QT_VERSION_DIR}/gcc_64
    if [ "${QT_VERSION}" = "5.2.1" ]; then
        export QT_ROOT=${QT_DIR}/${QT_VERSION}/gcc_64
    fi
fi

if [ "$BUILD_TARGERT" != "windows_msvc" ]; then
    RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBIT_MAKE_JOB_PARA" = "-j1" ];then
        RABBIT_MAKE_JOB_PARA="-j2"
    fi
    export RABBIT_MAKE_JOB_PARA
fi

export PATH=${QT_ROOT}/bin:$PATH

cd ${SOURCE_DIR}
mkdir -p build_${BUILD_TARGERT}
cd build_${BUILD_TARGERT}
qmake ../SerialPortAssistant.pro  $PARA "CONFIG+=release"
echo "$MAKE install ...."

if [ "${BUILD_TARGERT}" = "windows_msvc" ]; then
    MAKE=nmake
else
    MAKE="make ${RABBIT_MAKE_JOB_PARA}"
fi

$MAKE -f Makefile
echo "$MAKE install ...."
$MAKE install

if [ "${BUILD_TARGERT}" = "windows_msvc" ]; then
cd ${SOURCE_DIR}
cp Install/Install.nsi build_${BUILD_TARGERT}
"/C/Program Files (x86)/NSIS/makensis.exe" "build_${BUILD_TARGERT}/Install.nsi"
fi
