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

void SinTaskQueue::pushBackCapData(CapData data)
{
	m_pCapData.enqueue(data);
	emit signalCapData();
}

CapData SinTaskQueue::popBackCapData()
{
	CapData data = m_pCapData.dequeue();
	return data;
}
int SinTaskQueue::getCapDataSize()
{
	return m_pCapData.size();
}