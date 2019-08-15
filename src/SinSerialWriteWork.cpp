#include "SinSerialWriteWork.h"
#include "SinSerial.h"
#include "SinTaskQueue.h"
#include <QDebug>
#include "CommonHeader.hpp"

SinSerialWriteWork::SinSerialWriteWork(QObject *parent)
	:QObject(parent)
{
	m_bIsRun = false;
	m_nCurrentWriteIndex = 0;

	connect(&sinTaskQueueSingle::getInstance(), SIGNAL(signalWriteNextData()), this, SLOT(sendData()));
}

SinSerialWriteWork::~SinSerialWriteWork()
{

}

void SinSerialWriteWork::start()
{
	m_bIsRun = true;
}

void SinSerialWriteWork::stop()
{
	m_bIsRun = false;
}

void SinSerialWriteWork::setWriteData(QList<QList<QByteArray>> fileListData)
{
	m_pWriteData.clear();
	for (int i = 0; i < fileListData.size(); i++)
	{
		QList<QByteArray> fileData = fileListData.at(i);
		for (int j = 0; j < fileData.size(); j++)
		{
			ReqInterrFace reqStruct;
			//头标记
			reqStruct.Header = "eba846b9";


			//binFileId
			int nBinFileId = 1;
			//reqStruct.BinFileId = QByteArray::number(nBinFileId);
			reqStruct.BinFileId = "0000000D";

			// 当前第i 个文件的大小
			int nFileSize = fileListData[i].size();
			QString strfileSize = QString("%1").arg(nFileSize, 4, 16, QLatin1Char('0'));
			reqStruct.BinFileSize = strfileSize.toUtf8().data();

			//事务id，当前文件标记
			int nTransId = i;
			QString strTranId = QString("%1").arg(nTransId, 4, 16, QLatin1Char('0'));
			reqStruct.TransId = strTranId.toUtf8().data();

			//当前文件传输序列号
			int nTransSeqNum = j;
			QString strTransDeqNum = QString("%1").arg(nTransSeqNum, 4, 16, QLatin1Char('0'));
			reqStruct.TransSeqNum = strTransDeqNum.toUtf8().data();

			//当前第j 个包的大小
			int nDataLength = fileData[j].size();
			QString strDataLength = QString("%1").arg(nDataLength, 4, 16, QLatin1Char('0'));
			reqStruct.DataLength = strDataLength.toUtf8().data();

			//传输的内容
			reqStruct.data = fileData[j].toHex();

			m_pWriteData.push_back(reqStruct);

		}
	}
}

ReqInterrFace SinSerialWriteWork::startUploadReq()
{
	ReqInterrFace req;
	req.Header = "eba846b9";
	req.Length = "0016";
	req.Command = "0005";
	req.BinFileId = "00000001";
	req.BinFileSize = "00000000";
	req.TransId = "00000000";
	req.TransSeqNum = "00000000";
	req.DataLength = "0000";
	req.Padding = "00000000";
	return req;
}

ReqInterrFace SinSerialWriteWork::getIndexWriteData(int index)
{
	ReqInterrFace req  =  m_pWriteData.at(index);
	return req;
}


void SinSerialWriteWork::sendData()
{
	
	//ReqInterrFace req = startUploadReq();
	//sinserialSingle::getInstance().sendData(req);
}


void SinSerialWriteWork::sendWriteDataPack(QString strLog,QByteArray bytedata)
{
	sinserialSingle::getInstance().sendData(strLog,bytedata);
}