#include "SinSerialReadWork.h"
#include <QDebug>
SinSerialReadWork::SinSerialReadWork(QObject *parent)
	:QObject(parent)
{
	m_bIsRun = false;
	connect(&sinserialSingle::getInstance(), SIGNAL(readyRead()), this, SLOT(getReadData()));
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
	}
	//return readData;
}