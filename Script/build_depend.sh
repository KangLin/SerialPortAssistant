#!/bin/bash
# Author: Kang Lin <kl222@126.com>

#See: https://blog.csdn.net/alwaysbefine/article/details/114187380
#set -x
set -e
#set -v

PACKAGE_TOOL=apt
PACKAGE=
APT_UPDATE=0
BASE_LIBS=0
DEFAULT_LIBS=0
QT=0
RabbitCommon=0

usage_long() {
    echo "$0 [--install=<install directory>] [--source=<source directory>] [--tools=<tools directory>] [--build=<build directory>] [--package=<'package1 package2 ...'>] [--package-tool=<apt|dnf>] [--apt_update=[0|1]] [--base[=0|1]] [--default[=0|1]] [--qt[=0|1]] [--rabbitcommon[=0|1]]"
    echo "Directory:"
    echo "  --install: Set install directory"
    echo "  --source: Set source directory"
    echo "  --tools: Set tools directory"
    echo "  --build: set build directory"
    echo "Depend:"
    echo "  --base: Install the base libraries with apt"
    echo "  --default: Install the default dependency libraries that comes with the system"
    echo "  --apt_update: Update system"
    echo "  --package-tool: Package install tool, apk or dnf"
    echo "  --package: Install package"
    echo "  --qt: Install QT"
    echo "  --rabbitcommon: Install RabbitCommon"
}

# [如何使用getopt和getopts命令解析命令行选项和参数](https://zhuanlan.zhihu.com/p/673908518)
# [【Linux】Shell命令 getopts/getopt用法详解](https://blog.csdn.net/arpospf/article/details/103381621)
if command -V getopt >/dev/null; then
    echo "getopt is exits"
    #echo "original parameters=[$@]"
    # -o 或 --options 选项后面是可接受的短选项，如 ab:c:: ，表示可接受的短选项为 -a -b -c ，
    # 其中 -a 选项不接参数，-b 选项后必须接参数，-c 选项的参数为可选的
    # 后面没有冒号表示没有参数。后跟有一个冒号表示有参数。跟两个冒号表示有可选参数。
    # -l 或 --long 选项后面是可接受的长选项，用逗号分开，冒号的意义同短选项。
    # -n 选项后接选项解析错误时提示的脚本名字
    OPTS=help,install:,source:,tools:,build:,package:,package-tool:,apt_update::,base::,default::,qt::,rabbitcommon::
    ARGS=`getopt -o h -l $OPTS -n $(basename $0) -- "$@"`
    if [ $? != 0 ]; then
        echo "exec getopt fail: $?"
        exit 1
    fi
    #echo "ARGS=[$ARGS]"
    #将规范化后的命令行参数分配至位置参数（$1,$2,......)
    eval set -- "${ARGS}"
    #echo "formatted parameters=[$@]"

    while [ $1 ]
    do
        #echo "\$1: $1"
        #echo "\$2: $2"
        case $1 in
        --install)
            INSTALL_DIR=$2
            shift 2
            ;;
        --source)
            SOURCE_DIR=$2
            shift 2
            ;;
        --tools)
            TOOLS_DIR=$2
            shift 2
            ;;
        --build)
            BUILD_DIR=$2
            shift 2
            ;;
        --package)
            PACKAGE=$2
            shift 2
            ;;
        --package-tool)
            PACKAGE_TOOL=$2
            shift 2
            ;;
        --apt_update)
            case $2 in
                "")
                    APT_UPDATE=1;;
                *)
                    APT_UPDATE=$2;;
            esac
            shift 2
            ;;
        --base)
            case $2 in
                "")
                    BASE_LIBS=1;;
                *)
                    BASE_LIBS=$2;;
            esac
            shift 2
            ;;
        --default)
            case $2 in
                "")
                    DEFAULT_LIBS=1;;
                *)
                    DEFAULT_LIBS=$2;;
            esac
            shift 2
            ;;
        --qt)
            case $2 in
                "")
                    QT=1;;
                *)
                    QT=$2;;
            esac
            shift 2
            ;;
        --rabbitcommon)
            case $2 in
                "")
                    RabbitCommon=1;;
                *)
                    RabbitCommon=$2;;
            esac
            shift 2
            ;;
        --) # 当解析到“选项和参数“与“non-option parameters“的分隔符时终止
            shift
            break
            ;;
        -h | -help)
            usage_long
            shift
            ;;
        *)
            usage_long
            break
            ;;
        esac
    done
