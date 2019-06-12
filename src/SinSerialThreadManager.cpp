#include "SinSerialThreadManager.h"
#include <QThread>
#include "SinSerial.h"

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

	connect(m_pReadThread, SIGNAL(started()), m_pReadWork, SLOT(getReadData()));
	connect(m_pReadThread, SIGNAL(finished()), m_pReadWork, SLOT(deleteLater()));
	m_pReadWork->moveToThread(m_pReadThread);

	//读线程默认开启
	m_pReadWork->start();
	m_pReadThread->start();

	connect(&sinserialSingle::getInstance(), SIGNAL(signalWriteData(QString,QByteArray)), this, SLOT(slotWriteData(QString,QByteArray)));


}

SinSerialThreadManager::~SinSerialThreadManager()
{

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

void SinSerialThreadManager::slotWriteData(QString qstrLog,QByteArray byteData)
{
	m_pWriteWork->sendWriteDataPack(qstrLog,byteData);
}