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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef RABBITCOMMON
    QString szLocale = CGlobal::Instance()->GetLanguage();
    if("Default" == szLocale)
    {
        szLocale = QLocale::system().name();
    }
    RabbitCommon::CTools::Instance()->Init(szLocale);
#endif

    CMainWindow w;

    a.setApplicationName("SerialPortAssistant");
    a.setApplicationDisplayName(QObject::tr("SerialPort Assistant"));
#ifdef BUILD_VERSION
    a.setApplicationVersion(BUILD_VERSION);
#endif

#ifdef RABBITCOMMON 
    CFrmUpdater *pUpdate = new CFrmUpdater(); 
    if(!pUpdate->GenerateUpdateXml()) 
        return 0; 
#endif

    w.show();

    return a.exec();
}
