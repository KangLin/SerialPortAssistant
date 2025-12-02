/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    MainWindow.cpp

Abstract:

    This file contains main windows implement.
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Global/Global.h"
#include <QDockWidget>
#include <QMessageBox>
#include <QTime>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QFileDevice>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QLoggingCategory>
#include <QFileDialog>
#include <QFontDatabase>

#ifdef RABBITCOMMON
    #include "DlgAbout.h"
    #include "FrmUpdater.h"
    #include "RabbitCommonTools.h"
#endif

static Q_LOGGING_CATEGORY(log, "MainWindow")

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow),
    m_SerialPort(this),
    m_nSend(0),    
    m_nReceive(0),
    m_nDrop(0),
    m_cmbPortIndex(-1),
    m_Timer(this),    
    m_nTransmissions(0)
{
    bool check = false;
    /*QDir d;
    if(!d.exists(QStandardPaths::writableLocation(
                      QStandardPaths::DataLocation)))
        d.mkdir(QStandardPaths::writableLocation(
                  QStandardPaths::DataLocation));*/

    ui->setupUi(this);

#ifdef RABBITCOMMON 
    CFrmUpdater updater;
    ui->actionUpdate_U->setIcon(updater.windowIcon());
#endif

    RabbitCommon::CTools::InsertStyleMenu(ui->menuTools_T,
                                          ui->actionOpen_save_file);
    ui->menuTools_T->insertMenu(ui->actionOpen_save_file,
                             RabbitCommon::CTools::GetLogMenu(this));
    ui->menuTools_T->addSeparator();

    InitStatusBar();
    InitToolBar();
    InitLeftBar();

    foreach(const qint32 &baudRate, QSerialPortInfo::standardBaudRates())
    {
        ui->cmbBoudRate->addItem(QString::number(baudRate));
    }

    ui->cmbDataBit->addItem("8", QSerialPort::DataBits::Data8);
    ui->cmbDataBit->addItem("7", QSerialPort::DataBits::Data7);
    ui->cmbDataBit->addItem("6", QSerialPort::DataBits::Data6);
    ui->cmbDataBit->addItem("5", QSerialPort::DataBits::Data5);

    ui->cmbParity->addItem(tr("None"), QSerialPort::Parity::NoParity);
    ui->cmbParity->addItem(tr("Even"), QSerialPort::Parity::EvenParity);
    ui->cmbParity->addItem(tr("Odd"), QSerialPort::Parity::OddParity);
    ui->cmbParity->addItem(tr("Space"), QSerialPort::Parity::SpaceParity);
    ui->cmbParity->addItem(tr("Mark"), QSerialPort::Parity::MarkParity);

    ui->cmbStopBit->addItem("1", QSerialPort::StopBits::OneStop);
    ui->cmbStopBit->addItem("1.5", QSerialPort::StopBits::OneAndHalfStop);
    ui->cmbStopBit->addItem("2", QSerialPort::StopBits::TwoStop);

    ui->cmbFlowControl->addItem(tr("None"), QSerialPort::FlowControl::NoFlowControl);
    ui->cmbFlowControl->addItem(tr("HardWare"), QSerialPort::FlowControl::HardwareControl);
    ui->cmbFlowControl->addItem(tr("SoftWare"), QSerialPort::FlowControl::SoftwareControl);
    
    RefreshSerialPorts();
    SetSaveFileName();

    ui->cmbRecent->setDuplicatesEnabled(false);

    ui->gpSendLoop->setChecked(CGlobal::Instance()->GetSendLoop());
    m_nLoopNumber = ui->sbLoopNumber->value();
    ui->sbLoopTime->setValue(CGlobal::Instance()->GetSendLoopTime());
    check = connect(&m_Timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
    Q_ASSERT(check);
    
    CGlobal::SEND_R_N v = CGlobal::Instance()->GetSendRN();
    ui->cbr->setChecked(v & CGlobal::SEND_R_N::R);
    ui->cbn->setChecked(v & CGlobal::SEND_R_N::N);
    
    ui->cbDisplaySend->setChecked(CGlobal::Instance()->GetReceiveDisplaySend());
    ui->cbDisplayTime->setChecked(CGlobal::Instance()->GetReceiveDisplayTime());
    ui->cbSaveToFile->setChecked(CGlobal::Instance()->GetSaveFile());
    
    check = connect(&m_SerialPort, SIGNAL(dataTerminalReadyChanged(bool)),
                    this, SLOT(slotDataTerminalReadyChanged(bool)));
    Q_ASSERT(check);
    
    check = connect(&m_SerialPort, SIGNAL(requestToSendChanged(bool)),
                    this, SLOT(slotRequestToSendChanged(bool)));
    Q_ASSERT(check);

    InitPinout();

    // Add fixed font. See: https://github.com/KangLin/SerialPortAssistant/issues/26
    QFont receiverFont = ui->teReceive->font();
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    //qDebug(log) << "fixedFont:" << fixedFont;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if(!QFontDatabase::isFixedPitch(receiverFont.family(), receiverFont.styleName())
        && QFontDatabase::isFixedPitch(fixedFont.family(), fixedFont.styleName()))
#else
    QFontDatabase fd;
    if(!fd.isFixedPitch(receiverFont.family(), receiverFont.styleName())
        && fd.isFixedPitch(fixedFont.family(), fixedFont.styleName()))
#endif
    {
        ui->teReceive->setFont(fixedFont);
        ui->teSend->setFont(fixedFont);
    }

#ifdef HAVE_RABBITCOMMON_GUI
    RabbitCommon::CTools::RestoreWidget(this);
#endif
}

