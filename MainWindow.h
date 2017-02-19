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

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

private slots:
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
    void AddRecive(QString &szText);
    void on_actionAbout_A_triggered();
    void changeEvent(QEvent *e);
    
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
    
private:
    Ui::CMainWindow *ui;
    QSerialPort m_SerialPort;
    QTimer m_Timer;
};

#endif // MAINWINDOW_H
