#include "circularplot.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CircularPlot w;
    w.show();
    return a.exec();
}
