#ifndef SINSERIALTHREADMANAGER_H
#define SINSERIALTHREADMANAGER_H

#include <QObject>
#include "SinSerialReadWork.h"
#include "SinSerialWriteWork.h"
#include "CSingleTon.h"
#include <QThread>

class SinSerialThreadManager : public QObject
{
	Q_OBJECT
public:
	SinSerialThreadManager(QObject *parent = 0);
	~SinSerialThreadManager();
	void stop();
	void setWriteData(QList<QList<QByteArray>> fileListData);
	void getReadData();
	void sendData();
protected:
	SinSerialReadWork *m_pReadWork;
	SinSerialWriteWork *m_pWriteWork;

	QThread *m_pReadThread;
	QThread *m_pWriteThread;;
private:
	public slots :
		void slotHandSharkOver();
	void slotWriteData(QByteArray byteData);
};
typedef CSingleton<SinSerialThreadManager> sinSerialThreadManagerSingle;
#endif
