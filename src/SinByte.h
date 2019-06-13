#ifndef SINBYTE_H
#define SINBYTE_H
#include <QByteArray>
#include <QObject>
#include <QList>

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
enum
{
	FILE_OK = 0x0,
	FILE_NOT_EXIST = 0x1,
	FILE_SIZE_TOO_BIG = 0x2,
	FILE_CRC_ERROR = 0x3,
	FILE_HDR_INFO_NOT_MATCH = 0x4,
	FILE_SIGNATURE_ERROR = 0x5,
	FILE_WRITE_FLASH_ERROR = 0x6,
	FILE_READ_FLASH_ERROR = 0x7,
	FILE_MISSING_PACKET_ERROR = 0x8,
	FILE_INNER_ERROR = 0x9,
};
struct ReqInterrFace
{
	QByteArray Header; //头标记 固定值
	QByteArray Length; //后面所有数据长度（出去header 和本字段外） 如果是命令包则到dataLength字段
	QByteArray Command; //命令 见含义
	QByteArray BinFileId; //当前烧录文件id,boot中记录每个文件id 以及对应flash地址
	QByteArray BinFileSize; //当前输入文件总大小
	QByteArray TransId; //每个文件传输事务id， 如果有多个文件则每个文件传输id不一样
	QByteArray TransSeqNum; //传输包序列
	QByteArray DataLength; //data 数据长度
	QByteArray DataCRC; //校验算法
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

	void setCRC()
	{
		char  chekSum = 0;
		for (int i = 0; i < this->data.size();i++)
		{
			char temp = this->data.at(i);
			chekSum = chekSum ^ temp;
		}

		this->DataCRC.clear();
		QString str = QString("%1").arg(chekSum, 4, 16, QLatin1Char('0'));
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
