#include "SinTabWindow.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QAction>
#include <QApplication>
#include <QMimeData>
#include <QLinkedList>
#include <QDrag>
#include <QTimer>
#include <QShortcut>

class QTabDropOverlay : public QWidget
{
public:
  QTabDropOverlay(QWidget* parent) : QWidget(parent)
  {
    setPalette(Qt::transparent);
    setAttribute(Qt::WA_TransparentForMouseEvents);
  }

private:
  void paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(88, 88, 88, 88)));
    painter.drawRect(rect());
  }
};

SinTabBar::SinTabBar(SinTabContainer* tabContainer) : QTabBar(tabContainer), tabContainer(tabContainer), pressedIndex(-1)
{
  //setMovable(true);
  setTabsClosable(true);
  setUsesScrollButtons(true);
  setElideMode(Qt::ElideRight);
  connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void SinTabBar::mousePressEvent(QMouseEvent* event)
{
  if (event->button() != Qt::LeftButton)
  {
    event->ignore();
    return;
  }

  pressedIndex = tabAt(event->pos());
  if(pressedIndex >= 0)
  {
    dragStartPosition = event->pos();
  }

  QTabBar::mousePressEvent(event);
}

void SinTabBar::mouseReleaseEvent(QMouseEvent* event)
{
  if (event->button() != Qt::LeftButton)
  {
      event->ignore();
      return;
  }

  QTabBar::mouseReleaseEvent(event);
}

void SinTabBar::mouseMoveEvent(QMouseEvent* event)
{
  // Start drag
  if(pressedIndex >= 0)
  {
    if ((event->pos() - dragStartPosition).manhattanLength() > QApplication::startDragDistance())
    {
      QWidget* dragWidget = tabContainer->widget(pressedIndex);

      QMimeData* mimeData = new QMimeData;
      QRect tabRect = this->tabRect(pressedIndex);
      int tabWidth = tabRect.width();

      QByteArray data;
      data.append((const char*)&tabWidth, sizeof(tabWidth));
      mimeData->setData("application/x-tabwidget", QByteArray());

      QDrag* drag = new QDrag(dragWidget);
      drag->setMimeData(mimeData);

      QPixmap pixmap = QPixmap::grabWidget(this, tabRect);
      drag->setPixmap(pixmap);
      drag->setHotSpot(QPoint(event->pos().x() - tabRect.x(), event->pos().y() - tabRect.y()));
      //drag->setHotSpot(QPoint(dragStartPosition.x() - tabRect.x(), dragStartPosition.y() - tabRect.y()));

      Qt::DropAction ret = drag->exec(Qt::MoveAction);
      if(ret != Qt::MoveAction)
        tabContainer->tabWindow->tabFramework->moveTabLater(dragWidget, 0, SinTabFrameWork::InsertFloating, -1);

      pressedIndex = -1;
      return;
    }
  }
  else
    QTabBar::mouseMoveEvent(event);
}

void SinTabBar::closeTab(int index)
{
  QWidget* widget = tabContainer->widget(index);
  tabContainer->tabWindow->tabFramework->hideTab(widget);
}

SinTabSplitter::SinTabSplitter(Qt::Orientation orientation, QWidget* parent, SinMainWindow* tabWindow) : QSplitter(orientation, parent), tabWindow(tabWindow)
{
  setChildrenCollapsible(false);
}

void SinTabSplitter::writeLayout(QDataStream& stream)
{
  stream << saveState();
  stream << (quint32)orientation();
  for(int i = 0, count = this->count(); i < count; ++i)
  {
    QWidget* widget = this->widget(i);
    SinTabSplitter* tabSplitter = dynamic_cast<SinTabSplitter*>(widget);
    if(tabSplitter)
    {
      stream << (quint32)SinTabFrameWork::SplitterType;
      tabSplitter->writeLayout(stream);
    }
    else
    {
      SinTabContainer* tabContainer = dynamic_cast<SinTabContainer*>(widget);
      if(tabContainer)
      {
        stream << (quint32)SinTabFrameWork::ContainerType;
        tabContainer->writeLayout(stream);
      }
    }
  }
  stream << (quint32)SinTabFrameWork::NoneType;
}

void SinTabSplitter::readLayout(QDataStream& stream)
{
  QByteArray state;
  stream >> state;
  quint32 orientation;
  stream >> orientation;
  setOrientation((Qt::Orientation)orientation);
  quint32 type;
  stream >> type;
  while(type != SinTabFrameWork::NoneType)
  {
    switch((SinTabFrameWork::LayoutType)type)
    {
    case SinTabFrameWork::SplitterType:
      {
        SinTabSplitter* tabSplitter = new SinTabSplitter(Qt::Horizontal, this, tabWindow);
        addWidget(tabSplitter);
        tabSplitter->readLayout(stream);
      }
      break;
    case SinTabFrameWork::ContainerType:
      {
        SinTabContainer* tabContainer = new SinTabContainer(this, tabWindow);
        addWidget(tabContainer);
        tabContainer->readLayout(stream);
      }
      break;
    default:
      Q_ASSERT(false);
      break;
    }
    stream >> type;
  }
  restoreState(state);
}

SinTabContainer::SinTabContainer(QWidget* parent, SinMainWindow* tabWindow) : QTabWidget(parent), tabWindow(tabWindow)
{
  setTabBar(new SinTabBar(this));
  //setDocumentMode(true);
  setAcceptDrops(true);
  connect(this, SIGNAL(currentChanged(int)), this, SLOT(handleCurrentChanged(int)));
}

int SinTabContainer::addTab(QWidget* widget, const QString& label)
{
  int index = QTabWidget::addTab(widget, label);
  setTabToolTip(index, label);
  return index;
}

int SinTabContainer::insertTab(int index, QWidget* widget, const QString& label)
{
  index = QTabWidget::insertTab(index, widget, label);
  setTabToolTip(index, label);
  return index;
}

void SinTabContainer::removeTab(int index)
{
  QWidget* widget = this->widget(index);
  QTabWidget::removeTab(index);
  if(widget == tabWindow->focusTab)
    tabWindow->tabFramework->handleFocusChanged(widget, this->widget(this->currentIndex()));
}

void SinTabContainer::handleCurrentChanged(int index)
{
  QWidget* widget = this->widget(index);
  if(widget)
  {
    if(tabWindow->isActiveWindow())
    {
      QWidget* focusWidget = widget->focusWidget();
      if(focusWidget)
        focusWidget->setFocus();
      else
        widget->setFocus();
    }
    else if(tabWindow != tabWindow->tabFramework)
    {
      tabWindow->focusTab = widget;
      tabWindow->setWindowTitle(widget->windowTitle());
    }
  }
}

QRect SinTabContainer::findDropRect(const QPoint& globalPos, int tabWidth, SinTabFrameWork::InsertPolicy& insertPolicy, QRect& tabRectResult, int& tabIndex)
{
  QPoint pos = mapFromGlobal(globalPos);
  QRect containerRect = rect();
  QRect result;
  tabIndex = -1;
  if(containerRect.contains(pos))
  {
    if(count() == 0)
    {
      insertPolicy = SinTabFrameWork::InsertOnTop;
      result = containerRect;
    }
    else if(tabBar()->geometry().contains(pos))
    {
      insertPolicy = SinTabFrameWork::Insert;
      result = containerRect;
      QTabBar* tabBar = this->tabBar();
      for(int i = 0, count = tabBar->count(); i < count; ++i)
      {
        QRect tabRect = tabBar->tabRect(i);
        if(tabRect.contains(pos))
        {
          tabRectResult = tabRect;
          tabRectResult.setRight(tabRect.left() + tabWidth);
          tabRectResult.translate(tabBar->mapToGlobal(QPoint(0, 0)));
          tabIndex = i;
          break;
        }
      }
    }
    else if(pos.x() < containerRect.x() + containerRect.width() / 3)
    {
      insertPolicy = SinTabFrameWork::InsertLeft;
      result = QRect(containerRect.topLeft(), QPoint(containerRect.x() + containerRect.width() / 3, containerRect.bottom()));
    }
    else if(pos.x() >= containerRect.x() + containerRect.width() * 2 / 3)
    {
      insertPolicy = SinTabFrameWork::InsertRight;
      result = QRect(QPoint(containerRect.x() + containerRect.width() * 2 / 3, containerRect.y()), containerRect.bottomRight());
    }
    else if(pos.y() < containerRect.y() + tabBar()->geometry().height())
    {
      insertPolicy = SinTabFrameWork::Insert;
      result = containerRect;
      tabIndex = this->tabBar()->count();
    }
    else if(pos.y() < containerRect.y() + containerRect.height() / 3)
    {
      insertPolicy = SinTabFrameWork::InsertTop;
      result = QRect(containerRect.topLeft(), QPoint(containerRect.right(), containerRect.y() + containerRect.height() / 3));
    }
    else if(pos.y() >= containerRect.y() + containerRect.height() * 2 / 3)
    {
      insertPolicy = SinTabFrameWork::InsertBottom;
      result = QRect(QPoint(containerRect.x(), containerRect.y() + containerRect.height() * 2 / 3), containerRect.bottomRight());
    }
    else
    {
      insertPolicy = SinTabFrameWork::InsertOnTop;
      result = containerRect;
    }
  }
  else
  {
    insertPolicy = SinTabFrameWork::InsertFloating;
    return QRect();
  }
  result.translate(mapToGlobal(QPoint(0, 0)));
  return result;
}

void SinTabContainer::writeLayout(QDataStream& stream)
{
  stream << (quint32)currentIndex();
  for(int i = 0, count = this->count(); i < count; ++i)
  {
    stream << (quint32)SinTabFrameWork::TabType;
    QWidget* widget = this->widget(i);
    QString tabObjectName = tabWindow->tabFramework->tabObjectName(widget);
    stream << tabObjectName;
  }
  stream << (quint32)SinTabFrameWork::NoneType;
}

void SinTabContainer::readLayout(QDataStream& stream)
{
  quint32 currentIndex;
  stream >> currentIndex;
  quint32 type;
  stream >> type;
  while(type != SinTabFrameWork::NoneType)
  {
    if(type == SinTabFrameWork::TabType)
    {
      QString objectName;
      stream >> objectName;
      tabWindow->tabFramework->unhideTab(objectName, this);
    }
    else
    {
      Q_ASSERT(false);
    }
    stream >> type;
  }
  setCurrentIndex(currentIndex);
}

void SinTabContainer::dragEnterEvent(QDragEnterEvent* event)
{
  const QMimeData* mimeData = event->mimeData();
  if(mimeData->hasFormat("application/x-tabwidget"))
  {
    QByteArray tabMimeData = mimeData->data("application/x-tabwidget");
    int tabWidth = tabMimeData.size() >= (int)sizeof(int) ? *(const int*)tabMimeData.data() : 100;
    SinTabContainer* sourceTabContainer = dynamic_cast<SinTabContainer*>(event->source()->parent()->parent());
    SinTabFrameWork::InsertPolicy insertPolicy;
    QRect tabRect;
    int tabIndex;
    QRect rect = findDropRect(mapToGlobal(event->pos()), tabWidth, insertPolicy, tabRect, tabIndex);
    if(sourceTabContainer == this && sourceTabContainer->count() == 1)
      tabRect = QRect();
    if(sourceTabContainer == this && (insertPolicy == SinTabFrameWork::InsertOnTop || (sourceTabContainer->count() == 1 && insertPolicy != SinTabFrameWork::Insert)) && !tabRect.isValid())
    {
      tabWindow->setDropOverlayRect(QRect());
      event->acceptProposedAction();
    }
    else
    {
      tabWindow->setDropOverlayRect(rect, tabRect);
      event->acceptProposedAction();
    }
    return;
  }
  event->ignore();
}

void SinTabContainer::dragLeaveEvent(QDragLeaveEvent* event)
{
  tabWindow->setDropOverlayRect(QRect());
}

void SinTabContainer::dragMoveEvent(QDragMoveEvent* event)
{
  const QMimeData* mineData = event->mimeData();
  if(mineData->hasFormat("application/x-tabwidget"))
  {
    QByteArray tabMimeData = mineData->data("application/x-tabwidget");
    int tabWidth = tabMimeData.size() >= (int)sizeof(int) ? *(const int*)tabMimeData.data() : 100;
    SinTabContainer* sourceTabContainer = dynamic_cast<SinTabContainer*>(event->source()->parent()->parent());
    SinTabFrameWork::InsertPolicy insertPolicy;
    QRect tabRect;
    int tabIndex;
    QRect rect = findDropRect(mapToGlobal(event->pos()), tabWidth, insertPolicy, tabRect, tabIndex);
    if(sourceTabContainer == this && sourceTabContainer->count() == 1)
      tabRect = QRect();
    if(sourceTabContainer == this && (insertPolicy == SinTabFrameWork::InsertOnTop || (sourceTabContainer->count() == 1 && insertPolicy != SinTabFrameWork::Insert)) && !tabRect.isValid())
    {
      tabWindow->setDropOverlayRect(QRect());
      event->acceptProposedAction();
    }
    else
    {
      tabWindow->setDropOverlayRect(rect, tabRect);
      event->acceptProposedAction();
    }
    return;
  }
  event->ignore();
}

void SinTabContainer::dropEvent(QDropEvent* event)
{
  tabWindow->setDropOverlayRect(QRect());

  const QMimeData* mineData = event->mimeData();
  if(mineData->hasFormat("application/x-tabwidget"))
  {
    QByteArray tabMimeData = mineData->data("application/x-tabwidget");
    int tabWidth = tabMimeData.size() >= (int)sizeof(int) ? *(const int*)tabMimeData.data() : 100;
    SinTabContainer* sourceTabContainer = dynamic_cast<SinTabContainer*>(event->source()->parent()->parent());
    SinTabFrameWork::InsertPolicy insertPolicy;
    QRect tabRect;
    int tabIndex;
    findDropRect(mapToGlobal(event->pos()), tabWidth, insertPolicy, tabRect, tabIndex);
    if(sourceTabContainer == this && sourceTabContainer->count() == 1)
      tabRect = QRect();
    if(sourceTabContainer == this && (insertPolicy == SinTabFrameWork::InsertOnTop || (sourceTabContainer->count() == 1  && insertPolicy != SinTabFrameWork::Insert)) && !tabRect.isValid())
    {
		tabWindow->tabFramework->moveTabLater((QWidget*)event->source(), this, SinTabFrameWork::InsertFloating, tabIndex);
      event->acceptProposedAction();
    }
    else
    {
      if(sourceTabContainer == this && tabIndex - 1 > indexOf((QWidget*)event->source()))
        --tabIndex;

	  tabWindow->tabFramework->moveTabLater((QWidget*)event->source(), this, insertPolicy, tabIndex);
      event->acceptProposedAction();
    }
    tabWindow->setDropOverlayRect(QRect());
    return;
  }
  event->ignore();
}

SinMainWindow::SinMainWindow(SinTabFrameWork* tabFramework) : tabFramework(tabFramework), overlayWidget(0), overlayWidgetTab(0), focusTab(0)
{
  new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this, SLOT(hideCurrent()));
}

