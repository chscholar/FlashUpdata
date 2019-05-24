#ifndef SINTABWIDGET_H
#define SINTABWIDGET_H
#include <QTabWidget>
#include <QWidget>
#include <QMainWindow>
#include <QTabBar>

class SinTabWindow;
class TabBar;

class GhostWindow : public QWidget
{
	Q_OBJECT

public:
	explicit GhostWindow(TabBar *tabbar, const QPoint& pos);
	const QPoint offset() { return m_offset; }
	const int index() { return m_index; }
	void moveWithOffset(const QPoint&);
	bool dragStarted(const QPoint&);

private:
	QPoint m_offset;
	int m_index;
	QPoint m_originPos;
};


class TabBar : public QTabBar
{
	Q_OBJECT

public:
	TabBar(QWidget *parent = 0);
	~TabBar();
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

protected:
	void createNewWindow(GhostWindow*);
	void moveToWindow(SinTabWindow*, const QPoint&, GhostWindow*);
	void tabRemoved(int index);

private:
	GhostWindow *m_ghost;
};


class TabView : public QTabWidget
{
	Q_OBJECT

public:
	explicit TabView(QWidget *parent = 0);
	int tabAt(const QPoint &pos);
};

class TabbedWindowPrivate : public QObject
{
	Q_OBJECT

public:
	TabbedWindowPrivate(SinTabWindow* q_ptr);

	int addView(QWidget* view, const QString &title);
	int insertView(const QPoint &pos, QWidget *page, const QString &text);
	void removeView(int index);
	void setCurrentView(int index);
	QWidget* currentView();

private:
	SinTabWindow *q_ptr;
	TabView* tabs;
};

class SinTabWindow:public QMainWindow
{
	Q_OBJECT

public:
	explicit SinTabWindow(QWidget *parent = 0);
	~SinTabWindow();

	int addView(QWidget* view, const QString &title);
	int insertView(const QPoint &pos, QWidget *page, const QString &text);
	void removeView(int index);
	void setCurrentView(int index);
	QWidget* currentView();

private:
	TabbedWindowPrivate* d_ptr;
};

#endif
