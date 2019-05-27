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


TransInterFace FileUtil::toTransFace(QByteArray byteData)
{
	TransInterFace tif;
	int nSizeOf = sizeof(tif);



	int nHeader = sizeof(tif.Header) / sizeof(char);
	strncpy(tif.Header, byteData, nHeader);
	byteData += nHeader;

	int nLength = sizeof(tif.Length) / sizeof(char);
	strncpy(tif.Length, byteData, nLength);
	byteData += nLength;

	int nComman = sizeof(tif.Command) / sizeof(char);
	strncpy(tif.Command, byteData, nComman);
	byteData += nComman;

	int nBinFileId = sizeof(tif.BinFileId) / sizeof(char);
	strncpy(tif.BinFileId, byteData, nBinFileId);
	byteData += nBinFileId;

	int nBinFileSize = sizeof(tif.BinFileSize) / sizeof(char);
	strncpy(tif.BinFileId, byteData, nBinFileSize);
	byteData += nBinFileSize;

	int nTransId = sizeof(tif.TransId) / sizeof(char);
	strncpy(tif.TransId, byteData, nTransId);
	byteData += nBinFileSize;

	int nTransSeqNum = sizeof(tif.TransSeqNum) / sizeof(char);
	strncpy(tif.TransSeqNum, byteData, nTransSeqNum);
	byteData += nTransSeqNum;

	int nDataLength = sizeof(tif.DataLength) / sizeof(char);
	strncpy(tif.DataLength, byteData, nDataLength);
	byteData += nDataLength;

	int nDataCRC = sizeof(tif.DataCRC) / sizeof(char);
	strncpy(tif.DataCRC, byteData, nDataCRC);
	byteData += nDataCRC;

	return tif;
}


QList<TransInterFace> FileUtil::toDecode(QByteArray fileByteData){

	QList <TransInterFace> Result;
	for (int i = 0; i < fileByteData.size(); i += 10)
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