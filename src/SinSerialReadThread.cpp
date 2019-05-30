#include "SinSerialReadThread.h"

SinSerialReadThread::SinSerialReadThread(QObject *parent)
	:QObject(parent)
{

}

SinSerialReadThread::~SinSerialReadThread()
{

}

void SinSerialReadThread::start()
{

}

void SinSerialReadThread::stop()
{

}

QByteArray SinSerialReadThread::getReadData()
{
	QByteArray readData = sinserialSingle::getInstance().getReadData();
	return readData;
}