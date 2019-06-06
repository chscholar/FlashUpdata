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
	//emit signalWriteNextData();
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


void SinTaskQueue::pushBackPktHeaderData(const pcap_pkthdr* data){
	m_pPktHeader.push_back(const_cast<pcap_pkthdr*> (data));
}

pcap_pkthdr* SinTaskQueue::popIndexPktHeaderData(int index)
{
	return m_pPktHeader.at(index);
}


void SinTaskQueue::pushBackPktDataData(const u_char *data){
	m_pPktData.push_back(const_cast<u_char *> (data));
}


u_char * SinTaskQueue::popIndexPktDataData(int index){
	return m_pPktData.at(index);
}