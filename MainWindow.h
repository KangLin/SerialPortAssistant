#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

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

    void on_pbOpen_clicked();

    void on_pbSend_clicked();

private:
    Ui::CMainWindow *ui;
    QSerialPort m_SerialPort;
};

#endif // MAINWINDOW_H
