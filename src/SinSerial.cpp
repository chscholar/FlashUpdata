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
		serialPort->setReadBufferSize(0);
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
	writeByte = QByteArray::fromHex(req.Header + req.Length + req.Command + req.BinFileId + req.BinFileSize + req.TransId + req.TransSeqNum + req.DataLength + req.DataCRC + req.data + req.Padding);

	qint64 writebyte =  getSerialPort()->write(writeByte);
	qDebug() << "sinSerial::sendData" << writeByte << "currentThreadId:" << QThread::currentThread();
}

void SinSerial::sendData(QString strLog,QByteArray bytedata)
{
	QByteArray writeData = QByteArray::fromHex(bytedata);
	int writeByte = getSerialPort()->write(writeData);
	qDebug() << strLog << bytedata << "currentThreadId:" << QThread::currentThread();
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

bool SinSerial::isCompare(QByteArray src, QByteArray dest)
{
	QString strSrc = src;
	QString strDest = dest;

	int result = strSrc.compare(strDest, Qt::CaseInsensitive);
	return result == 0;
}

bool SinSerial::isCompare(QByteArray byteData, int nError)
{
	bool ok;
	QString strData = byteData.data();
	int data = strData.toInt(&ok, 16);

	return data == nError;
	
}

ReqInterrFace SinSerial::indexToReq(QByteArray data, int Index)
{
	bool ok;
	ReqInterrFace req;
	req.Header = data.mid(Index, 8);
	req.Length = data.mid(Index+8, 4);
	QString qstrLength = req.Length;
	int Length = qstrLength.toInt(&ok, 16);

	req.Command = data.mid(Index + 12, 4);
	req.BinFileId = data.mid(Index + 16, 8);
	req.BinFileSize = data.mid(Index + 24, 8);

	req.TransId = data.mid(Index + 32, 8);
	req.TransSeqNum = data.mid(Index + 40, 8);
	req.DataLength = data.mid(Index + 48, 4);
	req.DataCRC = data.mid(Index + 52, 4);

	
	QString qstrDataLength = req.DataLength;
	int dataLength =  qstrDataLength.toInt(&ok, 16);
	
	if (isCompare(req.Command, MSG_CMD_UPLOADFILE_DATA))
	{
		int a = 1;
	}
	req.data = data.mid(Index + 56, dataLength * 2);

	return req;
}

QList<int> SinSerial::indexOfHeader(QString strSrc,QByteArray header)
{
	QList<int> result;
	QString qstrMid;
	int indexOf = 0;
	
	for (int i = 0; i < strSrc.length();i++)
	{
		strSrc = strSrc.mid(i + indexOf);
		indexOf = strSrc.indexOf(header, 0, Qt::CaseInsensitive);
		if (indexOf != -1)
		{
			result.push_back( i + indexOf);
		}
	}
	
	return result;
}


QByteArray SinSerial::getReadData()
{;
	QByteArray readData;
	//if (getSerialPort()->canReadLine())
	
	if (getSerialPort()->bytesAvailable())
	{
		//QThread::msleep(500);
		readData = getSerialPort()->readAll();
		QString qstrTest = readData.toHex().data();
		if (qstrTest.contains("0216"))
		{
			int a = 1;
		}
		readData = readData.trimmed();
		QString qstrtest1 = readData.toHex().data();
		if (!readData.isEmpty())
		{
			qDebug() << " sinSerial::getReadData to Log" << readData << "currentThreadId:" << QThread::currentThread();

			QString qstrReadData = readData.toHex().data();

			QList<int> indexofList = indexOfHeader(qstrReadData, MSG_PROTO_HEADER_TAG);
			
			if (indexofList.size() <= 0) return readData;
			
			for (int i = 0; i < indexofList.size();i++)
			{
				ReqInterrFace req = indexToReq(readData.toHex(), indexofList.at(i));

				bool isValid = isCompare(req.Header, MSG_PROTO_HEADER_TAG);
				if (isValid)
				{
					if (isCompare(req.Command,MSG_CMD_HANDSHAKE_SYN)) //握手协议
					{
						qDebug() << " reciveUE Handle :" << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();
						ReqInterrFace handleReq;
						handleReq = req;
						handleReq.Command = MSG_CMD_HANDSHAKE_SYNARK;
						handleReq.setLength();

						QByteArray handByteData = reqToByteArray(handleReq);
						emit signalWriteData("pc send respon Handle",handByteData);
						
					}
					if (isCompare(req.Command,MSG_CMD_HANDSHAKE_ARK)) //握手成功
					{
						qDebug() << "reciveUE Handle Ok :" << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();
						
						ReqInterrFace handleReq;
						handleReq = req;
						handleReq.Command = MSG_CMD_UPLOADFILE_REQ;
						handleReq.BinFileId = "0000000B";
						handleReq.setDataLength();
						handleReq.setLength();
					
						QByteArray handByteData = reqToByteArray(handleReq);
						emit signalWriteData("pc send upload  req",handByteData);
					}

					if (isCompare(req.Command,MSG_CMD_UPLOADFILE_REQ_RSP)) //upload_Req
					{
						qDebug() << "reciveUE upload Req: " << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();

						if (isCompare(req.data,FILE_OK))
						{
							int a = 1;
							
						}
					}

					if (isCompare(req.Command,MSG_CMD_UPLOADFILE_DATA)) //upload data
					{
						qDebug() << "reciveUE upload Data: " << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();

						QByteArray fileData = req.data;
						QByteArray fromhexData = QByteArray::fromHex(fileData).data();
						
						ReqInterrFace handleReq;
						handleReq = req;
						handleReq.data = "0000";
						handleReq.Command = MSG_CMD_UPLOADFILE_DATA_RSP;

						handleReq.setDataLength();
						handleReq.setLength();

						QByteArray handByteData = reqToByteArray(handleReq);
						emit signalWriteData("pc send upload data",handByteData);
					}

					if (isCompare(req.Command, MSG_CMD_UPLOADFILE_END)) // upload end
					{
						qDebug() << "reciveUE upload End: " << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();
						ReqInterrFace handleReq;
						handleReq = req;
						handleReq.Command = MSG_CMD_UPLOADFILE_END_RSP;
						handleReq.data = "0000";
						handleReq.setDataLength();
						handleReq.setLength();

						QByteArray handByteData = reqToByteArray(handleReq);
						emit signalWriteData("pc send upload end", handByteData);

					}

				}
			}

		}
	}
	
	
	return readData;

}
