#ifndef		SINSERIAL_H
#define  SINSERIAL_H
#include <qDebug>
#include <QSerialPort>
#include <QStringList>
#include "CSingleTon.h"
#include "QObject"
#include "SinByte.h"
#include <QList>
#include <QTimer>

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
	ReqInterrFace indexToReqHeader(QByteArray data, int index);
	QByteArray reqToByteArray(ReqInterrFace req);

	QMap<int, QString> portMap;
	QMap<int, QString> rateMap;
	QMap<int, QString> dataMap;
	QMap<int, QString> stopMap;
	QMap<int, QString> parityMap;
	QMap<int, QString> flowMap;
	bool m_bIsUpLoadTrans;
	QString m_pUpFileSavePath;
	QList<ReqInterrFace> m_pWriteData;
	QByteArray m_pReadData;
	QByteArray m_pReciveData;
	QTimer *m_pErrorTimer;
	ReqInterrFace m_pErrorPreReq;
	QString m_pStrErrorLog;
public:
	SinSerial(QObject *parent = 0);
	~SinSerial();

	QStringList getEnablePorts();
	QStringList getEnableRates();
	QStringList getDataBits();
	QStringList getStopBits();
	QStringList getFlowControl();
	QStringList getParity();
	void sendData(ReqInterrFace req, QString strLogPrefix, QByteArray command, int index = 0, QByteArray dataError = FILE_OK);
	void sendData(QString strLog,QByteArray bytedata);
	
	bool isOPen();
	int openCom(int portIndex,int rateIndex,int flowIndex,int dataIndex,int stopIndex,int parityIndex);
	void closeCom();
	void clearError();
	QList<int> indexOfHeader(QString strSrc,QByteArray header);
	bool isCompare(QByteArray src, QByteArray dest);
	bool isCompare(QByteArray src, int nError);
	QSerialPort::SerialPortError getError();
	void setTransTypeWriteData(bool isUplodType, QList<QList<QByteArray>> writeData);
	void handleTransError(QByteArray dataError,int currentIndex = 0);
	ReqInterrFace findFirstReqFromReciveData(QByteArray reciveData);
	void handleUploadError(QByteArray dataError);
	QByteArray getValueFromData (QByteArray data, int findIndex, int offset, int length);
	void delReqFromReadBuffer(QByteArray readData,ReqInterrFace req);
	public slots:
	void slotTest();
signals :
	void signalWriteData(QString  strLog,QByteArray bytedata);
	void signalReadData();
public slots :
	void slotGetReadData();
	void slotUpdateTransType(bool,QString);
	void slotTimerOut();
};

typedef CSingleton<SinSerial> sinserialSingle;
#endif