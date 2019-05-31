#include "SinTaskQueue.h"

SinTaskQueue::SinTaskQueue(QObject *parent)
	:QObject(parent)
{

	m_pReadData.clear();
}

SinTaskQueue::~SinTaskQueue()
{

}

void SinTaskQueue::pushBackReadData(QByteArray data)
{
	m_pReadData.enqueue(data);
	emit signalReadData();
}

QByteArray SinTaskQueue::popBackReadData()
{
	QByteArray data = m_pReadData.dequeue();
	return data;
}