CMainWindow::~CMainWindow()
{
    if(m_SerialPort.isOpen())
        on_pbOpen_clicked();

    delete ui;
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
#ifdef HAVE_RABBITCOMMON_GUI
    RabbitCommon::CTools::SaveWidget(this);
#endif
    QMainWindow::closeEvent(event);
}

int CMainWindow::RefreshSerialPorts()
{
    ui->cmbPort->clear();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        /*
        qDebug(log)
                 << "Port:" << info.portName() << " "
                 << "Location:" << info.systemLocation() << " "
                 << "Description:" << info.description() << " "
                 << "Manufacturer:" << info.manufacturer() << " "
                 << "Serial number:" << info.serialNumber() << " "
                 << "Vendor Identifier:"
                 << (info.hasVendorIdentifier()
                         ? QByteArray::number(info.vendorIdentifier(), 16)
                         : QByteArray()) << " "
                 << "Product Identifier:"
                 << (info.hasProductIdentifier()
                         ? QByteArray::number(info.productIdentifier(), 16)
                         : QByteArray()); //*/
        QString szPort;
        szPort = info.portName();
        //szPort = info.systemLocation();
        if(!info.description().isEmpty())
        {
            szPort += "(" + info.description() + ")";
        }
        ui->cmbPort->addItem(szPort);
    }

    CGlobal::Para para;
    bool bRet = CGlobal::Instance()->LoadSerialPort(para);

    int nIndex = -1;
    if(!bRet) {
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            nIndex++;
            if(info.portName() != para.name)
                continue;
            
            ui->cmbPort->setCurrentIndex(nIndex);
            return InitSerialPortPara(para);
        }
    }

    nIndex = ui->cmbPort->currentIndex();
    if(-1 == nIndex)
        return -1;
    bRet = CGlobal::Instance()->LoadSerialPort(para, nIndex);

    return InitSerialPortPara(para);
}

int CMainWindow::InitSerialPortPara(CGlobal::Para &para)
{
    int idx = ui->cmbBoudRate->findText(QString::number(para.baudRate));
    if(-1 == idx) {
        ui->cmbBoudRate->addItem(QString::number(para.baudRate));
        ui->cmbBoudRate->setCurrentText(QString::number(para.baudRate));
    } else
        ui->cmbBoudRate->setCurrentIndex(idx);
    
    idx = ui->cmbDataBit->findData(para.dataBit);
    if(-1 < idx)
        ui->cmbDataBit->setCurrentIndex(idx);
    
    idx = ui->cmbParity->findData(para.parity);
    if(-1 < idx)
        ui->cmbParity->setCurrentIndex(idx);
    
    idx = ui->cmbStopBit->findData(para.stopBits);
    if(-1 < idx)
        ui->cmbStopBit->setCurrentIndex(idx);
    
    idx = ui->cmbFlowControl->findData(para.flowControl);
    if(-1 < idx)
        ui->cmbFlowControl->setCurrentIndex(idx);
    
    return 0;
}

int CMainWindow::InitStatusBar()
{
    ui->actionStatusBar_S->setChecked(
                CGlobal::Instance()->GetStatusbarVisible());
    statusBar()->setVisible(CGlobal::Instance()->GetStatusbarVisible());

    SetStatusInfo(tr("Ready"));
    m_statusRx.setText(tr("Rx: 0 Bytes"));
    m_statusTx.setText(tr("Tx: 0 Bytes"));
    m_statusDrop.setText(tr("Drop: 0 Bytes"));
    m_statusInfo.setSizePolicy(QSizePolicy::Policy::Expanding,
                               QSizePolicy::Policy::Preferred);
    m_statusTx.setSizePolicy(QSizePolicy::Policy::Preferred,
                             QSizePolicy::Policy::Preferred);
    m_statusRx.setSizePolicy(QSizePolicy::Policy::Preferred,
                             QSizePolicy::Policy::Preferred);
    m_statusDrop.setSizePolicy(QSizePolicy::Policy::Preferred,
                             QSizePolicy::Policy::Preferred);
    
    this->statusBar()->addWidget(&m_statusInfo);
    this->statusBar()->addWidget(&m_statusRx);
    this->statusBar()->addWidget(&m_statusTx);
    this->statusBar()->addWidget(&m_statusDrop);
    
    return 0;
}

int CMainWindow::InitToolBar()
{
    ui->actionToolBar_T->setChecked(
                CGlobal::Instance()->GetToolbarVisible());
    ui->mainToolBar->setVisible(CGlobal::Instance()->GetToolbarVisible());
    return 0;
}

int CMainWindow::InitLeftBar()
{
    m_bInitEncodeCombox = true;
    InitEncodeComboBox(ui->cbReceiveEncoded);
    InitEncodeComboBox(ui->cbSendEncode);
    m_bInitEncodeCombox = false;
    CGlobal::ENCODE c = CGlobal::Instance()->GetReceiveEncode();
    ui->cbReceiveEncoded->setCurrentIndex(ui->cbReceiveEncoded->findData(c));
    c = CGlobal::Instance()->GetSendEncode();
    ui->cbSendEncode->setCurrentIndex(ui->cbSendEncode->findData(c));
    /* TODO: add dock widget
    m_dockLeft = new QDockWidget(this);
    if(m_dockLeft)
    {
        m_dockLeft->setWindowTitle(tr("Setting"));
        m_dockLeft->setWidget(ui->frmLeftBar);
        // Must set ObjectName then restore it. See: saveState help document
        m_dockLeft->setObjectName("dockSetting");
        //m_dockLeft->hide();
        ui->menuView_V->addAction(m_dockLeft->toggleViewAction());
        addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_dockLeft);
    }//*/

    return 0;
}

