#!/bin/bash
# Author: Kang Lin <kl222@126.com>

# See: - https://docs.appimage.org/packaging-guide/from-source/native-binaries.html#examples
#      - https://luyuhuang.tech/2024/04/19/appimage.html

#See: https://blog.csdn.net/alwaysbefine/article/details/114187380
#set -x
set -e
#set -v

## building in temporary directory to keep system clean
## use RAM disk if possible (as in: not building on CI system like Travis, and RAM disk is available)
#if [ "$CI" == "" ] && [ -d /dev/shm ]; then
#    TEMP_BASE=/dev/shm
#else
#    TEMP_BASE=/tmp
#fi
#BUILD_DIR=$(mktemp -d -p "$TEMP_BASE" SerialPortAssistant-appimage-build-XXXXXX)
## make sure to clean up build dir, even if errors occur

# store repo root as variable
REPO_ROOT=$(readlink -f $(dirname $(dirname $(readlink -f $0))))
OLD_CWD=$(readlink -f .)

pushd "$REPO_ROOT"

if [ -z "$BUILD_DIR" ]; then
    BUILD_DIR=build_appimage
fi
BUILD_DIR=$(readlink -f ${BUILD_DIR})
mkdir -p $BUILD_DIR
pushd "$BUILD_DIR"

if [ -z "$TOOLS_DIR" ]; then
    TOOLS_DIR=Tools
fi
if [ -z "$SOURCE_DIR" ]; then
    SOURCE_DIR=Source
fi
if [ -z "$INSTALL_DIR" ]; then
    INSTALL_DIR=Install
fi

TOOLS_DIR=$(readlink -f ${TOOLS_DIR})
mkdir -p $TOOLS_DIR
SOURCE_DIR=$(readlink -f ${SOURCE_DIR})
mkdir -p $SOURCE_DIR
INSTALL_DIR=$(readlink -f ${INSTALL_DIR})
mkdir -p $INSTALL_DIR

echo "Repo folder: $REPO_ROOT"
echo "Old folder: $OLD_CWD"
echo "Current folder: `pwd`"
echo "BUILD_DIR: $BUILD_DIR"
echo "TOOLS_DIR: $TOOLS_DIR"
echo "SOURCE_DIR: $SOURCE_DIR"
echo "INSTALL_DIR: $INSTALL_DIR"

cleanup () {
    if [ -d "${BUILD_DIR}" ]; then
        rm -rf "${BUILD_DIR}"
    fi
}
if [ "$CI" != "" ]; then
    trap cleanup EXIT
fi

pushd "${TOOLS_DIR}"
if [ ! -f linuxdeploy-`uname -m`.AppImage ]; then
    wget https://github.com/linuxdeploy/linuxdeploy/releases/download/1-alpha-20250213-2/linuxdeploy-`uname -m`.AppImage
    chmod u+x linuxdeploy-`uname -m`.AppImage
fi
if [ ! -f linuxdeploy-plugin-qt-`uname -m`.AppImage ]; then
    wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/1-alpha-20250213-1/linuxdeploy-plugin-qt-`uname -m`.AppImage
    chmod u+x linuxdeploy-plugin-qt-`uname -m`.AppImage
fi
popd

INSTALL_APP_DIR=AppDir/usr
cmake "$REPO_ROOT" \
  -DCMAKE_INSTALL_PREFIX=/usr \
  -DCMARK_SHARED=OFF \
  -DCMARK_TESTS=OFF \
  -DCMARK_STATIC=ON \
  -DQT_DEBUG_FIND_PACKAGE=ON \
  -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(nproc)
cmake --install . --config Release --component DependLibraries --prefix ${INSTALL_APP_DIR}
cmake --install . --config Release --component Runtime --prefix ${INSTALL_APP_DIR}
cmake --install . --config Release --component Application --prefix ${INSTALL_APP_DIR}

# See: https://github.com/linuxdeploy/linuxdeploy-plugin-qt
#export QMAKE=$Qt6_DIR/bin/qmake6
export EXTRA_PLATFORM_PLUGINS="libqxcb.so"
# Icons from theme are not displayed in QtWidgets Application: https://github.com/linuxdeploy/linuxdeploy-plugin-qt/issues/17
export EXTRA_QT_MODULES="svg"
${TOOLS_DIR}/linuxdeploy-`uname -m`.AppImage --appdir=AppDir -v0 \
    --plugin qt \
    --output appimage

chmod a+x Serial_Port_Assistant-`uname -m`.AppImage

if [ -z "$SerialPortAssistant_VERSION" ]; then
    cp Serial_Port_Assistant-`uname -m`.AppImage $REPO_ROOT
else
    mv Serial_Port_Assistant-`uname -m`.AppImage ../SerialPortAssistant_${SerialPortAssistant_VERSION}_Linux_`uname -m`.AppImage
fi

popd
popd
