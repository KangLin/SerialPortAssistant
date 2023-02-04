#include "SendFile.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(logSendFile, "SendFile");
CSendFile::CSendFile(const QString &szFile, QObject *parent) : QObject(parent)
{
    Open(szFile);
}

int CSendFile::Open(const QString &szFile)
{
    Close();

    m_nSend = 0;
    m_nBuffLen = BUFFER_SIZE;
    m_nBuffStart = 0;
    m_nTotal = 0;

    if(!szFile.isEmpty())
    {
        m_File.setFileName(szFile);
        if(!m_File.open(QFile::ReadOnly))
        {
            qCritical(logSendFile) << "Open file fail:"
                                   << szFile.toStdString().c_str();
            return -1;
        }
        m_nTotal = m_File.size();
    }

    return 0;
}

int CSendFile::Close()
{
    if(m_File.isOpen())
        m_File.close();

    m_nSend = 0;
    m_nBuffLen = BUFFER_SIZE;
    m_nBuffStart = 0;
    m_nTotal = 0;
    return 0;
}

bool CSendFile::isOpen()
{
    return m_File.isOpen() && !m_File.atEnd();
}

qint64 CSendFile::Write(QIODevice *device)
{
    qint64 nRet = 0;
    qint64 nLen = 0;

    if(0 == m_nBuffStart)
    {
        if(!m_File.isOpen())
        {
            qCritical(logSendFile) << "Read file isn't open";
            return -1;
        }

        if(m_File.atEnd())
        {
            Close();
            return 0;
        }

        // Read data from file
        nLen = m_File.read(buf, m_nBuffLen);
        if(nLen < 0)
        {
            qCritical(logSendFile) << "Read file fail: %s" << m_File.errorString().toStdString().c_str();
            return nLen;
        }
    }

    // Write data to device
    nRet = device->write(buf + m_nBuffStart, nLen);
    if(nRet < 0)
    {
        qCritical(logSendFile) << "Write file fail: %s" << device->errorString().toStdString().c_str();
        return nRet;
    }

    m_nSend += nRet;

    if(nRet < m_nBuffLen)
    {
        m_nBuffStart = m_nBuffStart + nRet;
        m_nBuffLen -= nRet;
    } else {
        m_nBuffLen = BUFFER_SIZE;
        m_nBuffStart = 0;
    }

    return nRet;
}
