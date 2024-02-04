＃ 串口助手

作者：康林（kl222@126.com)

--------------------------------

[:us: English](README.md)

[![GitHub 点赞量](https://img.shields.io/github/stars/KangLin/SerialPortAssistant?label=Github%20点赞量)](https://star-history.com/#KangLin/SerialPortAssistant&Date)
[![star](https://gitee.com/kl222/SerialPortAssistant/badge/star.svg?theme=dark)](https://gitee.com/kl222/SerialPortAssistant/stargazers)

[![GitHub forks](https://img.shields.io/github/forks/KangLin/SerialPortAssistant)](https://github.com/KangLin/SerialPortAssistant/forks)
[![GitHub issues](https://img.shields.io/github/issues/KangLin/SerialPortAssistant)](https://github.com/KangLin/SerialPortAssistant/issues)

[![github 最后发行版本下载](https://img.shields.io/github/release/KangLin/SerialPortAssistant?label=Github%20最后发行版本下载)](https://github.com/KangLin/SerialPortAssistant/releases/latest)
[![sourceforge 最后发行版本下载](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/serialportassistant/files/latest/download)

[![github 下载量](https://img.shields.io/github/downloads/KangLin/SerialPortAssistant/total?label=Github%20下载量)](https://github.com/KangLin/SerialPortAssistant/releases)
[![Download SerialPortAssistant](https://img.shields.io/sourceforge/dt/serialportassistant.svg?label=Sourceforge%20下载量)](https://sourceforge.net/projects/serialportassistant/files/latest/download)

[![build](https://github.com/KangLin/SerialPortAssistant/actions/workflows/build.yml/badge.svg)](https://github.com/KangLin/SerialPortAssistant/actions/workflows/build.yml)

--------------------------------

### 简介
- 项目位置:
  + 主库: https://github.com/KangLin/SerialPortAssistant
  + 镜像库:
    - 中国： https://gitee.com/kl222/SerialPortAssistant
    - https://gitlab.com/kl222/SerialPortAssistant
    - https://sourceforge.net/projects/serialportassistant/
    - https://invent.kde.org/kanglin/SerialPortAssistant
- [项目主页](http://kanglin.github.io/SerialPortAssistant)
- 本项目是一个跨平台的串口助手。  
  它能运行在:
  + [x] WINDOWS
  + [x] Linux
  + [x] Android
  + [x] mac、os  本人没有mac设备，请有些设备的同学自行编译和测试
  
  详见：[Qt5 支持平台](https://doc.qt.io/qt-5/supported-platforms.html)、[Qt6 支持平台](https://doc.qt.io/qt-6/supported-platforms.html)。
- 程序截屏
  + 多语言
    - 中文  
    ![中文](Docs/ui-zh.jpg "中文")
    - 英语      
    ![英文](Docs/ui-en.jpg "英文")
- 多样式


### 下载
- [![github 最后发行版本下载](https://img.shields.io/github/release/KangLin/SerialPortAssistant?label=Github%20最后发行版本下载)](https://github.com/KangLin/SerialPortAssistant/releases/latest)
- [![Download SerialPortAssistant](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/serialportassistant/files/latest/download) 

- windows: 支持 Windows 7 及以后系统
  + [Download SerialPortAssistant_Setup_v0.5.17.exe from github](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.17/SerialPortAssistant_Setup_v0.5.17.exe)
  + [Download SerialPortAssistant_Setup_v0.5.17.exe from sourceforge](https://sourceforge.net/projects/serialportassistant/files/v0.5.17/SerialPortAssistant_Setup_v0.5.17.exe/download)

- ubuntu
  + serialportassistant_0.5.17_amd64.deb
    - [从 sourceforge 下载](https://sourceforge.net/projects/serialportassistant/files/v0.5.17/serialportassistant_0.5.17_amd64.deb/download)
    - [从 github 下载](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.17/serialportassistant_0.5.17_amd64.deb)
  
          sudo apt install ./serialportassistant_0.5.17_amd64.deb
          # 如果你是老的 Linux 发行系统。你需要用下列命令替换：
          # sudo dpkg -i serialportassistant_0.5.17_amd64.deb
          # sudo apt-get install -f # Install dependencies

- android
  + 从 github 下载:
    - [SerialPortAssistant_v0.5.17_android_armv7_qt6.6.0_Release.apk](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.17/SerialPortAssistant_v0.5.17_android_armv7_qt6.6.0_Release.apk)
    - [SerialPortAssistant_v0.5.17_android_x86_qt6.6.0_Release.apk](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.17/SerialPortAssistant_v0.5.17_android_x86_qt6.6.0_Release.apk)

  + 从 sourceforge 下载
    - [SerialPortAssistant_v0.5.17_android_armv7_qt6.6.0_Release.apk](https://sourceforge.net/projects/serialportassistant/files/v0.5.17/SerialPortAssistant_v0.5.17_android_armv7_qt6.6.0_Release.apk/download)
    - [SerialPortAssistant_v0.5.17_android_x86_qt6.6.0_Release.apk](https://sourceforge.net/projects/serialportassistant/files/v0.5.17/SerialPortAssistant_v0.5.17_android_x86_qt6.6.0_Release.apk/download)

### 捐赠  
如果你用得满意，可以对本人进行捐赠  

[![捐赠](https://gitlab.com/kl222/RabbitCommon/-/raw/master/Src/Resource/image/Contribute.png "捐赠")](https://gitlab.com/kl222/RabbitCommon/-/raw/master/Src/Resource/image/Contribute.png "捐赠")

### 开发  
#### 依赖

- [Qt (LGPL v2.1)](http://qt.io/)
- RabbitCommon: https://github.com/KangLin/RabbitCommon  
  RabbitCommon 下载到与本项目同级目录中。如果不在同级目录，则需要指定环境变量： RabbitCommon_DIR

      git clone https://github.com/KangLin/RabbitCommon.git


#### 编译  

- 下载源码

      git clone https://github.com/KangLin/SerialPortAssistant.git

- 下载 RabbitCommon

      git clone https://github.com/KangLin/RabbitCommon.git
      
- 用 QtCreator 编译
  * 直接用 QtCreator 打开工程文件 [CMakeLists.txt](CMakeLists.txt)
  * 如果 RabbitCommon 不在本项目同级目录中，设置参数 RabbitCommon_DIR 指定其位置。 
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

        sudo apt-get install debhelper fakeroot

  + 用系统自带的 QT

        sudo apt-get install -y -qq qttools5-dev qttools5-dev-tools \
                 libqt5serialport5-dev qtbase5-dev qtbase5-dev-tools

- deploy.sh: 此脚本用于产生新的发行版本号和标签。仅程序发布者使用。
