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
		readData = sinserialSingle::getInstance().getReadData();
		
		if (!readData.isEmpty())
		{
			qDebug() << " sinSerial::getReadData to Log" << readData << "currentThreadId:" << QThread::currentThread();
			
			sinTaskQueueSingle::getInstance().pushBackReadData(readData);
		}

	}
	//return readData;
}