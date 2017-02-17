#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Log.h"
#include <QMessageBox>
#include <QTime>
#include "Widgets/DlgAbout/DlgAbout.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow),
    m_SerialPort(this),
    m_Timer(this)
{
    bool check = false;
    ui->setupUi(this);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->cmbPort->addItem(info.description() + "(" + info.portName() + ")");
    }

    foreach(const qint32 &baudRate, QSerialPortInfo::standardBaudRates())
    {
        ui->cmbBoudRate->addItem(QString::number(baudRate));
    }
    
    ui->cmbBoudRate->setCurrentIndex(
                ui->cmbBoudRate->findText(
                    QString::number(m_SerialPort.baudRate())));
    
    ui->cmbParity->addItem(tr("None"));
    ui->cmbParity->addItem(tr("Even"));
    ui->cmbParity->addItem(tr("Odd"));
    ui->cmbParity->addItem(tr("Space"));
    ui->cmbParity->addItem(tr("Mark"));

    ui->cmbDataBit->addItem("8");
    ui->cmbDataBit->addItem("7");
    ui->cmbDataBit->addItem("6");
    ui->cmbDataBit->addItem("5");

    ui->cmbStopBit->addItem("1");
    ui->cmbStopBit->addItem("2");

    ui->cmbFlowControl->addItem(tr("None"));
    ui->cmbFlowControl->addItem(tr("HardWare"));
    ui->cmbFlowControl->addItem(tr("SoftWare"));

    ui->cmbRecent->setDuplicatesEnabled(false);
    
    check = connect(&m_Timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
    Q_ASSERT(check);
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::slotTimeOut()
{
    on_pbSend_clicked();
}

void CMainWindow::AddRecive(QString &szText)
{
    if(ui->cbDisplayTime->isChecked())
        ui->teRecive->insertPlainText(QTime::currentTime().toString());
    ui->teRecive->insertPlainText(szText);
}

void CMainWindow::slotRead()
{
    if(!m_SerialPort.isOpen())
    {
        LOG_MODEL_ERROR("MainWindow", "SerialPort don't open!");
        return;
    }

    AddRecive(QString(m_SerialPort.readAll()));
}

void CMainWindow::on_pbOpen_clicked()
{
    bool bCheck = false;
    int index = 0;

    if(m_SerialPort.isOpen())
    {
        if(m_Timer.isActive())
            m_Timer.stop();
        m_SerialPort.close();
        ui->pbOpen->setText(tr("Open(&O)"));
        ui->actionOpen->setText(tr("Open(&O)"));
        ui->pbSend->setEnabled(false);
        bCheck = m_SerialPort.disconnect();
        Q_ASSERT(bCheck);
        return;
    }

    if(QSerialPortInfo::availablePorts().isEmpty())
        return;

    m_SerialPort.setPort(
                QSerialPortInfo::availablePorts()
                .at(ui->cmbPort->currentIndex()));
    m_SerialPort.setBaudRate(ui->cmbBoudRate->currentText().toInt());
    index = ui->cmbParity->currentIndex();
    index == 0 ? 0 : index = index + 1;
    m_SerialPort.setParity((QSerialPort::Parity)index);
    m_SerialPort.setDataBits(
                (QSerialPort::DataBits)ui->cmbDataBit->currentText().toInt());
    m_SerialPort.setStopBits(
                (QSerialPort::StopBits)ui->cmbStopBit->currentText().toInt());
    m_SerialPort.setFlowControl(
                (QSerialPort::FlowControl)ui->cmbFlowControl->currentIndex());
    bCheck = m_SerialPort.open(QIODevice::ReadWrite);
    if(!bCheck)
    {
        LOG_MODEL_ERROR("MainWindows", "Serial Port open fail: %s[%d]",
                        ui->cmbPort->currentText().toStdString().c_str(),
                        m_SerialPort.error());
        return;
    }

    bCheck = connect(&m_SerialPort, SIGNAL(readyRead()), this, SLOT(slotRead()));
    Q_ASSERT(bCheck);
    ui->pbOpen->setText(tr("Close(&C)"));
    ui->actionOpen->setText(tr("Close(&C)"));
    ui->pbSend->setEnabled(true);

    if(ui->cbSendLoop->isChecked())
        m_Timer.start(ui->sbLoopTime->value());
}

void CMainWindow::on_pbSend_clicked()
{
    if(ui->teSend->toPlainText().isEmpty())
    {
        LOG_MODEL_WARNING("CMainWindow", "Send text is empty");
        return;
    }
 
    m_SerialPort.write(ui->teSend->toPlainText().toStdString().c_str());
    if(ui->cbDisplaySend->isChecked())
        AddRecive(ui->teSend->toPlainText());
    
    if(-1 == ui->cmbRecent->findText(
                ui->teSend->toPlainText().toStdString().c_str()))
        ui->cmbRecent->addItem(ui->teSend->toPlainText().toStdString().c_str());
}

//TODO: 测试当串打开时，选取消改变是否未变  
void CMainWindow::on_cmbPort_currentIndexChanged(int index)
{
    if(m_SerialPort.isOpen())
    {
        if(QMessageBox::Cancel == QMessageBox::warning(this, tr("Warning"),
                             tr("Serial is opened, be sure cloase?"),
                             QMessageBox::Button::Ok,
                             QMessageBox::Button::Cancel))
            return;
        else
            on_pbOpen_clicked();
    }
}

void CMainWindow::on_cmbRecent_currentIndexChanged(const QString &szText)
{
    ui->teSend->setText(szText);
}

void CMainWindow::on_cbSendLoop_clicked()
{
    if(ui->cbSendLoop->isChecked())
    {
        if((!m_Timer.isActive()) && m_SerialPort.isOpen())
            m_Timer.start(ui->sbLoopTime->value());
    }
    else
    {
        if(m_Timer.isActive())
            m_Timer.stop();
    }
}

void CMainWindow::on_actionClear_triggered()
{
    ui->teRecive->clear();
}

void CMainWindow::on_actionOpen_triggered()
{
    on_pbOpen_clicked();
}

void CMainWindow::on_actionExit_triggered()
{
    this->close();
}

void CMainWindow::on_actionClear_Send_History_triggered()
{
    ui->teSend->clear();
}

void CMainWindow::on_actionAbout_A_triggered()
{
    CDlgAbout about(this);
    about.exec();
}
