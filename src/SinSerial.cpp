#include "SinSerial.h"
#include <QSerialPortInfo>
#include <QSettings>
#include <QSerialPortInfo>
#include <QDataStream>
#include <QDebug>
#include <QThread>
#include <qendian.h>
#include "SinTaskQueue.h"
#include <QTime>
#include <QFile>
#include <QTextStream>
SinSerial::SinSerial(QObject *parent):
QObject(parent)
, serialPort(nullptr)
, m_bIsUpLoadTrans(true)
{
	connect(getSerialPort(), SIGNAL(readyRead), this, SLOT(slotGetReadData()));
}

SinSerial::~SinSerial()
{

}

QSerialPort* SinSerial::getSerialPort()
{
	if (serialPort == nullptr)
	{
		serialPort = new QSerialPort();
		connect(serialPort, SIGNAL(readyRead()), this, SLOT(slotGetReadData()));
		serialPort->setReadBufferSize(4096);
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
		
		//getSerialPort()->waitForBytesWritten(500); //设置写超时
		//getSerialPort()->waitForReadyRead(500); //设置读超时
		//getSerialPort()->clear(); //清空缓存
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
	QString strShowLog = strLog +":"+ bytedata;

	sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog.toLatin1());
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
	byteData = qFromLittleEndian(byteData);
	bool ok;
	QString strData = byteData.data();
	int data = strData.toInt(&ok, 16);

	return data == nError;
	
}

