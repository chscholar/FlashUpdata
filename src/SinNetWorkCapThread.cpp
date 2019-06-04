#include "SinNetWorkCapThread.h"

SinNetWorkCapThread:: SinNetWorkCapThread(QObject *parent)
	:QObject(parent)
{
	m_pThread = new QThread();
	connect(m_pThread, SIGNAL(started()), this, SLOT(startCap()));
	connect(m_pThread, SIGNAL(finished()), this, SLOT(deleteLater()));
	this->moveToThread(m_pThread);


}

SinNetWorkCapThread::~SinNetWorkCapThread()
{

}


void SinNetWorkCapThread::startThread()
{
	m_pThread->start();
}

void SinNetWorkCapThread::stopThread()
{
	m_pThread->quit();
}

void SinNetWorkCapThread::startCap()
{
	int a = 1;
}