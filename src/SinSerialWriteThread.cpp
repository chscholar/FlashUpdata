#include "SinSerialWriteThread.h"
#include "SinSerial.h"
#include "SinSerial.h"
#include "SinSerial.h"

SinSerialWriteThread::SinSerialWriteThread(QObject *parent)
	:QObject(parent)
{

}

SinSerialWriteThread::~SinSerialWriteThread()
{

}

void SinSerialWriteThread::start()
{

}

void SinSerialWriteThread::stop()
{

}

void SinSerialWriteThread::setWriteData(QList<QList<QByteArray>> fileListData)
{
	m_pWriteData = fileListData;


	//for (int i = 0; i < fileListData.size(); i++)
	//{
	//	QList<QByteArray> fileData = fileListData.at(i);
	//	for (int j = 0; j < fileData.size(); j++)
	//	{
	//		ReqInterrFace reqStruct;
	//		//ͷ���
	//		char header[] = { 0x11, 0x22, 0x33, 0x44 };
	//		reqStruct.Header = header;


	//		//binFileId
	//		int nBinFileId = 1;
	//		reqStruct.BinFileId = QByteArray::number(nBinFileId, 10).toHex();

	//		// ��ǰ��i ���ļ��Ĵ�С
	//		int nFileSize = fileListData[i].size();
	//		reqStruct.BinFileSize = QByteArray::number(nFileSize, 10).toHex();

	//		//����id����ǰ�ļ����
	//		int nTransId = i;
	//		reqStruct.TransId = QByteArray::number(nTransId, 10).toHex();

	//		//��ǰ�ļ��������к�
	//		int nTransSeqNum = j;
	//		reqStruct.TransSeqNum = QByteArray::number(nTransSeqNum, 10).toHex();

	//		//��ǰ��j �����Ĵ�С
	//		int nDataLength = fileData[j].size();
	//		reqStruct.DataLength = QByteArray::number(nDataLength, 10).toHex();
	//		//���������
	//		reqStruct.data = fileData[j].toHex().data();


	//		int a = 1;
	//	}
	//}
}

void SinSerialWriteThread::sendData()
{
	ReqInterrFace req;
	sinserialSingle::getInstance().sendData(req);
}
