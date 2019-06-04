#ifndef	SINTASKQUEUE_H
#define  SINTASKQUEUE_H
#include <QObject>
#include "QList"
#include <QQueue>
#include "CSingleTon.h"
#include "SinNetWorkCapThread.h"

class SinTaskQueue : public QObject
{
	Q_OBJECT
public:
	SinTaskQueue(QObject *parent = 0);
	~SinTaskQueue();
	void pushBackReadData(QByteArray data);
	QByteArray popBackReadData();

	void pushBackCapData(CapData data);
	CapData popBackCapData();
	int getCapDataSize();
private:

	QQueue<QByteArray> m_pReadData;
	QQueue<CapData> m_pCapData;
	public slots:
signals :
	void signalReadData();
	void signalWriteNextData();
	void signalCapData();
	//void signalWriteNextData();
};

typedef CSingleton<SinTaskQueue> sinTaskQueueSingle;
#endif
