串口助手
=======

作者：康林（kl222@126.com)
--------

[<img src="Resource/png/English.png" alt="English" title="English" width="16" height="16" />英语](README.md)

1. ![项目位置:https://github.com/KangLin/SerialPortAssistant](https://github.com/KangLin/SerialPortAssistant)  
本项目是一个跨平台的串口助手。它能运行在WINDOWS、linux系统上。

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

6. 捐赠  
如果你用得满意，可以对本人进行捐赠
    * 扫描下面二维码进行任意金额捐赠  
    ![支付宝(大于 ￥20 )](Resource/png/zhifubao.png  "支付宝( 大于 ￥20 )")
    ![微信( 大于 ￥20 )](Resource/png/weixinpay.png "微信( 大于 ￥20 )")
    * 扫描下面二维码捐赠￥20  
    ![支付宝（￥20）](Resource/png/zhifubao20.png  "支付宝捐赠")
    ![微信捐赠](Resource/png/weixinpay20.png "微信捐赠")

7. 编译
    - 用 QtCreator 编译
      * 直接用 QtCreator 打开工程文件 ![SerialPortAssistant.pro](SerialPortAssistant.pro) 
      * 直接编译就可以生成程序
    - 用命令行编译
      * 在项目根目录下建立 build 目录
      * qmake 生成编译工程
      * make install 用本地工具链生成程序
      * 在 install 目录下会生成程序

    ```sh
    mkdir build
    cd build
    qmake ../SerialPortAssistant.pro
    make install
    cd install #进入生成的程序目录
    ```