void SinMainWindow::hideCurrent()
{
  QObject* widget = QApplication::focusWidget();
  while(widget)
  {
    SinTabContainer* tabContainer = dynamic_cast<SinTabContainer*>(widget);
    if(tabContainer)
    {
      tabFramework->hideTab(tabContainer->currentWidget());
      return;
    }
    widget = widget->parent();
  }
}

void SinMainWindow::setDropOverlayRect(const QRect& globalRect, const QRect& globalTabRect)
{
  if(globalRect.isValid())
  {
    if(!overlayWidget)
      overlayWidget = new QTabDropOverlay(this);
    overlayWidget->raise();
    overlayWidget->show();
    if(globalTabRect.isValid())
    {
      QRect wrect = globalRect;
      wrect.setTop(globalTabRect.bottom() + 1);
      QRect rect = wrect.translated(mapFromGlobal(QPoint(0, 0)));
      overlayWidget->setGeometry(rect);
      if(!overlayWidgetTab)
        overlayWidgetTab = new QTabDropOverlay(this);
      overlayWidgetTab->raise();
      overlayWidgetTab->show();
      QRect tabRect = globalTabRect.translated(mapFromGlobal(QPoint(0, 0)));
      overlayWidgetTab->setGeometry(tabRect);
    }
    else
    {
      QRect rect = globalRect.translated(mapFromGlobal(QPoint(0, 0)));
      overlayWidget->setGeometry(rect);
      if(overlayWidgetTab)
        overlayWidgetTab->hide();
    }
  }
  else if(overlayWidget)
  {
    if(overlayWidgetTab)
      overlayWidgetTab->hide();
    overlayWidget->hide();
  }
}

