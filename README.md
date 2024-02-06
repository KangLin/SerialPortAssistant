# Serial Port Assistant

Author: Kang Lin (kl222@126.com)

--------------------------------

[:cn: 中文](README_zh_CN.md)

[![GitHub stars](https://img.shields.io/github/stars/KangLin/SerialPortAssistant?label=Github%20stars)](https://star-history.com/#KangLin/SerialPortAssistant&Date)
[![star](https://gitee.com/kl222/SerialPortAssistant/badge/star.svg?theme=dark)](https://gitee.com/kl222/SerialPortAssistant/stargazers)

[![GitHub forks](https://img.shields.io/github/forks/KangLin/SerialPortAssistant)](https://github.com/KangLin/SerialPortAssistant/forks)
[![GitHub issues](https://img.shields.io/github/issues/KangLin/SerialPortAssistant)](https://github.com/KangLin/SerialPortAssistant/issues)

[![Release](https://img.shields.io/github/release/KangLin/SerialPortAssistant?label=Github%20release)](https://github.com/KangLin/SerialPortAssistant/releases/latest)
[![Download SerialPortAssistant](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/serialportassistant/files/latest/download)

[![All releases downloads](https://img.shields.io/github/downloads/KangLin/SerialPortAssistant/total?label=Github%20downloads)](https://github.com/KangLin/SerialPortAssistant/releases)
[![Download SerialPortAssistant](https://img.shields.io/sourceforge/dt/serialportassistant.svg?label=Sourceforge%20downloads)](https://sourceforge.net/projects/serialportassistant/files/latest/download)

[![build](https://github.com/KangLin/SerialPortAssistant/actions/workflows/build.yml/badge.svg)](https://github.com/KangLin/SerialPortAssistant/actions/workflows/build.yml)

--------------------------------

## Introduction
- Project position:
  + Main repository: https://github.com/KangLin/SerialPortAssistant
  + Mirrory repository:
    - https://gitlab.com/kl222/SerialPortAssistant
    - https://sourceforge.net/projects/serialportassistant/
    - https://gitee.com/kl222/SerialPortAssistant
    - https://invent.kde.org/kanglin/SerialPortAssistant
- [Project home](http://kanglin.github.io/SerialPortAssistant)
- This project is a cross-platform serial assistant.
  It can run on:
  + [x] WINDOWS
  + [x] linux
  + [x] Android
  + [x] mac, os  
    Mac os, I don't have the corresponding equipment,
    please compile and test the user with the corresponding equipment.
    
  See: [Qt5 supported platforms](https://doc.qt.io/qt-5/supported-platforms.html), [Qt6 supported platforms](https://doc.qt.io/qt-6/supported-platforms.html).
- Multi-language
  + Chinese
  
    ![Chinese](Docs/ui-zh.jpg)

  + Englisth

    ![Englisth](Docs/ui-en.jpg)

- Multi style

## Download

- Download from:
  - [![Github release](https://img.shields.io/github/release/KangLin/SerialPortAssistant?label=Github%20release)](https://github.com/KangLin/SerialPortAssistant/releases/latest)
  - [![Download SerialPortAssistant](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/serialportassistant/files/latest/download)

- Install
  - ubuntu

        sudo apt install ./serialportassistant_0.5.17_amd64.deb
        # If you're on an older Linux distribution, you will need to run this instead:         
        # sudo dpkg -i serialportassistant_0.5.17_amd64.deb
        # sudo apt-get install -f # Install dependencies

## Donate

If you are satisfied, you can donate to me

[![donation](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "donation")](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "donation") 

If it cannot be displayed, please open:
- https://gitlab.com/kl222/RabbitCommon/-/raw/master/Src/Resource/image/Contribute.png
- https://sourceforge.net/p/rabbitcommon/code/ci/master/tree/Src/Resource/image/Contribute.png

## Developer  
### Dependence  

+ [Qt (LGPL v2.1)](http://qt.io/)
+ RabbitCommon: https://github.com/KangLin/RabbitCommon  
  It can be downloaded to the same directory of the project.
  If it is a different directory, set the environment variable RabbitCommon_DIR 

        git clone https://github.com/KangLin/RabbitCommon.git

### Compile  
  
- Download source code

        git clone https://github.com/KangLin/SerialPortAssistant.git

- Compiled with QtCreator
  + Open the project file [CMakeLists.txt](CMakeLists.txt) with QtCreator
  + The RabbitCommon can be downloaded to the same directory of the project.
    If it is a different directory, Set the parameter RabbitCommon_DIR
  + Direct compilation can generate programs
- Compile with the command line
  + Download source code and the deplendence in same directory
  
        git clone https://github.com/KangLin/RabbitCommon.git
        git clone https://github.com/KangLin/SerialPortAssistant.git

  + Make the build directory in the project root directory

        cd SerialPortAssistant
        mkdir build
        cd build
        # cmake configure
        cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install [-DRabbitCommon_DIR=...]
        # compiling
        cmake --build . --config Release --target install
        cd install  # The program is installed in the install/bin directory

### Script

- build_debpackage.sh
  + This script generates the deb package under linux.
    Before use, please make sure the following programs are installed
  
        sudo apt-get install debhelper fakeroot 
    
  + Use system qt

        sudo apt-get install -y -qq qttools5-dev qttools5-dev-tools \
                 libqt5serialport5-dev qtbase5-dev qtbase5-dev-tools

- deploy.sh: This script is used to generate new release version numbers and tag.
  Used only by the program publisher.
