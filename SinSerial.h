#ifndef		SINSERIAL_H
#define  SINSERIAL_H
#include <qDebug>
#include <QSerialPort>
#include <QStringList>
#include "CSingleTon.h"
#include "QObject"

class SinSerial :public QObject
{
private:

	QSerialPort *serialPort;
	QSerialPort *getSerialPort();
public:
	SinSerial(QObject *parent = 0);
	~SinSerial();

	QStringList getEnablePorts();
	QStringList getEnableRates();
	bool isOPen();
	int openCom(QString comName, QString rate);
	void closeCom();
};

typedef CSingleton<SinSerial> sinserial;
#endif