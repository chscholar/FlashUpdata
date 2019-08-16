#ifndef	SINTASKQUEUE_H
#define  SINTASKQUEUE_H
#include <QObject>
#include "QList"
#include <QQueue>
#include "CSingleTon.h"
#include "SinNetWorkCapThread.h"
#include "pcap.h"
#include "remote-ext.h"

class SinTaskQueue : public QObject
{
	Q_OBJECT
public:
	SinTaskQueue(QObject *parent = 0);
	~SinTaskQueue();
	void pushBackReadData(QString data);
	QString popBackReadData();

	void pushBackCapData(CapData data);
	CapData popBackCapData();
	int getCapDataSize();

	void pushBackPktHeaderData(const pcap_pkthdr* data);
	pcap_pkthdr* popIndexPktHeaderData(int index);

	void pushBackPktDataData(const u_char *data);
	u_char *popIndexPktDataData(int index);
private:

	QQueue<QString> m_pReadData;
	QQueue<CapData> m_pCapData;

	QList<pcap_pkthdr *> m_pPktHeader;
	QList<u_char*> m_pPktData;
	public slots:
signals :
	void signalReadData();
	void signalWriteNextData();
	void signalCapData();
	//void signalWriteNextData();
};

typedef CSingleton<SinTaskQueue> sinTaskQueueSingle;
#endif