void SinMainWindow::writeLayout(QDataStream& stream)
{
  stream << saveGeometry();
  stream << saveState();
  QWidget* widget = centralWidget();
  SinTabSplitter* tabSplitter = dynamic_cast<SinTabSplitter*>(widget);
  if(tabSplitter)
  {
    stream << (quint32)SinTabFrameWork::SplitterType;
    tabSplitter->writeLayout(stream);
  }
  else
  {
    SinTabContainer* tabContainer = dynamic_cast<SinTabContainer*>(widget);
    if(tabContainer)
    {
      stream << (quint32)SinTabFrameWork::ContainerType;
      tabContainer->writeLayout(stream);
    }
    else
      stream << (quint32)SinTabFrameWork::NoneType;
  }
}

void SinMainWindow::readLayout(QDataStream& stream)
{
  QByteArray geometry, state;
  stream >> geometry;
  stream >> state;
  quint32 type;
  stream >> type;
  switch((SinTabFrameWork::LayoutType)type)
  {
  case SinTabFrameWork::SplitterType:
    {
      SinTabSplitter* tabSplitter = new SinTabSplitter(Qt::Horizontal, this, this);
      setCentralWidget(tabSplitter);
      tabSplitter->readLayout(stream);
    }
    break;
  case SinTabFrameWork::ContainerType:
    {
      SinTabContainer* tabContainer = new SinTabContainer(this, this);
      setCentralWidget(tabContainer);
      tabContainer->readLayout(stream);
    }
    break;
  default:
    Q_ASSERT(false);
    break;
  }
  restoreGeometry(geometry);
  restoreState(state);
}

