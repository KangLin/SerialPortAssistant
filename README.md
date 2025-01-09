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

[![All releases downloads](https://img.shields.io/github/downloads/KangLin/SerialPortAssistant/total?label=Github%20downloads)](http://gra.caldis.me/?user=KangLin&repo=SerialPortAssistant)
[![Download SerialPortAssistant](https://img.shields.io/sourceforge/dt/serialportassistant.svg?label=Sourceforge%20downloads)](https://sourceforge.net/projects/serialportassistant/files/latest/download)

[![build](https://github.com/KangLin/SerialPortAssistant/actions/workflows/build.yml/badge.svg)](https://github.com/KangLin/SerialPortAssistant/actions/workflows/build.yml)
[![Build status](https://ci.appveyor.com/api/projects/status/y77e828ysqc79r9o?svg=true)](https://ci.appveyor.com/project/KangLin/serialportassistant)

--------------------------------

## Introduction
- Project position:
  + Main repository: https://github.com/KangLin/SerialPortAssistant
  + Mirrory repository:
    - https://gitlab.com/kl222/SerialPortAssistant
    - https://sourceforge.net/projects/serialportassistant/
    - https://gitee.com/kl222/SerialPortAssistant
    - https://invent.kde.org/kanglin/SerialPortAssistant
    - https://bitbucket.org/kl222/serialportassistant
- [Project home](http://kanglin.github.io/SerialPortAssistant)
- This project is a cross-platform serial assistant.
  It can run on:
  + [x] WINDOWS
  + [x] linux
  + [x] Android
  + [x] mac, os  
    - Mac os, I don't have the corresponding equipment,
    please compile and test the user with the corresponding equipment.
    - Currently compiles successfully.
    but the loading dynamic library path is not set correctly.
    Please help you set it.
    
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
  
    [serialportassistant_0.5.28_amd64.deb](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/serialportassistant_0.5.28_amd64.deb)

        sudo apt install ./serialportassistant_0.5.28_amd64.deb
        # If you're on an older Linux distribution, you will need to run this instead:         
        # sudo dpkg -i serialportassistant_0.5.28_amd64.deb
        # sudo apt-get install -f # Install dependencies

  - Windows
    - Windows xp
      - [SerialPortAssistant_v0.5.28_windows_xp_x86_Setup.exe](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/SerialPortAssistant_v0.5.28_windows_xp_x86_Setup.exe)
    - Windows 7 or later
      - [SerialPortAssistant_v0.5.28_win32_msvc2017_qt5.12.12_Setup.exe](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/SerialPortAssistant_v0.5.28_win32_msvc2017_qt5.12.12_Setup.exe)
      - [SerialPortAssistant_v0.5.28_win64_msvc2022_64_qt6.8.1_Setup.exe](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/SerialPortAssistant_v0.5.28_win64_msvc2022_64_qt6.8.1_Setup.exe)

  - Android 9 or later
    - [SerialPortAssistant_v0.5.28_android_arm64_v8a_qt6.8.1_Release.apk](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/SerialPortAssistant_v0.5.28_android_arm64_v8a_qt6.8.1_Release.apk)
    - [SerialPortAssistant_v0.5.28_android_x86_64_qt6.8.1_Release.apk](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.5.28/SerialPortAssistant_v0.5.28_android_x86_64_qt6.8.1_Release.apk)

## Donate

If you are satisfied, you can donate to me

[![donation](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute_en.png "donation")](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute_en.png "donation") 

If it cannot be displayed, please open:
- https://gitlab.com/kl222/RabbitCommon/-/raw/master/Src/Resource/image/Contribute.png
- https://sourceforge.net/p/rabbitcommon/code/ci/master/tree/Src/Resource/image/Contribute.png

Bitcoin:

- Address: bc1qvlmsp9agt035h7kue0v8eae4wcjhj93zdzmuqp
- [![bitcoin](Docs/bitcoin.png "bitcoin")](docs/Image/Screenshot/bitcoin.png "bitcoin")

## Developer  
### Dependence  

+ [Qt (LGPL v2.1)](http://qt.io/)
+ RabbitCommon: https://github.com/KangLin/RabbitCommon  
  It can be downloaded to the same directory of the project.
  If it is a different directory, set the environment variable `RabbitCommon_DIR`

        git clone https://github.com/KangLin/RabbitCommon.git

### Compile  
  
- Download source code

        git clone https://github.com/KangLin/SerialPortAssistant.git

- Compiled with QtCreator
  + Open the project file [CMakeLists.txt](CMakeLists.txt) with QtCreator
  + The RabbitCommon can be downloaded to the same directory of the project.
    If it is a different directory, Set the parameter `RabbitCommon_DIR`
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
  
        sudo apt-get install debhelper fakeroot build-essential cmake

  + Use system qt
    - qt6

          sudo apt-get install \
            qt6-tools-dev qt6-tools-dev-tools qt6-base-dev qt6-base-dev-tools \
            qt6-l10n-tools qt6-translations-l10n qt6-scxml-dev \
            qt6-webengine-dev qt6-webengine-dev-tools libqt6serialport6-dev

    See: [ubuntu.yml](.github/workflows/ubuntu.yml)
    - qt5
    
          sudo apt-get install qttools5-dev qttools5-dev-tools \
              qtbase5-dev qtbase5-dev-tools qttranslations5-l10n \
              libqt5scxml5-dev libqt5svg5-dev libqt5serialport5-dev \
              qtwebengine5-dev qtwebengine5-dev-tools qtwebengine5-private-dev

- deploy.sh: This script is used to generate new release version numbers and tag.
  Used only by the program publisher.
