#include "SinSerialChoose.h"
#include <QSerialPort>
#include "SinByte.h"

SinSerialChoose::SinSerialChoose(QWidget *parent)
	:m_bHandOk(false)
{
	m_pSerialPort = new QSerialPort();
	connect(m_pSerialPort, SIGNAL(readyRead), this, SLOT(slotReadData()));

}


SinSerialChoose::~SinSerialChoose()
{

}

int SinSerialChoose::openCom(QString portName, QString rateValue, QString flowValue, QString dataValue, QString stopValue, QString parityValue)
{
	m_pSerialPort->setPortName(portName);
	if (m_pSerialPort->open(QIODevice::ReadWrite))
	{
		m_pSerialPort->setBaudRate(rateValue.toInt());
		m_pSerialPort->setDataBits(QSerialPort::DataBits(dataValue.toInt()));
		m_pSerialPort->setParity(QSerialPort::Parity(parityValue.toInt()));
		m_pSerialPort->setFlowControl(QSerialPort::FlowControl(flowValue.toInt()));
		m_pSerialPort->setStopBits(QSerialPort::StopBits(stopValue.toInt()));

		//getSerialPort()->waitForBytesWritten(500); //设置写超时
		//getSerialPort()->waitForReadyRead(500); //设置读超时
		//getSerialPort()->clear(); //清空缓存
		//getSerialPort()->setReadBufferSize(0); 

		return 0;
	}
	else {
		return -1;
	}
}

void SinSerialChoose::closeCom()
{
	m_pSerialPort->close();
}

QSerialPort* SinSerialChoose::getSerialPort()
{
	return m_pSerialPort;
}

void SinSerialChoose::slotReadData()
{
	QByteArray readData;
	m_pSerialPort->waitForReadyRead(10);
	if (m_pSerialPort->bytesAvailable())
	{
		readData = m_pSerialPort->readAll();
		m_pSerialPort->clear();

		QString qstrtest1 = readData.toHex().data();
		if (!readData.isEmpty())
		{
			QString qstrReadData = readData.toHex().data();
			//qDebug() << "读到数据: " << readData;

			m_pReadData.append(readData.toHex());
		}

		bool isValid = isValidHandPackage();
		if (isValid)
		{
			connectSerialWithHandPack();
		}
	}
	else {

	}
	
}

bool SinSerialChoose::isValidHandPackage()
{
	ReqInterrFace req = findFirstReqFromReciveData(m_pReadData);
	if (req.isNull())
	{
		return false;
	}
	m_pCurrentReq = req;
	return true;
}

QSerialPort* SinSerialChoose::chooseSerial()
{
	if (m_bHandOk)
	{
		return m_pSerialPort;
	} 

	return NULL;
}

void SinSerialChoose::connectSerialWithHandPack()
{
	if (isCompare(m_pCurrentReq.Command, MSG_CMD_HANDSHAKE_SYN)) //收到UE握手请求
	{
		ReqInterrFace sendReq = m_pCurrentReq;
		sendReq.Command = MSG_CMD_HANDSHAKE_SYNARK;

		sendReq.setCRC();
		sendReq.setLength();
		QByteArray handByteData = reqToByteArray(sendReq);
		int writeByte = m_pSerialPort->write(handByteData);
	}
	else if (isCompare(m_pCurrentReq.Command, MSG_CMD_HANDSHAKE_ARK))
	{
		if (m_pCurrentReq.data == FILE_OK)
		{
			m_bHandOk = true; //握手成功
		}
	}
	
}

QByteArray SinSerialChoose  ::reqToByteArray(ReqInterrFace req)
{
	QByteArray writeByte;
	writeByte = req.Header + req.Length + req.Command + req.BinFileId + req.BinFileSize + req.TransId + req.TransSeqNum + req.DataLength + req.DataCRC + req.data + req.Padding;
	return writeByte;
}

QByteArray SinSerialChoose::getValueFromData(QByteArray data, int findIndex, int offset, int length)
{
	QByteArray temp;

	if (length == 0 || findIndex + offset + length > data.size())
	{
		return temp;;
	}

	return data.mid(findIndex + offset, length);

}

bool SinSerialChoose::isCompare(QByteArray src, QByteArray dest)
{
	QString strSrc = src;
	QString strDest = dest;

	int result = strSrc.compare(strDest, Qt::CaseInsensitive);
	return result == 0;
}

ReqInterrFace SinSerialChoose::indexToReq(QByteArray data, int Index)
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
		//头字段有空值 不正确
		//m_pStrErrorLog = "头部字段缺失 长度不正确";
		return tempReq;
	}

	QString qstrLength = req.Length;
	int Length = qstrLength.toInt(&ok, 16);

	QString qstrDataLength = req.DataLength;
	int dataLength = qstrDataLength.toInt(&ok, 16);

	req.data = getValueFromData(data, Index, 56, dataLength * 2);
	if (req.data.size() <= 0 && dataLength != 0)
	{
		//data长队不正确
		//m_pStrErrorLog = "Data字段缺失 长度不正确";
		return tempReq;
	}

	int paddingLength = dataLength % 4;
	req.Padding = getValueFromData(data, Index, 56 + dataLength * 2, paddingLength * 2);


	int reqLength = req.Command.size() + req.BinFileId.size() + req.BinFileSize.size() + req.TransId.size() + req.TransSeqNum.size() + req.DataLength.size() + req.DataCRC.size() + req.data.size();

	//if (Length != (reqLength/2))
	//{
	//	//长度不够
	//	m_pStrErrorLog = "总长度不正确";
	//	return tempReq;
	//}

	//if ((req.getSize() % 8) != 0)
	//{
	//	//没对齐
	//	m_pStrErrorLog = "没四字节对齐";
	//	return tempReq;
	//}

	return req;


}


ReqInterrFace SinSerialChoose::indexToReqHeader(QByteArray data, int Index)
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
		//头字段有空值 不正确
		return tempReq;
	}
	else {
		return req;
	}

}

ReqInterrFace SinSerialChoose::findFirstReqFromReciveData(QByteArray reciveData)
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
	


	reqHeader = indexToReqHeader(reciveData, findIndex);
	if (!reqHeader.isNull())
	{
		//m_pErrorPreReq = reqHeader;
	}
	//m_pStrErrorLog = "";
	req = indexToReq(reciveData, findIndex);
	int reqSize = req.getSize();


	if (reqSize == 0)
	{
		ReqInterrFace temp;
		return temp;;
	}


	if (findIndex != 0)
	{
		m_pReadData.remove(0, findIndex);
	}



	QByteArray CalcCRC = req.getCRC();
	QByteArray reqCRC = req.DataCRC;
	if (CalcCRC != reqCRC)
	{
		//handleUploadError(FILE_CRC_ERROR);
		ReqInterrFace temp;
		return temp;;
	}
	return req;
}