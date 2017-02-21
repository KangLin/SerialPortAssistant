/*
 * Author: KangLin(Email:kl222@126.com)
 */
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Global/Log.h"
#include "Global/Global.h"
#include "Common/Tool.h"
#include <QMessageBox>
#include <QTime>
#include <QFile>
#include <QSettings>
#include "Widgets/DlgAbout/DlgAbout.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow),
    m_SerialPort(this),
    m_Timer(this), 
    m_nSend(0),
    m_nRecive(0),
    m_ActionGroupTranslator(this),
    m_ActionGroupStyle(this),
    m_cmbPortIndex(-1)
{
    bool check = false;

    ui->setupUi(this);
    
    LoadTranslate();
    LoadStyle();
    InitMenu();
    
    InitStatusBar();
    InitToolBar();
    InitLeftBar();

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QString szPort;
        szPort = info.portName();
        if(!info.description().isEmpty())
        {
            szPort += "(" + info.description() + ")";
        }
        ui->cmbPort->addItem(szPort);
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
    
    ui->cbSendLoop->setChecked(CGlobal::Instance()->GetSendLoop());
    ui->sbLoopTime->setValue(CGlobal::Instance()->GetSendLoopTime());
    check = connect(&m_Timer, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
    Q_ASSERT(check);
    
    CGlobal::SEND_R_N v = CGlobal::Instance()->GetSendRN();
    ui->cbr->setChecked(v & CGlobal::SEND_R_N::R);
    ui->cbn->setChecked(v & CGlobal::SEND_R_N::N);
    
    ui->cbDisplaySend->setChecked(CGlobal::Instance()->GetReciveDisplaySend());
    ui->cbDisplayTime->setChecked(CGlobal::Instance()->GetReciveDisplayTime());
}

CMainWindow::~CMainWindow()
{
    on_pbOpen_clicked();
    ClearMenu();
    ClearTranslate();

    delete ui;
}

int CMainWindow::InitStatusBar()
{
    ui->actionStatusBar_S->setChecked(
                CGlobal::Instance()->GetStatusbarVisable());
    statusBar()->setVisible(CGlobal::Instance()->GetStatusbarVisable());

    SetStatusInfo(tr("Ready"));
    m_statusRx.setText(tr("Rx: 0 Bytes"));
    m_statusTx.setText(tr("Tx: 0 Bytes"));
    m_statusInfo.setSizePolicy(QSizePolicy::Policy::Expanding,
                               QSizePolicy::Policy::Preferred);
    m_statusTx.setSizePolicy(QSizePolicy::Policy::Preferred,
                             QSizePolicy::Policy::Preferred);
    m_statusRx.setSizePolicy(QSizePolicy::Policy::Preferred,
                             QSizePolicy::Policy::Preferred);

    this->statusBar()->addWidget(&m_statusInfo);
    this->statusBar()->addWidget(&m_statusRx);
    this->statusBar()->addWidget(&m_statusTx);

    return 0;
}

int CMainWindow::InitToolBar()
{
    ui->actionToolBar_T->setChecked(
                CGlobal::Instance()->GetToolbarVisable());
    ui->mainToolBar->setVisible(CGlobal::Instance()->GetToolbarVisable());
    return 0;
}

int CMainWindow::InitLeftBar()
{
    bool bVisable = false;
    bVisable = CGlobal::Instance()->GetLeftBarVisable();
    ui->actionLeftBar_L->setChecked(bVisable);
    ui->frmLeftBar->setVisible(bVisable);
    return 0;
}

void CMainWindow::slotTimeOut()
{
    on_pbSend_clicked();
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
        ui->pbOpen->setIcon(QIcon(":/icon/Start"));
        ui->actionOpen->setText(tr("Open(&O)"));
        ui->actionOpen->setIcon(QIcon(":/icon/Start"));
        ui->pbSend->setEnabled(false);
        bCheck = m_SerialPort.disconnect();
        Q_ASSERT(bCheck);

        SetStatusInfo(tr("Serail Port Close"));
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
        SetStatusInfo(tr("Open serail port %1 fail.").
                                   arg(ui->cmbPort->currentText()),
                      Qt::red);
        return;
    }

    bCheck = connect(&m_SerialPort, SIGNAL(readyRead()), this, SLOT(slotRead()));
    Q_ASSERT(bCheck);
    ui->pbOpen->setText(tr("Close(&C)"));
    ui->pbOpen->setIcon(QIcon(":/icon/Stop"));
    ui->actionOpen->setText(tr("Close(&C)"));
    ui->actionOpen->setIcon(QIcon(":/icon/Stop"));
    ui->pbSend->setEnabled(true);

    SetStatusInfo(GetSerialPortSettingInfo(),
                  Qt::green);
    m_nSend = 0;
    m_nRecive = 0;    
    m_statusRx.setText(tr("Rx: 0 Bytes"));
    m_statusTx.setText(tr("Tx: 0 Bytes"));
    if(ui->cbSendLoop->isChecked())
        m_Timer.start(ui->sbLoopTime->value());
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
    return ui->cmbPort->currentText() + tr(" Open. ")
            + ui->cmbBoudRate->currentText() + "|"
            + ui->cmbDataBit->currentText() + "|"
            + ui->cmbParity->currentText() + "|"
            + ui->cmbStopBit->currentText() + "|"
            + ui->cmbFlowControl->currentText();
}

