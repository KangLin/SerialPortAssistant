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

#include <QLoggingCategory>
static Q_LOGGING_CATEGORY(log, "main")
    
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
    RabbitCommon::CTools::Instance()->Init();
    RabbitCommon::CTools::Instance()->InstallTranslator("SerialPortAssistant");
#endif
    qInfo(log) << QObject::tr("SerialPortAssistant version: %1 ").arg(a.applicationVersion())
#ifdef SerialPortAssistant_Revision
                      + QObject::tr("(Revision:")
                      + " [" + SerialPortAssistant_Revision
                      + "](http://github.com/KangLin/RabbitCommon/tree/"
                      + SerialPortAssistant_Revision + ")"
#endif
               + "); "
               + QObject::tr("RabbitCommon") + RabbitCommon::CTools::Version();

    CMainWindow *w = new CMainWindow();
    if(!w)
    {
        qCritical(log) << "new CMainWindow fail";
        return -1;
    }
    a.setApplicationDisplayName(w->windowTitle());

#ifdef HAVE_UPDATE
    QSharedPointer<CFrmUpdater> pUpdater;
    // Check update version
    if(qEnvironmentVariable("SNAP").isEmpty()
        && qEnvironmentVariable("FLATPAK_ID").isEmpty()) {
        pUpdater = QSharedPointer<CFrmUpdater>(new CFrmUpdater());
        if(pUpdater) {
            pUpdater->setAttribute(Qt::WA_DeleteOnClose, false);
            pUpdater->SetTitle(QImage(":/icon/AppIcon"));
            if(a.arguments().length() > 1) {
                try {
                    pUpdater->GenerateUpdateJson();
                    pUpdater->GenerateUpdateXml();
                } catch(...) {
                    qCritical(log) << "Generate update fail";
                }
                qInfo(log) << a.applicationName() + " " + a.applicationVersion()
                                  + " " + QObject::tr("Generate update json file End");
                return 0;
            }
        } else {
            qCritical(log) << "new CFrmUpdater() fail";
        }
    }
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
