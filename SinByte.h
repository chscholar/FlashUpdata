#ifndef SINBYTE_H
#define SINBYTE_H
#include <QByteArray>
#include <QObject>
#include <QList>

#pragma pack(1) 
struct TransInterFace
{
	char Header[4]; //头标记 固定值
	char Length[2]; //后面所有数据长度（出去header 和本字段外） 如果是命令包则到dataLength字段
	char Command[2]; //命令 见含义
	char BinFileId[4]; //当前烧录文件id,boot中记录每个文件id 以及对应flash地址
	char BinFileSize[4]; //当前输入文件总大小
	char TransId[4]; //每个文件传输事务id， 如果有多个文件则每个文件传输id不一样
	char TransSeqNum[4]; //传输包序列
	char DataLength[4]; //data 数据长度
	char DataCRC[4]; //校验算法
	char Padding[1]; //填充字段，保证每个字段4字节对齐
	//QByteArray data;


};

class SinByte
{
public:
	SinByte();
	~SinByte();
};


#endif
