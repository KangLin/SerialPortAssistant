// Author: Kang Lin <kl222@126.com>

#pragma once

#include <QAtomicInteger>
#include <QAtomicInt>
#include <QDateTime>
#include <QTimer>

/*!
 * \brief The CStats class
 * \note Not thread-safe
 */
class CStats : public QObject
{
    Q_OBJECT

public:
    explicit CStats(QObject* parent = nullptr);
    virtual ~CStats();

    [[nodiscard]] static QString Convertbytes(quint64 bytes);

    virtual void AddSends(quint64 count);
    virtual void AddReceives(quint64 count);
    virtual void AddDrops(quint64 count);
    int Reset();

    enum StringType {
        Counting = 0x01,
        Rate = 0x02,
        All = Counting | Rate
    };
    Q_DECLARE_FLAGS(StringTypes, StringType)
    Q_FLAG(StringTypes)
    void SetStringType(StringTypes types);
    [[nodiscard]] QString ToString();
    [[nodiscard]] QString SendToString();
    [[nodiscard]] QString ReceiveToString();
    [[nodiscard]] QString DropToString();
    [[nodiscard]] QString TotalSends();
    [[nodiscard]] QString TotalReceives();
    [[nodiscard]] QString Drops();
    [[nodiscard]] QString SendRate();
    [[nodiscard]] QString ReceiveRate();

    //! Total send counts. Unit: byte
    [[nodiscard]] quint64 GetTotalSends();
    //! Total receive counts. Unit: byte
    [[nodiscard]] quint64 GetTotalReceives();
    //! Total drop counts. Unit: byte
    [[nodiscard]] quint64 GetDrops();

    /*!
     * \brief Send rate. The average rate over the interval period. Unit: Byte/Seconds
     */
    [[nodiscard]] virtual double GetSendRate();
    /*!
     * \brief Receive rate. The average rate over the interval period. Unit: Byte/Seconds
     */
    [[nodiscard]] virtual double GetReceiveRate();

    /*!
     * \brief Get interval. Unit: seconds
     */
    [[nodiscard]] int GetInterval();
    /*!
     * \brief Set interval and start the timer. Unit: seconds
     * \param interval: > 0: start timer. Unit: seconds.
     *                  <= 0: stop timer
     */
    int SetInterval(int interval = 1);

public Q_SLOTS:
    virtual void slotCalculating();
Q_SIGNALS:
    void sigCalculationComplete();

private:
    quint64 m_TotalSends;
    quint64 m_TotalReceives;
    quint64 m_lastSends;
    quint64 m_lastReceives;
    quint64 m_Drops;

    QDateTime m_lastTime;
    double m_dbSendRate;
    double m_dbReceiveRate;

    int m_tmInterval;
    QTimer m_Timer;
    
    StringTypes m_StringTypes;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CStats::StringTypes)
