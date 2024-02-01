/*++
Copyright (c) Kang Lin studio, All Rights Reserved

Author:
	Kang Lin(kl222@126.com）

Module Name:

    main.cpp

Abstract:

    This file contains main implement.
 */

#ifdef RABBITCOMMON
    #include "RabbitCommonTools.h"
    #include "FrmUpdater.h"
#endif
#include "MainWindow.h"
#include <QApplication>
#include <QDir>
#include "Global/Global.h"
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    #include <QtAndroidExtras/QtAndroid>
#endif
#ifdef BUILD_QUIWidget
    #include "QUIWidget/QUIWidget.h"
#endif

int main(int argc, char *argv[])
{
#if defined (_DEBUG)
    Q_INIT_RESOURCE(translations_SerialPortAssistant);
#endif

    QApplication a(argc, argv);
    a.setApplicationName("SerialPortAssistant");
#ifdef SerialPortAssistant_VERSION
    a.setApplicationVersion(SerialPortAssistant_VERSION);
#endif
    
#ifdef RABBITCOMMON
    QString szLocale = CGlobal::Instance()->GetLanguage();
    if("Default" == szLocale)
    {
        szLocale = QLocale::system().name();
    }
    RabbitCommon::CTools::Instance()->Init(szLocale);
#endif

    a.setApplicationDisplayName(QObject::tr("Serial Port Assistant"));
    
    CMainWindow *w = new CMainWindow();

#ifdef RABBITCOMMON 
    CFrmUpdater *pUpdater = new CFrmUpdater(); 
    pUpdater->SetTitle(QImage(":/icon/AppIcon"));
    if(!pUpdater->GenerateUpdateXml()) 
        return 0; 
#endif

#if defined(BUILD_QUIWidget) && !defined(Q_OS_ANDROID)
    QUIWidget* quiwidget = new QUIWidget(nullptr, true);
    quiwidget->setMainWidget(w);
    //quiwidget->setPixmap(QUIWidget::Lab_Ico, ":/icon/AppIcon");
    //quiwidget.setTitle(a.applicationDisplayName());
    quiwidget->show();
#else
    w->show();
#endif

    int nRet = a.exec();
    
#ifndef BUILD_QUIWidget
    delete w;
#endif
    
#ifdef RABBITCOMMON
    RabbitCommon::CTools::Instance()->Clean();
#endif
#if defined (_DEBUG)
    Q_CLEANUP_RESOURCE(translations_SerialPortAssistant);
#endif
    return nRet;
}
