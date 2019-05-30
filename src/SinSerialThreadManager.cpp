#include "SinSerialThreadManager.h"

SinSerialThreadManager::SinSerialThreadManager(QObject *parent)
	:QObject(parent)
{
	m_pRedeThread = new SinSerialReadThread();
	m_pWriteThread = new SinSerialWriteThread();
}

SinSerialThreadManager::~SinSerialThreadManager()
{

}

void SinSerialThreadManager::start()
{
	m_pWriteThread->start();
	m_pRedeThread->start();
}

void SinSerialThreadManager::stop()
{
	m_pWriteThread->stop();
	m_pRedeThread->stop();
}