void SinMainWindow::hideAllTabs()
{
  QWidget* centralWidget = this->centralWidget();
  if(!centralWidget)
    return;

  QLinkedList<QWidget*> widgets;
  widgets.append(centralWidget);
  for(QLinkedList<QWidget*>::Iterator i = widgets.begin(); i != widgets.end(); ++i)
  {
    QWidget* widget = *i;
    SinTabSplitter* tabSplitter = dynamic_cast<SinTabSplitter*>(widget);
    if(tabSplitter)
    {
      for(int i = 0, count = tabSplitter->count(); i < count; ++i)
      {
        QWidget* widget = tabSplitter->widget(i);
        widgets.append(widget);
      }
    }
    else
    {
      SinTabContainer* tabContainer = dynamic_cast<SinTabContainer*>(widget);
      if(tabContainer)
        for(int i = tabContainer->count() - 1; i >= 0; --i)
          tabFramework->hideTab(tabContainer->widget(i), false);
    }
  }
}

int SinMainWindow::tabCount()
{
  QWidget* centralWidget = this->centralWidget();
  if(!centralWidget)
    return 0;

  int count = 0;
  QLinkedList<QWidget*> widgets;
  widgets.append(centralWidget);
  for(QLinkedList<QWidget*>::Iterator i = widgets.begin(); i != widgets.end(); ++i)
  {
    QWidget* widget = *i;
    SinTabSplitter* tabSplitter = dynamic_cast<SinTabSplitter*>(widget);
    if(tabSplitter)
    {
      for(int i = 0, count = tabSplitter->count(); i < count; ++i)
      {
        QWidget* widget = tabSplitter->widget(i);
        widgets.append(widget);
      }
    }
    else
    {
      SinTabContainer* tabContainer = dynamic_cast<SinTabContainer*>(widget);
      if(tabContainer)
        count += tabContainer->count();
    }
  }
  return count;
}

void SinMainWindow::closeEvent(QCloseEvent* event)
{
  //event->ignore();
  //QMainWindow::closeEvent(event);
  //
  //QTimer::singleShot(0, tabFramework, SLOT(close()));

  //QTabContainer* centralTabContainer = dynamic_cast<QTabContainer*>(centralWidget());
  //if(!centralTabContainer || centralTabContainer->count() > 1)
  {
    event->ignore();
    //QMainWindow::closeEvent(event);
 
    QTimer::singleShot(0, tabFramework, SLOT(close()));

    //int tabCount = this->tabCount();
    //if(QMessageBox::question(this, tr("Close Window"), tabCount == 2 ? tr("Do you really want to close both tabs?") : tr("Do you really want to close all %1 tabs?").arg(tabCount), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
    //{
    //  event->ignore();
    //  return;
    //}
    return;
  }
  
  //hideAllTabs();
  //
  //QMainWindow::closeEvent(event);
  //
  //tabFramework->removeWindow(this);
}

SinTabFrameWork::SinTabFrameWork() : SinMainWindow(this), moveTabWidget(0)
{
  connect(&signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(toggleVisibility(QWidget*)));
  
  connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(handleFocusChanged(QWidget*, QWidget*)));
}

SinTabFrameWork::~SinTabFrameWork()
{
  for(QHash<QWidget*, TabData>::Iterator i = tabs.begin(), end = tabs.end(); i != end; ++i)
  {
    TabData& tabData = i.value();
    if(tabData.hidden)
      delete i.key();
  }

  qDeleteAll(floatingWindows);
}

