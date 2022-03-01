# Serial Port Assistant

Author: Kang Lin (kl222@126.com)

--------------------------------

[:cn: 中文](README_zh_CN.md)

[![GitHub stars](https://img.shields.io/github/stars/KangLin/SerialPortAssistant?label=Github%20stars)](https://github.com/KangLin/SerialPortAssistant/stars)
[![star](https://gitee.com/kl222/SerialPortAssistant/badge/star.svg?theme=dark)](https://gitee.com/kl222/SerialPortAssistant/stargazers)

[![GitHub forks](https://img.shields.io/github/forks/KangLin/SerialPortAssistant)](https://github.com/KangLin/SerialPortAssistant/forks)
[![GitHub issues](https://img.shields.io/github/issues/KangLin/SerialPortAssistant)](https://github.com/KangLin/SerialPortAssistant/issues)

[![Release](https://img.shields.io/github/release/KangLin/SerialPortAssistant?label=Github%20release)](https://github.com/KangLin/SerialPortAssistant/releases/latest)
[![Download SerialPortAssistant](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/serialportassistant/files/latest/download)

[![All releases downloads](https://img.shields.io/github/downloads/KangLin/SerialPortAssistant/total?label=Github%20downloads)](https://github.com/KangLin/SerialPortAssistant/releases)
[![Download SerialPortAssistant](https://img.shields.io/sourceforge/dt/serialportassistant.svg?label=Sourceforge%20downloads)](https://sourceforge.net/projects/serialportassistant/files/latest/download)

[![windows、android](https://ci.appveyor.com/api/projects/status/y77e828ysqc79r9o?svg=true)](https://ci.appveyor.com/project/KangLin/serialportassistant)
[![Linux Build status](https://github.com/kanglin/SerialPortAssistant/actions/workflows/ubuntu.yml/badge.svg?branch=master)](https://github.com/kanglin/SerialPortAssistant/actions)
[![Android Build status](https://github.com/kanglin/SerialPortAssistant/actions/workflows/android.yml/badge.svg?branch=master)](https://github.com/kanglin/SerialPortAssistant/actions)

--------------------------------

## Introduction
- Project position:
  + Main repository: https://github.com/KangLin/SerialPortAssistant
  + Mirrory repository:
    - https://gitlab.com/kl222/SerialPortAssistant
    - https://sourceforge.net/projects/serialportassistant/
    - https://gitee.com/kl222/SerialPortAssistant
- [Project home](http://kanglin.github.io/SerialPortAssistant)
- This project is a cross-platform serial assistant. It can run on:
  + [x] WINDOWS
  + [x] linux
  + [x] Android
  + [x] mac, os  
    Mac os, I don't have the corresponding equipment,
    please compile and test the user with the corresponding equipment.
- Multi-language
  + Chinese
  
    ![Chinese](Docs/ui-zh.jpg)

  + Englisth

    ![Englisth](Docs/ui-en.jpg)

- Multi style

## Download
- [![Github release](https://img.shields.io/github/release/KangLin/SerialPortAssistant?label=Github%20release)](https://github.com/KangLin/SerialPortAssistant/releases/latest)
- [![Download SerialPortAssistant](https://a.fsdn.com/con/app/sf-download-button)](https://sourceforge.net/projects/serialportassistant/files/latest/download) 

- windows
  + [Download SerialPortAssistant-Setup-v0.5.9.exe from github](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.9/SerialPortAssistant-Setup-v0.5.9.exe)
  + [Download SerialPortAssistant-Setup-v0.5.9.exe from sourceforge](https://sourceforge.net/projects/serialportassistant/files/v0.5.9/SerialPortAssistant-Setup-v0.5.9.exe/download)

- ubuntu
  + Download from sourceforge:
    - [serialportassistant_0.5.9_Ubuntu-20.04_amd64.deb](https://sourceforge.net/projects/serialportassistant/files/v0.5.9/serialportassistant_0.5.9_Ubuntu-20.04_amd64.deb/download)
    - [serialportassistant_0.5.9_Ubuntu-18.04_amd64.deb](https://sourceforge.net/projects/serialportassistant/files/v0.5.9/serialportassistant_0.5.9_Ubuntu-18.04_amd64.deb/download)
  + Download from github:
    - [serialportassistant_0.5.9_Ubuntu-20.04_amd64.deb](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.9/serialportassistant_0.5.9_Ubuntu-20.04_amd64.deb)
    - [serialportassistant_0.5.9_Ubuntu-18.04_amd64.deb](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.9/serialportassistant_0.5.9_Ubuntu-18.04_amd64.deb)

          sudo apt install ./serialportassistant_0.5.9_Ubuntu-20.04_amd64.deb
          # If you're on an older Linux distribution, you will need to run this instead:         
          # sudo dpkg -i serialportassistant_0.5.9_Ubuntu-20.04_amd64.deb
          # sudo apt-get install -f # Install dependencies

- android
  + Download from github:
    - [SerialPortAssistant_armeabi-v7a_v0.5.9.apk](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.9/SerialPortAssistant_armeabi-v7a_v0.5.9.apk)
    - [SerialPortAssistant_x86_v0.5.9.apk](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.9/SerialPortAssistant_x86_v0.5.9.apk)

## Donate  
If you are satisfied, you can donate to me

[![donation](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "donation")](https://gitee.com/kl222/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "donation")

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
  + Open the project file ![SerialPortAssistant.pro](SerialPortAssistant.pro) with QtCreator
  + The RabbitCommon can be downloaded to the same directory of the project.
    If it is a different directory, Set the parameter RabbitCommon_DIR

        Project -> Build & Run -> Build -> Build settings -> Build step -> qmake 

  + Direct compilation can generate programs
- Compile with the command line
  + Build the build directory in the project root directory
  + *qmake* build compilation project
    - *make* install Build the program with the local toolchain
    - The program is generated in the *install* directory

          mkdir build
          cd build
          qmake ../SerialPortAssistant.pro
          make install
          cd install  #The program is generated in the install directory

### Script  

- build_debpackage.sh
  + This script generates the deb package under linux.
    Before use, please make sure the following programs are installed
  
        sudo apt-get install debhelper fakeroot 
    
  + Use system qt

        sudo apt-get install -y -qq qttools5-dev qttools5-dev-tools \
                 libqt5serialport5-dev qtbase5-dev qtbase5-dev-tools

- tag.sh: This script is used to generate new release version numbers and tag
