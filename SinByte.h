#ifndef SINBYTE_H
#define SINBYTE_H
#include <QByteArray>
#include <QObject>

class SinByte
{
public:
	SinByte(int maxByte = 0);
	~SinByte();
private:
	QByteArray *sByte;
	int m_iMaxId;
};


#endif
