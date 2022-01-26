#include "MainWindow.h"

#include <QApplication>

#include "PointGenerator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PointGenerator g;
    //g.slotStart();
    MainWindow w;
    w.show();
    return a.exec();
}