int CMainWindow::InitEncodeComboBox(QComboBox* comboBox)
{
    comboBox->addItem("ASCII", CGlobal::ASCII);
    comboBox->addItem("HEX", CGlobal::HEX);
    if(comboBox == ui->cbReceiveEncoded)
    {
        comboBox->addItem("HEX ASCII", CGlobal::HEX_ASCII);
    }
    comboBox->addItem("UTF8", CGlobal::UTF8);
    return 0;
}

void CMainWindow::on_cbReceiveEncoded_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    if(m_bInitEncodeCombox) return;
    CGlobal::Instance()->SetReceiveEncode(
        static_cast<CGlobal::ENCODE>(ui->cbReceiveEncoded->currentData().toInt()));
}

void CMainWindow::on_cbSendEncode_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    if(m_bInitEncodeCombox) return;
    CGlobal::Instance()->SetSendEncode(
        static_cast<CGlobal::ENCODE>(ui->cbSendEncode->currentData().toInt()));
}

void CMainWindow::slotTimeOut()
{
    on_pbSend_clicked();
    if(m_nLoopNumber > 0)
        m_nLoopNumber--;
    if(0 == m_nLoopNumber)
    {
        ui->gpSendLoop->setChecked(false);
        m_Timer.stop();
    }
}

void CMainWindow::slotReadChannelFinished()
{
    bool bCheck = false;
    //if(m_SerialPort.isOpen())
    {
        if(m_Timer.isActive())
            m_Timer.stop();
        ui->pbOpen->setText(tr("Open(&O)"));
        ui->pbOpen->setIcon(QIcon(":/icon/Start"));
        ui->actionOpen->setText(tr("Open(&O)"));
        ui->actionOpen->setIcon(QIcon(":/icon/Start"));
        ui->pbSend->setEnabled(false);
        bCheck = m_SerialPort.disconnect();
        Q_ASSERT(bCheck);

        SetStatusInfo(tr("Serial Port Close"));
        return;
    }
}

void CMainWindow::on_pbOpen_clicked()
{
    bool bCheck = false;

    if(m_SerialPort.isOpen())
    {
        if("SendFile" == ui->tbSendSettings->currentWidget()->objectName())
            CloseSendFile();

        bCheck = m_SerialPort.disconnect();
        Q_ASSERT(bCheck);

        m_SerialPort.close();

        if(m_Timer.isActive())
            m_Timer.stop();
        ui->pbOpen->setText(tr("Open(&O)"));
        ui->pbOpen->setIcon(QIcon(":/icon/Start"));
        ui->actionOpen->setText(tr("Open(&O)"));
        ui->actionOpen->setIcon(QIcon(":/icon/Start"));
        ui->pbSend->setEnabled(false);

        SetStatusInfo(tr("Serial Port Close"));
        ui->actionRefresh_R->setVisible(true);
        
        InitPinout();
        return;
    }
    
    QString szPortName = ui->cmbPort->currentText();
    if(QSerialPortInfo::availablePorts().isEmpty() && szPortName.isEmpty())
        return;
    
    m_cmbPortIndex = ui->cmbPort->currentIndex();
    if(-1 == m_cmbPortIndex) {
        if(szPortName.isEmpty()) {
            qCritical(log) << "The port index is invalid: " << m_cmbPortIndex;
            return;
        }
    }
    
    QSerialPortInfo info = QSerialPortInfo::availablePorts()
            .at(m_cmbPortIndex);
    QString infoName = info.portName();
    if(!info.description().isEmpty())
    {
        infoName += "(" + info.description() + ")";
    }
    if(infoName != szPortName) {
        m_SerialPort.setPortName(szPortName);
    } else {
        //#if defined(Q_OS_WIN32)
        //    m_SerialPort.setPortName("\\\\.\\" + info.portName());
        //#else
        m_SerialPort.setPort(info);
        //#endif
    }
    m_SerialPort.setBaudRate(ui->cmbBoudRate->currentText().toInt());
    QSerialPort::Parity parity =
        (QSerialPort::Parity)ui->cmbParity->currentData().toInt();
    m_SerialPort.setParity(parity);
    m_SerialPort.setDataBits(
        (QSerialPort::DataBits)ui->cmbDataBit->currentData().toInt());
    m_SerialPort.setStopBits(
        (QSerialPort::StopBits)ui->cmbStopBit->currentData().toInt());
    m_SerialPort.setFlowControl(
        (QSerialPort::FlowControl)ui->cmbFlowControl->currentData().toInt());
    bCheck = m_SerialPort.open(QIODevice::ReadWrite);
    if(!bCheck)
    {
        QString szError;
        szError = tr("Open Serial port %1[%2] fail errNo[%3]: %4").
                  arg(ui->cmbPort->currentText(),
                      m_SerialPort.portName(),
                      QString::number(m_SerialPort.error()), m_SerialPort.errorString());
        qCritical(log) << szError;
        SetStatusInfo(szError, Qt::red);
        return;
    }

    CGlobal::Instance()->SaveSerialPort(m_SerialPort, m_cmbPortIndex);

    bCheck = connect(&m_SerialPort, SIGNAL(readyRead()), this, SLOT(slotRead()));
    Q_ASSERT(bCheck);
    /*bCheck = connect(&m_SerialPort, SIGNAL(readChannelFinished()),
                     this, SLOT(slotReadChannelFinished()));
    Q_ASSERT(bCheck);*/
    ui->pbOpen->setText(tr("Close(&C)"));
    ui->pbOpen->setIcon(QIcon(":/icon/Stop"));
    ui->actionOpen->setText(tr("Close(&C)"));
    ui->actionOpen->setIcon(QIcon(":/icon/Stop"));
    ui->pbSend->setEnabled(true);

    SetStatusInfo(GetSerialPortSettingInfo(), Qt::green);
    m_nSend = 0;
    m_nReceive = 0;
    m_nDrop = 0;
    m_nTransmissions = 0;
    m_statusRx.setText(tr("Rx: 0 Bytes"));
    m_statusTx.setText(tr("Tx: 0 Bytes"));
    m_statusDrop.setText(tr("Drop: 0 Bytes"));
    ui->lbTransmissions->setText(QString::number(m_nTransmissions));
    if(ui->gpSendLoop->isChecked())
    {
        m_nLoopNumber = ui->sbLoopNumber->value();
        m_Timer.start(ui->sbLoopTime->value());
    }

    ui->actionRefresh_R->setVisible(false);

    setPinoutStatus();
}

