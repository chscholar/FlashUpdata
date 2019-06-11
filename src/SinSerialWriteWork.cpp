#include "SinSerialWriteWork.h"
#include "SinSerial.h"
#include "SinTaskQueue.h"
#include <QDebug>

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
			reqStruct.BinFileId = QByteArray::number(nBinFileId);

			// 当前第i 个文件的大小
			int nFileSize = fileListData[i].size();
			reqStruct.BinFileSize = QByteArray::number(nFileSize, 10);

			//事务id，当前文件标记
			int nTransId = i;
			reqStruct.TransId = QByteArray::number(nTransId, 10);

			//当前文件传输序列号
			int nTransSeqNum = j;
			reqStruct.TransSeqNum = QByteArray::number(nTransSeqNum, 10);

			//当前第j 个包的大小
			int nDataLength = fileData[j].size();
			reqStruct.DataLength = QByteArray::number(nDataLength, 10);
			//传输的内容
			reqStruct.data = fileData[j];

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
	req.BinFileId = "0000000A";
	req.BinFileSize = "00000000";
	req.TransId = "00000000";
	req.TransSeqNum = "00000000";
	req.DataLength = "0000";
	req.Padding = "00000000";
	return req;
}

void SinSerialWriteWork::sendData()
{
	
	ReqInterrFace req = startUploadReq();
	sinserialSingle::getInstance().sendData(req);
	
		//req.Header = "eba846b9";

		
		/*if (m_nCurrentWriteIndex <= m_pWriteData.size())
		{
		req = m_pWriteData.at(m_nCurrentWriteIndex);
		sinserialSingle::getInstance().sendData(req);
		m_nCurrentWriteIndex++;
		qDebug() << "send insex :" << m_nCurrentWriteIndex;
		}
		else {
		qDebug() << "send Over";
		}*/
		
}


void SinSerialWriteWork::sendHandSharkPack(QByteArray bytedata)
{
	sinserialSingle::getInstance().sendData(bytedata);
}