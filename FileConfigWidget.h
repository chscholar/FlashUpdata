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
	void setDownload();
	int findItemById(int itemId);
	QString getFilePath();
protected:
	void initUi();

	public slots:
	void slotDel();
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
	QVector < FileConfigItem*> fileConfigVec ;
	int m_iItemId;
};

#endif