int CMainWindow::SetStatusInfo(QString szText, QColor color)
{
    QPalette pe;
    pe.setColor(QPalette::WindowText, color);
    m_statusInfo.setPalette(pe);
    m_statusInfo.setText(szText);
    return 0;
}

QString CMainWindow::GetSerialPortSettingInfo()
{
    QString szInfo = ui->cmbPort->currentText();
    if(m_SerialPort.isOpen())
        szInfo = szInfo + tr(" is opened. ");
    else
        szInfo = szInfo + tr(" is closed. ");
    szInfo = szInfo + ui->cmbBoudRate->currentText() + "|"
            + ui->cmbDataBit->currentText() + "|"
            + ui->cmbParity->currentText() + "|"
            + ui->cmbStopBit->currentText() + "|"
            + ui->cmbFlowControl->currentText();
    return szInfo;
}

//限制QTextEdit内容的长度  
void CMainWindow::slotQTextEditMaxLength()
{
    int maxLength = 102400;
    int length = ui->teReceive->toPlainText().length();
    if(length > maxLength << 1)
    {
        QTextCursor cursor = ui->teReceive->textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.setPosition(length - maxLength, QTextCursor::KeepAnchor);
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor.movePosition(QTextCursor::End);  //把光标移动到文档最后  
        //ui->teReceive->setTextCursor(cursor);
        qDebug(log) << "slotQTextEditMaxLength";
    }
}

void CMainWindow::AddReceive(const QString &szText, bool bReceive)
{
    QString szPrex;

    if(ui->cbDisplayTime->isChecked())
        szPrex = QTime::currentTime().toString("hh:mm:ss.zzz") + " ";

    if(ui->cbDisplaySend->isChecked())
    {
        if(bReceive)
            szPrex += "<- ";
        else
            szPrex += "-> ";
    }

    if(!szPrex.isEmpty())
    {
        ui->teReceive->insertPlainText("\n");
        ui->teReceive->insertPlainText(szPrex);
    }

    ui->teReceive->insertPlainText(szText);

    slotQTextEditMaxLength();
    if(!ui->actionPause_P->isChecked())
    {
        QTextCursor cursor = ui->teReceive->textCursor();
        cursor.movePosition(QTextCursor::End);  //把光标移动到文档最后  
        ui->teReceive->setTextCursor(cursor);
    }
}

void CMainWindow::slotRead()
{
    if(!m_SerialPort.isOpen())
    {
        qCritical(log) << "SerialPort don't open!";
        return;
    }

    QByteArray d = m_SerialPort.readAll();
    if(d.isEmpty())
    {
        qCritical(log) << "read data fail";
        return;
    }
    m_nReceive += d.length();
    qDebug() << "slotRead: length:" << d.size() <<  d;
    
    if(ui->cbSaveToFile->isChecked())
    {
        QFile f(ui->leSaveToFile->text());
        if(f.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            f.write(d);
            f.close();
        }
    }
    
    QString szText;
    CGlobal::ENCODE c = static_cast<CGlobal::ENCODE>(
                ui->cbReceiveEncoded->currentData().toInt());
    switch (c) {
    case CGlobal::ASCII:
#if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 3))
        szText = QString::fromStdString(d.toStdString());
#else
        szText = d;
#endif
        break;
    case CGlobal::UTF8:
        szText = QString::fromUtf8(d.constData(), d.size());
        break;
    case CGlobal::HEX:
        {
            QString szOut;
            int nLen = d.size();
            for(int i = 0; i < nLen; i++)
            {
                if(i)
                    szOut += " ";
                char buff[16] = {0};
                unsigned char c = d.at(i);
                sprintf(buff, "0x%X", c);
                szOut += buff;
            }
            szText = szOut;
            break;
        }
    case CGlobal::HEX_ASCII:
        {
            int nLen = d.size();
            while(nLen)
            {
                QString szHex, szASCII;
                int i = 0;
                for(; i < 8; i++)
                {
                    if(nLen <= 0) break;
                    char buff[16] = {0};
                    unsigned char c = d.at(d.size() - nLen);
                    if(QChar(c).isPrint())
                        szASCII = szASCII + " " + QChar(c);
                    else
                        szASCII = szASCII + " .";
                    sprintf(buff, "0x%X ", c);
                    szHex += buff;
                    nLen--;
                }
                if(i < 8)
                {
                    szHex += QString((8 - i) * 5, ' ');
                }
                szText = szText + szHex + szASCII + "\n";
            }
            szText = "\n" + szText;
            break;
        }
    }

    //显示接收
    AddReceive(szText, true);

    m_statusRx.setText(tr("Rx: ") + QString::number(m_nReceive) + tr(" Bytes"));
}

