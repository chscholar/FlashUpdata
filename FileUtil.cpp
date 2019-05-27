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

TransInterFace FileUtil::toTransFace(QByteArray byteData)
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

	TransInterFace tif;
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


QList<TransInterFace> FileUtil::toDecode(QByteArray fileByteData){

	QList <TransInterFace> Result;
	for (int i = 0; i < fileByteData.size(); i += 100)
	{
		TransInterFace transIFace;
		transIFace = toTransFace(fileByteData);
		Result.push_back(transIFace);
	}
	return Result;
}

QList<QList<TransInterFace>> FileUtil::getDataFramFromFilePath(QStringList pathList)
{
	QList<QList<TransInterFace>> result;
	for (int i = 0; i < pathList.size(); i++)
	{
		QString fileName = pathList.at(i);
		QFile file(fileName);

		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			break;

		int nFileSize = file.bytesAvailable();
		QByteArray fileData = file.read(nFileSize);

		QList<TransInterFace> FileResultfileData = toDecode(fileData);
		result.push_back(FileResultfileData);
		file.close();
	}

	return result;
}