void CMainWindow::AddRecive(QString &szText, bool bRecive)
{
    QString szPrex;
    
    if(ui->cbDisplayTime->isChecked())
        szPrex = QTime::currentTime().toString() + " ";
        
    if(ui->cbDisplaySend->isChecked())
    {
        if(bRecive)
            szPrex += "<- ";
        else
            szPrex += "-> ";
    }
    
    if(!szPrex.isEmpty())
    {
        ui->teRecive->insertPlainText("\n");
        ui->teRecive->insertPlainText(szPrex);
    }
    
    ui->teRecive->insertPlainText(szText);

    QTextCursor cursor = ui->teRecive->textCursor();
    cursor.movePosition(QTextCursor::End);  //把光标移动到文档最后  
    ui->teRecive->setTextCursor(cursor);
}

void CMainWindow::slotRead()
{
    if(!m_SerialPort.isOpen())
    {
        LOG_MODEL_ERROR("MainWindow", "SerialPort don't open!");
        return;
    }

    QString szText(m_SerialPort.readAll());
    AddRecive(szText, true);
    m_nRecive += szText.toStdString().length();
    m_statusRx.setText(tr("Rx: ") + QString::number(m_nRecive) + tr(" Bytes"));
}

void CMainWindow::on_pbSend_clicked()
{
    int nRet = 0;
    if(ui->teSend->toPlainText().isEmpty())
    {
        LOG_MODEL_WARNING("CMainWindow", "Send text is empty");
        return;
    }
    QString szText = ui->teSend->toPlainText();
    if(ui->cbr->isChecked())
        szText += "\r";
    if(ui->cbn->isChecked())
        szText += "\n";
    nRet = m_SerialPort.write(szText.toStdString().c_str());
    LOG_MODEL_DEBUG("CMainWindows", "Send %d bytes", nRet);
    if(ui->cbDisplaySend->isChecked())
        AddRecive(szText, false);

    m_nSend += ui->teSend->toPlainText().toStdString().length();
    m_statusTx.setText(tr("Tx: ") + QString::number(m_nSend) + tr(" Bytes"));

    if(-1 == ui->cmbRecent->findText(
                ui->teSend->toPlainText().toStdString().c_str()))
        ui->cmbRecent->addItem(ui->teSend->toPlainText().toStdString().c_str());
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
        else
            on_pbOpen_clicked();
    }
    m_cmbPortIndex = index;
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
    CGlobal::Instance()->SetSendLoop(ui->cbSendLoop->isChecked());
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
    ui->cmbRecent->clear();
}

