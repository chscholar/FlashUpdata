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
	
protected:
	bool m_bIsRun;
public slots:
	void getReadData();

private:
};

#endif
