#include "SinSerialThreadManager.h"
#include <QThread>

SinSerialThreadManager::SinSerialThreadManager(QObject *parent)
	:QObject(parent)
{
	m_pReadWork = new SinSerialReadWork();
	m_pWriteWork = new SinSerialWriteWork();
	
	m_pReadThread = new QThread();
	m_pWriteThread = new QThread();

	
	connect(m_pWriteThread, SIGNAL(started()), m_pWriteWork, SLOT(sendData()));
	connect(m_pWriteThread, SIGNAL(finished()), m_pWriteWork, SLOT(deleteLater()));
	m_pWriteWork->moveToThread(m_pWriteThread);

	connect(m_pReadThread, SIGNAL(started()), m_pReadWork, SLOT(doWork()));
	connect(m_pReadThread, SIGNAL(finished()), m_pReadWork, SLOT(deleteLater()));
	m_pReadWork->moveToThread(m_pReadThread);
}

SinSerialThreadManager::~SinSerialThreadManager()
{

}

void SinSerialThreadManager::start()
{
	m_pReadWork->start();
	m_pWriteWork->start();

	m_pWriteThread->start();
	m_pReadThread->start();
}

void SinSerialThreadManager::stop()
{
	m_pReadWork->stop();
	m_pWriteWork->stop();

}

void SinSerialThreadManager::setWriteData(QList<QList<QByteArray>> fileListData)
{
	m_pWriteWork->setWriteData(fileListData);
}

void SinSerialThreadManager::getReadData()
{
	m_pReadWork->getReadData();
}

void SinSerialThreadManager::sendData()
{
	m_pWriteWork->sendData();
}

//握手协议，PC读取串口指定信息，并发送回应
void SinSerialThreadManager::handShake()
{
	QByteArray readData =  m_pReadWork->getReadData();

	m_pWriteWork->sendData();
}