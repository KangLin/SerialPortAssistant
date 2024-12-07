# 串口助手

作者：康林（kl222@126.com)

--------------------------------

[:us: English](README.md)

[![GitHub 点赞量](https://img.shields.io/github/stars/KangLin/SerialPortAssistant?label=Github%20点赞量)](https://star-history.com/#KangLin/SerialPortAssistant&Date)
[![star](https://gitee.com/kl222/SerialPortAssistant/badge/star.svg?theme=dark)](https://gitee.com/kl222/SerialPortAssistant/stargazers)

[![GitHub forks](https://img.shields.io/github/forks/KangLin/SerialPortAssistant)](https://github.com/KangLin/SerialPortAssistant/forks)
[![GitHub issues](https://img.shields.io/github/issues/KangLin/SerialPortAssistant)](https://github.com/KangLin/SerialPortAssistant/issues)

[![github 最后发行版本下载](https://img.shields.io/github/release/KangLin/SerialPortAssistant?label=Github%20最后发行版本下载)](https://github.com/KangLin/SerialPortAssistant/releases/latest)
[![sourceforge 最后发行版本下载](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/serialportassistant/files/latest/download)

[![github 下载量](https://img.shields.io/github/downloads/KangLin/SerialPortAssistant/total?label=Github%20下载量)](http://gra.caldis.me/?user=KangLin&repo=SerialPortAssistant)
[![Download SerialPortAssistant](https://img.shields.io/sourceforge/dt/serialportassistant.svg?label=Sourceforge%20下载量)](https://sourceforge.net/projects/serialportassistant/files/latest/download)

[![build](https://github.com/KangLin/SerialPortAssistant/actions/workflows/build.yml/badge.svg)](https://github.com/KangLin/SerialPortAssistant/actions/workflows/build.yml)
[![Build status](https://ci.appveyor.com/api/projects/status/y77e828ysqc79r9o?svg=true)](https://ci.appveyor.com/project/KangLin/serialportassistant)

--------------------------------

### 简介
- 项目位置:
  + 主库: https://github.com/KangLin/SerialPortAssistant
  + 镜像库:
    - 中国： https://gitee.com/kl222/SerialPortAssistant
    - https://gitlab.com/kl222/SerialPortAssistant
    - https://sourceforge.net/projects/serialportassistant/
    - https://invent.kde.org/kanglin/SerialPortAssistant
    - https://bitbucket.org/kl222/serialportassistant
- [项目主页](http://kanglin.github.io/SerialPortAssistant)
- 本项目是一个跨平台的串口助手。  
  它能运行在:
  + [x] WINDOWS
  + [x] Linux
  + [x] Android
  + [x] mac、os
    - 本人没有mac设备，请有设备的朋友自行编译和测试
    - 当前可以成功编译，但是没有正确设置加载动态库路径。请会设置的朋友帮助设置。
  
  详见：[Qt5 支持平台](https://doc.qt.io/qt-5/supported-platforms.html)、[Qt6 支持平台](https://doc.qt.io/qt-6/supported-platforms.html)。
- 程序截屏
  + 多语言
    - 中文  
    ![中文](Docs/ui-zh.jpg "中文")
    - 英语  
    ![英文](Docs/ui-en.jpg "英文")
- 多样式

### 下载

- 从以下位置下载：
  - [![github 最后发行版本下载](https://img.shields.io/github/release/KangLin/SerialPortAssistant?label=Github%20最后发行版本下载)](https://github.com/KangLin/SerialPortAssistant/releases/latest)
  - [![Download SerialPortAssistant](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/serialportassistant/files/latest/download) 

- 安装说明：
  - ubuntu

    [serialportassistant_0.5.28_amd64.deb](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/serialportassistant_0.5.28_amd64.deb)

        sudo apt install ./serialportassistant_0.5.28_amd64.deb
        # 如果你是老的 Linux 发行系统。你需要用下列命令替换：
        # sudo dpkg -i serialportassistant_0.5.28_amd64.deb
        # sudo apt-get install -f # Install dependencies

  - Windows
    - Windows xp
      - [SerialPortAssistant_v0.5.28_windows_xp_x86_Setup.exe](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/SerialPortAssistant_v0.5.28_windows_xp_x86_Setup.exe)
    - Windows 7 及以后
      - [SerialPortAssistant_v0.5.28_win32_msvc2017_qt5.12.12_Setup.exe](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/SerialPortAssistant_v0.5.28_win32_msvc2017_qt5.12.12_Setup.exe)
      - [SerialPortAssistant_v0.5.28_win64_msvc2022_64_qt6.8.1_Setup.exe](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/SerialPortAssistant_v0.5.28_win64_msvc2022_64_qt6.8.1_Setup.exe)

  - Andoird 9 及以后
    - [SerialPortAssistant_v0.5.28_android_arm64_v8a_qt6.8.1_Release.apk](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/SerialPortAssistant_v0.5.28_android_arm64_v8a_qt6.8.1_Release.apk)
    - [SerialPortAssistant_v0.5.28_android_x86_64_qt6.8.1_Release.apk](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/SerialPortAssistant_v0.5.28_android_x86_64_qt6.8.1_Release.apk)

### 捐赠  
如果你用得满意，可以对本人进行捐赠  

[![捐赠](https://gitlab.com/kl222/RabbitCommon/-/raw/master/Src/Resource/image/Contribute.png "捐赠")](https://gitlab.com/kl222/RabbitCommon/-/raw/master/Src/Resource/image/Contribute.png "捐赠")

### 开发  
#### 依赖

- [Qt (LGPL v2.1)](http://qt.io/)
- RabbitCommon: https://github.com/KangLin/RabbitCommon  
  RabbitCommon 下载到与本项目同级目录中。如果不在同级目录，则需要指定环境变量： `RabbitCommon_DIR`

      git clone https://github.com/KangLin/RabbitCommon.git

#### 编译  

- 下载源码

      git clone https://github.com/KangLin/SerialPortAssistant.git

- 下载 RabbitCommon

      git clone https://github.com/KangLin/RabbitCommon.git

- 用 QtCreator 编译
  * 直接用 QtCreator 打开工程文件 [CMakeLists.txt](CMakeLists.txt)
  * 如果 RabbitCommon 不在本项目同级目录中，设置参数 `RabbitCommon_DIR` 指定其位置。 
  * 直接编译就可以生成程序
- 用命令行编译
  * 在同一目录中下载源码与依赖

        git clone https://github.com/KangLin/RabbitCommon.git
        git clone https://github.com/KangLin/SerialPortAssistant.git
  
  * 在项目根目录下建立 build 目录

        cd SerialPortAssistant
        mkdir build
        cd build
        # cmake 配置
        cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install [-DRabbitCommon_DIR=...]
        # 编译
        cmake --build . --config Release --target install
        cd install  # 程序安装在 install/bin 目录下

#### 脚本

- build_debpackage.sh
  + 此脚本是 linux 下生成 deb 包的。使用前，请确保安装了下面程序

        sudo apt-get install debhelper fakeroot build-essential cmake

  + 用系统自带的 QT
    - qt6
    
          sudo apt-get install \
              qt6-tools-dev qt6-tools-dev-tools qt6-base-dev qt6-base-dev-tools \
              qt6-l10n-tools qt6-translations-l10n qt6-scxml-dev \
              qt6-webengine-dev qt6-webengine-dev-tools libqt6serialport6-dev

    详见: [ubuntu.yml](.github/workflows/ubuntu.yml)
    - qt5
    
          sudo apt-get install qttools5-dev qttools5-dev-tools \
              qtbase5-dev qtbase5-dev-tools qttranslations5-l10n \
              libqt5scxml5-dev libqt5svg5-dev libqt5serialport5-dev \
              qtwebengine5-dev qtwebengine5-dev-tools qtwebengine5-private-dev

- deploy.sh: 此脚本用于产生新的发行版本号和标签。仅程序发布者使用。
