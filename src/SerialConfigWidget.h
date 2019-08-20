#ifndef SERIALCONFIGWIDGET_H
#define  SERIALCONFIGWIDGET_H
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVector>
#include "SinSerialChoose.h"

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
	void setDefaultIndex(int defaultIndex);
	void setComandBoxEnable(bool isEnable);
	QStringList getValues();
	QString getSelectValue();
protected:
	
private:
	void initUi();
	bool m_bIsCombox;
	QLabel *m_pLabel;
	QComboBox *m_pCombox;
	QLabel *m_pLabelStatus;
	QString m_green_SheetStyle;
	QString m_grey_SheetStyle;
	QString m_combox_sheetStyle;
};

class SerialConfigWidget : public QWidget
{
	Q_OBJECT
public:
	SerialConfigWidget(QWidget *parent = 0);
	~SerialConfigWidget();

	QSerialPort* chooseSerial();
public slots:
void slotOpenCloseCom();
void slotChooseCom();
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

	QPushButton *m_pCloseOpenButton;

	QVector <SinSerialChoose*> m_vSerialPortList;
};

#endif
