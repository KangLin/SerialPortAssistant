串口助手
=======

作者：康林（kl222@126.com)
--------

[<img src="Resource/png/English.png" alt="English" title="English" width="16" height="16" />英语](README.md)

1. ![项目位置:https://github.com/KangLin/SerialPortAssistant](https://github.com/KangLin/SerialPortAssistant)  
本项目是一个跨平台的串口助手。它能运行在:
    - [x] WINDOWS
    - [x] linux
    - [x] mac  
        本人没有mac设备，请有些设备的同学自行编译和测试

2. 编译状态

    |平台|状态|
    |:---:|:------:|
    |windows、android|[![windows、android](https://ci.appveyor.com/api/projects/status/y77e828ysqc79r9o?svg=true)](https://ci.appveyor.com/project/KangLin/serialportassistant)|
    |linux|[![linux](https://travis-ci.org/KangLin/SerialPortAssistant.svg?branch=master)](https://travis-ci.org/KangLin/SerialPortAssistant)|

3. 多语言
    * 中文  
    ![中文](Docs/ui-zh.jpg)
    * 英语  
    ![中文](Docs/ui-en.jpg)

4. 多样式

5. ![发行版本下载](https://github.com/KangLin/SerialPortAssistant/releases)  
![最后发行版本下载](https://github.com/KangLin/SerialPortAssistant/releases/latest)

- windows
    + [SerialPortAssistant-Setup-v0.3.9.exe](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.3.9/SerialPortAssistant-Setup-v0.3.9.exe)

- linux
    + [SerialPortAssistant_v0.3.9.tar.gz](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.3.9/SerialPortAssistant_v0.3.9.tar.gz)  
        AppImage格式的执行程序，可直接运行在linux系统，详见：https://appimage.org/  
        使用:    
        1. 解压。复制 SerialPortAssistant_v0.3.9.tar.gz 到安装目录，然后解压：
    
                  mkdir SerialPortAssistant
                  cd SerialPortAssistant
                  cp $DOWNLOAD/SerialPortAssistant_v0.3.9.tar.gz .
                  tar xvfz SerialPortAssistant_v0.3.9.tar.gz
    
        2. install
    
                  ./install.sh install
    
        3. If you want to uninstall
    
                  ./install.sh remove

- ubuntu
    + [serialportassistant_0.3.9_amd64.deb](https://github.com/KangLin/SerialPortAssistant/releases/download/v0.3.9/serialportassistant_0.3.9_amd64.deb)

            sudo dpkg -i serialportassistant_0.3.9_amd64.deb

6. 捐赠  
如果你用得满意，可以对本人进行捐赠  

- 捐赠(大于￥20)：  
![捐赠( 大于 ￥20 )](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute.png "捐赠(大于￥20)")

- 捐赠￥20  
![捐赠￥20](https://github.com/KangLin/RabbitCommon/raw/master/Src/Resource/image/Contribute20.png "捐赠￥20")

7. 依赖
  RabbitCommon: https://github.com/KangLin/RabbitCommon
  
  ```
  git clon https://github.com/KangLin/RabbitCommon.git
  ```

8. 编译
    - 用 QtCreator 编译
      * 直接用 QtCreator 打开工程文件 ![SerialPortAssistant.pro](SerialPortAssistant.pro) 
      * 设置参数 RabbitCommon_DIR
      * 直接编译就可以生成程序
    - 用命令行编译
      * 在项目根目录下建立 build 目录
      * qmake 生成编译工程
      * make install 用本地工具链生成程序
      * 在 install 目录下会生成程序

    ```sh
    mkdir build
    cd build
    qmake ../SerialPortAssistant.pro RabbitCommon_DIR=
    make install
    cd install #进入生成的程序目录
    ```
