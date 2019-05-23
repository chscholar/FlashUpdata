#include "SinByte.h"

SinByte::SinByte(int maxByte)
{
	m_iMaxId = maxByte;
	sByte = new QByteArray(maxByte, '0');

}

SinByte::~SinByte()
{

}