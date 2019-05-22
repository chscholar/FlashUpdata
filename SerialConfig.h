#ifndef SERIALCONFIG_H
#define  SERIALCONFIG_H
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#pragma execution_character_set("utf-8")

class SerialItem : public QWidget
{
	Q_OBJECT
public:
	SerialItem(bool isCombox = true,QWidget *parent = 0);
	~SerialItem();
	void setTipText(QString);
	void setValuItems(QStringList strList);
	void setStatus(bool isOpen);
	int getSelectIndex();
protected:
	
private:
	void initUi();
	bool m_bIsCombox;
	QLabel *m_pLabel;
	QComboBox *m_pCombox;
	QLabel *m_pLabelStatus;
	QString m_green_SheetStyle;
	QString m_grey_SheetStyle;
};

class SerialConfigWidget : public QWidget
{
	Q_OBJECT
public:
	SerialConfigWidget(QWidget *parent = 0);
	~SerialConfigWidget();
public slots:
void slotOpenCloseCom();
protected:
	void initUi();
private:

	QLabel *m_pStatusLabel;

	SerialItem *m_pPortName;
	SerialItem *m_pRate;
	SerialItem *m_pFlow;
	SerialItem *m_pParity;
	SerialItem *m_pDataBit;
	SerialItem *m_pStopDataBit;
	SerialItem *m_pStatus;
};

#endif