bool CMainWindow::CheckHexChar(QChar c)
{
    if(!((c >= 'a' && c <= 'f')
         || (c >= 'A' && c <= 'F')
         || (c >= '0' && c <= '9')
         ))
    {
        QString szErr;
        szErr = "Invalid symbol: ";
        szErr += c;
        qCritical(log) << szErr;
        //m_statusInfo.setText(szErr);
        SetStatusInfo(szErr, Qt::red);
        return false;
    }
    return true;
}

// Must is format: 0xXX, etc: 0x31 0xAF 0x5D
int CMainWindow::SendHexChar(QString szText, int &nLength)
{
    QByteArray out;
    QString szInput;
    
    int nLen = szText.length();
    unsigned char outBin = 0;
    bool isFirst = true;
    for (int i = 0; i < nLen; i++)
    {
        QChar c = szText.at(i);
        if(QChar(0x20) == c || '\r' == c || '\n' == c) //space \r \n
            continue;
        if(i + 1 < nLen)
        {
            if(QChar('0') == c && QChar('x') == szText.at(i + 1).toLower()) // 0x
            {
                if(!isFirst) out.append(outBin); //只有一位
                i++;
                isFirst = true;
                continue;
            }
        }
        
        bool check = CheckHexChar(c);
        if(!check)
            return -1;
        
        if(c >= 'a' && c <= 'f')
            c = c.toUpper();
        
        if(isFirst)
        {
            //处理第前4位
            if(c >= 'A' && c <= 'F')
                outBin = c.toLatin1() - 'A' + 10;
            else
                outBin = c.toLatin1() & ~0x30;
            isFirst = false;
            if(i + 1 >= nLen)
                out.append(outBin);
        } else {
            //处理后4位, 并组合起来
            outBin <<= 4;
            if(c >= 'A' && c <= 'F')
                outBin |= (c.toLatin1() - 'A' + 10);
            else
                outBin |= (c.toLatin1() & ~0x30);
            out.append(outBin);
            isFirst = true;
        }
    }
    qDebug() << "CMainWindow::SetHexChar: length: " << out.size() << out;
    nLength = out.size();
    return m_SerialPort.write(out, out.size());
}

void CMainWindow::on_pbSend_clicked()
{
    if("Input" == ui->tbSendSettings->currentWidget()->objectName())
        SendInput();
    if("SendFile" == ui->tbSendSettings->currentWidget()->objectName())
        SendFile();
}

int CMainWindow::SendInput()
{
    int nRet = 0;
    if(ui->teSend->toPlainText().isEmpty())
    {
        qWarning(log) << "Send text is empty";
        return -1;
    }
    QString szText = ui->teSend->toPlainText();
    if(ui->cbr->isChecked())
        szText += "\r";
    if(ui->cbn->isChecked())
        szText += "\n";

    int nSendLength = szText.size();
    CGlobal::ENCODE c = static_cast<CGlobal::ENCODE>(
                ui->cbSendEncode->currentData().toInt());
    switch(c) {
    case CGlobal::ASCII:
        nSendLength = szText.toStdString().size();
        nRet = m_SerialPort.write(szText.toStdString().c_str(), nSendLength);
        break;
    case CGlobal::UTF8:
        nSendLength = szText.toUtf8().size();
        nRet = m_SerialPort.write(szText.toUtf8(), nSendLength);
        break;
    case CGlobal::HEX:
        nRet = SendHexChar(szText, nSendLength);
        break;
    default:
        break;
    }
    if(0 > nRet)
    {
        //m_statusInfo.setText(tr("Send fail"));
        SetStatusInfo(tr("Send fail"), Qt::red);
        qCritical(log) << "Write fail: nRet:" << nRet << "; Serial error["
                 << m_SerialPort.error() << "]：" << m_SerialPort.errorString();
                        
        on_pbOpen_clicked(); //close serial port  
        return nRet;
    }
    qInfo(log) << "Send" << nRet << "bytes";

    m_nTransmissions++;
    ui->lbTransmissions->setText(QString::number(m_nTransmissions));
    m_nSend += nRet;
    m_statusTx.setText(tr("Tx: ") + QString::number(m_nSend) + tr(" Bytes"));
    if(nSendLength != nRet)
    {
        m_nDrop += (nSendLength - nRet);
        m_statusDrop.setText(tr("Drop: ") + QString::number(m_nDrop) + tr(" Bytes"));
    }

    //display send
    if(ui->cbDisplaySend->isChecked())
        AddReceive(szText, false);

    //Add to recently sent list
    if(-1 == ui->cmbRecent->findText(
                ui->teSend->toPlainText().toStdString().c_str()))
        ui->cmbRecent->addItem(ui->teSend->toPlainText().toStdString().c_str());

    return nRet;
}

int CMainWindow::SendFile()
{
    int nRet = 0;
    
    if(m_SendFile.isOpen())
    {
        qWarning(log) << "There is send file";
        return 0;
    }
    
    nRet = m_SendFile.Open(ui->leSendFile->text());
    if(nRet < 0)
        return nRet;
    
    ui->pbSend->setEnabled(false);

    bool check = connect(&m_SerialPort, SIGNAL(bytesWritten(qint64)),
                         this, SLOT(slotSendFile(qint64)));
    Q_ASSERT(check);

    slotSendFile(0);
    return nRet;
}

int CMainWindow::CloseSendFile()
{
    bool check = disconnect(&m_SerialPort, SIGNAL(bytesWritten(qint64)),
                            this, SLOT(slotSendFile(qint64)));
    Q_ASSERT(check);
    m_SendFile.Close();
    ui->pbSend->setEnabled(true);
    return 0;
}

