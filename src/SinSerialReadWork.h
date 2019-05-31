#ifndef SINSERIALREADWORK_H
#define SINSERIALREADWORK_H
#include <QObject>
#include "SinSerial.h"


class SinSerialReadWork : public QObject
{
	Q_OBJECT
public:
	SinSerialReadWork(QObject *parent = 0);
	~SinSerialReadWork();
	void start();
	void stop();
	QByteArray getReadData();
protected:
	bool m_bIsRun;

private:
};

#endif
