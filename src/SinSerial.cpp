#include "SinSerial.h"
#include <QSerialPortInfo>
#include <QSettings>
#include <QSerialPortInfo>
#include <QDataStream>
#include <QDebug>
#include <QThread>
#include <qendian.h>
#include "sinTaskQueue.h"
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "SinByte.h"
#include "CommonHeader.hpp"

SinSerial::SinSerial(QObject *parent):
QObject(parent)
, serialPort(nullptr)
, m_bIsUpLoadTrans(true)
{
	m_pReciveData.clear();
	m_pErrorTimer = new QTimer();
	m_pWriteErrorTimer = new QTimer();
	connect(m_pErrorTimer, SIGNAL(timeout()), this, SLOT(slotTimerOut()));
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
		//serialPort->setReadBufferSize(4096);
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
	flowControls << "������" << "Ӳ������" << "�������";

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

	parityList << "��У��" << "żУ��" <<  "��У��" << "У��λΪ0" << "У��λΪ1";

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
		
		//getSerialPort()->waitForBytesWritten(500); //����д��ʱ
		//getSerialPort()->waitForReadyRead(500); //���ö���ʱ
		//getSerialPort()->clear(); //��ջ���
		//getSerialPort()->setReadBufferSize(0); 
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

void SinSerial::sendData(QString strLog,QByteArray bytedata,bool isShowOnWidget)
{
	QByteArray writeData = QByteArray::fromHex(bytedata);
	int writeByte = getSerialPort()->write(writeData);
	qDebug() << strLog << bytedata;
	
	if (isShowOnWidget)
	{
		sinTaskQueueSingle::getInstance().pushBackReadData(strLog);
	}
	
	m_pWriteErrorTimer->start(6000);
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

void SinSerial::slotUpdateTransType(bool upTransType,QString upFileSavePath)
{
	m_bIsUpLoadTrans = upTransType;
	m_pUpFileSavePath = upFileSavePath;
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
		//ͳ�Ƶ�i���ļ���size
		int iSize = 0;
		for (int size = 0; size < fileData.size();size ++)
		{
			 iSize = iSize +  fileData.at(size).size();
		}


		for (int j = 0; j < fileData.size(); j++)
		{
			ReqInterrFace reqStruct;
			//ͷ���
			reqStruct.Header = "eba846b9";

			//binFileId
			int nBinFileId = 1;
			//reqStruct.BinFileId = QByteArray::number(nBinFileId);
			reqStruct.BinFileId = "00000003";

			// ��ǰ��i ���ļ��Ĵ�С
			int nFileSize = iSize;
			QString strfileSize = QString("%1").arg(nFileSize, 8, 16, QLatin1Char('0'));
			reqStruct.BinFileSize = strfileSize.toUtf8().data();

			//����id����ǰ�ļ����
			int nTransId = i;
			QString strTranId = QString("%1").arg(nTransId, 8, 16, QLatin1Char('0'));
			reqStruct.TransId = strTranId.toUtf8().data();

			//��ǰ�ļ��������к�
			int nTransSeqNum = j;
			QString strTransDeqNum = QString("%1").arg(nTransSeqNum, 8, 16, QLatin1Char('0'));
			reqStruct.TransSeqNum = strTransDeqNum.toUtf8().data();

			//��ǰ��i���ļ��ĵ�j �����Ĵ�С
			int nDataLength = fileListData[i][j].size();
			QString strDataLength = QString("%1").arg(nDataLength, 4, 16, QLatin1Char('0'));
			reqStruct.DataLength = strDataLength.toUtf8().data();

			//���������
			reqStruct.data = fileData[j].toHex();

			m_pWriteData.push_back(reqStruct);

		}
	}
}

QByteArray SinSerial::getValueFromData(QByteArray data, int findIndex, int offset, int length)
{
	QByteArray temp;

	if (length == 0 || findIndex + offset + length > data.size())
	{
		return temp;;
	}

	return data.mid(findIndex + offset, length);
	
}

ReqInterrFace SinSerial::indexToReqHeader(QByteArray data, int Index)
{
	bool ok;
	ReqInterrFace req;
	ReqInterrFace tempReq;

	req.Header = getValueFromData(data, Index, 0, 8);
	req.Length = getValueFromData(data, Index, 8, 4);
	req.Command = getValueFromData(data, Index, 12, 4);
	req.BinFileId = getValueFromData(data, Index, 16, 8);
	req.BinFileSize = getValueFromData(data, Index, 24, 8);
	req.TransId = getValueFromData(data, Index, 32, 8);
	req.TransSeqNum = getValueFromData(data, Index, 40, 8);
	req.DataLength = getValueFromData(data, Index, 48, 4);
	req.DataCRC = getValueFromData(data, Index, 52, 4);


	if (req.isNull())
	{
		//ͷ�ֶ��п�ֵ ����ȷ
		return tempReq;
	}
	else {
		return req;
	}

}


ReqInterrFace SinSerial::indexToReq(QByteArray data, int Index)
{
	bool ok;
	ReqInterrFace req;
	ReqInterrFace tempReq;

	req.Header = getValueFromData(data, Index, 0, 8);
	req.Length = getValueFromData(data, Index, 8, 4);
	req.Command = getValueFromData(data, Index, 12, 4);
	req.BinFileId = getValueFromData(data, Index, 16, 8);
	req.BinFileSize = getValueFromData(data, Index, 24, 8);
	req.TransId = getValueFromData(data, Index, 32, 8);
	req.TransSeqNum = getValueFromData(data, Index, 40, 8);
	req.DataLength = getValueFromData(data, Index, 48, 4);
	req.DataCRC = getValueFromData(data, Index, 52, 4);


	if (req.isNull())
	{
		//ͷ�ֶ��п�ֵ ����ȷ
		m_pStrErrorLog = "ͷ���ֶ�ȱʧ ���Ȳ���ȷ";
		return tempReq;
	}

	QString qstrLength = req.Length;
	int Length = qstrLength.toInt(&ok, 16);	

	QString qstrDataLength = req.DataLength;
	int dataLength =  qstrDataLength.toInt(&ok, 16);

	req.data = getValueFromData(data, Index, 56, dataLength * 2);
	if (req.data.size()<=0 && dataLength != 0)
	{
		//data���Ӳ���ȷ
		m_pStrErrorLog = "Data�ֶ�ȱʧ ���Ȳ���ȷ";
		return tempReq;
	}

	int paddingLength = dataLength % 4; 
	req.Padding = getValueFromData(data, Index, 56 + dataLength * 2, paddingLength * 2);


	int reqLength = req.Command.size() + req.BinFileId.size() + req.BinFileSize.size() + req.TransId.size() + req.TransSeqNum.size() + req.DataLength.size() + req.DataCRC.size() + req.data.size();

	//if (Length != (reqLength/2))
	//{
	//	//���Ȳ���
	//	m_pStrErrorLog = "�ܳ��Ȳ���ȷ";
	//	return tempReq;
	//}

	//if ((req.getSize() % 8) != 0)
	//{
	//	//û����
	//	m_pStrErrorLog = "û���ֽڶ���";
	//	return tempReq;
	//}

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

void SinSerial::handleTransError(QByteArray dataError,int currentIndex)
{
	if (isCompare(dataError, FILE_NOT_EXIST)) // 02
	{
		qDebug() << "recive UE Error" << "FILE_NOT_EXIST";
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

void SinSerial::sendData(ReqInterrFace req, QString strLogPrefix, QByteArray command, int index,QByteArray dataError)
{
	ReqInterrFace sendReq;
	bool isShowOnWidget;
	if (isCompare(command, MSG_CMD_HANDSHAKE_SYNARK))  //����
	{
		sendReq = req;
		sendReq.Command = command;
		sendReq.data.clear();
		isShowOnWidget = true;
	}
	else if (isCompare(command, MSG_CMD_UPLOADFILE_REQ)) //�ϴ�����
	{
		sendReq = req;
		m_pReadData.clear();
		sendReq.Command = command;
		sendReq.BinFileId = "00000003";
		sendReq.data.clear();
		isShowOnWidget = true;
	}
	else if (isCompare(command, MSG_CMD_UPLOADFILE_DATA_RSP)) //�ϴ���
	{
		sendReq = req;
		m_pReadData.append(req.data);
		sendReq.Command = command;
		if (dataError == FILE_OK)
		{
			sendReq.data = FILE_OK;
		}
		else { //�ϴ�����
			sendReq = req;
		}

		int transSeq = sendReq.getIntValue(req.TransSeqNum);
		if (transSeq == 0){

			strLogPrefix = "�����ϴ���......";
			isShowOnWidget = true;;
		}
		else {
			isShowOnWidget = false;
		}
		

		m_nTotalPackage = (req.getIntValue(req.BinFileSize) / PACKBUFFER) + 1; //��¼�ܰ���
		m_nCurrentPackage = req.getIntValue(req.TransSeqNum); //��ǰ����

		//���½�������Ϣ
		int nProgressValue = (100 * m_nCurrentPackage) / m_nTotalPackage;
		emit signalUpDateProgressBar(m_nCurrentPackage);
		
		
	}
	else if (isCompare(command, MSG_CMD_UPLOADFILE_END_RSP)) //�ϴ�����
	{
		sendReq = req;
		QString fileName =m_pUpFileSavePath + "/"+ req.BinFileId + "_" + QString::number(QDateTime::currentDateTime().toTime_t())+ ".bin";
		
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly))
		{
			qDebug() << "save file error";
		}
		QDataStream out(&file);
		out << m_pReadData;
		file.close();
		bool ok;

		QString strFileSize = req.BinFileSize;
		int nFileSize = strFileSize.toInt(&ok, 16);
		if (m_pReadData.size() / 2 == nFileSize)
		{
			int a = 1;
		}

		m_pReadData.clear();
		sendReq.Command = command;
		sendReq.data.clear();

		m_nCurrentPackage = m_nTotalPackage; //���ؽ��� ��ǰ�����ڽ�����
		isShowOnWidget = true;
	}
	else if (isCompare(command, MSG_CMD_DOWNLOADFILE_REQ)) // ��������
	{
		ReqInterrFace writeReq = m_pWriteData.at(0);
		sendReq = writeReq;
		sendReq.Command = command;
		sendReq.data.clear();
		isShowOnWidget = true;
	}
	else if (isCompare(command, MSG_CMD_DOWNLOADFILE_DATA)) //������
	{
		
		if (index == m_pWriteData.size())
		{
			//MSG_CMD_DOWNLOADFILE_END
			sendReq = req;
			sendReq.Command = MSG_CMD_DOWNLOADFILE_END;
			sendReq.data.clear();

			m_nCurrentPackage = m_nTotalPackage; //���ؽ��� ��ǰ�����ڽ�����
			isShowOnWidget = true;
		}
		else {

			if (index  == 0)
			{
				strLogPrefix = "����������......";
				isShowOnWidget = true;
			}
			else {
				isShowOnWidget = false;
			}

			ReqInterrFace writeReq = m_pWriteData.at(index);
			sendReq = writeReq;
			sendReq.Command = command; 
		}

		m_nTotalPackage = (req.getIntValue(req.BinFileSize) / PACKBUFFER) + 1; //��¼�а���;
		m_nCurrentPackage = index; //��¼��ǰ���͵��ǵڼ���

		//���½�������Ϣ
		int nProgressValue = (100 * m_nCurrentPackage) / m_nTotalPackage;
		emit signalUpDateProgressBar(nProgressValue);
		
	}

	sendReq.setDataLength();
	sendReq.setCRC();
	sendReq.setLength();
	QByteArray handByteData = reqToByteArray(sendReq);
	//emit signalWriteData(strLogPrefix, handByteData); //���̷߳���
	//Sleep(1000);
	sendData(strLogPrefix, handByteData,isShowOnWidget);
	
	
	//�����꣬�����Ѿ����͵Ļ���
	int reqSize = req.getSize();
	QByteArray sendDataArray = reqToByteArray(req);
	QString strTempReciveData = m_pReciveData;
	int findIndex = strTempReciveData.indexOf(sendDataArray, 0, Qt::CaseInsensitive);

	m_pReciveData.remove(findIndex, reqSize);
	m_pLastSendReq = sendReq;
}

void SinSerial::slotTimerOut()
{
	handleUploadError(FILE_MISSING_PACKET_ERROR);
}

//����ʧ�� д�ش�
void SinSerial::slotWriteErrorTimeOut()
{
	//sendData(QString::fromLocal8Bit("����ʧ�� �ش���һ�ΰ�"), reqToByteArray(m_pLastSendReq),false);
	sendData(m_pLastSendReq,"����ʧ�ܣ��ش���һ�ΰ�", m_pLastSendReq.Command, false);
}

void SinSerial::delReqFromReadBuffer(QByteArray readData, ReqInterrFace req)
{
	QByteArray reqByte =  reqToByteArray(req);
	int findIndex = readData.indexOf(reqByte);
	
	if (findIndex != -1)
	{
		m_pReciveData.remove(findIndex, reqByte.size());
	} 
}
 
void SinSerial::handleUploadError(QByteArray dataError){


	m_pErrorPreReq.data.clear();

	if (isCompare(m_pErrorPreReq.Command, MSG_CMD_DOWNLOADFILE_REQ_RSP)) //8002
	{
		m_pErrorPreReq.Command = MSG_CMD_DOWNLOADFILE_REQ;
	}
	else if (isCompare(m_pErrorPreReq.Command, MSG_CMD_DOWNLOADFILE_DATA_RSP)) //8003
	{
		m_pErrorPreReq.Command = MSG_CMD_DOWNLOADFILE_DATA;
	}
	else if (isCompare(m_pErrorPreReq.Command, MSG_CMD_DOWNLOADFILE_END_RSP)) //8004
	{
		m_pErrorPreReq.Command = MSG_CMD_DOWNLOADFILE_END;
	}
	else if (isCompare(m_pErrorPreReq.Command, MSG_CMD_UPLOADFILE_REQ_RSP)) //8005
	{
		m_pErrorPreReq.Command = MSG_CMD_UPLOADFILE_DATA_RSP;
	}
	else if (isCompare(m_pErrorPreReq.Command, MSG_CMD_UPLOADFILE_DATA)) //8006
	{
		m_pErrorPreReq.Command = MSG_CMD_UPLOADFILE_DATA_RSP;
	}
	else if (isCompare(m_pErrorPreReq.Command, MSG_CMD_UPLOADFILE_END)) //8007
	{
		m_pErrorPreReq.Command = MSG_CMD_UPLOADFILE_END_RSP;
	}

	//bool ok;
	//QString strSeqNum = m_pErrorPreReq.TransSeqNum;
	//int tempInt = strSeqNum.toInt(&ok, 16);
	//int nSeqNum = strSeqNum.toInt(&ok, 16)  +1;
	//QString strTransSeqNum = QString("%1").arg(nSeqNum, 8, 16, QLatin1Char('0'));

	//m_pErrorPreReq.TransSeqNum = strTransSeqNum.toUtf8().data();

	m_pReciveData.clear();//�������֮ǰ�Ļ���
	if (isCompare(dataError,FILE_CRC_ERROR))
	{
		m_pErrorPreReq.data = FILE_CRC_ERROR;
		sendData(m_pErrorPreReq, "CRCУ��ʧ���ش�", MSG_CMD_UPLOADFILE_DATA_RSP, 0, false);
	}
	else if (isCompare(dataError, FILE_MISSING_PACKET_ERROR))
	{
		m_pErrorPreReq.data = FILE_MISSING_PACKET_ERROR;
		sendData(m_pErrorPreReq, "���ճ�ʱ��"+m_pStrErrorLog+",�ļ���ȱʧ�ش�", MSG_CMD_UPLOADFILE_DATA_RSP, 0, false);
	}
}

ReqInterrFace SinSerial::findFirstReqFromReciveData(QByteArray reciveData)
{
	ReqInterrFace req;
	QString strReciveData = reciveData;
	ReqInterrFace reqHeader;
	if (strReciveData.isEmpty() || strReciveData.size() <= 0)
	{
		return req;
	}
	int findIndex = strReciveData.indexOf(MSG_PROTO_HEADER_TAG, 0, Qt::CaseInsensitive);

	if (findIndex == -1)
	{
		return req;
	}
	else{

	}
	

	reqHeader = indexToReqHeader(reciveData, findIndex);
	if (!reqHeader.isNull())
	{
		m_pErrorPreReq = reqHeader;
	}
	m_pStrErrorLog = "";
	req = indexToReq(reciveData, findIndex);
	int reqSize = req.getSize();

	
	if (reqSize == 0)
	{
		m_pErrorTimer->start(3000);
		ReqInterrFace temp;
		return temp;;
	}


	if (findIndex != 0)
	{
		m_pReciveData.remove(0, findIndex);
	}

	m_pErrorTimer->stop();

	QByteArray CalcCRC = req.getCRC();
	QByteArray reqCRC = req.DataCRC;
	if (CalcCRC != reqCRC)
	{
		handleUploadError(FILE_CRC_ERROR);
		ReqInterrFace temp;
		return temp;;
	}
	return req;
}

void SinSerial::slotGetReadData()
{;
	QByteArray readData;
	getSerialPort()->waitForReadyRead(10);
	if (getSerialPort()->bytesAvailable())
	{
		readData = getSerialPort()->readAll();
		getSerialPort()->clear();
		QString qstrTest = readData.toHex().data();
		
		//readData = readData.trimmed();
		QString qstrtest1 = readData.toHex().data();
		if (!readData.isEmpty())
		{
			m_pWriteErrorTimer->stop(); //�������� ֹͣ���Ͷ�ʱ��
			int currentPackIndex = 0;
			
			QString qstrReadData = readData.toHex().data();

			qDebug() << "��������: " << readData;

			m_pReciveData.append(readData.toHex());
			//�ӻ����б��ҵ�һ��req
			ReqInterrFace req =  findFirstReqFromReciveData(m_pReciveData);

			if (req.Header.isEmpty())
			{
				return;
			}

			if (isCompare(req.Command,MSG_CMD_HANDSHAKE_SYN)) //����Э��
			{
				qDebug() << "�յ�UE�������� :" << reqToByteArray(req) ;
						
				QString strShowLog = QString::fromLocal8Bit("�յ�UE��������");
				sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog);
						
				sendData(req, "PC�������ֻظ�", MSG_CMD_HANDSHAKE_SYNARK);
			}
			else if (isCompare(req.Command, MSG_CMD_HANDSHAKE_ARK)) //���ֳɹ�
			{
				qDebug() << "�յ�UE���ֳɹ��ظ�:" << reqToByteArray(req) ;

				QString strShowLog = "�յ�UE���ֳɹ��ظ�";
				sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog);

				//m_bIsUpLoadTrans = false;
				QByteArray command;
				QString strLogPrefix;
				if (m_bIsUpLoadTrans)
				{
					command = MSG_CMD_UPLOADFILE_REQ;
					strLogPrefix = "PC�����ϴ��ļ�����";
				}
				else {
					strLogPrefix = "PC ���������ļ�����";
					command = MSG_CMD_DOWNLOADFILE_REQ;
				}

				sendData(req, strLogPrefix, command);
			}
			else if (isCompare(req.Command, MSG_CMD_DOWNLOADFILE_REQ_RSP)) //��������
			{
				qDebug() << "�յ�UE�����ļ�����ظ��� " << reqToByteArray(req) ;
						 
				QString strShowLog = "�յ�UE�����ļ�����ظ�";
				sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog);

				if (isCompare(req.data, FILE_OK))
				{	
					sendData(req, "PC ���������ļ����ݰ�", MSG_CMD_DOWNLOADFILE_DATA);
				}
				else {
					handleTransError(req.data);
				}
			}
			else if (isCompare(req.Command, MSG_CMD_DOWNLOADFILE_DATA_RSP)) //������ 
			{
				qDebug() << "�յ�UE�����ļ����ݰ��ظ��� " << reqToByteArray(req) ;

				bool ok;
				QString strSeqNum = req.TransSeqNum;
				int nSeqNum = strSeqNum.toInt(&ok, 16);
				currentPackIndex = nSeqNum;

				QString strShowLog = "�յ�UE�����ļ�"+QString::number(nSeqNum)+"���ݰ��ظ�";
				//sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog);

				

				if (isCompare(req.data, FILE_OK)) //01 ���ͳɹ�������һ����
				{
					sendData(req, "PC ��������"+QString::number((nSeqNum+1))+"�ļ����ݰ�", MSG_CMD_DOWNLOADFILE_DATA, nSeqNum+1);
				}
				else { //error ���Ͳ��ɹ�
					handleTransError(req.data,nSeqNum);
				}
			}
			else if (isCompare(req.Command, MSG_CMD_DOWNLOADFILE_END_RSP)) //���ؽ���
			{
				qDebug() << "�յ�UE�����ļ������ظ���" << reqToByteArray(req) ;

				QString strShowLog = "�յ�UE�����ļ������ظ�";
				sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog);

				if (isCompare(req.data, FILE_OK))
				{
					int a = 1;
				}
				else {
					handleTransError(req.data);
				}
			}
			else if (isCompare(req.Command, MSG_CMD_UPLOADFILE_REQ_RSP)) // �ϴ�����
			{
				qDebug() << "�յ�UE�ϴ��ļ�����ظ�: " << reqToByteArray(req) ;

				QString strShowLog = "�յ�UE�ϴ��ļ�����ظ�";
				sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog);

				if (isCompare(req.data, FILE_OK))
				{
					//MSG_CMD_UPLOADFILE_REQ
					//sendData(req, "PC�����ϴ��ļ�����", MSG_CMD_UPLOADFILE_REQ);
					delReqFromReadBuffer(m_pReciveData, req);

				}
				else {
					handleTransError(req.data);
				}
			}
			else if (isCompare(req.Command, MSG_CMD_UPLOADFILE_DATA)) //�ϴ���
			{
				qDebug() << "�յ�UE�ϴ��ļ����ݰ�: " << reqToByteArray(req) ;

				bool ok;
				QString strShowLog = "�յ�UE�ϴ��ļ�"+QString::number(req.TransSeqNum.toInt(&ok,16))+"���ݰ�";
				//sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog);

				QByteArray fileData = req.data;
				QByteArray fromhexData = QByteArray::fromHex(fileData).data();

				//if (isCompare(req.data, FILE_OK))
				//{
				sendData(req, "PC �����ϴ��ļ�" + QString::number(req.TransSeqNum.toInt(&ok, 16)) + "���ݰ��ظ�", MSG_CMD_UPLOADFILE_DATA_RSP);

				//}
				//else {
				//	handleTransError(req.data);
				//}
			}
			else if (isCompare(req.Command, MSG_CMD_UPLOADFILE_END)) // �ϴ�����
			{
				qDebug() << "�յ�UE�ϴ��ļ����ݰ�����: " << reqToByteArray(req) ;

				QString strShowLog = "�յ�UE�ϴ��ļ����ݰ����� ";
				sinTaskQueueSingle::getInstance().pushBackReadData(strShowLog);

				sendData(req, "PC�����ϴ��ļ����ݰ������ظ�", MSG_CMD_UPLOADFILE_END_RSP);
			} 
			else { //unknow
				qDebug() << "reciveUE show: " << readData ;
				sinTaskQueueSingle::getInstance().pushBackReadData(readData);
					
			}
		}
	}
	
	
	//return readData;

}
