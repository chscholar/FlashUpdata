#ifndef SINBYTE_H
#define SINBYTE_H
#include <QByteArray>
#include <QObject>
#include <QList>
#include "JQChecksum.h"
#include <QDebug>

#pragma pack(1) 

/* define command values. */
const QByteArray    MSG_CMD_HANDSHAKE_SYN = "8000";
const QByteArray     MSG_CMD_HANDSHAKE_SYNARK = "0001";
const QByteArray     MSG_CMD_HANDSHAKE_ARK = "8001";

/* PC-->UE */
const QByteArray    MSG_CMD_DOWNLOADFILE_REQ = "0002";
const QByteArray    MSG_CMD_DOWNLOADFILE_DATA = "0003";
const QByteArray     MSG_CMD_DOWNLOADFILE_END = "0004";
const QByteArray     MSG_CMD_UPLOADFILE_REQ = "0005";
const QByteArray     MSG_CMD_UPLOADFILE_DATA_RSP = "0006";
const QByteArray     MSG_CMD_UPLOADFILE_END_RSP = "0007";

/* UE-->PC */
const QByteArray     MSG_CMD_DOWNLOADFILE_REQ_RSP = "8002";
const QByteArray     MSG_CMD_DOWNLOADFILE_DATA_RSP = "8003";
const QByteArray     MSG_CMD_DOWNLOADFILE_END_RSP = "8004";
const QByteArray     MSG_CMD_UPLOADFILE_REQ_RSP = "8005";
const QByteArray     MSG_CMD_UPLOADFILE_DATA = "8006";
const QByteArray    MSG_CMD_UPLOADFILE_END = "8007";


QByteArray     MSG_PROTO_HEADER_TAG = "EBA846B9";


/*
{0x0000000A, 0x10000000,   3*SIZE_1M,   0x0},  /* Test with PC 

*/

/* error code  */
const QByteArray FILE_OK = "0000";
const QByteArray FILE_NOT_EXIST = "0100";
const QByteArray FILE_SIZE_TOO_BIG = "0200";
const QByteArray FILE_SIZE_TOO_SMALL = "0300";
const QByteArray FILE_CRC_ERROR = "0400";
const QByteArray FILE_HDR_INFO_NOT_MATCH = "0500";
const QByteArray FILE_SIGNATURE_ERROR = "0600";
const QByteArray FILE_WRITE_FLASH_ERROR = "0700";
const QByteArray FILE_READ_FLASH_ERROR = "0800";
const QByteArray FILE_MISSING_PACKET_ERROR = "0900";
const QByteArray FILE_INNER_ERROR = "0A00";


struct ReqInterrFace
{
	QByteArray Header; //四字节					头标记 固定值 
	QByteArray Length; //两字节					后面所有数据长度（出去header 和本字段外） 如果是命令包则到dataLength字段 
	QByteArray Command; //两字节				命令 见含义
	QByteArray BinFileId; // 四字节				当前烧录文件id,boot中记录每个文件id 以及对应flash地址
	QByteArray BinFileSize; //四字节			当前输入文件总大小
	QByteArray TransId; //四字节				每个文件传输事务id， 如果有多个文件则每个文件传输id不一样
	QByteArray TransSeqNum; //四字节			传输包序列
	QByteArray DataLength; //两字节				data 数据长度
	QByteArray DataCRC; //2字节					校验算法
	QByteArray data;
	QByteArray Padding; //填充字段，保证每个字段4字节对齐

	void  setLength()
	{
		this->Padding.clear();
		bool ok;
		QString qstrDataLength = this->DataLength;
		int dataByteLength = qstrDataLength.toInt(&ok, 16);
	
		int divLength = dataByteLength % 4;
		if (divLength != 0)
		{
			for (int i = 0; i < divLength * 2; i++ )
			{
				this->Padding.push_front('0');
			}
		}

		int nlength = (Command.size() + BinFileId.size() + BinFileSize.size() + TransId.size() + TransSeqNum.size() + DataLength.size() + DataCRC.size() + data.size()) / 2;
		QString str = QString("%1").arg(nlength, 4, 16, QLatin1Char('0'));
		this->Length = str.toUtf8().data();
	}

	void setDataLength()
	{
		int nDataLength = data.size() / 2;
		QString str = QString("%1").arg(nDataLength, 4, 16, QLatin1Char('0'));
		this->DataLength = str.toUtf8().data();
	}

	QString getXORresult(QString str1, QString str2)
	{
		//QString str1 = "e7";
		//QString str2 = "46";
		bool OK;
		int val1 = str1.toInt(&OK, 16);
		int val2 = str2.toInt(&OK, 16);

		int result = val1 ^ val2;
		QString strCrc = QString::number(result, 16);
		return strCrc;
	}
	void setCRC()
	{

		

		/*char  chekSum = data[0];
		for (int i = 0; i < this->data.size(); i++)
		{
		char temp = this->data.at(i);
		chekSum = chekSum ^ temp;
		}

		this->DataCRC.clear();
		QString str = QString("%1").arg(chekSum, 4, 16, QLatin1Char('0'));
		this->DataCRC = str.toUtf8().data();*/

		QStringList dataCrc;
		for (int i = 0; i < this->data.size();i+=2)
		{  
			QString temp = this->data.mid(i, 2);
			dataCrc.push_back(temp);
		}

		QString strXor;
		if (dataCrc.size() > 0)
		{
			strXor = dataCrc.at(0);
		}
		else {
			this->DataCRC = "0000";
			return;
		}

		for (int i = 1; i < dataCrc.size(); i++)
		{
			QString str1 = dataCrc.at(i);
			
			strXor = getXORresult(strXor, str1);
		}
		bool ok;
		QString str;
		int divCount = 4 - strXor.size();
		if (divCount == 3)
		{
			str = "000" + strXor;
		}
		else if (divCount == 2){
			str = "00" + strXor;
		} 
		else if (divCount == 1)
		{
			str = "0" + strXor;
		}
		this->DataCRC = str.toUtf8().data();
	}
	
};
Q_DECLARE_METATYPE(ReqInterrFace)

class SinByte:public QObject
{
public:
	SinByte(QByteArray byteData,QObject *parent = 0);
	~SinByte();
	 QByteArray getNData(int n);
private:
	QByteArray m_pByte;
	int m_nCurrentId;
	int m_nTotalSize;
};


#endif