void SinSerial::setTransTypeWriteData(bool isUplodType, QList<QList<QByteArray>> fileListData)
{
	m_bIsUpLoadTrans = isUplodType;

	m_pWriteData.clear();

	for (int i = 0; i < fileListData.size(); i++)
	{
		QList<QByteArray> fileData = fileListData.at(i);
		//统计第i个文件的size
		int iSize = 0;
		for (int size = 0; size < fileData.size();size ++)
		{
			 iSize = iSize +  fileData.at(size).size();
		}


		for (int j = 0; j < fileData.size(); j++)
		{
			ReqInterrFace reqStruct;
			//头标记
			reqStruct.Header = "eba846b9";

			//binFileId
			int nBinFileId = 1;
			//reqStruct.BinFileId = QByteArray::number(nBinFileId);
			reqStruct.BinFileId = "00000003";

			// 当前第i 个文件的大小
			int nFileSize = iSize;
			QString strfileSize = QString("%1").arg(nFileSize, 8, 16, QLatin1Char('0'));
			reqStruct.BinFileSize = strfileSize.toUtf8().data();

			//事务id，当前文件标记
			int nTransId = i;
			QString strTranId = QString("%1").arg(nTransId, 8, 16, QLatin1Char('0'));
			reqStruct.TransId = strTranId.toUtf8().data();

			//当前文件传输序列号
			int nTransSeqNum = j;
			QString strTransDeqNum = QString("%1").arg(nTransSeqNum, 8, 16, QLatin1Char('0'));
			reqStruct.TransSeqNum = strTransDeqNum.toUtf8().data();

			//当前第i个文件的第j 个包的大小
			int nDataLength = fileListData[i][j].size();
			QString strDataLength = QString("%1").arg(nDataLength, 4, 16, QLatin1Char('0'));
			reqStruct.DataLength = strDataLength.toUtf8().data();

			//传输的内容
			reqStruct.data = fileData[j].toHex();

			m_pWriteData.push_back(reqStruct);

		}
	}
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

	int paddingLength = dataLength % 4;
	if (paddingLength != 0)
	{
		req.Padding = data.mid(Index + 56 + dataLength * 2, paddingLength * 2);
	}
	
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

void SinSerial::sendData(ReqInterrFace req, QString strLogPrefix, QByteArray command, int index)
{
	ReqInterrFace sendReq;

	if (isCompare(command, MSG_CMD_HANDSHAKE_SYNARK))  //握手
	{
		sendReq = req;
		sendReq.Command = command;
		sendReq.data.clear();
	}
	else if (isCompare(command, MSG_CMD_UPLOADFILE_REQ)) //上传请求
	{
		sendReq = req;
		m_pReadData.clear();
		sendReq.Command = command;
		sendReq.data.clear();

	}
	else if (isCompare(command, MSG_CMD_UPLOADFILE_DATA_RSP)) //上传中
	{
		sendReq = req;
		m_pReadData.append(req.data);
		sendReq.Command = command;;
		sendReq.data.clear();
	}
	else if (isCompare(command, MSG_CMD_UPLOADFILE_END_RSP)) //上传结束
	{
		sendReq = req;
		QString fileName = req.BinFileId + "_" + QTime::currentTime().toString()+".bin";
		
		QFile file(fileName);
		if (!file.open(QIODevice::ReadWrite))
		{
			qDebug() << "save file error";
		}
		QTextStream in(&file);
		file.close();

		m_pReadData.clear();
		sendReq.Command = command;
		sendReq.data.clear();
	}
	else if (isCompare(command, MSG_CMD_DOWNLOADFILE_REQ)) // 下载请求
	{
		ReqInterrFace writeReq = m_pWriteData.at(0);
		sendReq = writeReq;
		sendReq.Command = command;
		sendReq.data.clear();
	}
	else if (isCompare(command, MSG_CMD_DOWNLOADFILE_DATA)) //下载中
	{
		
		if (index == m_pWriteData.size())
		{
			//MSG_CMD_DOWNLOADFILE_END
			sendReq = req;
			sendReq.Command = MSG_CMD_DOWNLOADFILE_END;
			sendReq.data.clear();
		}
		else {
			ReqInterrFace writeReq = m_pWriteData.at(index);
			sendReq = writeReq;
			sendReq.Command = command;
		}
	}

	sendReq.setDataLength();
	sendReq.setCRC();
	sendReq.setLength();
	QByteArray handByteData = reqToByteArray(sendReq);
	//emit signalWriteData(strLogPrefix, handByteData); //主线程发送
	//Sleep(1000);
	sendData(strLogPrefix, handByteData);
}

void SinSerial::handleTransError(QByteArray dataError)
{
	if (isCompare(dataError, FILE_NOT_EXIST)) // 02
	{
		qDebug() << "recive UE Error"<< "FILE_NOT_EXIST";
	}
	else if (isCompare(dataError, FILE_SIZE_TOO_BIG)) //03
	{
		qDebug() << "recive UE Error" << "FILE_SIZE_TOO_BIG";
	}
	else if (isCompare(dataError, FILE_HDR_INFO_NOT_MATCH)) //04
	{
		qDebug() << "recive UE Error" << "FILE_HDR_INFO_NOT_MATCH";
	}
	else if (isCompare(dataError, FILE_SIGNATURE_ERROR)) //05
	{
		qDebug() << "recive UE Error" << "FILE_SIGNATURE_ERROR";
	}
	else if (isCompare(dataError, FILE_WRITE_FLASH_ERROR)) //06
	{
		qDebug() << "recive UE Error" << "FILE_WRITE_FLASH_ERROR";
	}
	else if (isCompare(dataError, FILE_READ_FLASH_ERROR)) //07
	{
		qDebug() << "recive UE Error" << "FILE_READ_FLASH_ERROR";
	}
	else if (isCompare(dataError, FILE_MISSING_PACKET_ERROR)) //08
	{
		qDebug() << "recive UE Error" << "FILE_MISSING_PACKET_ERROR";
	}
	else if (isCompare(dataError, FILE_INNER_ERROR)) //09
	{
		qDebug() << "recive UE Error" << "FILE_INNER_ERROR";
	}
}


void SinSerial::slotGetReadData()
{;
	QByteArray readData;
	//if (getSerialPort()->canReadLine())
	if (getSerialPort()->bytesAvailable())
	{
		//getSerialPort()->waitForReadyRead(500);
		readData = getSerialPort()->readAll();
		getSerialPort()->clear();
		QString qstrTest = readData.toHex().data();
		
		readData = readData.trimmed();
		QString qstrtest1 = readData.toHex().data();
		if (!readData.isEmpty())
		{
			int currentPackIndex = 0;
			QString qstrReadData = readData.toHex().data();

			//qDebug() << "reciveUE Log ReaData: " << readData << "currentThreadId:" << QThread::currentThread();

			QList<int> indexofList = indexOfHeader(qstrReadData, MSG_PROTO_HEADER_TAG);
			
			if (indexofList.size() <= 0)
			{
				qDebug() << "reciveUE Log: " << readData << "currentThreadId:" << QThread::currentThread();
				QString strShowLog = "reciveUE Log: " + readData;
				sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog.toLatin1());
				//return readData;
				return;
			}
			bool isValid = false;
			for (int i = 0; i < indexofList.size();i++)
			{
				ReqInterrFace req = indexToReq(readData.toHex(), indexofList.at(i));

				isValid = isCompare(req.Header, MSG_PROTO_HEADER_TAG);
				if (isValid)
				{	

					if (isCompare(req.Command,MSG_CMD_HANDSHAKE_SYN)) //握手协议
					{
						qDebug() << " reciveUE handshake syn :" << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();
						
						QString strShowLog = " reciveUE handshake syn :" + reqToByteArray(req);
						sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog.toLatin1());
						
						sendData(req, "pc send handshake synark", MSG_CMD_HANDSHAKE_SYNARK);
					}
					else if (isCompare(req.Command, MSG_CMD_HANDSHAKE_ARK)) //握手成功
					{
						qDebug() << "reciveUE Handle Ok :" << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();

						QString strShowLog = "reciveUE Handle Ok :" + reqToByteArray(req);
						sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog.toLatin1());

						m_bIsUpLoadTrans = false;
						QByteArray command;
						QString strLogPrefix;
						if (m_bIsUpLoadTrans)
						{
							command = MSG_CMD_UPLOADFILE_REQ;
							strLogPrefix = "pc send uploadfile req";
						}
						else {
							strLogPrefix = "pc send downloadfile req";
							command = MSG_CMD_DOWNLOADFILE_REQ;
						}

						sendData(req, strLogPrefix, command);
					}
					else if (isCompare(req.Command, MSG_CMD_DOWNLOADFILE_REQ_RSP)) //下载请求
					{
						qDebug() << "reciveUE Download Req: " << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();
						 
						QString strShowLog = "reciveUE Download Req: " + reqToByteArray(req);
						sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog.toLatin1());

						if (isCompare(req.data, FILE_OK))
						{	
							sendData(req, "pc send downloadfile data", MSG_CMD_DOWNLOADFILE_DATA);
						}
						else {
							handleTransError(req.data);
						}
					}
					else if (isCompare(req.Command, MSG_CMD_DOWNLOADFILE_DATA_RSP)) //下载中
					{
						qDebug() << "reciveUE Download Data: " << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();

						QString strShowLog = "reciveUE Download Data: " + reqToByteArray(req);
						sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog.toLatin1());

						if (isCompare(req.data, FILE_OK)) //01
						{
							bool ok;
							QString strSeqNum = req.TransSeqNum;
							int nSeqNum = strSeqNum.toInt(&ok, 16);
							currentPackIndex = nSeqNum;
							
							sendData(req, "pc send downloadfile data", MSG_CMD_DOWNLOADFILE_DATA, nSeqNum+1);
						}
						else { //error
							handleTransError(req.data);
						}
					}
					else if (isCompare(req.Command, MSG_CMD_DOWNLOADFILE_END_RSP)) //下载结束
					{
						qDebug() << "reciveUE download End: " << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();

						QString strShowLog = "reciveUE download End: " + reqToByteArray(req);
						sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog.toLatin1());

						if (isCompare(req.data, FILE_OK))
						{
							int a = 1;
						}
						else {
							handleTransError(req.data);
						}
					}
					else if (isCompare(req.Command, MSG_CMD_UPLOADFILE_REQ_RSP)) // 上传请求
					{
						qDebug() << "reciveUE upload Req Rsp: " << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();

						QString strShowLog = "reciveUE upload Req Rsp: " + reqToByteArray(req);
						sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog.toLatin1());

						if (isCompare(req.data, FILE_OK))
						{
							//MSG_CMD_UPLOADFILE_REQ
							sendData(req, "pc send upload Req", MSG_CMD_UPLOADFILE_REQ);

						}
					}
					else if (isCompare(req.Command, MSG_CMD_UPLOADFILE_DATA)) //上传中
					{
						qDebug() << "reciveUE upload Data: " << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();

						QString strShowLog = "reciveUE upload Data: " + reqToByteArray(req);
						sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog.toLatin1());

						QByteArray fileData = req.data;
						QByteArray fromhexData = QByteArray::fromHex(fileData).data();
				
						
						sendData(req, "pc send Upload Data Rsp:", MSG_CMD_UPLOADFILE_DATA_RSP);
					}
					else if (isCompare(req.Command, MSG_CMD_UPLOADFILE_END)) // 上传结束
					{
						qDebug() << "reciveUE upload End: " << reqToByteArray(req) << "currentThreadId:" << QThread::currentThread();

						QString strShowLog = "reciveUE upload End: " + reqToByteArray(req);
						sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog.toLatin1());

						sendData(req, "PC send Upload End Rsp", MSG_CMD_UPLOADFILE_END_RSP);
					} 
					else { //unknow
						qDebug() << "reciveUE show: " << readData << "currentThreadId:" << QThread::currentThread();

					
					}

				} //un validate
				 else {
					 int a = 1;
				 }
			}
		}
	}
	
	
	//return readData;

}
