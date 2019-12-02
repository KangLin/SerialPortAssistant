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
    #include "FrmUpdater/FrmUpdater.h"
    #include "RabbitCommonDir.h"
#endif
#include "MainWindow.h"
#include <QApplication>
#include <QDir>
#include "Global/Global.h"
#if defined(Q_OS_ANDROID)
    #include <QtAndroidExtras/QtAndroid>
#endif

int main(int argc, char *argv[])
{
#if (QT_VERSION > QT_VERSION_CHECK(5,6,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#if defined(Q_OS_ANDROID) && QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
    QtAndroid::hideSplashScreen();
#endif
    
    QApplication a(argc, argv);
    a.setApplicationName("SerialPortAssistant");
    
#ifdef RABBITCOMMON
    QString szLocale = CGlobal::Instance()->GetLanguage();
    if("Default" == szLocale)
    {
        szLocale = QLocale::system().name();
    }
    RabbitCommon::CTools::Instance()->Init(szLocale);
#endif

    CMainWindow w;
    
    a.setApplicationDisplayName(QObject::tr("SerialPort Assistant"));
#ifdef BUILD_VERSION
    a.setApplicationVersion(BUILD_VERSION);
#endif

#ifdef RABBITCOMMON 
    CFrmUpdater *pUpdater = new CFrmUpdater(); 
    pUpdater->SetTitle(QImage(":/icon/AppIcon"));
    if(!pUpdater->GenerateUpdateXml()) 
        return 0; 
#endif

    w.show();

    return a.exec();
}
