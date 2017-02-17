#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

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
    
private:
    Ui::CMainWindow *ui;
    QSerialPort m_SerialPort;
    QTimer m_Timer;
};

#endif // MAINWINDOW_H
