#ifndef SINBYTE_H
#define SINBYTE_H
#include <QByteArray>
#include <QObject>
#include <QList>
#include "JQChecksum.h"
#include <QDebug>
#include "CommonHeader.hpp"


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
