#include "SinTabWindow.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QMouseEvent>
#include <QMenuBar>

GhostWindow::GhostWindow(TabBar *tabbar, const QPoint& pos) :
QWidget()
{
	QWidget *wnd = tabbar->window();
	QPalette palette;

#if QT_VERSION >= 0x050000
	palette.setBrush(this->backgroundRole(), QBrush(wnd->grab()));
#else
	palette.setBrush(this->backgroundRole(), QBrush(QPixmap::grabWidget(wnd)));
#endif

	this->setPalette(palette);
	this->setGeometry(wnd->geometry());
	this->setWindowOpacity(0.5);
	this->setAttribute(Qt::WA_TransparentForMouseEvents);

	m_offset = tabbar->mapToGlobal(pos) - wnd->pos();
	m_index = tabbar->tabAt(pos);
	m_originPos = tabbar->mapToGlobal(pos);
}


void GhostWindow::moveWithOffset(const QPoint& pos)
{
	QWidget::move(pos - m_offset);
}


bool GhostWindow::dragStarted(const QPoint &pos)
{
	return (pos - m_originPos).manhattanLength()
		>= QApplication::startDragDistance();
}


TabBar::TabBar(QWidget *parent) :
QTabBar(parent),
m_ghost(NULL)
{
}


TabBar::~TabBar()
{
	delete m_ghost;
}


void TabBar::mouseMoveEvent(QMouseEvent *event)
{
	if (m_ghost) {
		m_ghost->moveWithOffset(event->globalPos());
	}
}


void TabBar::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && tabAt(event->pos()) > -1) {
		m_ghost = new GhostWindow(this, event->pos());
		m_ghost->show();
	}
	QTabBar::mousePressEvent(event);
}


void TabBar::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() != Qt::LeftButton) {
		QTabBar::mouseReleaseEvent(event);
		return;
	}
	if (m_ghost->dragStarted(event->globalPos())) {
		TabBar *w = dynamic_cast<TabBar*>(
			QApplication::widgetAt(event->globalPos()));

		if (w == NULL) {
			if (count() == 1) {
				window()->move(m_ghost->pos());
			}
			else {
				createNewWindow(m_ghost);
			}
		}
		else {
			SinTabWindow *wnd = dynamic_cast<SinTabWindow*>(w->window());

			if (wnd != NULL) {
				moveToWindow(wnd, event->globalPos(), m_ghost);
			}
		}
	}
	m_ghost->close();
}


void TabBar::moveToWindow(SinTabWindow *wnd, const QPoint &pos,
	GhostWindow *ghost)
{
	TabView *view = static_cast<TabView*>(parent());
	int index = ghost->index();
	QString text = tabText(index);
	QWidget *page = view->widget(index);

	view->removeTab(index);
	index = wnd->insertView(pos, page, text);
	wnd->setCurrentView(index);
	wnd->raise();
}


void TabBar::tabRemoved(int index)
{
	if (count() == 0) {
		window()->close();
	}
}


void TabBar::createNewWindow(GhostWindow *ghost)
{
	SinTabWindow *wnd = new SinTabWindow();

	wnd->setGeometry(ghost->geometry());
	TabView *view = static_cast<TabView*>(parent());
	int index = ghost->index();
	QWidget *tab = view->widget(index);
	QString text = view->tabText(index);

	view->removeTab(index);
	wnd->addView(tab, text);
	wnd->show();
}


TabView::TabView(QWidget *parent) :
QTabWidget(parent)
{
	setTabBar(new TabBar);
}


int TabView::tabAt(const QPoint &pos)
{
	return tabBar()->tabAt(pos);
}

SinTabWindow::SinTabWindow(QWidget *parent) :
QMainWindow(parent)
{
	tabs = new TabView();
	tabs->setDocumentMode(true);
	this->setCentralWidget(tabs);
}


SinTabWindow::~SinTabWindow()
{
	delete tabs;
}


QWidget* SinTabWindow::currentView()
{
	return tabs->currentWidget();
}

void SinTabWindow::setCurrentView(int index)
{
	tabs->setCurrentIndex(index);
}


int SinTabWindow::insertView(const QPoint &pos, QWidget *page,
	const QString &text)
{
	int index = tabs->tabAt(tabs->mapFromGlobal(pos));
	return tabs->insertTab(index, page, text);
}


int SinTabWindow::addView(QWidget *view, const QString &title)
{
	return tabs->addTab(view, title);
}


void SinTabWindow::removeView(int index)
{
	tabs->removeTab(index);
}