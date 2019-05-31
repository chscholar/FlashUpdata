#include "SinTaskQueue.h"

SinTaskQueue::SinTaskQueue(QObject *parent)
	:QObject(parent)
{

	m_pReadData.clear();
}

SinTaskQueue::~SinTaskQueue()
{

}

//�����ݿ��Զ�
void SinTaskQueue::pushBackReadData(QByteArray data)
{
	m_pReadData.enqueue(data);
	emit signalReadData();
}

//���Լ��� д��һ������
QByteArray SinTaskQueue::popBackReadData()
{
	QByteArray data = m_pReadData.dequeue();
	emit signalWriteNextData();
	return data;
}