void CMainWindow::on_actionAbout_A_triggered()
{
    CDlgAbout about(this);
    about.exec();
}
int CMainWindow::InitMenuTranslate()
{
    m_ActionTranslator["Default"] = ui->menuLanguage_A->addAction(
                QIcon(":/icon/Language"), tr("Default"));
    m_ActionTranslator["en"] = ui->menuLanguage_A->addAction(
                QIcon(":/icon/English"), tr("English"));
    m_ActionTranslator["zh_CN"] = ui->menuLanguage_A->addAction(
                QIcon(":/icon/China"), tr("Chinese"));
    m_ActionTranslator["zh_TW"] = ui->menuLanguage_A->addAction(
                QIcon(":/icon/China"), tr("Chinese(TaiWan)"));
    
    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionTranslator.begin(); it != m_ActionTranslator.end(); it++)
    {
        it.value()->setCheckable(true);
        m_ActionGroupTranslator.addAction(it.value());
    }

    LOG_MODEL_DEBUG("MainWindow",
                    "MainWindow::InitMenuTranslate m_ActionTranslator size:%d",
                    m_ActionTranslator.size());

    bool check = connect(&m_ActionGroupTranslator, SIGNAL(triggered(QAction*)),
                        SLOT(slotActionGroupTranslateTriggered(QAction*)));
    Q_ASSERT(check);

    QString szLocale = CGlobal::Instance()->GetLanguage();
    QAction* pAct = m_ActionTranslator[szLocale];
    if(pAct)
    {
        LOG_MODEL_DEBUG("MainWindow",
                        "MainWindow::InitMenuTranslate setchecked locale:%s",
                        szLocale.toStdString().c_str());
        pAct->setChecked(true);
        ui->menuLanguage_A->setIcon(pAct->icon());
        LOG_MODEL_DEBUG("MainWindow",
                        "MainWindow::InitMenuTranslate setchecked end");
    }
    
    return 0;
}

int CMainWindow::ClearMenuTranslate()
{
    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionTranslator.begin(); it != m_ActionTranslator.end(); it++)
    {
        m_ActionGroupTranslator.removeAction(it.value());
    }
    m_ActionGroupTranslator.disconnect();
    m_ActionTranslator.clear();
    ui->menuLanguage_A->clear();    

    LOG_MODEL_DEBUG("MainWindow",
                    "MainWindow::ClearMenuTranslate m_ActionTranslator size:%d",
                    m_ActionTranslator.size());
    
    return 0;
}

int CMainWindow::ClearTranslate()
{
    if(!m_TranslatorQt.isNull())
    {
        qApp->removeTranslator(m_TranslatorQt.data());
        m_TranslatorQt.clear();
    }

    if(m_TranslatorApp.isNull())
    {
        qApp->removeTranslator(m_TranslatorApp.data());
        m_TranslatorApp.clear();
    }
    return 0;
}

int CMainWindow::LoadTranslate(QString szLocale)
{
    if(szLocale.isEmpty())
    {
        szLocale = CGlobal::Instance()->GetLanguage();
    }

    if("Default" == szLocale)
    {
        szLocale = QLocale::system().name();
    }

    LOG_MODEL_DEBUG("main", "locale language:%s",
                    szLocale.toStdString().c_str());

    ClearTranslate();
    LOG_MODEL_DEBUG("MainWindow", "Translate dir:%s",
                    qPrintable(CGlobalDir::Instance()->GetDirTranslate()));

    m_TranslatorQt = QSharedPointer<QTranslator>(new QTranslator(this));
    m_TranslatorQt->load("qt_" + szLocale + ".qm",
                         CGlobalDir::Instance()->GetDirTranslate());
    qApp->installTranslator(m_TranslatorQt.data());

    m_TranslatorApp = QSharedPointer<QTranslator>(new QTranslator(this));
#ifdef ANDROID
    m_TranslatorApp->load(":/translations/app_" + szLocale + ".qm");
#else
    m_TranslatorApp->load("app_" + szLocale + ".qm",
                          CGlobalDir::Instance()->GetDirTranslate());
#endif
    qApp->installTranslator(m_TranslatorApp.data());

    ui->retranslateUi(this);
    return 0;
}

