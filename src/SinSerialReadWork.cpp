#include "SinSerialReadWork.h"
#include <QDebug>
#include "SinTaskQueue.h"

SinSerialReadWork::SinSerialReadWork(QObject *parent)
	:QObject(parent)
{
	m_bIsRun = false;

	m_pSinSerial = &sinserialSingle::getInstance();
	connect(m_pSinSerial, SIGNAL(readyRead()), this, SLOT(getReadData()));
}

SinSerialReadWork::~SinSerialReadWork()
{

}

void SinSerialReadWork::start()
{
	m_bIsRun = true;
}

void SinSerialReadWork::stop()
{
	m_bIsRun = false;
}

void SinSerialReadWork::getReadData()
{
	QByteArray readData;

	while (m_bIsRun)
	{
		QThread::msleep(100);
		//sinserialSingle::getInstance().clearError();
		readData = sinserialSingle::getInstance().getReadData();
		QSerialPort::SerialPortError sError = sinserialSingle::getInstance().getError();

		if (sError != 0)
		{
			qDebug() << "readError :" << sError;
		}

		if (!readData.isEmpty())
		{
			
			sinTaskQueueSingle::getInstance().pushBackReadData(readData);
		}

	}
	//return readData;
}