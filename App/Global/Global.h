/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin <kl222@126.com>

Module Name:

    Global.h

Abstract:

    This file contains global variable define.
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include "RabbitCommonDir.h"

#include <QObject>
#include <QSerialPort>

/** 
 * @mainpage
 * 
 *
 */

/**
  * @defgroup RABBITGIS_GLOBAL 全局变量类模块
  * @brief 全局变量模块
  */

/**
 * @brief The CGlobal class
 * @ingroup RABBITGIS_GLOBAL
 */
class CGlobal : public QObject
{
    Q_OBJECT

private:
    explicit CGlobal(QObject *parent = nullptr);
    virtual ~CGlobal();

public:
    static CGlobal* Instance();

public:
    bool GetStatusbarVisible();
    int SetStatusbarVisible(bool bVisible);
private:
    bool m_StatusbarVisible;

public:
    bool GetToolbarVisible();
    int SetToolbarVisible(bool bVisible);
private:
    bool m_ToolbarVisible;
    
public:
    bool GetSendLoop();
    int SetSendLoop(bool bLoop);
private:
    bool m_bSendLoop;

public:
    int GetSendLoopTime();
    int SetSendLoopTime(int nTime);
private:
    int m_nSendLoopTime;

public:
    bool GetSaveFile();
    int SetSaveFile(bool bSaveFile);
private:
    bool m_bSaveFile;

public:
    enum SEND_R_N{
        R = 0x1,
        N = 0x2,
        RN = 0x3
    };

    SEND_R_N GetSendRN();
    int SetSendRN(SEND_R_N v);
private:
    SEND_R_N m_SendRN;

public:
    bool GetReceiveDisplayTime();
    int SetReceiveDisplayTime(bool bDisplay);
private:
    bool m_bReceiveDisplayTime;
    
public:
    bool GetReceiveDisplaySend();
    int SetReceiveDisplaySend(bool bDisplay);
private:
    bool m_bReceiveDisplaySend;
    
public:
    enum ENCODE{
        ASCII = 0,
        HEX,
        UTF8,
        HEX_ASCII
    };
    Q_ENUM(ENCODE)
    ENCODE GetReceiveEncode();
    int SetReceiveEncode(ENCODE code);
private:
    ENCODE m_ReceiveDisplayCode;
    
public:
    ENCODE GetSendEncode();
    int SetSendEncode(ENCODE code);
private:
    ENCODE m_SendDisplayCode;

signals:

public slots:
};
#endif // GLOBAL_H
