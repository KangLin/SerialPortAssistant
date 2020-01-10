#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include <QFile>
#include <QIODevice>

#define BUFFER_SIZE 512
class CSendFile : public QObject
{
    Q_OBJECT
public:
    explicit CSendFile(const QString &szFile = QString(), QObject *parent = nullptr);

    int Open(const QString &szFile);
    bool isOpen();
    int Close();
    qint64 Write(QIODevice *device);

private:
    QFile m_File;

    qint64 m_nTotal;
    qint64 m_nSend;
    int m_nBuffLen;
    int m_nBuffStart;
    char buf[BUFFER_SIZE];
};

#endif // SENDFILE_H
