#ifndef RANDOMSCATTERWIDGET_P_H
#define RANDOMSCATTERWIDGET_P_H

#include <QScatterSeries>

#include "provider/pointsprovider.h"
#include "randomscatterwidget.h"

/*!
 * \brief Приватный класс виджета для отображения данных
 * \ingroup private
 */
class RandomScatterWidgetPrivate {
    Q_DECLARE_PUBLIC(RandomScatterWidget)
    RandomScatterWidget* const q_ptr;

public:
    RandomScatterWidgetPrivate(RandomScatterWidget* q);

    /*!
     * \brief Удаляет текущий провайдер и останавливает процесс генерации
     */
    void removeProvider();

public:
    QtCharts::QScatterSeries* seriesCurrent
        = nullptr; ///< Series для отображения текущего набора точек
    QtCharts::QScatterSeries* seriesPrev
        = nullptr; ///< Series для отображения предыдущего набора точек

    RandomScatterWidget::State state
        = RandomScatterWidget::S_IDLE; ///< Текущее состояние

    PointsProvider* provider = nullptr; ///< Текущий поставщик данных
    QThread* providerThread = nullptr; ///< Поток, в котором работает поставщик
};

#endif // RANDOMSCATTERWIDGET_P_H
