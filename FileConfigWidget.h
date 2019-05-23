#ifndef FILECONFIGWIDGET_H
#define FILECONFIGWIDGET_H
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include <QPushButton>
#pragma execution_character_set("utf-8")
class FileConfigItem : public QWidget
{
	Q_OBJECT
public:
	FileConfigItem(int itemId,QWidget *parent = 0);
	~FileConfigItem();
	void setFirst();
	void setEnd();
	void setNiddle();
	int findItemById(int itemId);
protected:
	void initUi();

	public slots:
	void slotDel();
signals :
	void signalAddFileConfig();
		void signalDelFileConfig(int itemId);
private:
	QPushButton *addButton;
	QPushButton *delButton;
	int m_iItemId;
};


class FileConfigWidget : public QWidget
{
	Q_OBJECT
public:
	FileConfigWidget(QWidget *parent = 0);
	~FileConfigWidget();
protected:
	void initUi();
	void flushWidget();
	void delAllWidgetFromLayout();
	public slots:
	void slotAddFileConfig();
	void slotDelFileConfig(int );
private:

	QVBoxLayout *mainLayout;
	QVector < FileConfigItem*> fileConfigVec ;
	int m_iItemId;
};

#endif
