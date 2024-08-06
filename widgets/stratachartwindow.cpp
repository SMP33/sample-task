#include "stratachartwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>

#include "stratachartwindow_p.h"

StrataChartWindow::StrataChartWindow(QWidget* parent)
    : QMainWindow(parent)
    , d_ptr(new StrataChartWindowPrivate(this))
{
    Q_D(StrataChartWindow);

    d->createActions();
    d->createWidgets();
    d->createConnections();
}

StrataChartWindow::~StrataChartWindow()
{
}

QAction* StrataChartWindow::actStop() const
{
    Q_D(const StrataChartWindow);
    return d->actStop;
}

QList<QAction*> StrataChartWindow::controlActions() const
{
    return {actPlay(), actPause(), actStop()};
}

QAction* StrataChartWindow::actPause() const
{
    Q_D(const StrataChartWindow);
    return d->actPause;
}

QAction* StrataChartWindow::actPlay() const
{
    Q_D(const StrataChartWindow);
    return d->actPlay;
}

void StrataChartWindow::showEvent(QShowEvent* event)
{
    // Установка размера и положения окна в зависимости от размера экрана

    QRect screen = QApplication::desktop()->screenGeometry();
    int   hs     = screen.height();
    int   ws     = screen.width();
    int   h      = hs / 2;
    int   w      = ws / 2;

    setGeometry(ws / 4, hs / 5, w, h);
}

void StrataChartWindow::closeEvent(QCloseEvent* event)
{
    // Завершение работы при закрытии приложения
    actStop()->trigger();
    QMainWindow::closeEvent(event);
}

void StrataChartWindow::onControlActionTriggered()
{
    Q_D(StrataChartWindow);

    // Установка состояния в завимости от действия
    QAction* act   = static_cast<QAction*>(sender());
    auto     state = act->data().value<RandomScatterWidget::State>();

    if (act == actStop()) {
        actPlay()->setChecked(false);
        actPause()->setChecked(false);
    }

    d->chart->setState(state);
}

StrataChartWindowPrivate::StrataChartWindowPrivate(StrataChartWindow* q)
    : q_ptr(q)
{
}

void StrataChartWindowPrivate::createWidgets()
{
    Q_Q(StrataChartWindow);

    /*
     * Создание тулбара для кнопок управления
     */

    // FIXME: Magic number - необходимо сделать размер иконок настраиваемым,
    // либо перенести определение размера в стиль/stylesheet
    const QSize toolbarIconSize = {32, 32};

    QToolBar*   buttonToolBar = new QToolBar(q);
    buttonToolBar->setIconSize(toolbarIconSize);
    buttonToolBar->setFloatable(false);
    buttonToolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    buttonToolBar->setMovable(false);
    q->addToolBar(Qt::BottomToolBarArea, buttonToolBar);

    // Спейсеры для центровки кнопок
    QWidget* spacerLeft  = new QWidget(buttonToolBar);
    QWidget* spacerRight = new QWidget(buttonToolBar);

    for (auto* s: {spacerLeft, spacerRight}) {
        s->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    buttonToolBar->addWidget(spacerLeft);

    for (auto* act: q->controlActions()) {
        // Добавление тултипа с сочетанием клавиш
        act->setToolTip(QString("%1 <b>%2</b>")
                            .arg(act->text(), act->shortcut().toString()));

        // Создание кнопки
        QToolButton* btn = new QToolButton(buttonToolBar);
        btn->setDefaultAction(act);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        buttonToolBar->addWidget(btn);
    }

    buttonToolBar->addWidget(spacerRight);

    /*
     * Создание меню
     */

    // Меню управления - дублирует тулбар
    QMenu* controlMenu
        = q->menuBar()->addMenu(StrataChartWindow::tr("Control"));
    controlMenu->addActions(q->controlActions());

    /*
     * Создание чарта
     */

    chart = new RandomScatterWidget(q);
    q->setCentralWidget(chart);
}

void StrataChartWindowPrivate::createConnections()
{
    Q_Q(StrataChartWindow);

    // Все действия вызывают срабатывание onControlActionTriggered
    for (auto* act: q->controlActions()) {
        QObject::connect(act,
                         &QAction::triggered,
                         q,
                         &StrataChartWindow::onControlActionTriggered);
    }
}

void StrataChartWindowPrivate::createActions()
{
    Q_Q(StrataChartWindow);

    // Настройка действий и установка состояний, которые им соответствуют

    actPlay = new QAction(
        QIcon(":/icons/icon-play.png"), StrataChartWindow::tr("Start"), q);
    actPlay->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    actPlay->setData(RandomScatterWidget::S_RUNNING);

    actPause = new QAction(
        QIcon(":/icons/icon-pause.png"), StrataChartWindow::tr("Pause"), q);
    actPause->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    actPause->setData(RandomScatterWidget::S_PAUSED);

    actStop = new QAction(
        QIcon(":/icons/icon-stop.png"), StrataChartWindow::tr("Stop"), q);
    actStop->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    actStop->setData(RandomScatterWidget::S_IDLE);

    // Создание группы из действий для подстветки текущего состояния. Состояние
    // IDLE не подсвечивается
    QActionGroup* controlGroup = new QActionGroup(q);

    for (auto* act: {q->actPlay(), q->actPause()}) {
        act->setCheckable(true);
        controlGroup->addAction(act);
    }

    // Начальное состояние - IDLE
    q->actStop()->setChecked(true);
}
