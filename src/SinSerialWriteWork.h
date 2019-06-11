#ifndef SINSERIALWRITEWORK_H
#define SINSERIALWRITEWORK_H
#include <QObject>
#include "SinSerial.h"

class SinSerialWriteWork:public QObject
{
	Q_OBJECT
public:
	SinSerialWriteWork(QObject *parent = 0);
	~SinSerialWriteWork();
	void start();
	void stop();
	void setWriteData(QList<QList<QByteArray>> fileListData);
	ReqInterrFace startUploadReq();
	void sendWriteDataPack(QByteArray byteData);
protected:
	//QList<QList<QByteArray>> m_pWriteData;
	QList<ReqInterrFace> m_pWriteData;
	int m_nCurrentWriteIndex;
	bool m_bIsRun;
public slots:
	void sendData();

private:
};

#endif
