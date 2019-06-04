#ifndef SINNETWORKCAPTHREAD_H
#define SINNETWORKCAPTHREAD_H
#include <QObject>
#include "CSingleTon.h"
#include <QThread>
#include "pcap.h"
#include "remote-ext.h"
#include "TableCapModel.h"

class SinNetWorkCapThread : public QObject
{
	Q_OBJECT
public:
	SinNetWorkCapThread(QObject *parent = 0);
	~SinNetWorkCapThread();
	void startThread(pcap_if_t * device);
	void stopThread();
protected:
private:
	QThread *m_pThread;
	pcap_if_t *m_pDevice;
	bool m_bIsStartCap;
	void addDataToTableCap(const pcap_pkthdr *pkt_header, const u_char *pkt_data);

private slots :
	int startCap();
};

typedef CSingleton<SinNetWorkCapThread> SinNetWorkCapSingle;
#endif