void CMainWindow::slotSendFile(qint64 bytes)
{
    Q_UNUSED(bytes);
    //qDebug() << "CMainWindow::slotSendFile" << bytes;
    qint64 nRet = 0;
    nRet = m_SendFile.Write(&m_SerialPort);
    if(nRet <= 0)
    {
        CloseSendFile();
        return;
    }

    m_nSend += nRet;
    m_statusTx.setText(tr("Tx: ") + QString::number(m_nSend) + tr(" Bytes"));
}

void CMainWindow::on_cmbPort_currentIndexChanged(int index)
{
    if(m_SerialPort.isOpen() && index != m_cmbPortIndex)
    {
        QString szPort;
        szPort = QSerialPortInfo::availablePorts().at(m_cmbPortIndex).portName();
        if(!QSerialPortInfo::availablePorts().at(m_cmbPortIndex)
                .description().isEmpty())
        {
            szPort += "("
                   + QSerialPortInfo::availablePorts()
                    .at(m_cmbPortIndex).description() + ")";
        }
        if(QMessageBox::Cancel == QMessageBox::warning(this, tr("Warning"),
           tr("Serial [%1] is opened, be sure cloase?").arg(szPort),
                             QMessageBox::Button::Ok,
                             QMessageBox::Button::Cancel))
        {
            ui->cmbPort->setCurrentIndex(m_cmbPortIndex);
            return;
        }

        on_pbOpen_clicked();   
    }

    m_cmbPortIndex = index;
    CGlobal::Para para;
    bool bRet = CGlobal::Instance()->LoadSerialPort(para, m_cmbPortIndex);
    if(!bRet)
        InitSerialPortPara(para);

    SetSaveFileName();
}

int CMainWindow::SetSaveFileName()
{
    int nRet = 0;
    int nIndex = ui->cmbPort->currentIndex();
    if(nIndex < 0 || nIndex >= QSerialPortInfo::availablePorts().count())
    {
        qCritical(log) << "The index out of range. nIndex:" << nIndex
                          << "; Serial port count:"
                          << QSerialPortInfo::availablePorts().count();
        return -1;
    }
    QString szFile = QStandardPaths::writableLocation(
                QStandardPaths::TempLocation)
            + QDir::separator() + "SerialAssistantReceive.txt";
    if(!QSerialPortInfo::availablePorts().isEmpty())
        szFile = QStandardPaths::writableLocation(
                QStandardPaths::TempLocation)
            + QDir::separator() + "SerialAssistantReceive_"
            + QSerialPortInfo::availablePorts()
            .at(ui->cmbPort->currentIndex()).portName()
            + ".txt";

    while(QFile::exists(szFile))
    {
        int nPos = szFile.lastIndexOf("_");
        if(-1 != nPos)
        {
            QString szNum = szFile.mid(nPos + 1);
            int nPosNum = szNum.indexOf(".");
            szNum = szNum.left(nPosNum);
            szNum = QString::number(szNum.toInt() + 1);
            szFile = szFile.left(nPos) + "_" + szNum + ".txt";
        } else {
            nPos = szFile.lastIndexOf(".");
            szFile = szFile.left(nPos) + "_1.txt";
        }
    }
    ui->leSaveToFile->setText(szFile);
    return nRet;
}

void CMainWindow::on_cmbRecent_currentIndexChanged(const QString &szText)
{
    ui->teSend->setText(szText);
}

void CMainWindow::on_cmbRecent_activated(const QString &szText)
{
    ui->teSend->setText(szText);
}

void CMainWindow::on_gpSendLoop_clicked()
{
    if(ui->gpSendLoop->isChecked())
    {
        if((!m_Timer.isActive()) && m_SerialPort.isOpen())
        {
            m_nLoopNumber = ui->sbLoopNumber->value();
            m_Timer.start(ui->sbLoopTime->value());
        }
    }
    else
    {
        if(m_Timer.isActive())
            m_Timer.stop();
    }
    CGlobal::Instance()->SetSendLoop(ui->gpSendLoop->isChecked());
}

void CMainWindow::on_actionClear_triggered()
{
    ui->teReceive->clear();
}

void CMainWindow::on_actionOpen_triggered()
{
    on_pbOpen_clicked();
}

void CMainWindow::on_actionExit_triggered()
{
    close();
}

void CMainWindow::on_actionClear_Send_History_triggered()
{
    ui->teSend->clear();
    ui->cmbRecent->clear();
}

void CMainWindow::on_actionAbout_A_triggered()
{
#ifdef RABBITCOMMON
    CDlgAbout* about = new CDlgAbout(this);
    about->m_AppIcon = QImage(":/icon/AppIcon");
    about->m_szHomePage = "https://github.com/KangLin/SerialPortAssistant";
    about->m_szCopyrightStartTime = "2017";
    #ifdef SerialPortAssistant_Revision
        about->m_szVersionRevision = SerialPortAssistant_Revision;
    #endif
    RC_SHOW_WINDOW(about);
#endif
}

void CMainWindow::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CMainWindow::on_actionToolBar_T_triggered()
{
    ui->mainToolBar->setVisible(ui->actionToolBar_T->isChecked());
    CGlobal::Instance()->SetToolbarVisible(ui->actionToolBar_T->isChecked());
}

void CMainWindow::on_actionStatusBar_S_triggered()
{
    this->statusBar()->setVisible(ui->actionStatusBar_S->isChecked());
    CGlobal::Instance()->SetStatusbarVisible(ui->actionStatusBar_S->isChecked());
}

void CMainWindow::on_sbLoopTime_valueChanged(int v)
{
    CGlobal::Instance()->SetSendLoopTime(v);
}