void SinTabFrameWork::addTab(QWidget* widget, InsertPolicy insertPolicy, QWidget* position)
{
  if(tabs.contains(widget))
    return;

  // determine object name
  QString objectName = widget->objectName();
  if(objectName.isEmpty())
  {
    QString className = widget->metaObject()->className();
    for(int num = 0;; ++num)
    {
      objectName = className + "_" + QString::number(num);
      if(!tabsByName.contains(objectName))
        break;
    }
  }
  else
  {
    QString objectClassName = objectName;
    if(tabsByName.contains(objectName))
      for(int num = 0;; ++num)
      {
        objectName = objectClassName + "_" + QString::number(num);
        if(!tabsByName.contains(objectName))
          break;
      }
  }

  // create tab data
  TabData& tabData = *tabs.insert(widget, TabData());
  tabData.widget = widget;
  tabData.objectName = objectName;
  tabData.hidden = false;
  tabData.action = 0;
  tabsByName.insert(objectName, &tabData);
  widget->installEventFilter(this);

  //
  addTab(widget, position ? dynamic_cast<SinTabContainer*>(position->parent()->parent()) : 0, insertPolicy, -1);
}

void SinTabFrameWork::moveTab(QWidget* widget, InsertPolicy insertPolicy, QWidget* position)
{
  QHash<QWidget*, TabData>::Iterator it =  tabs.find(widget);
  if(it == tabs.end())
    return;
  TabData& tabData = it.value();
  if(tabData.hidden)
  {
    addTab(widget, position ? dynamic_cast<SinTabContainer*>(position->parent()->parent()) : 0, insertPolicy, -1);
    tabData.hidden = false;
    if(tabData.action)
      tabData.action->setChecked(true);
  }
  else
  {
    moveTab(widget, position ? dynamic_cast<SinTabContainer*>(position->parent()->parent()) : 0, insertPolicy, -1);
  }
}

void SinTabFrameWork::removeTab(QWidget* widget)
{
  QHash<QWidget*, TabData>::Iterator it = tabs.find(widget);
  if(it == tabs.end())
    return;
  TabData& tabData = it.value();
  if(!tabData.hidden)
  {
    SinTabContainer* tabContainer = dynamic_cast<SinTabContainer*>(widget->parent()->parent());
    int movedIndex = tabContainer->indexOf(widget);
    tabContainer->removeTab(movedIndex);
    widget->setParent(NULL);
    removeContainerIfEmpty(tabContainer);
  }
  if(tabData.action)
    delete tabData.action;
  tabsByName.remove(tabData.objectName);
  tabs.erase(it);
  widget->removeEventFilter(this);
}

void SinTabFrameWork::hideTab(QWidget* widget)
{
  QHash<QWidget*, TabData>::Iterator it = tabs.find(widget);
  if(it == tabs.end())
    return;
  TabData& tabData = it.value();
  if(!tabData.hidden)
    hideTab(widget, true);
}

bool SinTabFrameWork::isVisible(QWidget* widget) const
{
  QHash<QWidget*, TabData>::ConstIterator it = tabs.find(widget);
  if(it == tabs.end())
    return false;
  const TabData& tabData = it.value();
  return !tabData.hidden;
}

QAction* SinTabFrameWork::toggleViewAction(QWidget* widget)
{
  QHash<QWidget*, TabData>::Iterator it = tabs.find(widget);
  if(it == tabs.end())
    return 0;
  TabData& tabData = it.value();
  if(!tabData.action)
  {
    tabData.action = new QAction(widget->windowTitle(), this);
    tabData.action->setCheckable(true);
    tabData.action->setChecked(!tabData.hidden);
    connect(tabData.action, SIGNAL(triggered()), &signalMapper, SLOT(map()));
    signalMapper.setMapping(tabData.action, widget);
  }
  return tabData.action;
}

void SinTabFrameWork::restoreLayout(const QByteArray& layout)
{
  if(layout.isEmpty())
    return;

  // check format version
  QDataStream dataStream(layout);
  quint32 version;
  dataStream >> version;
  if(version != 101)
    return;

  // close all floating windows
  for(QList<SinMainWindow*>::Iterator i = floatingWindows.begin(), end = floatingWindows.end(); i != end; ++i)
  {
    SinMainWindow* tabWindow = *i;
    tabWindow->hideAllTabs();
    delete tabWindow;
  }
  floatingWindows.clear();

  // hide all tabs
  hideAllTabs();
  QWidget* centralWidget = this->centralWidget();
  if(centralWidget)
  {
    setCentralWidget(0);
    delete centralWidget;
  }

  // restore local layout
  quint32 type;
  dataStream >> type;
  if(type == WindowType)
    readLayout(dataStream);

  // create floating windows
  dataStream >> type;
  while(type != NoneType)
  {
    if(type == WindowType)
    {
      SinMainWindow* tabWindow = createWindow();
      tabWindow->readLayout(dataStream);
      if(QMainWindow::isVisible())
        tabWindow->show();
    }
    else
    {
      Q_ASSERT(false);
    }
    dataStream >> type;
  }
  QList<qint32> zOrder;
  dataStream >> zOrder;
  floatingWindowsZOrderToRestore.clear();
  for(QList<qint32>::Iterator i = zOrder.begin(), end = zOrder.end(); i != end; ++i)
  {
    qint32 index = *i;
    if(index < 0)
      floatingWindowsZOrderToRestore.append(this);
    else if(index < (qint32)floatingWindows.size())
      floatingWindowsZOrderToRestore.append(floatingWindows.at(index));
  }
}

