#include "SinSerial.h"
#include <QSerialPortInfo>
#include <QSettings>
#include <QSerialPortInfo>
#include <QDataStream>
#include <QDebug>
#include <QThread>

SinSerial::SinSerial(QObject *parent):
QObject(parent)
, serialPort(nullptr)
{
}

SinSerial::~SinSerial()
{

}

QSerialPort* SinSerial::getSerialPort()
{
	if (serialPort == nullptr)
	{
		serialPort = new QSerialPort();
	}
	return serialPort;
}

QStringList SinSerial::getEnablePorts()
{
	QStringList portList;
	QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
	for (int i = 0; i < ports.size();i++)
	{
		QString strPortName = ports.at(i).portName();
		portList.push_back(strPortName);
	}
	

	int index = 1;
	portMap.clear();
	for (int j = 0; j < portList.size(); j++)
	{
		portMap.insert(index++, portList.at(j));
	}

	return portList;
}

QStringList SinSerial::getEnableRates()
{
	QStringList rateList;

	QList<qint32> rates = QSerialPortInfo::standardBaudRates();
	for (int i = 0; i < rates.size();i++)
	{
		QString strRate = QString::number(rates.at(i));
		rateList.push_back(strRate);
	}

	int index = 1;
	rateMap.clear();
	for (int j = 0; j < rateList.size(); j++)
	{
		rateMap.insert(index++, rateList.at(j));
	}

	return rateList;
}

QStringList SinSerial::getDataBits()
{
	QStringList dataBitList;
	/*
	  Data5 = 5,
	  Data6 = 6,
	  Data7 = 7,
	  Data8 = 8,
	*/

	dataBitList << "5" << "6" << "7" << "8";

	int index = 1;
	dataMap.clear();
	for (int j = 0; j < dataBitList.size(); j++)
	{
		dataMap.insert(index++, dataBitList.at(j));
	}

	return dataBitList;
}

QStringList SinSerial::getStopBits()
{
	/*
	 OneStop = 1,
	 OneAndHalfStop = 3,
	 TwoStop = 2,
	 UnknownStopBits = -1
	*/
	QStringList stopBitsList;
	stopBitsList << "1" << "2" << "3";

	int index = 1;
	stopMap.clear();
	for (int j = 0; j < stopBitsList.size(); j++)
	{
		stopMap.insert(index++, stopBitsList.at(j));
	}

	return stopBitsList;
}

QStringList SinSerial::getFlowControl()
{
	/*
	  NoFlowControl,
	  HardwareControl,
	  SoftwareControl,
	  UnknownFlowControl = -1
	*/
	QStringList flowControls;
	flowControls << "无流控" << "硬件流控" << "软件流控";

	flowMap.clear();
	flowMap.insert(1, "0");
	flowMap.insert(2, "1");
	flowMap.insert(3, "2");

	return flowControls;
}

QStringList SinSerial::getParity()
{
	QStringList parityList;
	/*
	  NoParity = 0,
	  EvenParity = 2,
	  OddParity = 3,
	  SpaceParity = 4,
	  MarkParity = 5,
	  UnknownParity = -1
	*/

	parityList << "无校验" << "偶校验" <<  "奇校验" << "校验位为0" << "校验位为1";

	parityMap.clear();
	parityMap.insert(1, "0");
	parityMap.insert(2, "2");
	parityMap.insert(3, "3");
	parityMap.insert(4, "4");
	parityMap.insert(5, "5");

	return parityList;
}

bool SinSerial::isOPen()
{
	return getSerialPort()->isOpen();
}

QString SinSerial::findKeyFromMap(QMap<int, QString> fmap, int key)
{

	QMap<int, QString>::iterator it;
	for (it = fmap.begin(); it != fmap.end();it ++)
	{
		if (it.key() == key )
		{
			return it.value();
		}
	}
	return "";
}

int  SinSerial::openCom(int portIndex, int rateIndex, int flowIndex, int dataIndex, int stopIndex, int parityIndex)
{
	
	

	QString portName = findKeyFromMap(portMap, portIndex);
	QString rateValue = findKeyFromMap(rateMap,rateIndex);
	QString dataValue = findKeyFromMap(dataMap, dataIndex);
	QString parityValue = findKeyFromMap(parityMap, parityIndex);
	QString flowValue = findKeyFromMap(flowMap, flowIndex);
	QString stopValue = findKeyFromMap(stopMap, stopIndex);

	getSerialPort()->setPortName(portName);
	if (getSerialPort()->open(QIODevice::ReadWrite))
	{
		getSerialPort()->setBaudRate(rateValue.toInt());
		getSerialPort()->setDataBits(QSerialPort::DataBits(dataValue.toInt()));
		getSerialPort()->setParity(QSerialPort::Parity(parityValue.toInt()));
		getSerialPort()->setFlowControl(QSerialPort::FlowControl(flowValue.toInt()));
		getSerialPort()->setStopBits(QSerialPort::StopBits(stopValue.toInt()));
		return 0;
	}
	else
	{
		return -1 ;
	}
}

