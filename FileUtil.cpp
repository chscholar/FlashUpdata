#include "FileUtil.h"
#include <QFile>
#include <QIODevice>

FileUtil::FileUtil(QObject *parent)
	:QObject(parent)
{

}

FileUtil::~FileUtil()
{

}

void FileUtil::fillStructItem()
{
	
}

ReqInterrFace FileUtil::toTransFace(QByteArray byteData)
{

	/*
	QByteArray Header; //ͷ��� �̶�ֵ
	QByteArray Length; //�����������ݳ��ȣ���ȥheader �ͱ��ֶ��⣩ ������������dataLength�ֶ�
	QByteArray Command; //���� ������
	QByteArray BinFileId; //��ǰ��¼�ļ�id,boot�м�¼ÿ���ļ�id �Լ���Ӧflash��ַ
	QByteArray BinFileSize; //��ǰ�����ļ��ܴ�С
	QByteArray TransId; //ÿ���ļ���������id�� ����ж���ļ���ÿ���ļ�����id��һ��
	QByteArray TransSeqNum; //���������
	QByteArray DataLength; //data ���ݳ���
	QByteArray DataCRC; //У���㷨
	QByteArray Padding; //����ֶΣ���֤ÿ���ֶ�4�ֽڶ���
	QByteArray data;
	*/

	ReqInterrFace tif;
	int nSizeOf = sizeof(tif);

	SinByte *sByte = new SinByte(byteData);

	QByteArray hedadrData = sByte->getNData(4);
	tif.Header = hedadrData;

	QByteArray lengthData = sByte->getNData(4);
	tif.Length = lengthData;

	QByteArray commonData = sByte->getNData(4);
	tif.Command = lengthData;

	QByteArray BinFileIdData = sByte->getNData(4);
	tif.BinFileId = BinFileIdData;

	QByteArray BinFileSizeData = sByte->getNData(4);
	tif.BinFileSize = BinFileSizeData;

	QByteArray TransIdData = sByte->getNData(4);
	tif.TransId = TransIdData;

	QByteArray TransSeqNumData = sByte->getNData(4);
	tif.TransSeqNum = TransSeqNumData;

	QByteArray DataLengthData = sByte->getNData(4);
	tif.DataLength = DataLengthData;

	QByteArray DataCRCData = sByte->getNData(4);
	tif.DataCRC = DataCRCData;

	QByteArray PaddingData = sByte->getNData(4);
	tif.Padding = PaddingData;

	QByteArray DataData = sByte->getNData(4);
	tif.data = DataData;

	return tif;
}


QList<QByteArray> FileUtil::toFileDataDecode(QByteArray fileByteData){

	QList <QByteArray> Result;
	/*for (int i = 0; i < fileByteData.size(); i += 100)
	{
		TransInterFace transIFace;
		transIFace = toTransFace(fileByteData);
		Result.push_back(transIFace);
	}*/
	SinByte *sByte = new SinByte(fileByteData);
	int nResult = fileByteData.size() / 1000;
	int nDiv = fileByteData.size() - nResult * 1000;

	while (nResult >=1)
	{
		QByteArray data = sByte->getNData(1000);
		Result.push_back(data);
		nResult--;
	}

	QByteArray data = sByte->getNData(nDiv);
	Result.push_back(data);
	

	return Result;
}

QList<QList<QByteArray>> FileUtil::getDataFramFromFilePath(QStringList pathList)
{
	QList<QList<QByteArray>> result;
	for (int i = 0; i < pathList.size(); i++)
	{
		QString fileName = pathList.at(i);
		QFile file(fileName);

		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			break;

		int nFileSize = file.bytesAvailable();
		QByteArray fileData = file.read(nFileSize);

		QList<QByteArray> FileDivData = toFileDataDecode(fileData);
		result.push_back(FileDivData);
		file.close();
	}

	return result;
}