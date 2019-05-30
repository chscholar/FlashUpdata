#ifndef SINSERIALREADTHREAD_H
#define SINSERIALREADTHREAD_H
#include <QObject>
#include "SinSerial.h"


class SinSerialReadThread : public QObject
{
	Q_OBJECT
public:
	SinSerialReadThread(QObject *parent = 0);
	~SinSerialReadThread();
	void start();
	void stop();
	QByteArray getReadData();
protected:

private:
};

#endif
