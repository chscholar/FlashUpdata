#include "SinTaskQueue.h"

SinTaskQueue::SinTaskQueue(QObject *parent)
	:QObject(parent)
{

	m_pReadData.clear();
}

SinTaskQueue::~SinTaskQueue()
{

}

//有数据可以读
void SinTaskQueue::pushBackReadData(QByteArray data)
{
	m_pReadData.enqueue(data);
	emit signalReadData();
}

//可以继续 写下一个数据
QByteArray SinTaskQueue::popBackReadData()
{
	QByteArray data = m_pReadData.dequeue();
	emit signalWriteNextData();
	return data;
}