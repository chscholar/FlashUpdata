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
	bool m_bIsUpLoadTrans;
	QList<ReqInterrFace> m_pWriteData;
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
	void sendData(QString strLog,QByteArray bytedata);
	QByteArray getReadData();
	bool isOPen();
	int openCom(int portIndex,int rateIndex,int flowIndex,int dataIndex,int stopIndex,int parityIndex);
	void closeCom();
	void clearError();
	QList<int> indexOfHeader(QString strSrc,QByteArray header);
	bool isCompare(QByteArray src, QByteArray dest);
	bool isCompare(QByteArray src, int nError);
	QSerialPort::SerialPortError getError();
	void setTransTypeWriteData(bool isUplodType, QList<QList<QByteArray>> writeData);
	void fillWriteStruct(ReqInterrFace req, QString strLogPrefix, QByteArray command, QByteArray BinFileId, QByteArray BinFileSize, QByteArray TransId, QByteArray TransSeqNum,QByteArray dataCRC,QByteArray data);
	public slots:
	void slotTest();
signals :
	void signalWriteData(QString  strLog,QByteArray bytedata);
};

typedef CSingleton<SinSerial> sinserialSingle;
#endif