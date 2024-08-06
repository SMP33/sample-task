#include "pointsprovider.h"

#include <QMutexLocker>
#include <QTimerEvent>

#include "pointsprovider_p.h"

PointsProvider::PointsProvider(QObject* parent)
    : QObject{parent}
    , d_ptr(new PointsProviderPrivate(this))
{
}

PointsProvider::~PointsProvider()
{
    stop();
}

void PointsProvider::grabPoints(QVector<QPointF>& buffer)
{
    Q_D(PointsProvider);
    QMutexLocker locker(&d->mutex);

    buffer = std::move(d->buffer);
}

void PointsProvider::start()
{
    Q_D(PointsProvider);
    QMutexLocker locker(&d->mutex);

    if (d->timerId) {
        return;
    }

    // Например, период получения новых пакетов данных от сервера
    d->timerId = startTimer(500);
}

void PointsProvider::stop()
{
    Q_D(PointsProvider);
    QMutexLocker locker(&d->mutex);

    if (d->timerId) {
        killTimer(d->timerId);
        d->timerId = 0;
    }
}

QVector<QPointF> PointsProvider::generate()
{
    return {};
}

void PointsProvider::timerEvent(QTimerEvent* event)
{
    Q_D(PointsProvider);
    QMutexLocker locker(&d->mutex);

    if (d->timerId == event->timerId()) {
        d->next();
    }

    QObject::timerEvent(event);
}

PointsProviderPrivate::PointsProviderPrivate(PointsProvider* q)
    : q_ptr(q)
{
}

void PointsProviderPrivate::next()
{
    Q_Q(PointsProvider);

    QVector<QPointF> bufferTmp = q->generate();

    // Блокировка на время доступа к внутреннему буферу
    QMutexLocker locker(&mutex);

    // Дозапись новых данных в конец существующих
    buffer.append(bufferTmp);

    emit q->dataGenerated(PointsProvider::QPrivateSignal());
}
