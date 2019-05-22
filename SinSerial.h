#ifndef		SINSERIAL_H
#define  SINSERIAL_H
#include <QCoreApplication>
#include <QAtomicPointer>
#include <QReadWriteLock>
#include <QMutex>
#include <qDebug>
#include <QSerialPort>


class SinSerial
{
private:
	SinSerial();
	SinSerial(const SinSerial&);
	SinSerial& operator=(const SinSerial&);

	QReadWriteLock m_internalMutex;
	static QMutex m_mutex;
	static QAtomicPointer<QSerialPort> m_instance;

public:
	static QSerialPort& getInstance(void);
};
#endif