#include "randompointsprovider.h"

#include <QRandomGenerator>

RandomPointsProvider::RandomPointsProvider(QObject* parent)
    : PointsProvider{parent}
{
}

RandomPointsProvider::~RandomPointsProvider()
{
}

QVector<QPointF> RandomPointsProvider::generate()
{
    QVector<QPointF> vec;

    for (int i = 0; i < 10; i++) {
        double x = QRandomGenerator::global()->generateDouble();
        double y = QRandomGenerator::global()->generateDouble();
        vec.push_back({x, y});
    }

    return vec;
}
