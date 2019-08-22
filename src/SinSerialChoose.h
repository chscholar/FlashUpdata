#ifndef SINSERIALCHOOSE_H
#define SINSERIALCHOOSE_H

#include <QWidget>
#include <QSerialPort>
#include <QByteArray>
#include "CommonHeader.hpp"
#include <QThread>
#include <QTimer>
#pragma execution_character_set("utf-8")
struct SerialConfig
{
	QString portName;
	QString rateValue;
	QString flowValue;
	QString dataValue;
	QString stopValue;
	QString parityValue;
};


class SerialChooseOperation : public QObject
{
	Q_OBJECT
public:
	SerialChooseOperation(QObject *parent = nullptr);
	~SerialChooseOperation();
public:
	int openCom(QString portName, QString rate, QString flow, QString dataBit, QString stopBit, QString parity);
	QSerialPort *chooseSerial();
	bool isValidHandPackage();
	ReqInterrFace findFirstReqFromReciveData(QByteArray reciveData);
	QByteArray getValueFromData(QByteArray data, int findIndex, int offset, int length);
	ReqInterrFace indexToReqHeader(QByteArray data, int Index);
	ReqInterrFace indexToReq(QByteArray data, int Index);
	void connectSerialWithHandPack();
	bool isCompare(QByteArray src, QByteArray dest);
	QByteArray reqToByteArray(ReqInterrFace req);
	void closeCom();
	QSerialPort *getSerialPort();
	QString getPortName();

	QByteArray m_pReadData;
	QSerialPort *m_pSerialPort;
	ReqInterrFace m_pCurrentReq;
	bool m_bHandOk;
	QString m_qStrPortName;
private:
	
public slots:
	void slotReadData();
};

class SinSerialChoose : public QObject
{
	Q_OBJECT
public:
	SinSerialChoose(QObject *parent = NULL);
	~SinSerialChoose();
	void setSerialConfig(QVector<SerialConfig> config);
	
protected:
private:
	QVector<SerialConfig> m_vSerialConfig;
	QVector<SerialChooseOperation*> m_vSerialList;

	QTimer *m_pChooseTimer;

public slots:
	void slotRun();
	void slotTimerOut();
signals :
	void signalsHandOkSerial(QString portName);
};


#endif // !SINSERIALCHOOSE_H
