#ifndef POINTSPROVIDER_P_H
#define POINTSPROVIDER_P_H

#include "pointsprovider.h"

class RandomScatterWidgetPrivate;

/*!
 * \brief Приватный класс поставщика данных
 * \ingroup private
 */
class PointsProviderPrivate {
    Q_DECLARE_PUBLIC(PointsProvider)
    PointsProvider* const q_ptr;

public:
    PointsProviderPrivate(PointsProvider* q);

public:
    /*!
     * \brief Дополняет буфер новыми данными
     */
    void next();

public:
    mutable QRecursiveMutex mutex;

    QVector<QPointF>        buffer; ///< Внутренний буфер
    int timerId = 0; ///< ID таймера для периодической генерации данных во время
                     ///< timerEvent
};

#endif // POINTSPROVIDER_H
