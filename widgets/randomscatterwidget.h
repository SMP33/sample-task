#ifndef RANDOMSCATTERWIDGET_H
#define RANDOMSCATTERWIDGET_H

#include <QWidget>
#include <QtGlobal>

class RandomScatterWidgetPrivate;

/*!
 * \brief Класс виджета для отображения данных, генерируемых с помощью
 * PointsProvider
 */
class RandomScatterWidget : public QWidget {
    Q_OBJECT
public:
    /*!
     * \brief RandomScatterWidget
     */
    explicit RandomScatterWidget(QWidget* parent = nullptr);
    /*!
     * \brief ~RandomScatterWidget
     */
    virtual ~RandomScatterWidget();

    /*!
     * \brief Состояние виджета
     */
    enum State {
        S_IDLE = 0, ///< Состояние по умолчанию
        S_RUNNING,  ///< Идет генерация данных
        S_PAUSED    ///< Пауза
    };
    Q_ENUM(State)

    /*!
     * \brief Возвращает текущее состояние
     */
    State state() const;

public slots:
    /*!
     * \brief Устанавливает текущее состояние
     */
    void setState(State newState);

signals:
    /*!
     * \brief Запрос на остановку провайдера
     */
    void stopRequest(QPrivateSignal);

    /*!
     * \brief Запрос на запуск провайдера
     */
    void startRequest(QPrivateSignal);

private slots:
    // Обновляет данные на графике
    void updateChart();

protected:
    Q_DECLARE_PRIVATE(RandomScatterWidget)
    QScopedPointer<RandomScatterWidgetPrivate> d_ptr;
};

#endif // RANDOMSCATTERWIDGET_H
