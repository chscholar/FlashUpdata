#ifndef		SINSERIAL_H
#define  SINSERIAL_H
#include <qDebug>
#include <QSerialPort>
#include <QStringList>
#include "CSingleTon.h"
#include "QObject"
#include "SinByte.h"
#include <QList>

#pragma execution_character_set("utf-8")
class SinSerial :public QObject
{
	Q_OBJECT
private:

	QSerialPort *serialPort;
	QSerialPort *getSerialPort();
	QString findKeyFromMap(QMap<int,QString> fmap,int key);

	ReqInterrFace byteToReq(QByteArray data);
	ReqInterrFace indexToReq(QByteArray data, int Index);
	QByteArray reqToByteArray(ReqInterrFace req);
	//QByteArray fromHex(QByteArray byteData);

	QMap<int, QString> portMap;
	QMap<int, QString> rateMap;
	QMap<int, QString> dataMap;
	QMap<int, QString> stopMap;
	QMap<int, QString> parityMap;
	QMap<int, QString> flowMap;
public:
	SinSerial(QObject *parent = 0);
	~SinSerial();

	QStringList getEnablePorts();
	QStringList getEnableRates();
	QStringList getDataBits();
	QStringList getStopBits();
	QStringList getFlowControl();
	QStringList getParity();
	void sendData(ReqInterrFace req);
	void sendData(QByteArray bytedata);
	QByteArray getReadData();
	bool isOPen();
	int openCom(int portIndex,int rateIndex,int flowIndex,int dataIndex,int stopIndex,int parityIndex);
	void closeCom();
	void clearError();
	QList<int> indexOfHeader(QString strSrc,QByteArray header);
	QSerialPort::SerialPortError getError();
	public slots:
	void slotTest();
signals :
	void signalHandSharkOver();
	void signalSendHandleShark(QByteArray bytedata);
};

typedef CSingleton<SinSerial> sinserialSingle;
#endif