void CMainWindow::on_cbr_clicked(bool checked)
{
    CGlobal::SEND_R_N v = CGlobal::Instance()->GetSendRN();
    CGlobal::Instance()->SetSendRN((CGlobal::SEND_R_N)(
                checked ? v | CGlobal::R
                        : v & ~CGlobal::R));
}

void CMainWindow::on_cbn_clicked(bool checked)
{
    CGlobal::SEND_R_N v = CGlobal::Instance()->GetSendRN();
    CGlobal::Instance()->SetSendRN((CGlobal::SEND_R_N)(
                checked ? v | CGlobal::N
                        : v & ~CGlobal::N));
}

void CMainWindow::on_cbDisplaySend_clicked(bool checked)
{
    CGlobal::Instance()->SetReceiveDisplaySend(checked);
}

void CMainWindow::on_cbDisplayTime_clicked(bool checked)
{
    CGlobal::Instance()->SetReceiveDisplayTime(checked);
}

void CMainWindow::on_cmbBoudRate_currentTextChanged(const QString &szText)
{
    if(!m_SerialPort.isOpen())
        return;
    
    bool bRet;
    bRet = m_SerialPort.setBaudRate(szText.toInt());
    if(bRet) {
        CGlobal::Instance()->SaveSerialPort(m_SerialPort, m_cmbPortIndex);
        SetStatusInfo(GetSerialPortSettingInfo(),
                  m_SerialPort.isOpen() ? Qt::green : Qt::yellow);
    }
    else
        SetStatusInfo(tr("Set baud rate fail. error: ")
                          + QString::number(m_SerialPort.error()), Qt::red);
}

void CMainWindow::on_cmbDataBit_currentIndexChanged(int index)
{
    if(-1 == index)
        return;
    if(!m_SerialPort.isOpen())
        return;
    bool bRet = false;
    bRet = m_SerialPort.setDataBits(
        (QSerialPort::DataBits)ui->cmbDataBit->itemData(index).toInt());
    if(bRet) {
        CGlobal::Instance()->SaveSerialPort(m_SerialPort, m_cmbPortIndex);
        SetStatusInfo(GetSerialPortSettingInfo(),
                      m_SerialPort.isOpen() ? Qt::green : Qt::yellow);
    }
    else
        SetStatusInfo(tr("Set data bits fail. error: ")
                          + QString::number(m_SerialPort.error()), Qt::red);
}

void CMainWindow::on_cmbParity_currentIndexChanged(int index)
{
    if(-1 == index)
        return;
    if(!m_SerialPort.isOpen())
        return;
    bool bRet = false;
    bRet = m_SerialPort.setParity(
        (QSerialPort::Parity)ui->cmbParity->itemData(index).toInt());
    if(bRet) {
        CGlobal::Instance()->SaveSerialPort(m_SerialPort, m_cmbPortIndex);
        SetStatusInfo(GetSerialPortSettingInfo(),
                  m_SerialPort.isOpen() ? Qt::green : Qt::yellow);
    }
    else
        SetStatusInfo(tr("Set parity fail. error: ")
                          + QString::number(m_SerialPort.error()), Qt::red);
}

void CMainWindow::on_cmbStopBit_currentIndexChanged(int index)
{
    if(-1 == index)
        return;
    if(!m_SerialPort.isOpen())
        return;
    bool bRet = false;
    bRet = m_SerialPort.setStopBits(
        (QSerialPort::StopBits)ui->cmbStopBit->itemData(index).toInt());
    if(bRet) {
        CGlobal::Instance()->SaveSerialPort(m_SerialPort, m_cmbPortIndex);
        SetStatusInfo(GetSerialPortSettingInfo(),
                  m_SerialPort.isOpen() ? Qt::green : Qt::yellow);
    }
    else
        SetStatusInfo(tr("Set stop bits fail. error: ")
                          + QString::number(m_SerialPort.error()), Qt::red);
}

void CMainWindow::on_cmbFlowControl_currentIndexChanged(int index)
{
    if(-1 == index)
        return;
    if(!m_SerialPort.isOpen())
        return;
    bool bRet = false;
    bRet = m_SerialPort.setFlowControl(
        (QSerialPort::FlowControl)ui->cmbFlowControl->itemData(index).toInt());
    if(bRet) {
        CGlobal::Instance()->SaveSerialPort(m_SerialPort, m_cmbPortIndex);
        SetStatusInfo(GetSerialPortSettingInfo(),
                  m_SerialPort.isOpen() ? Qt::green : Qt::yellow);
    }
    else
        SetStatusInfo(tr("Set Flow Control fail. error: ")
                          + QString::number(m_SerialPort.error()), Qt::red);
}

void CMainWindow::on_actionLoad_File_F_triggered()
{
    on_pbBrowseSend_clicked();
    return;
    
    QString szFile = QFileDialog::getOpenFileName(this, tr("Load File"));
    if(szFile.isEmpty())
        return;
    
    QFile f(szFile);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    
    QByteArray r = f.readAll();
    ui->teSend->setText(QString(r));
    
    f.close();
}

void CMainWindow::on_actionOpen_send_file_triggered()
{
    if(ui->leSendFile->text().isEmpty())
        return;
    QDesktopServices::openUrl(QUrl::fromLocalFile(ui->leSendFile->text()));
}

void CMainWindow::on_actionOpen_save_file_triggered()
{
    if(ui->leSaveToFile->text().isEmpty())
        return;
    QDesktopServices::openUrl(QUrl::fromLocalFile(ui->leSaveToFile->text()));
}

