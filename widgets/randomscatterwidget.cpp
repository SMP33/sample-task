#include "randomscatterwidget.h"

#include <QChart>
#include <QChartView>
#include <QHBoxLayout>
#include <QThread>
#include <QValueAxis>

#include "provider/randompointsprovider.h"
#include "randomscatterwidget_p.h"

using namespace QtCharts;

RandomScatterWidget::RandomScatterWidget(QWidget* parent)
    : QWidget(parent)
    , d_ptr(new RandomScatterWidgetPrivate(this))
{
}

RandomScatterWidget::~RandomScatterWidget()
{
    Q_D(RandomScatterWidget);
    setState(S_IDLE);
}

RandomScatterWidget::State RandomScatterWidget::state() const
{
    Q_D(const RandomScatterWidget);

    return d->state;
}

void RandomScatterWidget::setState(State newState)
{
    Q_D(RandomScatterWidget);

    const State s0 = state();
    const State s1 = newState;

    // Состояние изменяется сразу, и при необходимости откатывается
    d->state = s1;

    /*
     * Обработка переходов между состояниями
     */

    // СОСТОЯНИЕ ПОКОЯ - РАБОТА: создание провайдера и запуск потока
    if (s0 == S_IDLE && s1 == S_RUNNING) {
        d->provider = new RandomPointsProvider();
        d->provider->moveToThread(d->providerThread);

        // График будет обновляться при обновлении данных поставщика
        connect(d->provider,
                &PointsProvider::dataGenerated,
                this,
                &RandomScatterWidget::updateChart,
                Qt::QueuedConnection);

        // Поставщик запустит обновление данных при запуске потока
        connect(d->providerThread,
                &QThread::started,
                d->provider,
                &PointsProvider::start,
                Qt::QueuedConnection);

        // Поставщик запустит обновление данных по сигналу от виджета
        connect(this,
                &RandomScatterWidget::startRequest,
                d->provider,
                &PointsProvider::start,
                Qt::QueuedConnection);

        // Поставщик остановит обновление данных по сигналу от виджета
        connect(this,
                &RandomScatterWidget::stopRequest,
                d->provider,
                &PointsProvider::stop,
                Qt::QueuedConnection);

        // Поток остановится при удалении поставщика
        connect(d->provider,
                &QObject::destroyed,
                d->providerThread,
                &QThread::quit,
                Qt::QueuedConnection);

        d->providerThread->start();
    }
    // РАБОТА - СОСТОЯНИЕ ПОКОЯ: отправка сигнала на удаление провайдера
    else if (s0 == S_RUNNING && s1 == S_IDLE) {
        d->removeProvider();
    }
    // РАБОТА - ПАУЗА: приостановка потока
    else if (s0 == S_RUNNING && s1 == S_PAUSED) {
        emit stopRequest(QPrivateSignal());
    }
    // ПАУЗА - РАБОТА: запуск потока
    else if (s0 == S_PAUSED && s1 == S_RUNNING) {
        emit startRequest(QPrivateSignal());
    }
    // ПАУЗА - СОСТОЯНИЕ ПОКОЯ: отправка сигнала на удаление провайдера и запуск
    // потока
    else if (s0 == S_PAUSED && s1 == S_IDLE) {
        d->removeProvider();
        d->providerThread->start();
    }
    // Остальные переходы невалидны
    else {
        // Откат к предыдущему состоянию
        d->state = s0;
        return;
    }
}

void RandomScatterWidget::updateChart()
{
    Q_D(RandomScatterWidget);

    // Возможно чтение данных для последнего пакета во время паузы, поэтому
    // отрисовка не производится только во время IDLE
    if (state() != S_IDLE) {
        QVector<QPointF> points;
        d->provider->grabPoints(points);

        d->seriesPrev->replace(d->seriesCurrent->points());
        d->seriesCurrent->replace(std::move(points));
    }
}

RandomScatterWidgetPrivate::RandomScatterWidgetPrivate(RandomScatterWidget* q)
    : q_ptr(q)
{
    // Объект потока существует все время работы только
    // запускается/останавливается, когда нужно
    providerThread = new QThread(q);

    // Создание чарта

    QChartView* view = new QChartView(q);
    view->setChart(new QChart());
    view->chart()->legend()->setVisible(false);
    seriesCurrent = new QScatterSeries(view->chart());
    seriesPrev    = new QScatterSeries(view->chart());

    view->chart()->addSeries(seriesCurrent);
    view->chart()->addSeries(seriesPrev);

    seriesPrev->setColor(seriesCurrent->color());
    seriesPrev->setOpacity(0.5);

    QValueAxis* axisX = new QValueAxis();
    axisX->setRange(0, 1);
    view->chart()->addAxis(axisX, Qt::AlignBottom);

    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 1);
    view->chart()->addAxis(axisY, Qt::AlignLeft);

    seriesCurrent->attachAxis(axisX);
    seriesCurrent->attachAxis(axisY);
    seriesPrev->attachAxis(axisX);
    seriesPrev->attachAxis(axisY);

    auto* l = new QVBoxLayout(q);
    l->addWidget(view);
}

void RandomScatterWidgetPrivate::removeProvider()
{
    provider->deleteLater();
    provider = nullptr;

    seriesCurrent->clear();
    seriesPrev->clear();
}
