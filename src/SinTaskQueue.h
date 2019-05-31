#ifndef	SINTASKQUEUE_H
#define  SINTASKQUEUE_H
#include <QObject>
#include "QList"
#include <QQueue>
#include "CSingleTon.h"


class SinTaskQueue : public QObject
{
	Q_OBJECT
public:
	SinTaskQueue(QObject *parent = 0);
	~SinTaskQueue();
	void pushBackReadData(QByteArray data);
	QByteArray popBackReadData();
private:

	QQueue<QByteArray> m_pReadData;
	public slots:
signals :
	void signalReadData();
};

typedef CSingleton<SinTaskQueue> sinTaskQueueSingle;
#endif
