#ifndef SINBYTE_H
#define SINBYTE_H
#include <QByteArray>
#include <QObject>
#include <QList>

#pragma pack(1) 

/* PC-->UE */
#define    MSG_CMD_HANDSHAKE_REQ     	 0x0001    
#define    MSG_CMD_DOWNLOADFILE_REQ      0x0002    
#define    MSG_CMD_DOWNLOADFILE_DATA     0x0003    
#define    MSG_CMD_DOWNLOADFILE_END      0x0004    
#define    MSG_CMD_UPLOADFILE_REQ        0x0005    
#define    MSG_CMD_UPLOADFILE_DATA_RSP   0x0006
#define    MSG_CMD_UPLOADFILE_END_RSP    0x0007    

/* UE-->PC */
#define    MSG_CMD_HANDSHAKE_REQ_RSP     0x8001
#define    MSG_CMD_DOWNLOADFILE_REQ_RSP  0x8002
#define    MSG_CMD_DOWNLOADFILE_DATA_RSP 0x8003
#define    MSG_CMD_DOWNLOADFILE_END_RSP  0x8004
#define    MSG_CMD_UPLOADFILE_REQ_RSP    0x8005
#define    MSG_CMD_UPLOADFILE_DATA       0x8006
#define    MSG_CMD_UPLOADFILE_END        0x8007  


#define    MSG_PROTO_HEADER_TAG  0xEBA846B9

enum {
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
	QByteArray Padding; //填充字段，保证每个字段4字节对齐
	QByteArray data;


};

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
