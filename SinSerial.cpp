#include "SinSerial.h"
#include <QSerialPortInfo>
#include <QSettings>
#include <QSerialPortInfo>

SinSerial::SinSerial(QObject *parent):
QObject(parent)
, serialPort(nullptr)
{

}

SinSerial::~SinSerial()
{

}

QSerialPort* SinSerial::getSerialPort()
{
	if (serialPort == nullptr)
	{
		serialPort = new QSerialPort();
	}
	return serialPort;
}

QStringList SinSerial::getEnablePorts()
{
	QStringList portList;
	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
	for (int i = 0; i < ports.size();i++)
	{
		QString strPortName = ports.at(i).portName();
		portList.push_back(strPortName);
	}

	return portList;
}

QStringList SinSerial::getEnableRates()
{
	QStringList rateList;

	QList<qint32> rates = QSerialPortInfo::standardBaudRates();
	for (int i = 0; i < rates.size();i++)
	{
		QString strRate = QString::number(rates.at(i));
		rateList.push_back(strRate);
	}
	return rateList;
}

bool SinSerial::isOPen()
{
	return getSerialPort()->isOpen();
}

int  SinSerial::openCom(QString comName, QString rate)
{
	getSerialPort()->setPortName(comName);
	if (getSerialPort()->open(QIODevice::ReadWrite))
	{
		getSerialPort()->setBaudRate(rate.toInt());
		getSerialPort()->setDataBits(QSerialPort::Data8);
		getSerialPort()->setParity(QSerialPort::NoParity);
		getSerialPort()->setFlowControl(QSerialPort::NoFlowControl);
		getSerialPort()->setStopBits(QSerialPort::OneStop);
		return 0;
	}
	else
	{
		return -1 ;
	}
}

void SinSerial::closeCom()
{
	getSerialPort()->close();
}