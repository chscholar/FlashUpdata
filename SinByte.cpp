#include "SinByte.h"

SinByte::SinByte()
{

}

SinByte::~SinByte()
{

}

void SinByte::operator=(QString str){

}



TransInterFace SinByte::toTransFace(const char ch[])
{
	TransInterFace tif;
	int nSizeOf = sizeof(tif);

	int nHeader = sizeof(tif.Header) / sizeof(char);
	strncpy(tif.Header, ch,nHeader);
	ch += nHeader;

	int nLength = sizeof(tif.Length) / sizeof(char);
	strncpy(tif.Length, ch, nLength);
	ch += nLength;

	int nComman = sizeof(tif.Command) / sizeof(char);
	strncpy(tif.Command, ch, nComman);
	ch += nComman;

	int nBinFileId = sizeof(tif.BinFileId) / sizeof(char);
	strncpy(tif.BinFileId, ch, nBinFileId);
	ch += nBinFileId;

	int nBinFileSize = sizeof(tif.BinFileSize) / sizeof(char);
	strncpy(tif.BinFileId, ch, nBinFileSize);
	ch += nBinFileSize;

	int nTransId = sizeof(tif.TransId) / sizeof(char);
	strncpy(tif.TransId, ch, nTransId);
	ch += nBinFileSize;

	int nTransSeqNum = sizeof(tif.TransSeqNum) / sizeof(char);
	strncpy(tif.TransSeqNum, ch, nTransSeqNum);
	ch += nTransSeqNum;

	int nDataLength = sizeof(tif.DataLength) / sizeof(char);
	strncpy(tif.DataLength, ch, nDataLength);
	ch += nDataLength;

	int nDataCRC = sizeof(tif.DataCRC) / sizeof(char);
	strncpy(tif.DataCRC, ch, nDataCRC);
	ch += nDataCRC;

	return tif;
}


QList<TransInterFace> SinByte::toDecode(const char ch[]){

	/*
	char Header[4];
	char Length[2];
	char Commandp[4];
	char BinFileId[4];
	char BinFileSize[4];
	char TransId[4];
	char TransSeqNum[4];
	char DataLength[4];
	char DataCRC[4];
	char Padding[1];
	QByteArray data;
	*/

	char srcCh[] = {
		0x11,0x22,0x33,0x44, //header
		0x11,0x22,			//length
		0x11,0x22,0x33,0x44, //command
		0x11,0x22,0x33,0x44, //binFileID
		0x11,0x22,0x33,0x44, //binFileSize
		0x11,0x22,0x33,0x44, //trandId
		0x11,0x22,0x33,0x44,//transSeq
		0x11,0x22,0x33,0x44, //DataLength;
		0x11,0x22,0x33,0x44, //DataCRC;
		0x11,0x22,0x33,0x44,//padding
		0x11,0x22,0x33,0x44,//data
	};


	transIFace = toTransFace(ch);

	QList <TransInterFace> Result;

	return Result;


}