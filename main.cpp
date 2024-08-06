#include <QApplication>

#include "widgets/stratachartwindow.h"

int main(int argc, char* argv[])
{
    QApplication      a(argc, argv);

    StrataChartWindow w;
    w.show();
    return a.exec();
}