void CMainWindow::on_actionUpdate_U_triggered()
{
#ifdef RABBITCOMMON
    CFrmUpdater *pUpdater = new CFrmUpdater();
    if(!pUpdater) {
        qCritical(log) << "new CFrmUpdater fail";
        return;
    }
    pUpdater->SetTitle(QImage(":/icon/AppIcon"));

#if defined (Q_OS_ANDROID)
    pUpdater->showMaximized();
#else
    pUpdater->show();
#endif

#endif
}

void CMainWindow::on_actionRefresh_R_triggered()
{
    RefreshSerialPorts();
}

void CMainWindow::on_pbBrowseSend_clicked()
{
    QString szFile = QFileDialog::getOpenFileName(this, tr("Open send file"));
    if(szFile.isEmpty())
        return;
    ui->leSendFile->setText(szFile);    
}

void CMainWindow::on_pbBrowseSave_clicked()
{
    QString szFile = QFileDialog::getOpenFileName(this, tr("Open save file"));
    if(szFile.isEmpty())
        return;
    ui->leSaveToFile->setText(szFile);
}

void CMainWindow::on_tbSendSettings_currentChanged(int index)
{
    Q_UNUSED(index);
    if(m_SerialPort.isOpen())
    {
        int nRet = QMessageBox::warning(this, tr("Close serial port"),
                                        tr("Will be close serial port ?"),
                                        QMessageBox::Ok | QMessageBox::No);
        if(QMessageBox::Ok == nRet)
            on_pbOpen_clicked(); // close serial port
    }
    
    if("SendFile" == ui->tbSendSettings->currentWidget()->objectName())
        ui->teSend->setEnabled(false);
    else
        ui->teSend->setEnabled(true);
}

void CMainWindow::on_cbSaveToFile_clicked(bool checked)
{
    CGlobal::Instance()->SetSaveFile(checked);
}

void CMainWindow::on_pbPortSeetings_clicked()
{
    bool bVisible = ui->gbPortSeetings->isHidden();
    ui->gbPortSeetings->setVisible(bVisible);
    ui->pbPortSeetings->setChecked(bVisible);
    if(bVisible)
        ui->pbPortSeetings->setIcon(QIcon(":/icon/Down"));
    else
        ui->pbPortSeetings->setIcon(QIcon(":/icon/Right"));
}

void CMainWindow::on_pbReceiveSettings_clicked()
{
    bool bVisible = ui->gbReceiveSettings->isHidden();
    ui->gbReceiveSettings->setVisible(bVisible);
    ui->pbReceiveSettings->setChecked(bVisible);
    if(bVisible)
        ui->pbReceiveSettings->setIcon(QIcon(":/icon/Down"));
    else
        ui->pbReceiveSettings->setIcon(QIcon(":/icon/Right"));
}

void CMainWindow::on_pbSendSettings_clicked()
{
    bool bVisible = ui->gbSendSettings->isHidden();
    ui->gbSendSettings->setVisible(bVisible);
    if(bVisible)
        ui->pbSendSettings->setIcon(QIcon(":/icon/Down"));
    else
        ui->pbSendSettings->setIcon(QIcon(":/icon/Right"));
}

void CMainWindow::slotDataTerminalReadyChanged(bool set)
{
    Q_UNUSED(set);
    setPinoutStatus();
}

void CMainWindow::slotRequestToSendChanged(bool set)
{
    Q_UNUSED(set);
    setPinoutStatus();
}

int CMainWindow::InitPinout()
{
    ui->ltCTS->setText(tr("CTS"));
    ui->ltCTS->setGray();
    ui->ltDCD->setText(tr("DCD"));
    ui->ltDCD->setGray();
    ui->ltDTR->setText(tr("DTR"));
    ui->ltDTR->setGray();
    ui->ltDSR->setText(tr("DSR"));
    ui->ltDSR->setGray();
    ui->ltRNG->setText(tr("PNG"));
    ui->ltRNG->setGray();
    ui->ltRTS->setText(tr("RTS"));
    ui->ltRTS->setGray();
    ui->ltSRD->setText(tr("SRD"));
    ui->ltSRD->setGray();
    ui->ltSTD->setText(tr("STD"));
    ui->ltSTD->setGray();
    return 0;
}

int CMainWindow::setPinoutStatus()
{
    if(!m_SerialPort.isOpen())
        return -1;
    QSerialPort::PinoutSignals ps = m_SerialPort.pinoutSignals();
    if(QSerialPort::DataTerminalReadySignal & ps)
        ui->ltDTR->setGreen();
    else
        ui->ltDTR->setGray();
    
    if(QSerialPort::DataCarrierDetectSignal & ps)
        ui->ltDCD->setGreen();
    else
        ui->ltDCD->setGray();
    
    if(QSerialPort::DataSetReadySignal & ps)
        ui->ltDSR->setGreen();
    else
        ui->ltDSR->setGray();
    
    if(QSerialPort::RingIndicatorSignal & ps)
        ui->ltRNG->setGreen();
    else
        ui->ltRNG->setGray();
    
    if(QSerialPort::RequestToSendSignal & ps)
        ui->ltRTS->setGreen();
    else
        ui->ltRTS->setGray();
    
    if(QSerialPort::ClearToSendSignal & ps)
        ui->ltCTS->setGreen();
    else
        ui->ltCTS->setGray();
    
    if(QSerialPort::SecondaryTransmittedDataSignal & ps)
        ui->ltSTD->setGreen();
    else
        ui->ltSTD->setGray();
    
    if(QSerialPort::SecondaryReceivedDataSignal & ps)
        ui->ltSRD->setGreen();
    else
        ui->ltSRD->setGray();
    
    return 0;
}
