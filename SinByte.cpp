#include "SinByte.h"

#define  TRANS_LENGTH 10;

SinByte::SinByte(QByteArray byte, QObject *parent) :QObject(parent)
{
	m_pByte = byte;
	m_nCurrentId = 0;
	m_nTotalSize = m_pByte.size();

}

SinByte::~SinByte()
{

}

QByteArray SinByte::getNData(int n)
{
	QByteArray byteData;
	for (int i = m_nCurrentId; m_nCurrentId+n <= m_nTotalSize; i++)
	{
		if (i < m_nCurrentId+n)
		{
			byteData.push_back(m_pByte[i]);
		}
		else {
			m_nCurrentId += n;
			break;
		}
	}
	return byteData;
}
