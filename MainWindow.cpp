#include "MainWindow.h"
#include <QtWidgets>
#include <QThread>
#include <QtCharts/QChart>
#include <QValueAxis>
#include <QLineSeries>
#include <QScatterSeries>
#include <QChartView>
#include "PointGenerator.h"


QChart *MainWindow::createChart()
{
    QValueAxis* pXAxis=new QValueAxis;
    pXAxis->setRange(0,50);
    pXAxis->setGridLineVisible(false);
    pXAxis->applyNiceNumbers();

    QValueAxis* pYAxis=new QValueAxis;
    pYAxis->setRange(0,50);
    pYAxis->setGridLineVisible(false);
    pYAxis->applyNiceNumbers();

    QChart* pChart=new QChart;
    pChart->addAxis(pYAxis, Qt::AlignLeft);
    pChart->addAxis(pXAxis, Qt::AlignBottom);
    pChart->legend()->hide();


    m_pSeries = new QScatterSeries();
    m_pSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_pSeries->setMarkerSize(20);
    pChart->addSeries(m_pSeries);
    m_pSeries->attachAxis(pXAxis);
    m_pSeries->attachAxis(pYAxis);
    return pChart;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_pThread{nullptr}, m_pPointGenerator{nullptr}
{


    m_pStartPushButton=new QPushButton(QObject::tr("Start"));
    QObject::connect(m_pStartPushButton, &QPushButton::clicked,
                     this, [&]()
    {
        if(m_pPointGenerator){
            m_pPointGenerator->slotQuit();
            delete m_pPointGenerator;
        }
        m_pPointGenerator=new PointGenerator();
        QObject::connect(m_pPointGenerator, &PointGenerator::signalPointGenerated,
                         this, &MainWindow::slotPointGenerated);
        QObject::connect(m_pPointGenerator, &PointGenerator::signalStatusChanged,
                         this, &MainWindow::slotStatusChanged);
        m_pThread=new QThread;
        m_pPointGenerator->moveToThread(m_pThread);
        QObject::connect(m_pThread, &QThread::started, m_pPointGenerator, &PointGenerator::slotStart);
        QObject::connect(m_pThread, &QThread::finished, m_pPointGenerator, &PointGenerator::deleteLater);
        m_pThread->start();
    });
    m_pPausePushButton=new QPushButton(QObject::tr("Pause"));
    QObject::connect(m_pPausePushButton, &QPushButton::clicked,
                     this, [&]()
    {
        if(m_pPointGenerator){
            m_pPointGenerator->slotPause();
        }
    });
    m_pStopPushButton=new QPushButton(QObject::tr("Stop"));
    QObject::connect(m_pStopPushButton, &QPushButton::clicked,
                     this, [&]()
    {
        if(m_pPointGenerator){
            m_pPointGenerator->slotQuit();
            m_pSeries->clear();
        }
    });

    QHBoxLayout* pHBoxLayout=new QHBoxLayout;
    pHBoxLayout->addStretch(10);
    pHBoxLayout->addWidget(m_pStartPushButton);
    pHBoxLayout->addWidget(m_pPausePushButton);
    pHBoxLayout->addWidget(m_pStopPushButton);

    QChartView* pView=new QChartView(this);
    pView->setChart(createChart());

    QVBoxLayout* pVBoxLayout=new QVBoxLayout;
    pVBoxLayout->addWidget(pView);
    pVBoxLayout->addLayout(pHBoxLayout);

    QWidget* pCentralWidget=new QWidget;
    pCentralWidget->setLayout(pVBoxLayout);
    setCentralWidget(pCentralWidget);
    resize(800,600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::slotStatusChanged(GeneratorStatus status)
{
    switch(status){
    case GeneratorStatus::STOPPED:
        m_pStartPushButton->setEnabled(true);
        m_pStopPushButton->setEnabled(false);
        m_pPausePushButton->setEnabled(false);
        break;
    case GeneratorStatus::PAUSED:
        m_pStartPushButton->setEnabled(false);
        m_pStopPushButton->setEnabled(false);
        m_pPausePushButton->setEnabled(true);
        break;
    case GeneratorStatus::RUNNING:
        m_pStartPushButton->setEnabled(false);
        m_pStopPushButton->setEnabled(true);
        m_pPausePushButton->setEnabled(true);
        break;
    }
}

void MainWindow::slotPointGenerated(const QPointF &point)
{
    m_pSeries->append(point);
}

