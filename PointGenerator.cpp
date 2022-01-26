#include "PointGenerator.h"
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

void PointGenerator::delay(int interval)
{
    QEventLoop loop;
    QTimer::singleShot(interval, &loop, &QEventLoop::quit);
    loop.exec();
}

PointGenerator::PointGenerator(QObject *parent)
    : QObject{parent}, m_stopFlag{true}, m_pauseFlag{false}
{

}

void PointGenerator::slotStart()
{
    emit signalStatusChanged(GeneratorStatus::RUNNING);
    while(m_stopFlag){

        m_mutex.lock();
        if(m_pauseFlag){
            emit signalStatusChanged(GeneratorStatus::PAUSED);
            m_waitCondition.wait(&m_mutex);
        }
        m_mutex.unlock();


        int x=QRandomGenerator::global()->bounded(0, 50);
        int y=QRandomGenerator::global()->bounded(0, 50);

        QPointF point(x,y);
        emit signalPointGenerated(point);
        delay(delayInterval);
    }
    emit signalStatusChanged(GeneratorStatus::STOPPED);
}

void PointGenerator::slotPause()
{
    m_mutex.lock();
    m_pauseFlag=!m_pauseFlag;
    if(!m_pauseFlag){
        m_waitCondition.wakeAll();
        emit signalStatusChanged(GeneratorStatus::RUNNING);
    }
    m_mutex.unlock();
}

void PointGenerator::slotQuit()
{
    m_stopFlag=false;
}
