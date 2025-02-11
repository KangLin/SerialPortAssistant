#!/bin/bash

# See: https://docs.appimage.org/packaging-guide/from-source/native-binaries.html#examples

set -x
set -e

## building in temporary directory to keep system clean
## use RAM disk if possible (as in: not building on CI system like Travis, and RAM disk is available)
#if [ "$CI" == "" ] && [ -d /dev/shm ]; then
#    TEMP_BASE=/dev/shm
#else
#    TEMP_BASE=/tmp
#fi
#BUILD_DIR=$(mktemp -d -p "$TEMP_BASE" serialportassistant-appimage-build-XXXXXX)
## make sure to clean up build dir, even if errors occur


BUILD_DIR=build_serialportassistant_appimage
mkdir -p $BUILD_DIR

cleanup () {
    if [ -d "${BUILD_DIR}" ]; then
        rm -rf "${BUILD_DIR}"
    fi
}
if [ "$CI" != "" ]; then
    trap cleanup EXIT
fi

# store repo root as variable
REPO_ROOT=$(readlink -f $(dirname $(dirname $0)))
OLD_CWD=$(readlink -f .)
INSTALL_DIR=AppDir/usr

# switch to build dir
pushd "${BUILD_DIR}"

cmake "$REPO_ROOT" \
  -DCMAKE_INSTALL_PREFIX=/usr \
  -DCMARK_SHARED=OFF \
  -DCMARK_TESTS=OFF \
  -DCMARK_STATIC=ON \
  -DQT_DEBUG_FIND_PACKAGE=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr \
  -DBUILD_QUIWidget=OFF
cmake --build . --parallel $(nproc)
cmake --install . --config Release --component DependLibraries --prefix ${INSTALL_DIR}
cmake --install . --config Release --component Runtime --prefix ${INSTALL_DIR}
cmake --install . --config Release --component Application --prefix ${INSTALL_DIR}

if [ ! -f linuxdeploy-`uname -m`.AppImage ]; then
	wget https://github.com/linuxdeploy/linuxdeploy/releases/download/2.0.0-alpha-1-20241106/linuxdeploy-`uname -m`.AppImage
	chmod u+x linuxdeploy-`uname -m`.AppImage
fi
if [ ! -f linuxdeploy-plugin-qt-`uname -m`.AppImage ]; then
    wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/2.0.0-alpha-1-20241106/linuxdeploy-plugin-qt-`uname -m`.AppImage
    chmod u+x linuxdeploy-plugin-qt-`uname -m`.AppImage
fi

# See: https://github.com/linuxdeploy/linuxdeploy-plugin-qt
export QMAKE=${QT_ROOT}/bin/qmake
export EXTRA_PLATFORM_PLUGINS="libqxcb.so"
# Icons from theme are not displayed in QtWidgets Application: https://github.com/linuxdeploy/linuxdeploy-plugin-qt/issues/17
export EXTRA_QT_MODULES="svg"
./linuxdeploy-`uname -m`.AppImage --appdir=AppDir -v0 \
    --deploy-deps-only=${INSTALL_DIR}/lib/`uname -m`-linux-gnu \
    --plugin qt \
    --output appimage

cp Serial_Port_Assistant-`uname -m`.AppImage $REPO_ROOT
