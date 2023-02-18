/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    Global.h

Abstract:

    This file contains global variable define.
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include "RabbitCommonDir.h"

#include <QObject>

/** 
 * @mainpage
 * 
 *
 */

/**
  * @defgroup RABBITGIS_GLOBAL 全局变更类模块  
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
    bool GetStatusbarVisable();
    int SetStatusbarVisable(bool bVisable);
private:
    bool m_StatusbarVisable;

public:
    bool GetToolbarVisable();
    int SetToolbarVisable(bool bVisable);
private:
    bool m_ToolbarVisable;

public:
    bool GetLeftBarVisable();
    int SetLeftBarVisable(bool bVisable);
private:
    bool m_LeftBarVisable;
    
public:
    QString GetLanguage();
    int SetLanguage(QString szLanguage);
private:
    QString m_szLanguage;

public:
    QString GetStyle();
    QString GetStyleMenu();
    int SetStyleMenu(QString szMenu, QString szFile);
private:
    QString m_szStyleFile;
    QString m_szStyleMenu;
    
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