QByteArray SinTabFrameWork::saveLayout()
{
  QByteArray result;
  QDataStream dataStream(&result, QIODevice::WriteOnly);
  quint32 version = 101;
  dataStream << version;
  dataStream << (quint32)WindowType;
  writeLayout(dataStream);
  for(QList<SinMainWindow*>::Iterator i = floatingWindows.begin(), end = floatingWindows.end(); i != end; ++i)
  {
    SinMainWindow* tabWindow = *i;
    dataStream << (quint32)WindowType;
    tabWindow->writeLayout(dataStream);
  }
  dataStream << (quint32)NoneType;
  {
    QList<qint32> zOrder;
    for(QList<SinMainWindow*>::Iterator i = floatingWindowsZOrder.begin(), end = floatingWindowsZOrder.end(); i != end; ++i)
      zOrder.append(floatingWindows.indexOf(*i));
    dataStream << zOrder;
  }
  return result;
}

void SinTabFrameWork::addTab(QWidget* widget, SinTabContainer* container, InsertPolicy insertPolicy, int tabIndex)
{
  QWidget* centralWidget = this->centralWidget();
  bool createWindow = !container || insertPolicy == InsertFloating || !centralWidget;
  if(createWindow)
  {
    if(centralWidget)
    {
      SinMainWindow* tabWindow = this->createWindow();
      SinTabContainer* container = new SinTabContainer(tabWindow, tabWindow);
      container->addTab(widget, widget->windowTitle());
      tabWindow->setCentralWidget(container);
      if(QMainWindow::isVisible())
        tabWindow->show();
    }
    else
    {
      SinTabContainer* container = new SinTabContainer(this, this);
      container->addTab(widget, widget->windowTitle());
      setCentralWidget(container);
    }
  }
  else
  {
    if(insertPolicy == InsertPolicy::InsertOnTop || (insertPolicy == InsertPolicy::Insert && tabIndex < 0))
    {
      insertPolicy = InsertPolicy::Insert;
      tabIndex = container->count();
    }
    if(insertPolicy == InsertPolicy::Insert)
    {
      container->insertTab(tabIndex, widget, widget->windowTitle());
      container->setCurrentIndex(tabIndex);
    }
    else if(insertPolicy == InsertPolicy::InsertLeft || insertPolicy == InsertPolicy::InsertRight ||
      insertPolicy == InsertPolicy::InsertTop || insertPolicy == InsertPolicy::InsertBottom)
    {
      Qt::Orientation orientation = (insertPolicy == InsertPolicy::InsertLeft || insertPolicy == InsertPolicy::InsertRight) ? Qt::Horizontal : Qt::Vertical;
      int widthOrHeight = 0;
      if(orientation == Qt::Horizontal)
        widthOrHeight = container->width();
      else
        widthOrHeight = container->height();
      SinTabSplitter* splitter = dynamic_cast<SinTabSplitter*>(container->parent());
      if(splitter && splitter->orientation() == orientation)
      {
        SinTabContainer* newContainer = new SinTabContainer(splitter, container->tabWindow);
        newContainer->addTab(widget, widget->windowTitle());
        int containerIndex = splitter->indexOf(container);
        if(insertPolicy == InsertPolicy::InsertRight || insertPolicy == InsertPolicy::InsertBottom)
        {
          QList<int> sizes = splitter->sizes();
          splitter->insertWidget(containerIndex + 1, newContainer);
          sizes[containerIndex] -= widthOrHeight / 3;
          sizes.insert(containerIndex + 1, widthOrHeight / 3 - splitter->handleWidth());
          splitter->setSizes(sizes);
        }
        else
        {
          QList<int> sizes = splitter->sizes();
          splitter->insertWidget(containerIndex, newContainer);
          sizes[containerIndex] -= widthOrHeight / 3;
          sizes.insert(containerIndex, widthOrHeight / 3 - splitter->handleWidth());
          splitter->setSizes(sizes);
        }
      }
      else
      {
        SinTabSplitter* newSplitter = new SinTabSplitter(orientation, splitter ? (QWidget*)splitter : (QWidget*)container->tabWindow, container->tabWindow);
        SinTabContainer* newContainer = new SinTabContainer(newSplitter, container->tabWindow);
        int containerIndex = -1;
        QList<int> sizes;
        if(splitter)
        {
          containerIndex = splitter->indexOf(container);
          sizes = splitter->sizes();
        }
        newContainer->addTab(widget, widget->windowTitle());
        container->setParent(NULL); // remove container from splitter or tabWindow
        if(!splitter)
        {
          container->tabWindow->setCentralWidget(newSplitter);
          //container->tabWindow->removeDropOverlay();
        }
        else
        {
          splitter->insertWidget(containerIndex, newSplitter);
          splitter->setSizes(sizes);
        }
        if(insertPolicy == InsertPolicy::InsertRight || insertPolicy == InsertPolicy::InsertBottom)
        {
          newSplitter->addWidget(container);
          newSplitter->addWidget(newContainer);
          QList<int> sizes;
          sizes.append(widthOrHeight - widthOrHeight / 3);
          sizes.append(widthOrHeight / 3 - newSplitter->handleWidth());
          newSplitter->setSizes(sizes);
        }
        else
        {
          newSplitter->addWidget(newContainer);
          newSplitter->addWidget(container);
          QList<int> sizes;
          sizes.append(widthOrHeight / 3 - newSplitter->handleWidth());
          sizes.append(widthOrHeight - widthOrHeight / 3);
          newSplitter->setSizes(sizes);
        }
      }
    }
    else
    {
      Q_ASSERT(false);
    }
  }
}

