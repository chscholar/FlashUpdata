#ifndef SINSERIALTHREADMANAGER_H
#define SINSERIALTHREADMANAGER_H

#include <QObject>
#include "SinSerialReadThread.h"
#include "SinSerialWriteThread.h"
#include "CSingleTon.h"

class SinSerialThreadManager : public QObject
{
	Q_OBJECT
public:
	SinSerialThreadManager(QObject *parent = 0);
	~SinSerialThreadManager();
	void start();
	void stop();
	void setWriteData(QList<QList<QByteArray>> fileListData);
	void getReadData();
	void handShake();
protected:
	SinSerialReadThread *m_pReadThread;
	SinSerialWriteThread *m_pWriteThread;
private:
};
typedef CSingleton<SinSerialThreadManager> sinSerialThreadManagerSingle;
#endif
