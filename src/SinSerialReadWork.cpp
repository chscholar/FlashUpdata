#include "SinSerialReadWork.h"
SinSerialReadWork::SinSerialReadWork(QObject *parent)
	:QObject(parent)
{
	m_bIsRun = false;
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

QByteArray SinSerialReadWork::getReadData()
{
	QByteArray readData;
	if (m_bIsRun)
	{
		readData = sinserialSingle::getInstance().getReadData();
	}
	return readData;
}