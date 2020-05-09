＃ 串口助手

作者：康林（kl222@126.com)
--------

[<img src="Resource/png/English.png" alt="English" title="English" width="16" height="16" />英语](README.md)

### 简介
- [项目位置](https://github.com/KangLin/SerialPortAssistant)
- [项目主页](http://kanglin.github.io/SerialPortAssistant)
- 本项目是一个跨平台的串口助手。它能运行在:
  - [x] WINDOWS
  - [x] Linux
  - [x] Android
  - [x] mac、os  本人没有mac设备，请有些设备的同学自行编译和测试
- 编译状态
[![windows、android](https://ci.appveyor.com/api/projects/status/y77e828ysqc79r9o?svg=true)](https://ci.appveyor.com/project/KangLin/serialportassistant)
[![linux](https://travis-ci.org/KangLin/SerialPortAssistant.svg?branch=master)](https://travis-ci.org/KangLin/SerialPortAssistant)

### 程序截屏

- 多语言
  * 中文  
    ![中文](Docs/ui-zh.jpg "中文")
  * 英语  
    ![英文](Docs/ui-en.jpg "英文")
- 多样式


### [发行版本下载](https://github.com/KangLin/SerialPortAssistant/releases)

- [最后发行版本下载](https://github.com/KangLin/SerialPortAssistant/releases/latest)
  - windows
    + [SerialPortAssistant-Setup-v0.5.4.exe](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.4/SerialPortAssistant-Setup-v0.5.4.exe)
  - linux
    + [SerialPortAssistant_v0.5.4.tar.gz](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.4/SerialPortAssistant_v0.5.4.tar.gz)

    AppImage格式的执行程序，可直接运行在linux系统，详见：https://appimage.org/

    使用:    
       1. 解压。复制 SerialPortAssistant_v0.5.4.tar.gz 到安装目录，然后解压：
    
        sudo mkdir /opt/SerialPortAssistant
        cd SerialPortAssistant
        sudo wget https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.4/SerialPortAssistant_v0.5.4.tar.gz
        sudo tar xvfz SerialPortAssistant_v0.5.4.tar.gz
    
       2. 安装
    
        sudo ./install1.sh install SerialPortAssistant
    
       3. 如果你想卸裁
    
        sudo ./install1.sh remove SerialPortAssistant

  - ubuntu
    + [serialportassistant_0.5.4_amd64.deb](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.4/serialportassistant_0.5.4_amd64.deb)

        sudo dpkg -i serialportassistant_0.5.4_amd64.deb

  - android
    + [SerialPortAssistant_v0.5.4.apk](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.4/SerialPortAssistant_v0.5.4.apk)

### 捐赠  
如果你用得满意，可以对本人进行捐赠  

[![捐赠](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠")](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠")

### 开发  
#### 依赖

- [Qt (LGPL v2.1)](http://qt.io/)
- RabbitCommon: https://github.com/KangLin/RabbitCommon
  
        git clone https://github.com/KangLin/RabbitCommon.git
 
#### 编译  

- 下载源码

        git clone https://github.com/KangLin/SerialPortAssistant.git

- 用 QtCreator 编译
  * 直接用 QtCreator 打开工程文件 ![SerialPortAssistant.pro](SerialPortAssistant.pro) 
  * 设置参数 RabbitCommon_DIR  
      项目 -> 编译和运行 -> 编译 -> 编译设置 -> 编译步骤 -> qmake -> 额外参数
  * 直接编译就可以生成程序
- 用命令行编译
  * 在项目根目录下建立 build 目录
  * qmake 生成编译工程
  * make install 用本地工具链生成程序
  * 在 install 目录下会生成程序

        mkdir build
        cd build
        qmake ../SerialPortAssistant.pro RabbitCommon_DIR=
        make install
        cd install #进入生成的程序目录

#### 脚本  

- build_debpackage.sh
  + 此脚本是 linux 下生成 deb 包的。使用前，请确保安装了下面程序

        sudo apt-get install debhelper fakeroot

  + 用系统自带的 QT

        sudo apt-get install -y -qq qttools5-dev qttools5-dev-tools \
                 libqt5serialport5-dev qtbase5-dev qtbase5-dev-tools

- tag.sh: 此脚本用于产生新的发行版本号和标签
