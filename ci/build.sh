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
    if [ "$TRAVIS" = "true" ]; then
        export JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64
    fi
    case $BUILD_ARCH in
        arm*)
            export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/${QT_VERSION}/android_armv7
            ;;
        x86)
        export QT_ROOT=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}/${QT_VERSION}/android_x86
        ;;
    esac
    export PATH=${SOURCE_DIR}/Tools/apache-ant/bin:$JAVA_HOME:$PATH
fi

if [ "${BUILD_TARGERT}" = "unix" ]; then
    if [ "$BUILD_DOWNLOAD" = "TRUE" ]; then
        QT_DIR=${SOURCE_DIR}/Tools/Qt/${QT_VERSION}
        export QT_ROOT=${QT_DIR}/${QT_VERSION}/gcc_64
    else
        #source /opt/qt${QT_VERSION_DIR}/bin/qt${QT_VERSION_DIR}-env.sh
        export QT_ROOT=/opt/qt${QT_VERSION_DIR}
    fi
    export PATH=$QT_ROOT/bin:$PATH
    export LD_LIBRARY_PATH=$QT_ROOT/lib/i386-linux-gnu:$QT_ROOT/lib:$LD_LIBRARY_PATH
    export PKG_CONFIG_PATH=$QT_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH
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

if [ "ON" = "${STATIC}" ]; then
    CONFIG_PARA="CONFIG*=static"
fi

export VERSION="v0.4.6"
if [ "${BUILD_TARGERT}" = "unix" ]; then
    cd $SOURCE_DIR
    bash build_debpackage.sh ${QT_ROOT}
    sudo dpkg -i ../serialportassistant_*_amd64.deb
    $SOURCE_DIR/test/test_linux.sh

    cd debian/serialportassistant/opt
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${QT_ROOT}/bin:${QT_ROOT}/lib:`pwd`/SerialPortAssistant/bin:`pwd`/SerialPortAssistant/lib
    
    #wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage" -O linuxdeployqt.AppImage
    wget -c -nv https://github.com/probonopd/linuxdeployqt/releases/download/6/linuxdeployqt-6-x86_64.AppImage -O linuxdeployqt.AppImage
    chmod a+x linuxdeployqt.AppImage
    cd SerialPortAssistant

    ../linuxdeployqt.AppImage share/applications/*.desktop \
            -qmake=${QT_ROOT}/bin/qmake -appimage
    

    # Create appimage install package
    #cp ../SerialPort_Assistant-${VERSION}-x86_64.AppImage .
    cp $SOURCE_DIR/Install/install.sh .
    ln -s SerialPort_Assistant-${VERSION}-x86_64.AppImage SerialPortAssistant-x86_64.AppImage
    tar -czf SerialPortAssistant_${VERSION}.tar.gz \
        SerialPort_Assistant-${VERSION}-x86_64.AppImage \
        SerialPortAssistant-x86_64.AppImage \
        install.sh share
    
    #Create update_linux_appimage.xml
    MD5=`md5sum SerialPortAssistant_${VERSION}.tar.gz|awk '{print $1}'`
    echo "MD5:${MD5}"
    ./SerialPortAssistant-x86_64.AppImage \
            -f "`pwd`/update_linux_appimage.xml" \
            --md5 ${MD5} \
            --url "https://github.com/KangLin/SerialPortAssistant/releases/download/${VERSION}/SerialPortAssistant_${VERSION}.tar.gz"
    cat update_linux_appimage.xml
    
    #Create update_linux.xml
    MD5=`md5sum $SOURCE_DIR/../serialportassistant_*_amd64.deb|awk '{print $1}'`
    echo "MD5:${MD5}"
    ./bin/SerialPortAssistant \
            -f "`pwd`/update_linux.xml" \
            --md5 ${MD5}
    cat update_linux.xml
    
    if [ "$TRAVIS_TAG" != "" -a "${QT_VERSION_DIR}" = "512" ]; then
        export UPLOADTOOL_BODY="Release SerialPortAssistant-${VERSION}"
        #export UPLOADTOOL_PR_BODY=
        wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
        bash upload.sh $SOURCE_DIR/../serialportassistant_*_amd64.deb update_linux.xml
        bash upload.sh SerialPortAssistant_${VERSION}.tar.gz update_linux_appimage.xml
    fi
    exit 0
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
        install/bin/SerialPortAssistant.exe -f "`pwd`/update_windows.xml" --md5 ${MD5}
        
        cat update_windows.xml
	fi
fi