void SinSerial::closeCom()
{
	getSerialPort()->close();
}

void SinSerial::clearError()
{
	getSerialPort()->clearError();
}

QSerialPort::SerialPortError SinSerial::getError()
{
	return getSerialPort()->error();
}

void SinSerial::sendData(ReqInterrFace req)
{
	QByteArray writeByte;
	writeByte = req.Header + req.Length + req.Command + req.BinFileId + req.BinFileSize + req.TransId + req.TransSeqNum + req.DataLength + req.DataCRC + req.data + req.Padding;

	qint64 writebyte =  getSerialPort()->write(writeByte);
	qDebug() << "sinSerial::sendData" << writeByte << "currentThreadId:" << QThread::currentThread();
}

void SinSerial::sendData(QByteArray bytedata)
{
	QByteArray writeData = QByteArray::fromHex(bytedata);
	int writeByte = getSerialPort()->write(writeData);
	qDebug() << "sinSerial::sendData" << bytedata << "currentThreadId:" << QThread::currentThread();
}

QByteArray SinSerial::reqToByteArray(ReqInterrFace req)
{
	QByteArray writeByte;
	writeByte = req.Header + req.Length + req.Command + req.BinFileId + req.BinFileSize + req.TransId + req.TransSeqNum + req.DataLength + req.DataCRC + req.data + req.Padding;
	return writeByte;
}


ReqInterrFace SinSerial::byteToReq(QByteArray data){

	ReqInterrFace req;
	req.Header = data.mid(0, 8);
	req.Length = data.mid(8, 4);
	req.Command = data.mid(12,4);
	req.BinFileId = data.mid(16, 8);
	req.BinFileSize = data.mid(24,8);
	req.TransId = data.mid(32, 8);
	req.TransSeqNum = data.mid(40, 8);
	req.DataLength = data.mid(48, 4);
	req.DataCRC = data.mid(52, 4);

	int DataSize = QByteArray::fromHex(req.DataLength).toInt();
	req.data = data.mid(56, DataSize);

	return req;
}

void SinSerial::slotTest()
{

}

ReqInterrFace SinSerial::indexToReq(QByteArray data, int Index)
{
	ReqInterrFace req;
	req.Header = data.mid(Index, 8);
	req.Length = data.mid(Index+8, 4);
	req.Command = data.mid(Index + 12, 4);
	req.BinFileId = data.mid(Index + 16, 8);
	req.BinFileSize = data.mid(Index + 24, 8);

	req.TransId = data.mid(Index + 32, 8);
	req.TransSeqNum = data.mid(Index + 40, 8);
	req.DataLength = data.mid(Index + 48, 4);
	req.DataCRC = data.mid(Index + 52, 4);

	int DataSize = QByteArray::fromHex(req.DataLength).toInt();
	req.data = data.mid(Index + 56, DataSize);

	return req;
}

QByteArray SinSerial::getReadData()
{
	QByteArray header = "eba846b9";
	QByteArray handle = "0001";
	QByteArray readData;
	//if (getSerialPort()->canReadLine())
	if (getSerialPort()->bytesAvailable())
	{
		
		readData = getSerialPort()->readAll();
		
		readData = readData.trimmed();
		if (!readData.isEmpty())
		{
			qDebug() << " sinSerial::getReadData to Log" << readData << "currentThreadId:" << QThread::currentThread();

			QString qstrReadData = readData.toHex();
			int indexof = qstrReadData.indexOf(header);
			ReqInterrFace req = indexToReq(readData.toHex(), indexof);

			bool isValid = req.Header == header ? true : false;
			if (isValid)
			{
				if (req.Command == handle) //握手协议
				{
					//qDebug() << " reciveUEHandle" << readData.toHex() << "currentThreadId:" << QThread::currentThread();
					ReqInterrFace handleReq;
					handleReq.Header = header;
					handleReq.BinFileId = req.BinFileId;
					handleReq.BinFileSize = req.BinFileSize;
					handleReq.Command = "8001";
					handleReq.data = req.data;
					handleReq.DataCRC = req.DataCRC;
					handleReq.DataLength = req.DataLength;
					handleReq.Length = req.Length;
					handleReq.TransId = req.TransId;
					handleReq.Padding = "00000000";
					QByteArray writeByte = reqToByteArray(handleReq);
					emit signalSendHandleShark(writeByte);
					//sendData(handleReq);
					//emit signalHandSharkOver(); // 完成握手
				}
				if (req.Command == "0002") //下载请求
				{

				}

				return readData.toHex();

			}


		}
	}
	
	
	return readData;

}
