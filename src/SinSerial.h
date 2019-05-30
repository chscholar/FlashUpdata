#ifndef		SINSERIAL_H
#define  SINSERIAL_H
#include <qDebug>
#include <QSerialPort>
#include <QStringList>
#include "CSingleTon.h"
#include "QObject"
#include "SinByte.h"

#pragma execution_character_set("utf-8")
class SinSerial :public QObject
{
private:

	QSerialPort *serialPort;
	QSerialPort *getSerialPort();
	QString findKeyFromMap(QMap<int,QString> fmap,int key);

	QMap<int, QString> portMap;
	QMap<int, QString> rateMap;
	QMap<int, QString> dataMap;
	QMap<int, QString> stopMap;
	QMap<int, QString> parityMap;
	QMap<int, QString> flowMap;
public:
	SinSerial(QObject *parent = 0);
	~SinSerial();

	QStringList getEnablePorts();
	QStringList getEnableRates();
	QStringList getDataBits();
	QStringList getStopBits();
	QStringList getFlowControl();
	QStringList getParity();

	void sendData(QList<QList <QByteArray>>);
	bool isOPen();
	int openCom(int portIndex,int rateIndex,int flowIndex,int dataIndex,int stopIndex,int parityIndex);
	void closeCom();
};

typedef CSingleton<SinSerial> sinserialSingle;
#endif