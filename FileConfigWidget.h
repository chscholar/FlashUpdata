#ifndef FILECONFIGWIDGET_H
#define FILECONFIGWIDGET_H
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#pragma execution_character_set("utf-8")
class FileConfigItem : public QWidget
{
	Q_OBJECT
public:
	FileConfigItem( bool isFirst = false,bool isEnd = false,QWidget *parent = 0);
	~FileConfigItem();
protected:
	void initUi(bool isFirst,bool isEnd);
	public slots:
signals :
	void signalAddFileConfig();
		void signalDelFileConfig();
private:
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
	void slotDelFileConfig();
private:

	QVBoxLayout *mainLayout;
	QVector < FileConfigItem*> fileConfigVec ;
};

#endif
