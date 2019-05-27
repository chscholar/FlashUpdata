#ifndef SINBYTE_H
#define SINBYTE_H
#include <QByteArray>
#include <QObject>
#include <QList>

#pragma pack(1) 
struct TransInterFace
{
	char Header[4]; //ͷ��� �̶�ֵ
	char Length[2]; //�����������ݳ��ȣ���ȥheader �ͱ��ֶ��⣩ ������������dataLength�ֶ�
	char Command[2]; //���� ������
	char BinFileId[4]; //��ǰ��¼�ļ�id,boot�м�¼ÿ���ļ�id �Լ���Ӧflash��ַ
	char BinFileSize[4]; //��ǰ�����ļ��ܴ�С
	char TransId[4]; //ÿ���ļ���������id�� ����ж���ļ���ÿ���ļ�����id��һ��
	char TransSeqNum[4]; //���������
	char DataLength[4]; //data ���ݳ���
	char DataCRC[4]; //У���㷨
	char Padding[1]; //����ֶΣ���֤ÿ���ֶ�4�ֽڶ���
	//QByteArray data;


};

class SinByte
{
public:
	SinByte();
	~SinByte();
};


#endif
