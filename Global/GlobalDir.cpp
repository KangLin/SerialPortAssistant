/*
 * Author: KangLin(Email:kl222@126.com)
 */
#include "GlobalDir.h"
#include "Log.h"
#include <QStandardPaths>
#include <QDir>
#include <QApplication>

CGlobalDir::CGlobalDir()
{
    //注意这个必须的在最前  
    m_szDocumentPath =  QStandardPaths::writableLocation(
                QStandardPaths::DocumentsLocation);
    if(m_szDocumentPath.isEmpty())
    {
        LOG_MODEL_ERROR("CGlobalDir", "document path is empty");
    }
}

CGlobalDir* CGlobalDir::Instance()
{
    static CGlobalDir* p = NULL;
    if(!p)
        p = new CGlobalDir;
    return p;
}

QString CGlobalDir::GetDirApplication()
{
#ifdef ANDROID
    //LOG_MODEL_DEBUG("CGlobalDir", "GetDirApplication:%s", qApp->applicationDirPath().toStdString().c_str());
    return qApp->applicationDirPath() + QDir::separator() + "..";
#else
    //LOG_MODEL_DEBUG("CGlobalDir", "GetDirApplication:%s", qApp->applicationDirPath().toStdString().c_str());
    return qApp->applicationDirPath();
#endif
}

QString CGlobalDir::GetDirDocument()
{
    QString szPath;
    if(m_szDocumentPath.right(1) == "/"
            || m_szDocumentPath.right(1) == "\\")
        szPath = m_szDocumentPath.left(m_szDocumentPath.size() - 1);
    else
        szPath = m_szDocumentPath;
    szPath += QDir::separator();
    szPath = szPath + "Rabbit"
             + QDir::separator() + "SerialPortAssistant";
    return szPath;
}

int CGlobalDir::SetDirDocument(QString szPath)
{
    m_szDocumentPath = szPath + QDir::separator() + "Rabbit"
            + QDir::separator() + "SerialPortAssistant";
    return 0;
}

QString CGlobalDir::GetDirData()
{
    return GetDirApplication() + QDir::separator() + "Data";
}

QString CGlobalDir::GetDirImage()
{
    return GetDirData() + QDir::separator() + "Image";
}

QString CGlobalDir::GetDirTranslate()
{
#ifdef ANDROID
    //TODO:android下应该在安装包中装好语言  
    return GetDirDocument() + QDir::separator() + "translations";
#endif
    return GetDirApplication() + QDir::separator() + "translations";
}

QString CGlobalDir::GetApplicationConfigureFile()
{
    return GetDirDocument() + QDir::separator() + "App.conf";
}
