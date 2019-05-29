#ifndef FILECONFIGWIDGET_H
#define FILECONFIGWIDGET_H
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QStringList>
#include "SinXml.h"

#pragma execution_character_set("utf-8")
class FileConfigItemWidget : public QWidget
{
	Q_OBJECT
public:
	FileConfigItemWidget(int itemId = 0,QString fileChecked = "checked",QString filePath = "", QWidget *parent = 0);
	~FileConfigItemWidget();
	void setFirst();
	void setEnd();
	void setNiddle();
	void setDownload();
	int findItemById(int itemId);
	QString getCheckedStatus();
	QString getFilePath();
protected:
	void initUi();

	public slots:
	void slotDel();
	void slotAdd();
	void slotBrowFile();
signals :
	void signalAddFileConfig();
		void signalDelFileConfig(int itemId);
private:
	QCheckBox *m_pFileCheckbox;
	QLabel *m_pAddressLabel;
	QLineEdit *m_pAddressEdit;
	QPushButton *addButton;
	QPushButton *delButton;
	int m_iItemId;
	QString m_sFileChecked;
	QString m_sFilePath;
};


class FileConfigWidget : public QWidget
{
	Q_OBJECT
public:
	FileConfigWidget(QWidget *parent = 0);
	~FileConfigWidget();
	void switchWidget(bool isUpLoad);
	QStringList getAllSelectPath();
protected:
	void initUi();
	void flushWidget();
	void delAllWidgetFromLayout();
	public slots:
	void slotAddFileConfig();
	void slotDelFileConfig(int );
private:

	QVBoxLayout *mainUpLoadLayout;
	QVBoxLayout *mainDownLoadLayout;
	QVector < FileConfigItemWidget*> fileConfigVec ;
	int m_iItemId;
};

#endif