void SinTabFrameWork::moveTab(QWidget* widget, SinTabContainer* position, InsertPolicy insertPolicy, int tabIndex)
{
  //QHash<QWidget*, TabData>::Iterator it =  tabs.find(widget);
  //if(it == tabs.end())
  //  return;
  //TabData& tabData = it.value();
  //if(tabData.hidden)
  //{
  //  addTab(widget, position, insertPolicy, tabIndex);
  //  tabData.hidden = false;
  //}
  //else
  //{
    SinTabContainer* tabContainer = dynamic_cast<SinTabContainer*>(widget->parent()->parent());
    int movedIndex = tabContainer->indexOf(widget);
    tabContainer->removeTab(movedIndex);
    addTab(widget, position, insertPolicy, tabIndex);
    removeContainerIfEmpty(tabContainer);
  //}
}

void SinTabFrameWork::moveTabLater(QWidget* widget, SinTabContainer* position, InsertPolicy insertPolicy, int tabIndex)
{
  moveTabWidget = widget;
  moveTabPosition = position;
  moveTabInsertPolicy = insertPolicy;
  moveTabTabIndex = tabIndex;
  QTimer::singleShot(0, this, SLOT(executeMoveTab()));
}

void SinTabFrameWork::executeMoveTab()
{
  if(!moveTabWidget)
    return;
  moveTab(moveTabWidget, moveTabPosition, moveTabInsertPolicy, moveTabTabIndex);
  moveTabWidget = 0;
}

void SinTabFrameWork::toggleVisibility(QWidget* widget)
{
  QHash<QWidget*, TabData>::Iterator it = tabs.find(widget);
  if(it == tabs.end())
    return;
  TabData& tabData = it.value();
  if(tabData.hidden)
    moveTab(widget, InsertFloating, 0);
  else
    hideTab(widget);
}

void SinTabFrameWork::showFloatingWindows()
{
  if(!QMainWindow::isVisible())
  {
    for(QList<SinMainWindow*>::Iterator i = floatingWindows.begin(), end = floatingWindows.end(); i != end; ++i)
      (*i)->hide();
  }
  else
  {
    if(!floatingWindowsZOrderToRestore.contains(this))
      floatingWindowsZOrderToRestore.append(this);
    for(QList<SinMainWindow*>::Iterator i = floatingWindows.begin(), end = floatingWindows.end(); i != end; ++i)
      (*i)->show();
    for(QList<SinMainWindow*>::Iterator i = floatingWindowsZOrderToRestore.begin(), end = floatingWindowsZOrderToRestore.end(); i != end; ++i)
      (*i)->raise();
    floatingWindowsZOrderToRestore.back()->activateWindow();
    floatingWindowsZOrderToRestore.clear();
  }
}

void SinTabFrameWork::handleFocusChanged(QWidget* old, QWidget* now)
{
  if(now)
  {
    QWidget* widget = now;
    QWidget* parent = dynamic_cast<QWidget*>(now->parent());
    QWidget* parentParent = dynamic_cast<QWidget*>(parent->parent());
    while(parentParent)
    {
      SinTabContainer* tabContainer = dynamic_cast<SinTabContainer*>(parentParent);
      if(tabContainer)
      {
        if(tabContainer->tabWindow != tabContainer->tabWindow->tabFramework)
        {
          tabContainer->tabWindow->focusTab = widget;
          tabContainer->tabWindow->setWindowTitle(widget->windowTitle());
        }
        break;
      }
      widget = parent;
      parent = parentParent;
      parentParent = dynamic_cast<QWidget*>(parent->parent());
    }
  }
  else
  {
    for(QList<SinMainWindow*>::Iterator i = floatingWindows.begin(), end = floatingWindows.end(); i != end; ++i)
    {
      SinMainWindow* tabWindow = *i;
      if(tabWindow->focusTab == old)
        tabWindow->focusTab = 0;
    }
  }
}

QString SinTabFrameWork::tabObjectName(QWidget* widget)
{
  QHash<QWidget*, TabData>::Iterator it = tabs.find(widget);
  if(it == tabs.end())
    return QString();
  TabData& tabData = it.value();
  return tabData.objectName;
}

void SinTabFrameWork::unhideTab(const QString& objectName, SinTabContainer* position)
{
  QHash<QString, TabData*>::Iterator it = tabsByName.find(objectName);
  if(it == tabsByName.end())
    return;
  TabData& tabData = *it.value();
  if(!tabData.hidden)
    return;
  position->addTab(tabData.widget, tabData.widget->windowTitle());
  tabData.hidden = false;
  if(tabData.action)
    tabData.action->setChecked(!tabData.hidden);
}

