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
protected:
private:
};

#endif
