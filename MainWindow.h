/*
 * Author: KangLin(Email:kl222@126.com)
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
    
private:
    int InitStatusBar();
    int InitToolBar();
    int InitLeftBar();
    void AddRecive(QString &szText, bool bRecive = false);
    int SetStatusInfo(QString szText, QColor color = Qt::black);
    
private:
    Ui::CMainWindow *ui;
    QSerialPort m_SerialPort;
    int m_nSend, m_nRecive;
    int m_cmbPortIndex;
    QTimer m_Timer;
    
    QLabel m_statusInfo;
    QLabel m_statusRx, m_statusTx;
    QHBoxLayout m_statusLay;
    QWidget m_Status;
    
};

#endif // MAINWINDOW_H
