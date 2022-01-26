#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "PointGenerator.h"

class QChart;
class QThread;
class PointGenerator;
class QScatterSeries;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QChart* m_pChart;
    QThread* m_pThread;
    QScatterSeries* m_pSeries;
    PointGenerator* m_pPointGenerator;

    QPushButton* m_pStartPushButton;
    QPushButton* m_pPausePushButton;
    QPushButton* m_pStopPushButton;

    QChart* createChart();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void slotStatusChanged(GeneratorStatus status);
    void slotPointGenerated(const QPointF& point);
};
#endif // MAINWINDOW_H
