#ifndef SINTABWINDOW_H
#define  SINTABWINDOW_H

#pragma once
#include <QMainWindow>
#include <QSignalMapper>
#include <QSplitter>
#include <QTabBar>


class SinTabFrameWork;
class SinTabContainer;

class SinMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  SinMainWindow(SinTabFrameWork* tabFramework);

protected:
  virtual void closeEvent(QCloseEvent* event);

private:
  enum LayoutType
  {
    NoneType,
    WindowType,
    SplitterType,
    ContainerType,
    TabType,
  };

private:
  SinTabFrameWork* tabFramework;
  QWidget* overlayWidget;
  QWidget* overlayWidgetTab;
  QWidget* focusTab;

private slots:
  void hideCurrent();

private:
  void setDropOverlayRect(const QRect& globalRect, const QRect& tabRect = QRect());
  void writeLayout(QDataStream& stream);
  void readLayout(QDataStream& stream);
  void hideAllTabs();
  int tabCount();

  friend class SinTabContainer;
  friend class SinTabBar;
  friend class SinTabFrameWork;
  friend class SinTabSplitter;
};

class SinTabFrameWork : public SinMainWindow
{
  Q_OBJECT

public:
  enum InsertPolicy
  {
    InsertFloating,
    InsertOnTop,
    Insert,
    InsertLeft,
    InsertRight,
    InsertTop,
    InsertBottom
  };

public:
  SinTabFrameWork();
  ~SinTabFrameWork();

  void addTab(QWidget* widget, InsertPolicy insertPolicy = InsertFloating, QWidget* position = 0);
  void moveTab(QWidget* widget, InsertPolicy insertPolicy = InsertFloating, QWidget* position = 0);
  void removeTab(QWidget* widget);
  void hideTab(QWidget* widget);
  bool isVisible(QWidget* widget) const;

  QAction* toggleViewAction(QWidget* widget);

  void restoreLayout(const QByteArray& layout);
  QByteArray saveLayout();

protected:
  virtual void closeEvent(QCloseEvent* event);
  virtual void showEvent(QShowEvent* event);
  virtual bool eventFilter(QObject* obj, QEvent* event);
  virtual bool event(QEvent* event);

private:
  struct TabData
  {
    QWidget* widget;
    QString objectName;
    bool hidden;
    QAction* action;
  };

private:
  QList<SinMainWindow*> floatingWindows;
  QList<SinMainWindow*> floatingWindowsZOrder;
  QList<SinMainWindow*> floatingWindowsZOrderToRestore;
  QHash<QWidget*, TabData> tabs;
  QHash<QString, TabData*> tabsByName;
  QSignalMapper signalMapper;

  QWidget* moveTabWidget;
  SinTabContainer* moveTabPosition;
  InsertPolicy moveTabInsertPolicy;
  int moveTabTabIndex;

private slots:
  void executeMoveTab();
  void toggleVisibility(QWidget* widget);
  void showFloatingWindows();
  void handleFocusChanged(QWidget* old, QWidget* now);

private:
  void addTab(QWidget* widget, SinTabContainer* position, InsertPolicy insertPolicy, int tabIndex);
  void moveTab(QWidget* widget, SinTabContainer* position, InsertPolicy insertPolicy, int tabIndex);
  void moveTabLater(QWidget* widget, SinTabContainer* position, InsertPolicy insertPolicy, int tabIndex);
  void removeContainerIfEmpty(SinTabContainer* tabContainer);
  SinMainWindow* createWindow();
  void removeWindow(SinMainWindow* window);
  void hideTab(QWidget* widget, bool removeContainerIfEmpty);
  QString tabObjectName(QWidget* widget);
  void unhideTab(const QString& objectName, SinTabContainer* position);

  friend class SinTabBar;
  friend class SinTabContainer;
  friend class SinMainWindow;
};

class SinTabSplitter : public QSplitter
{
public:
  SinTabSplitter(Qt::Orientation orientation, QWidget* parent, SinMainWindow* tabWindow);

private:
  SinMainWindow* tabWindow;

private:
  void writeLayout(QDataStream& stream);
  void readLayout(QDataStream& stream);

  friend class SinMainWindow;
};

class SinTabContainer : public QTabWidget
{
  Q_OBJECT

public:
  SinTabContainer(QWidget* parent, SinMainWindow* tabWindow);

  int addTab(QWidget* widget, const QString& label);
  int insertTab(int index, QWidget* widget, const QString& label);
  void removeTab(int index);

protected:
  virtual void dragEnterEvent(QDragEnterEvent* event);
  virtual void dragLeaveEvent(QDragLeaveEvent* event);
  virtual void dragMoveEvent(QDragMoveEvent* event);
  virtual void dropEvent(QDropEvent* event);

private:
  SinMainWindow* tabWindow;

private slots:
  void handleCurrentChanged(int index);

private:
  QRect findDropRect(const QPoint& globalPos, int tabWidth, SinTabFrameWork::InsertPolicy& insertPolicy, QRect& tabRect, int& tabIndex);

  void writeLayout(QDataStream& stream);
  void readLayout(QDataStream& stream);

  friend class SinTabBar;
  friend class SinTabFrameWork;
  friend class SinMainWindow;
  friend class SinTabSplitter;
};

class SinTabBar : public QTabBar
{
  Q_OBJECT

public:
  SinTabBar(SinTabContainer* tabContainer);

protected:
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);

private:
  SinTabContainer* tabContainer;
  int pressedIndex;
  QPoint dragStartPosition;

private slots:
  void closeTab(int index);
};

#endif