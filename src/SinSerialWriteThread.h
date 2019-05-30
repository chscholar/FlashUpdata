#ifndef SINSERIALWRITETHREAD_H
#define SINSERIALWRITETHREAD_H
#include <QObject>
#include "SinSerial.h"


class SinSerialWriteThread:public QObject
{
	Q_OBJECT
public:
	SinSerialWriteThread(QObject *parent = 0);
	~SinSerialWriteThread();
	void start();
	void stop();
	void setWriteData(QList<QList<QByteArray>> fileListData);
	void sendData();
protected:

	QList<QList<QByteArray>> m_pWriteData;
private:
};

#endif
