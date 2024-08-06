#ifndef STRATACHARTWINDOW_P_H
#define STRATACHARTWINDOW_P_H

#include "randomscatterwidget.h"
#include "stratachartwindow.h"

/*!
 * \brief Приватный класс главного окна приложения
 * \ingroup private
 */
class StrataChartWindowPrivate {
    Q_DECLARE_PUBLIC(StrataChartWindow)
    StrataChartWindow* const q_ptr;

public:
    /*!
     * \brief StrataChartWindowPrivate
     */
    StrataChartWindowPrivate(StrataChartWindow* q);

    /*!
     * \brief Создание действий
     */
    void createActions();

    /*!
     * \brief Создание виджетов
     */
    void createWidgets();

    /*!
     * \brief Создание соединений
     */
    void     createConnections();

    QAction* actPlay  = nullptr; ///< Действие запуска
    QAction* actPause = nullptr; ///< Действие паузы
    QAction* actStop  = nullptr; ///< Действие остановки

    RandomScatterWidget* chart = nullptr;
};

#endif // STRATACHARTWINDOW_P_H
