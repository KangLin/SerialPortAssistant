// Author: Kang Lin <kl222@126.com>
#include <QLoggingCategory>
#include "Stats.h"

static Q_LOGGING_CATEGORY(log, "Stats")
CStats::CStats(QObject *parent)
    : QObject{parent}
    , m_TotalSends(0)
    , m_TotalReceives(0)
    , m_lastSends(0)
    , m_lastReceives(0)
    , m_Drops(0)
    , m_lastTime(QDateTime::currentDateTime())
    , m_dbSendRate(0)
    , m_dbReceiveRate(0)
    , m_tmInterval(0)
    , m_StringTypes(StringType::All)
{
    bool check = connect(&m_Timer, &QTimer::timeout,
                         this, &CStats::slotCalculating);
    Q_ASSERT(check);
    //SetInterval(5);
}

CStats::~CStats()
{
    Reset();
}

void CStats::SetStringType(StringTypes types)
{
    m_StringTypes = types;
}

QString CStats::Convertbytes(quint64 bytes)
{
    QString szBytes;
    if((1 << 10) >= bytes)
        szBytes = QString::number(bytes) + " " + tr("B");
    else if((1 << 20) >= bytes)
        szBytes = QString::number((qreal)bytes / (1 << 10), 'f', 2) + " " + tr("KB");
    else if((1 << 30) >= bytes)
        szBytes = QString::number((qreal)bytes / (1 << 20), 'f', 2) + " " + tr("MB");
    else
        szBytes = QString::number((qreal)bytes / (1 << 30), 'f', 2) + " " + tr("GB");
    return szBytes;
}

int CStats::Reset()
{
    m_TotalSends = 0;
    m_TotalReceives = 0;
    m_lastTime = QDateTime::currentDateTime();
    m_lastSends = 0;
    m_lastReceives = 0;
    m_dbSendRate = 0;
    m_dbReceiveRate = 0;
    m_tmInterval = 0;
    if(m_Timer.isActive())
        m_Timer.stop();
    return 0;
}

QString CStats::ToString()
{
    return SendToString() + "; " + ReceiveToString() + "; " + DropToString();
}

QString CStats::SendToString()
{
    QString s;
    if(StringType::Counting & m_StringTypes)
        s = tr("TX") + ": " + TotalSends();
    if(StringType::Rate & m_StringTypes) {
        if(!s.isEmpty())
            s +=  + "; ";
        s += tr("Tx rate") + ": " + SendRate();
    }
    return s;
}

QString CStats::ReceiveToString()
{
    QString s;
    if(StringType::Counting & m_StringTypes)
        s = tr("RX") + ": " + TotalReceives();
    if(StringType::Rate & m_StringTypes) {
        if(!s.isEmpty())
            s +=  + "; ";
        s += tr("Rx rate") + ": " + ReceiveRate();
    }
    return s;
}

QString CStats::DropToString()
{
    return tr("Drop") + ": " + Drops();
}

QString CStats::TotalSends()
{
    return Convertbytes(GetTotalSends());
}

QString CStats::TotalReceives()
{
    return Convertbytes(GetTotalReceives());
}

QString CStats::Drops()
{
    return Convertbytes(GetDrops());
}

quint64 CStats::GetTotalSends()
{
    return m_TotalSends;
}

quint64 CStats::GetTotalReceives()
{
    return m_TotalReceives;
}

quint64 CStats::GetDrops()
{
    return m_Drops;
}

void CStats::AddSends(quint64 count)
{
    m_TotalSends += count;
}

void CStats::AddReceives(quint64 count)
{
    m_TotalReceives += count;
}

void CStats::AddDrops(quint64 count)
{
    m_Drops += count;
}

QString CStats::SendRate()
{
    return Convertbytes(GetSendRate()) + "/" + tr("S");
}

QString CStats::ReceiveRate()
{
    return Convertbytes(GetReceiveRate()) + "/" + tr("S");
}

double CStats::GetSendRate()
{
    return m_dbSendRate;
}

double CStats::GetReceiveRate()
{
    return m_dbReceiveRate;
}

int CStats::GetInterval()
{
    return m_tmInterval;
}

int CStats::SetInterval(int interval)
{
    if(m_tmInterval == interval)
        return m_tmInterval;
    int old = interval;
    m_tmInterval = interval;
    if(m_tmInterval > 0)
        m_Timer.start(m_tmInterval * 1000);
    else
        m_Timer.stop();
    return old;
}

void CStats::slotCalculating()
{
    if(m_lastTime.secsTo(QDateTime::currentDateTime()) < GetInterval())
        return;

    m_dbSendRate = (double)(m_TotalSends - m_lastSends) / (double)GetInterval();
    m_lastSends = m_TotalSends;
    m_dbReceiveRate = (double)(m_TotalReceives - m_lastReceives) / (double)GetInterval();
    m_lastReceives = m_TotalReceives;

    emit sigCalculationComplete();
}