fi

# store repo root as variable
REPO_ROOT=$(readlink -f $(dirname $(dirname $(readlink -f $0))))
OLD_CWD=$(readlink -f .)

pushd "$REPO_ROOT"

if [ -z "$BUILD_DIR" ]; then
    BUILD_DIR=build_depend
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

if [ $APT_UPDATE -eq 1 ]; then
    echo "apt update ......"
    apt-get update -y
    apt-get upgrade -y
fi

if [ -n "$PACKAGE" ]; then
    ${PACKAGE_TOOL} install -y -q $PACKAGE
fi

if [ $BASE_LIBS -eq 1 ]; then
    echo "Install base libraries ......"
    if [ "$PACKAGE_TOOL" = "apt" ]; then
        apt install -y -q build-essential \
            git cmake gcc g++ debhelper fakeroot graphviz gettext \
            xserver-xorg-input-mouse xserver-xorg-input-kbd \
            libglu1-mesa-dev libpulse-mainloop-glib0
        if [ -z "$SerialPortAssistant_VERSION" ]; then
            apt install -y -q xvfb xpra
        fi
        # Base dependency
        apt install -y -q libssl-dev libcrypt-dev libicu-dev zlib1g-dev
        # RabbitCommon dependency
        apt install -y -q libcmark-dev cmark
        # AppImage
        apt install -y -q libfuse-dev libfuse3-dev
    fi
    
    if [ "$PACKAGE_TOOL" = "dnf" ]; then
        dnf install -y make git rpm-build rpmdevtools gcc-c++ util-linux \
           automake autoconf libtool gettext gettext-autopoint \
           cmake desktop-file-utils appstream appstream-util curl wget
    fi
fi

if [ $DEFAULT_LIBS -eq 1 ]; then
    echo "Install default dependency libraries ......"
    if [ "$PACKAGE_TOOL" = "apt" ]; then
        # Qt6
        apt-get install -y -q qmake6 qt6-tools-dev qt6-tools-dev-tools \
            qt6-base-dev qt6-base-dev-tools qt6-qpa-plugins \
            libqt6svg6-dev qt6-l10n-tools qt6-translations-l10n \
            qt6-scxml-dev qt6-multimedia-dev libqt6serialport6-dev
    fi
    if [ "$PACKAGE_TOOL" = "dnf" ]; then
        dnf install -y qt6-qttools-devel qt6-qtbase-devel qt6-qtmultimedia-devel \
            qt6-qt5compat-devel qt6-qtmultimedia-devel qt6-qtscxml-devel \
            qt6-qtserialport-devel qt6-qtsvg-devel
    fi
fi

if [ $QT -eq 1 ]; then
    echo "Install qt ......"
    pushd "$TOOLS_DIR"
    # See: https://ddalcino.github.io/aqt-list-server/
    apt install -y -q python3-pip python3-pip-whl python3-pipdeptree cpio
    #pip install -U pip
    pip install aqtinstall
    export QTVERSION=6.8.2
    if [ "`uname -m`" == "x86_64" ]; then
        aqt install-qt linux desktop ${QTVERSION} linux_gcc_64 -m qtscxml
        mv ${QTVERSION}/gcc_64 qt
    elif [ "`uname -m`" == "aarch64" ]; then
        aqt install-qt linux_arm64 desktop ${QTVERSION} linux_gcc_arm64 -m qtscxml
        mv ${QTVERSION}/gcc_arm64 qt
    fi
    popd
fi

if [ $RabbitCommon -eq 1 ]; then
    pushd "$SOURCE_DIR"
    if [ ! -d RabbitCommon ]; then
        git clone https://github.com/KangLin/RabbitCommon.git
    else
        cd RabbitCommon
        git pull
        cd ..
    fi
    popd
fi
