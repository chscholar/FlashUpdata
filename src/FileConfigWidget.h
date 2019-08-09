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
#include <QShowEvent>
#include "SinXml.h"

#pragma execution_character_set("utf-8")
class FileConfigItemWidget : public QWidget
{
	Q_OBJECT
public:
	FileConfigItemWidget(bool isUpLoad = true,int itemId = 0,QString fileChecked = "checked",QString filePath = "", QWidget *parent = 0);
	~FileConfigItemWidget();
	void setFirst();
	void setEnd();
	void setNiddle();
	//void setDownload();
	void setDownloadStatus();
	void setUploadStatus();
	int findItemById(int itemId);
	QString getCheckedStatus();
	QString getFilePath();
protected:
	void initUi();

	public slots:
	void slotDel();
	void slotAdd();
	void slotBrowFile();
	void slotChecboxStateChange(int);
private:
	QCheckBox *m_pFileCheckbox;
	QLabel *m_pAddressLabel;
	QLineEdit *m_pAddressEdit;
	QPushButton *addButton;
	QPushButton *delButton;
	int m_iItemId;
	QString m_sFileChecked;
	QString m_sFilePath;
	bool m_bisUpLoad;
};


class FileConfigWidget : public QWidget
{
	Q_OBJECT
public:
	FileConfigWidget(QWidget *parent = 0);
	~FileConfigWidget();
	void switchWidget(bool isUpLoad);
	QStringList getAllSelectPath();
	QString getUpFileSavePath();
	
protected:
	void initUi();
	void flushWidget();
	void delAllWidgetFromLayout();
	void showEvent(QShowEvent *e);
	void fillItemVecFromConfig(bool isUpLoad = true);
private:

	QVBoxLayout *mainUpLoadLayout;
	QVBoxLayout *mainDownLoadLayout;
	QVector < FileConfigItemWidget*> fileConfigVec ;
	int m_iItemId;
	bool m_bUpLoadChecked;
public slots:
void slotConfigFileChange();

};

#endif
