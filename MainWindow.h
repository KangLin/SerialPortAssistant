/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    MainWindow.h

Abstract:

    This file contains main windows define.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QMenu>
#include <QMap>
#include <QSharedPointer> 
#include <QTranslator>
#include <QLabel>
#include <QLayout>

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();
    
    void InitMenu();
    void ClearMenu();
    // Language menu    
private:
    struct _MENU{
        QString icon;
        QString text;
    };
    int LoadTranslate(QString szLocale = QString());
    int ClearTranslate();
    int InitMenuTranslate();
    int ClearMenuTranslate();
    QMap<QString, QAction*> m_ActionTranslator;
    QActionGroup m_ActionGroupTranslator;
    QSharedPointer<QTranslator> m_TranslatorQt;
    QSharedPointer<QTranslator> m_TranslatorApp;
private slots:
    void slotActionGroupTranslateTriggered(QAction* pAct);
    
    //Style menu
private:
    QMenu m_MenuStyle;
    QActionGroup m_ActionGroupStyle;
    QMap<QString, QAction*> m_ActionStyles;
    int InitMenuStyles();
    int ClearMenuStyles();
    int OpenCustomStyleMenu();
    int LoadStyle();
private slots:
    void slotActionGroupStyleTriggered(QAction* act);
       
private slots:
    void changeEvent(QEvent *e);
    void slotRead();
    void slotTimeOut();
    void slotReadChannelFinished();
    void slotQTextEditMaxLength();
    void on_pbOpen_clicked();
    void on_pbSend_clicked();
    void on_cmbPort_currentIndexChanged(int index);
    void on_cmbRecent_currentIndexChanged(const QString &szText);
    void on_cbSendLoop_clicked();
    void on_actionClear_triggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionClear_Send_History_triggered();
    void on_actionAbout_A_triggered();
    void on_actionToolBar_T_triggered();
    void on_actionStatusBar_S_triggered();
    void on_actionLeftBar_L_triggered();
    void on_sbLoopTime_valueChanged(int v);
    void on_cbr_clicked(bool checked);
    void on_cbn_clicked(bool checked);
    void on_cbDisplaySend_clicked(bool checked);
    void on_cbDisplayTime_clicked(bool checked);   
    void on_cmbBoudRate_currentTextChanged(const QString &szText);
    void on_cmbDataBit_currentTextChanged(const QString &szText);
    void on_cmbParity_currentIndexChanged(int index);
    void on_cmbStopBit_currentTextChanged(const QString &szText);
    void on_cmbFlowControl_currentIndexChanged(int index);
    void on_rbReceiveASCII_clicked(bool checked);
    void on_rbReceiveHex_clicked(bool checked);
    void on_rbSendHex_clicked(bool checked);
    void on_rbSendASCII_clicked(bool checked);
    void on_cmbRecent_activated(const QString &szText);
    void on_actionPause_P_triggered();
    void on_actionLoad_File_F_triggered();
    void on_pbBrowse_clicked();
    void on_actionOpen_Log_G_triggered();
    void on_actionUpdate_U_triggered();
    
private:
    int InitStatusBar();
    int InitToolBar();
    int InitLeftBar();
    void AddReceive(const QString &szText, bool bReceive = false);
    int SetStatusInfo(QString szText, QColor color = Qt::black);
    QString GetSerialPortSettingInfo();
    bool CheckHexChar(QChar c);
    int SendHexChar(QString szText, int &nLength);
    
private:
    Ui::CMainWindow *ui;

    QSerialPort m_SerialPort;
    ulong m_nSend, m_nReceive, m_nDrop;
    int m_cmbPortIndex;
    QTimer m_Timer;
    int m_nLoopNumber;
    int m_nTransmissions; //已发送次数  
    
    QLabel m_statusInfo;
    QLabel m_statusRx, m_statusTx, m_statusDrop;    
};

#endif // MAINWINDOW_H
