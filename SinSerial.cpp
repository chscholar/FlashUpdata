#include "SinSerial.h"

QMutex SinSerial::m_mutex(QMutex::Recursive);

QAtomicPointer<QSerialPort>SinSerial::m_instance;


QSerialPort& SinSerial::getInstance(void)
{
#ifndef Q_ATOMIC_POINTER_TEST_AND_SET_IS_SOMETIMES_NATIVE
	if (!QAtomicPointer<QSerialPort>::isTestAndSetNative())
#endif
	{
		QMutexLocker locker(&m_mutex);
		if (m_instance.testAndSetOrdered(0, 0))
		{
			m_instance.testAndSetOrdered(0, new QSerialPort);
		}
		return *m_instance.load();
	}
}