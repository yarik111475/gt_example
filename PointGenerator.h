#ifndef POINTGENERATOR_H
#define POINTGENERATOR_H

#include <QPoint>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QRandomGenerator>

enum class GeneratorStatus{
    STOPPED,
    PAUSED,
    RUNNING
};

class PointGenerator : public QObject
{
    Q_OBJECT
private:
    bool m_stopFlag;
    bool m_pauseFlag;
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    void delay(int interval);
    const int delayInterval{200};
public:
    explicit PointGenerator(QObject *parent = nullptr);

public slots:
    void slotStart();
    void slotPause();
    void slotQuit();

signals:
    void signalStatusChanged(GeneratorStatus status);
    void signalPointGenerated(const QPointF& point);
};

#endif // POINTGENERATOR_H
