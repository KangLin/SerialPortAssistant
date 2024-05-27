/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    Global.cpp

Abstract:

    This file contains global variable implement.
 */

#include "Global.h"
#include <QSettings>
#include <QLocale>
#include <QSerialPortInfo>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(log, "Global")

CGlobal::CGlobal(QObject *parent) :
    QObject(parent)
{
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    m_StatusbarVisible = conf.value("UI/Visible/Statusbar", "true").toBool();
    m_ToolbarVisible = conf.value("UI/Visible/Toolbar", "true").toBool();
    m_bSendLoop = conf.value("Settings/Send/Loop", "false").toBool();
    m_nSendLoopTime = conf.value("Settings/Send/LoopTime", 1000).toInt();
    m_SendRN = (CGlobal::SEND_R_N)conf.value("Settings/Send/SendRN", 0).toInt();
    m_bReceiveDisplayTime = conf.value("Settings/Receive/DisplayTime", "false").toBool();
    m_bReceiveDisplaySend = conf.value("Settings/Receive/DisplaySend", "false").toBool();
    m_bSaveFile = conf.value("Settings/Receive/SaveFile", "false").toBool();
    m_ReceiveDisplayCode = (ENCODE)conf.value("Settings/Receive/DisplayReceiveCode", 0).toInt();
    m_SendDisplayCode = (ENCODE)conf.value("Settings/Receive/DisplaySendCode", 0).toInt();
}

CGlobal::~CGlobal()
{
}

CGlobal* CGlobal::Instance()
{
    static CGlobal* p = NULL;
    if(!p)
        p = new CGlobal;
    return p;
}

bool CGlobal::GetStatusbarVisible()
{
    return m_StatusbarVisible;
}

int CGlobal::SetStatusbarVisible(bool bVisible)
{
    m_StatusbarVisible = bVisible;
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("UI/Visible/Statusbar", m_StatusbarVisible);
    return 0;
}

bool CGlobal::GetToolbarVisible()
{
    return m_ToolbarVisible;
}

int CGlobal::SetToolbarVisible(bool bVisible)
{
    m_ToolbarVisible = bVisible;
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("UI/Visible/Toolbar", m_ToolbarVisible);
    return 0;
}

bool CGlobal::GetSendLoop()
{
    return m_bSendLoop;
}

int CGlobal::SetSendLoop(bool bLoop)
{
    m_bSendLoop = bLoop;
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("Settings/Send/Loop", m_bSendLoop);
    return 0;   
}

int CGlobal::GetSendLoopTime()
{
    return m_nSendLoopTime;
}

int CGlobal::SetSendLoopTime(int nTime)
{
    m_nSendLoopTime = nTime;
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("Settings/Send/LoopTime", m_nSendLoopTime);
    return 0;   
}

CGlobal::SEND_R_N CGlobal::GetSendRN()
{
    return m_SendRN;
}

int CGlobal::SetSendRN(SEND_R_N v)
{
    m_SendRN = v;
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("Settings/Send/SendRN", m_SendRN);
    return 0;  
}

bool CGlobal::GetReceiveDisplayTime()
{
    return m_bReceiveDisplayTime;
}

int CGlobal::SetReceiveDisplayTime(bool bDisplay)
{
    m_bReceiveDisplayTime = bDisplay;
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("Settings/Receive/DisplayTime", m_bReceiveDisplayTime);
    return 0; 
}

bool CGlobal::GetReceiveDisplaySend()
{
    return m_bReceiveDisplaySend;
}

int CGlobal::SetReceiveDisplaySend(bool bDisplay)
{
    m_bReceiveDisplaySend = bDisplay;
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("Settings/Receive/DisplaySend", m_bReceiveDisplaySend);
    return 0; 
}

bool CGlobal::GetSaveFile()
{
    return m_bSaveFile;
}

int CGlobal::SetSaveFile(bool bSaveFile)
{
    m_bSaveFile = bSaveFile;
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("Settings/Receive/SaveFile", m_bSaveFile);
    return 0;
}

CGlobal::ENCODE CGlobal::GetReceiveEncode()
{
    return m_ReceiveDisplayCode;
}

int CGlobal::SetReceiveEncode(ENCODE code)
{
    m_ReceiveDisplayCode = code;
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("Settings/Receive/DisplayReceiveCode", m_ReceiveDisplayCode);
    return 0; 
}

CGlobal::ENCODE CGlobal::GetSendEncode()
{
    return m_SendDisplayCode;
}

int CGlobal::SetSendEncode(ENCODE code)
{
    m_SendDisplayCode = code;
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("Settings/Receive/DisplaySendCode", m_SendDisplayCode);
    return 0; 
}

int CGlobal::SaveSerialPort(const QSerialPort &port, int nIndex)
{
    if(-1 == nIndex)
        return -1;
    QSerialPortInfo info = QSerialPortInfo::availablePorts().at(nIndex);
    if(info.portName() != port.portName())
    {
        qCritical(log) << "The port name is different:"
                       << info.portName() << "!=" << port.portName();
        return -2;
    }

    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    conf.setValue("SerialPort/Name", port.portName());
    conf.setValue("SerialPort/Default", port.portName());
    conf.beginGroup("SerialPort/" + port.portName());
    conf.setValue("BaudRate", port.baudRate());
    conf.setValue("DataBit", port.dataBits());
    conf.setValue("Parity", port.parity());
    conf.setValue("StopBits", port.stopBits());
    conf.setValue("FlowControl", port.flowControl());
    conf.endGroup();
    return 0;
}

int CGlobal::LoadSerialPort(Para &para, int nIndex)
{
    QSettings conf(RabbitCommon::CDir::Instance()->GetFileUserConfigure(),
                   QSettings::IniFormat);
    QSerialPort port;
    QString szName;

    if(-1 == nIndex)
    {
        szName = conf.value("SerialPort/Default").toString();
    } else {
        if(QSerialPortInfo::availablePorts().count() <= 0) {
            qCritical(log) << "There are not serial port";
            return -1;
        }
        QSerialPortInfo info = QSerialPortInfo::availablePorts().at(nIndex);
        szName = info.portName();
        port.setPort(info);
    }

    if(szName.isEmpty()) {
        qWarning(log) << "No default port in configure file:"
                      << RabbitCommon::CDir::Instance()->GetFileUserConfigure();
        return -2;
    }

    para.name = szName;
    conf.beginGroup("SerialPort/" + szName);
    para.baudRate = conf.value("BaudRate", port.baudRate()).toInt();
    para.dataBit = (QSerialPort::DataBits)conf.value("DataBit", port.dataBits()).toInt();
    para.parity = (QSerialPort::Parity)conf.value("Parity", port.parity()).toInt();
    para.stopBits = (QSerialPort::StopBits)conf.value("StopBits", port.stopBits()).toInt();
    para.flowControl = (QSerialPort::FlowControl)conf.value("FlowControl", port.flowControl()).toInt();
    conf.endGroup();
    return 0;
}
