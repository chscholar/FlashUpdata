#include "SinSerialThreadManager.h"
#include <QThread>

SinSerialThreadManager::SinSerialThreadManager(QObject *parent)
	:QObject(parent)
{
	m_pReadThread = new SinSerialReadThread();
	m_pWriteThread = new SinSerialWriteThread();

	QThread workThread;

	m_pReadThread->moveToThread(&workThread);
	m_pWriteThread->moveToThread(&workThread);
	
}

SinSerialThreadManager::~SinSerialThreadManager()
{

}

void SinSerialThreadManager::start()
{
	m_pWriteThread->start();
	m_pReadThread->start();
}

void SinSerialThreadManager::stop()
{
	m_pReadThread->stop();
	m_pReadThread->stop();
}

void SinSerialThreadManager::setWriteData(QList<QList<QByteArray>> fileListData)
{
	m_pWriteThread->setWriteData(fileListData);
}

void SinSerialThreadManager::getReadData()
{
	m_pReadThread->getReadData();
}

//握手协议，PC读取串口指定信息，并发送回应
void SinSerialThreadManager::handShake()
{
	QByteArray readData =  m_pReadThread->getReadData();

	m_pWriteThread->sendData();
}