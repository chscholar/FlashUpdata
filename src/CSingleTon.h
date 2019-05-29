#ifndef SINGLETON_H
#define SINGLETON_H
#include <QCoreApplication>
#include <QAtomicPointer>
#include <QReadWriteLock>
#include <QMutex>
#include <qDebug>

template<class T>
class CSingleton
{
private:
	CSingleton();
	CSingleton(const CSingleton<T> &);
	CSingleton<T>& operator=(const CSingleton<T>&);

	QReadWriteLock m_internalMutex;
	static QMutex m_mutex;
	static QAtomicPointer<T> m_instance;

public:
	static T& getInstance(void);
};

template<class T>
QMutex CSingleton<T>::m_mutex(QMutex::Recursive);

template<class T>
QAtomicPointer<T>CSingleton<T>::m_instance;

template<typename T>
T& CSingleton<T>::getInstance(void)
{
//#ifndef Q_ATOMIC_POINTER_TEST_AND_SET_IS_SOMETIMES_NATIVE
//	if (!QAtomicPointer<T>::isTestAndSetNative())
//#endif
	{
		QMutexLocker locker(&m_mutex);
		if (m_instance.testAndSetOrdered(0, 0))
		{
			m_instance.testAndSetOrdered(0, new T);
		}
		return *m_instance.load();
	}
}

#endif // !SINGLETON_H

