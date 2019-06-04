#ifndef SINNETWORKCAPTHREAD_H
#define SINNETWORKCAPTHREAD_H
#include <QObject>
#include "CSingleTon.h"
#include <QThread>

class SinNetWorkCapThread : public QObject
{
	Q_OBJECT
public:
	SinNetWorkCapThread(QObject *parent = 0);
	~SinNetWorkCapThread();
	void startThread();
	void stopThread();
protected:
private:
	QThread *m_pThread;
	public slots:
	void startCap();
};

typedef CSingleton<SinNetWorkCapThread> SinNetWorkCapSingle;
#endif
