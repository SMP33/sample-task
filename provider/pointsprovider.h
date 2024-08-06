#ifndef POINTSPROVIDER_H
#define POINTSPROVIDER_H

#include <QMutex>
#include <QObject>
#include <QtGlobal>

class PointsProviderPrivate;

/*!
 * \brief Класс поставщика данных
 */
class PointsProvider : public QObject {
    Q_OBJECT
public:
    /*!
     * \brief PointsProvider
     */
    explicit PointsProvider(QObject* parent = nullptr);

    /*!
     * \brief ~PointsProvider
     */
    virtual ~PointsProvider();

    /*!
     * \brief Выполняет перемещение накопленных данных из внутреннего буфера в
     * буфер, переданный по ссылке. Буфер при этом освобождается
     */
    void grabPoints(QVector<QPointF>& buffer);

public slots:
    /*!
     * \brief Запускает генерацию чисел
     */
    void start();

    /*!
     * \brief Останавливает генерацию чисел
     */
    void stop();

signals:
    /*!
     * \brief Вызывается при обновлении данных
     */
    void dataGenerated(QPrivateSignal);

protected:
    /*!
     * \brief Метод для заполнения вектора данных
     * \details Должен быть переопределен в наследнике
     */
    virtual QVector<QPointF> generate();

    /*!
     * \brief timerEvent
     * \param event
     */
    virtual void timerEvent(QTimerEvent* event) override;

private:
    Q_DECLARE_PRIVATE(PointsProvider)
    QScopedPointer<PointsProviderPrivate> d_ptr;
};

#endif // POINTSPROVIDER_H
