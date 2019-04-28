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
#endif
#include "MainWindow.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
     QString szPre;    
     #if defined(Q_OS_ANDROID) || _DEBUG
         szPre = ":/Translations";
     #else
         szPre = qApp->applicationDirPath() + QDir::separator() + ".." + QDir::separator() + "translations";
     #endif
     QTranslator translator;
     translator.load(szPre + "/SerialPortAssistant_" + QLocale::system().name() + ".qm");
     qApp->installTranslator(&translator);

#ifdef RABBITCOMMON
    CRabbitCommonTools::Instance()->Init();
#endif
    
    a.setApplicationName("SerialPortAssistant");
    a.setApplicationDisplayName(QObject::tr("SerialPort Assistant"));
    
#ifdef RABBITCOMMON 
    CFrmUpdater *pUpdate = new CFrmUpdater(); 
    if(!pUpdate->GenerateUpdateXml()) 
        return 0; 
#endif
    CMainWindow w;
    w.show();

    return a.exec();
}
