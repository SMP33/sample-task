#ifndef STRATACHARTWINDOW_H
#define STRATACHARTWINDOW_H

#include <QMainWindow>
#include <QtGlobal>

class StrataChartWindowPrivate;

/*!
 * \brief Главное окно приложения и панель управления для виджета
 * RandomScatterWidget
 */
class StrataChartWindow : public QMainWindow {
    Q_OBJECT
public:
    /*!
     * \brief StrataChartWindow
     */
    StrataChartWindow(QWidget* parent = nullptr);
    /*!
     * \brief ~StrataChartWindow
     */
    virtual ~StrataChartWindow();

    /*!
     * \brief Действие для запуска потока
     */
    QAction* actPlay() const;

    /*!
     * \brief Действие для приостановки потока
     */
    QAction* actPause() const;

    /*!
     * \brief Действие для остановки потока
     */
    QAction* actStop() const;

    /*!
     * \brief Возвращает список действий, управляющих воспроизведением
     */
    QList<QAction*> controlActions() const;

protected:
    /*!
     * \brief showEvent
     */
    virtual void showEvent(QShowEvent* event) override;

    /*!
     * \brief closeEvent
     */
    virtual void closeEvent(QCloseEvent* event) override;

private slots:
    void onControlActionTriggered();

protected:
    Q_DECLARE_PRIVATE(StrataChartWindow)
    QScopedPointer<StrataChartWindowPrivate> d_ptr;
};
#endif // STRATACHARTWINDOW_H
