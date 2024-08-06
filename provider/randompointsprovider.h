#ifndef RANDOMPOINTSPROVIDER_H
#define RANDOMPOINTSPROVIDER_H

#include "pointsprovider.h"

/*!
 * \brief Класс поставщика данных, генерирующий вейтор случайных точек
 */
class RandomPointsProvider : public PointsProvider {
    Q_OBJECT
public:
    /*!
     * \brief RandomPointsProvider
     */
    explicit RandomPointsProvider(QObject* parent = nullptr);

    /*!
     * \brief ~RandomPointsProvider
     */
    virtual ~RandomPointsProvider();

protected:
    /*!
     * \brief Возвращает вектор случайных точек с координатами в
     * диапазоне [0; 1)
     */
    virtual QVector<QPointF> generate() override;
};

#endif // RANDOMPOINTSPROVIDER_H
