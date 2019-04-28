#!/bin/bash
set -ev

SOURCE_DIR=`pwd`
if [ -n "$1" ]; then
    SOURCE_DIR=$1
fi

cd ${SOURCE_DIR}

if [ "$BUILD_TARGERT" = "android" ]; then
    export ANDROID_SDK_ROOT=${SOURCE_DIR}/Tools/android-sdk
    export ANDROID_NDK_ROOT=${SOURCE_DIR}/Tools/android-ndk
    if [ -n "$APPVEYOR" ]; then
        export JAVA_HOME="/C/Program Files (x86)/Java/jdk1.8.0"
    fi
    export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/${QT_VERSION}/android_armv7
    export PATH=${SOURCE_DIR}/Tools/apache-ant/bin:$JAVA_HOME:$PATH
fi

if [ "${BUILD_TARGERT}" = "unix" ]; then
    QT_DIR=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}
    export QT_ROOT=${QT_DIR}/${QT_VERSION}/gcc_64
fi

if [ "$BUILD_TARGERT" != "windows_msvc" ]; then
    RABBIT_MAKE_JOB_PARA="-j`cat /proc/cpuinfo |grep 'cpu cores' |wc -l`"  #make 同时工作进程参数
    if [ "$RABBIT_MAKE_JOB_PARA" = "-j1" ];then
        RABBIT_MAKE_JOB_PARA="-j2"
    fi
fi

if [ "$BUILD_TARGERT" = "windows_mingw" \
    -a -n "$APPVEYOR" ]; then
    export PATH=/C/Qt/Tools/mingw${TOOLCHAIN_VERSION}/bin:$PATH
fi
TARGET_OS=`uname -s`
case $TARGET_OS in
    MINGW* | CYGWIN* | MSYS*)
        export PKG_CONFIG=/c/msys64/mingw32/bin/pkg-config.exe
        ;;
    Linux* | Unix*)
    ;;
    *)
    ;;
esac

export PATH=${QT_ROOT}/bin:$PATH
echo "PATH:$PATH"
echo "PKG_CONFIG:$PKG_CONFIG"
cd ${SOURCE_DIR}
if [ "${BUILD_TARGERT}" = "windows_msvc" ]; then
    ./tag.sh
fi
mkdir -p build_${BUILD_TARGERT}
cd build_${BUILD_TARGERT}

case ${BUILD_TARGERT} in
    windows_msvc)
        MAKE=nmake
        ;;
    windows_mingw)
        if [ "${RABBIT_BUILD_HOST}"="windows" ]; then
            MAKE="mingw32-make ${RABBIT_MAKE_JOB_PARA}"
        fi
        ;;
    *)
        MAKE="make ${RABBIT_MAKE_JOB_PARA}"
        ;;
esac

if [ "${BUILD_TARGERT}" = "unix" ]; then
    cd $SOURCE_DIR
    ./build_debpackage.sh ${QT_ROOT}/lib/cmake/Qt5
    exit 0
fi


if [ "ON" = "${STATIC}" ]; then
    CONFIG_PARA="CONFIG*=static"
fi
if [ "${BUILD_TARGERT}" = "android" ]; then
    ${QT_ROOT}/bin/qmake ${SOURCE_DIR} \
         "CONFIG+=release" ${CONFIG_PARA}
            
    $MAKE
    $MAKE install INSTALL_ROOT=`pwd`/android-build
    ${QT_ROOT}/bin/androiddeployqt \
          --input `pwd`/App/android-SerialPortAssistant.so-deployment-settings.json \
          --output `pwd`/android-build \ 
          --android-platform ${ANDROID_API} \
          --gradle --verbose
          #--jdk ${JAVA_HOME}
else
    ${QT_ROOT}/bin/qmake ${SOURCE_DIR} \
         "CONFIG+=release" ${CONFIG_PARA}\
         PREFIX=`pwd`/install 
                
    $MAKE
    echo "$MAKE install ...."
    $MAKE install
fi

if [ "${BUILD_TARGERT}" = "windows_msvc" ]; then
    #cd ${SOURCE_DIR}
    #cp Install/Install.nsi build_${BUILD_TARGERT}

    if [ "${AUTOBUILD_ARCH}" = "x86" ]; then
        cp /C/OpenSSL-Win32/bin/libeay32.dll install/bin
        cp /C/OpenSSL-Win32/bin/ssleay32.dll install/bin
    elif [ "${AUTOBUILD_ARCH}" = "x64" ]; then
        cp /C/OpenSSL-Win64/bin/libeay32.dll install/bin
        cp /C/OpenSSL-Win64/bin/ssleay32.dll install/bin
    fi
    
    if [ -z "${STATIC}" ]; then
        "/C/Program Files (x86)/NSIS/makensis.exe" "Install.nsi"
        MD5=`md5sum SerialPortAssistant-Setup-*.exe|awk '{print $1}'`
        echo "MD5:${MD5}"
        build_${BUILD_TARGERT}/install/bin/SerialPortAssistant.exe -f "`pwd`/update_windows.xml" --md5 ${MD5}
        
        cat update_windows.xml
	fi
fi
