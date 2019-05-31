#include "SinSerialWriteWork.h"
#include "SinSerial.h"
SinSerialWriteWork::SinSerialWriteWork(QObject *parent)
	:QObject(parent)
{
	m_bIsRun = false;
	m_nCurrentWriteIndex = 0;
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
			//ͷ���
			char header[] = { 0x11, 0x22, 0x33, 0x44 };
			reqStruct.Header = header;


			//binFileId
			int nBinFileId = 1;
			reqStruct.BinFileId = QByteArray::number(nBinFileId, 10).toHex();

			// ��ǰ��i ���ļ��Ĵ�С
			int nFileSize = fileListData[i].size();
			reqStruct.BinFileSize = QByteArray::number(nFileSize, 10).toHex();

			//����id����ǰ�ļ����
			int nTransId = i;
			reqStruct.TransId = QByteArray::number(nTransId, 10).toHex();

			//��ǰ�ļ��������к�
			int nTransSeqNum = j;
			reqStruct.TransSeqNum = QByteArray::number(nTransSeqNum, 10).toHex();

			//��ǰ��j �����Ĵ�С
			int nDataLength = fileData[j].size();
			reqStruct.DataLength = QByteArray::number(nDataLength, 10).toHex();
			//���������
			reqStruct.data = fileData[j].toHex().data();

			m_pWriteData.push_back(reqStruct);

		}
	}
}

void SinSerialWriteWork::sendData()
{
	if (m_bIsRun)
	{
		ReqInterrFace req;
		
		req = m_pWriteData.at(m_nCurrentWriteIndex);
		sinserialSingle::getInstance().sendData(req);
	}
}