void CMainWindow::slotActionGroupTranslateTriggered(QAction *pAct)
{
    LOG_MODEL_DEBUG("MainWindow", "MainWindow::slotActionGroupTranslateTriggered");
    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionTranslator.begin(); it != m_ActionTranslator.end(); it++)
    {
        if(it.value() == pAct)
        {
            QString szLocale = it.key();
            CGlobal::Instance()->SetLanguage(szLocale);
            LoadTranslate(it.key());
            pAct->setChecked(true);
            QMessageBox::information(this, tr("Close"),
                   tr("Language changes, close the program, and please restart the program."));
            this->close();
            return;
        }
    }
}

int CMainWindow::InitMenuStyles()
{
    QMap<QString, QAction*>::iterator it;
    m_ActionStyles["Custom"] = ui->menuStype_S->addAction(tr("Custom"));
    m_ActionStyles["System"] = ui->menuStype_S->addAction(tr("System"));
    m_ActionStyles["Blue"] = ui->menuStype_S->addAction(tr("Blue"));
    m_ActionStyles["Dark"] = ui->menuStype_S->addAction(tr("Dark"));
    
    for(it = m_ActionStyles.begin(); it != m_ActionStyles.end(); it++)
    {
        it.value()->setCheckable(true);
        m_ActionGroupStyle.addAction(it.value());
    }
    bool check = connect(&m_ActionGroupStyle, SIGNAL(triggered(QAction*)),
                         SLOT(slotActionGroupStyleTriggered(QAction*)));
    Q_ASSERT(check);
    QAction* pAct = m_ActionStyles[CGlobal::Instance()->GetStyleMenu()];
    if(pAct)
    {
        pAct->setChecked(true);
    }
    return 0;
}

int CMainWindow::ClearMenuStyles()
{
    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionStyles.begin(); it != m_ActionStyles.end(); it++)
    {
        m_ActionGroupStyle.removeAction(it.value());
    }
    m_ActionGroupStyle.disconnect();
    m_ActionStyles.clear();
    ui->menuStype_S->clear();
    return 0;
}

int CMainWindow::LoadStyle()
{
    QString szFile = CGlobal::Instance()->GetStyle();
    if(szFile.isEmpty())
        qApp->setStyleSheet("");
    else
    {
        QFile file(szFile);
        if(file.open(QFile::ReadOnly))
        {
            QString stylesheet= file.readAll();
            qApp->setStyleSheet(stylesheet);
            file.close();
        }
        else
        {
            LOG_MODEL_ERROR("app", "file open file [%s] fail:%d",
                        CGlobal::Instance()->GetStyle().toStdString().c_str(),
                        file.error());
        }
    }
    return 0;
}

int CMainWindow::OpenCustomStyleMenu()
{
    QString szFile;
    QString szFilter("*.qss *.*");
    szFile = CTool::FileDialog(this, QString(), szFilter, tr("Open File"));
    if(szFile.isEmpty())
        return -1;

    QFile file(szFile);
    if(file.open(QFile::ReadOnly))
    {
        QString stylesheet= file.readAll();
        qApp->setStyleSheet(stylesheet);
        file.close();
        QSettings conf(CGlobalDir::Instance()->GetApplicationConfigureFile(),
                       QSettings::IniFormat);
        conf.setValue("UI/StyleSheet", szFile);
        
        CGlobal::Instance()->SetStyleMenu("Custom", szFile);
    }
    else
    {
        LOG_MODEL_ERROR("app", "file open file [%s] fail:%d", 
                        szFile.toStdString().c_str(), file.error());
    }
    return 0;
}

void CMainWindow::slotActionGroupStyleTriggered(QAction* act)
{
    QMap<QString, QAction*>::iterator it;
    for(it = m_ActionStyles.begin(); it != m_ActionStyles.end(); it++)
    {
        if(it.value() == act)
        {
            act->setChecked(true);
            if(it.key() == "Blue")
                CGlobal::Instance()->SetStyleMenu("Blue", ":/sink/Blue");
            else if(it.key() == "Dark")
                CGlobal::Instance()->SetStyleMenu("Dark", ":/sink/Dark");
            else if(it.key() == "Custom")
                OpenCustomStyleMenu();
            else
                CGlobal::Instance()->SetStyleMenu("System", "");
        }
    }

    LoadStyle();
}

