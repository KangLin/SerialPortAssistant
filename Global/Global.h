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

#include "GlobalDir.h"
#include "Log.h"
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
    explicit CGlobal(QObject *parent = 0);
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
    bool GetReciveDisplayTime();
    int SetReciveDisplayTime(bool bDisplay);
private:
    bool m_bReciveDisplayTime;
    
public:
    bool GetReciveDisplaySend();
    int SetReciveDisplaySend(bool bDisplay);
private:
    bool m_bReciveDisplaySend;
    
public:
    enum CODE{
        ASCII = 0,
        HEX = 1
    };
    CODE GetReciveDisplayCode();
    int SetReciveDisplayCode(CODE code);
private:
    CODE m_ReciveDisplayCode;
    
public:
    CODE GetSendDisplayCode();
    int SetSendDisplayCode(CODE code);
private:
    CODE m_SendDisplayCode;

signals:

public slots:
};
#endif // GLOBAL_H
