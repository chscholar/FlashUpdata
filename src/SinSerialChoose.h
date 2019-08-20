#ifndef SINSERIALCHOOSE_H
#define SINSERIALCHOOSE_H

#include <QWidget>
#include <QSerialPort>
#include <QByteArray>
#include "CommonHeader.hpp"

class SinSerialChoose : public QWidget
{
	Q_OBJECT
public:
	SinSerialChoose(QWidget *parent = nullptr);
	~SinSerialChoose();
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

	QByteArray m_pReadData;
	QSerialPort *m_pSerialPort;
	ReqInterrFace m_pCurrentReq;
	bool m_bHandOk;
private:
	
	public slots:
	void slotReadData();
};

#endif // !SINSERIALCHOOSE_H