void SinTabFrameWork::removeContainerIfEmpty(SinTabContainer* tabContainer)
{
  if(tabContainer->count() == 0)
  {
    QObject* parent = tabContainer->parent();
    SinTabSplitter* splitter = dynamic_cast<SinTabSplitter*>(parent);
    if(splitter)
    {
      if(splitter->count() == 2)
      { // the splitter will be pointless, remove it
        QWidget* sibling = splitter->widget(0);
        if(sibling == tabContainer)
          sibling = splitter->widget(1);
        sibling->setParent(NULL);

        SinTabSplitter* parentSplitter = dynamic_cast<SinTabSplitter*>(splitter->parent());
        if(parentSplitter)
        {
          QList<int> sizes = parentSplitter->sizes();
          int splitterIndex = parentSplitter->indexOf(splitter);
          splitter->setParent(NULL);
          SinTabSplitter* siblingSplitter = dynamic_cast<SinTabSplitter*>(sibling);
          if(!siblingSplitter || siblingSplitter->orientation() != parentSplitter->orientation())
          {
            parentSplitter->insertWidget(splitterIndex, sibling);
            parentSplitter->setSizes(sizes);
          }
          else
          {
            QList<int> sibSizes = siblingSplitter->sizes();
            while(siblingSplitter->count() > 0)
            {
              QWidget* widget = siblingSplitter->widget(0);
              widget->setParent(NULL);
              parentSplitter->insertWidget(splitterIndex, widget);
              sizes.insert(splitterIndex, sibSizes[0]);
              sibSizes.removeFirst();
              ++splitterIndex;
            }
            delete siblingSplitter;
          }
          delete splitter;
        }
        else
        {
          splitter->setParent(NULL);
          tabContainer->tabWindow->setCentralWidget(sibling);
          delete splitter;
        }
      }
      else
      {
        // remove container
        int splitCount = splitter->count();
        tabContainer->setParent(NULL);
        delete tabContainer;
        Q_ASSERT(splitter->count() == splitCount - 1);
      }
    }
    else
    {
      tabContainer->tabWindow->tabFramework->removeWindow(tabContainer->tabWindow);
    }
  }
}

void SinTabFrameWork::hideTab(QWidget* widget, bool removeContainerIfEmpty)
{
  QHash<QWidget*, TabData>::Iterator it =  tabs.find(widget);
  if(it == tabs.end())
    return;
  TabData& tabData = it.value();
  if(tabData.hidden)
    return;

  SinTabContainer* tabContainer = dynamic_cast<SinTabContainer*>(widget->parent()->parent());
  int movedIndex = tabContainer->indexOf(widget);
  tabContainer->removeTab(movedIndex);
  widget->setParent(NULL);
  if(removeContainerIfEmpty)
    this->removeContainerIfEmpty(tabContainer);

  tabData.hidden = true;
  if(tabData.action)
    tabData.action->setChecked(!tabData.hidden);
}

SinMainWindow* SinTabFrameWork::createWindow()
{
  SinMainWindow* tabWindow = new SinMainWindow(this);
  tabWindow->setWindowIcon(windowIcon());
  tabWindow->setAttribute(Qt::WA_DeleteOnClose);
  tabWindow->installEventFilter(this);
  floatingWindows.append(tabWindow);
  floatingWindowsZOrder.append(tabWindow);
  return tabWindow;
}

void SinTabFrameWork::removeWindow(SinMainWindow* window)
{
  if(window == this)
    return;
  floatingWindows.removeOne(window);
  floatingWindowsZOrder.removeOne(window);
  delete window;
}

void SinTabFrameWork::closeEvent(QCloseEvent* event)
{
  qDeleteAll(floatingWindows);
  floatingWindows.clear();
  QWidget* centralWidget = this->centralWidget();
  if(centralWidget)
  {
    setCentralWidget(0);
    delete centralWidget;
  }
  for(QHash<QWidget*, TabData>::Iterator i = tabs.begin(), end = tabs.end(); i != end; ++i)
  {
    TabData& tabData = i.value();
    if(tabData.hidden)
      delete i.key();
  }
  tabs.clear();
  tabsByName.clear();

  QMainWindow::closeEvent(event); // skip QTabWindow closeEvent handler
}

void SinTabFrameWork::showEvent(QShowEvent* event)
{
  QTimer::singleShot(0, this, SLOT(showFloatingWindows()));

  SinMainWindow::showEvent(event);
}

bool SinTabFrameWork::eventFilter(QObject* obj, QEvent* event)
{
  switch(event->type())
  {
  case QEvent::WindowTitleChange:
    {
      QWidget* widget = dynamic_cast<QWidget*>(obj);
      QHash<QWidget*, TabData>::Iterator it = tabs.find(widget);
      if(it != tabs.end())
      {
        QWidget* widgetParent = dynamic_cast<QWidget*>(widget->parent());
        if(widgetParent)
        {
          SinTabContainer* tabContainer = dynamic_cast<SinTabContainer*>(widgetParent->parent());
          if(tabContainer)
          {
            int index = tabContainer->indexOf(widget);
            QString title = widget->windowTitle();
            tabContainer->setTabText(index, title);
            tabContainer->setTabToolTip(index, title);
            if(tabContainer->tabWindow->focusTab == widget)
              tabContainer->tabWindow->setWindowTitle(title);
            TabData& tabData = it.value();
            if(tabData.action)
              tabData.action->setText(title);
          }
        }
      }
    }
    break;
  case QEvent::WindowActivate:
    {
      SinMainWindow* tabWindow = dynamic_cast<SinMainWindow*>(obj);
      if(floatingWindows.contains(tabWindow))
      {
        floatingWindowsZOrder.removeOne(tabWindow);
        floatingWindowsZOrder.append(tabWindow);
      }
    }
    break;
  default:
    break;
  }

  return QMainWindow::eventFilter(obj, event);
}

bool SinTabFrameWork::event(QEvent* event)
{
  if (event->type() == QEvent::WindowActivate)
  {
    floatingWindowsZOrder.removeOne(this);
    floatingWindowsZOrder.append(this);
  }
  return QMainWindow::event(event);
}