void CMainWindow::InitMenu()
{
    InitMenuStyles();
    InitMenuTranslate();
}

void CMainWindow::ClearMenu()
{
    ClearMenuTranslate();
    ClearMenuStyles();
}

void CMainWindow::changeEvent(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    }
}

void CMainWindow::on_actionToolBar_T_triggered()
{
    ui->mainToolBar->setVisible(ui->actionToolBar_T->isChecked());
    CGlobal::Instance()->SetToolbarVisable(ui->actionToolBar_T->isChecked());
}

void CMainWindow::on_actionStatusBar_S_triggered()
{
    this->statusBar()->setVisible(ui->actionStatusBar_S->isChecked());
    CGlobal::Instance()->SetStatusbarVisable(ui->actionStatusBar_S->isChecked());
}

void CMainWindow::on_actionLeftBar_L_triggered()
{
    bool bVisable = false;
    bVisable = ui->actionLeftBar_L->isChecked();
    ui->frmLeftBar->setVisible(bVisable);
    CGlobal::Instance()->SetLeftBarVisable(bVisable);
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
    CGlobal::Instance()->SetReciveDisplaySend(checked);
}

void CMainWindow::on_cbDisplayTime_clicked(bool checked)
{
    CGlobal::Instance()->SetReciveDisplayTime(checked);
}

void CMainWindow::on_cmbBoudRate_currentTextChanged(const QString &szText)
{
    if(!m_SerialPort.isOpen())
        return;
    
    bool bRet;
    bRet = m_SerialPort.setBaudRate(szText.toInt());
    if(bRet)
        SetStatusInfo(GetSerialPortSettingInfo(),
                  Qt::green);
    else
        SetStatusInfo(tr("Set baud rate fail"), Qt::red);
}

void CMainWindow::on_cmbDataBit_currentTextChanged(const QString &szText)
{
    if(!m_SerialPort.isOpen())
        return;
    bool bRet;
    bRet = m_SerialPort.setDataBits((QSerialPort::DataBits)szText.toInt());
    if(bRet)
        SetStatusInfo(GetSerialPortSettingInfo(),
                  Qt::green);
    else
        SetStatusInfo(tr("Set data bits fail"), Qt::red);
}

void CMainWindow::on_cmbParity_currentIndexChanged(int index)
{
    if(-1 == index)
        return;
    
    if(!m_SerialPort.isOpen())
        return;
    
    bool bRet = false;
    if(index > 0)
        m_SerialPort.setParity((QSerialPort::Parity)(index + 1));
    else if(0 == index)
        m_SerialPort.setParity(QSerialPort::NoParity); 
    
    if(bRet)
        SetStatusInfo(GetSerialPortSettingInfo(),
                  Qt::green);
    else
        SetStatusInfo(tr("Set parity fail"), Qt::red);
}

void CMainWindow::on_cmbStopBit_currentTextChanged(const QString &szText)
{
    if(!m_SerialPort.isOpen())
        return;
    
    bool bRet;
    bRet = m_SerialPort.setStopBits((QSerialPort::StopBits)szText.toInt());
    if(bRet)
        SetStatusInfo(GetSerialPortSettingInfo(),
                  Qt::green);
    else
        SetStatusInfo(tr("Set stop bits fail"), Qt::red);
}

void CMainWindow::on_cmbFlowControl_currentIndexChanged(int index)
{
    if(!m_SerialPort.isOpen())
        return;
    if(-1 == index)
        return;
    bool bRet;
    bRet = m_SerialPort.setFlowControl((QSerialPort::FlowControl)index);
    if(bRet)
        SetStatusInfo(GetSerialPortSettingInfo(),
                  Qt::green);
    else
        SetStatusInfo(tr("Set Flow Control fail"), Qt::red);
}
