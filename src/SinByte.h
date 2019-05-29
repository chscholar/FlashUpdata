#ifndef SINBYTE_H
#define SINBYTE_H
#include <QByteArray>
#include <QObject>
#include <QList>

#pragma pack(1) 
struct ReqInterrFace
{
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
