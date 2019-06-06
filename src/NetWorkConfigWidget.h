#ifndef NETWORKCONFIGWIDGET_H
#define NETWORKCONFIGWIDGET_H
#include <QWidget>
#include <QStringList>
#include <QTableView>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QPushButton>
#include "pcap.h"
#include "remote-ext.h"
#include "TableCapModel.h"
#include <QListView>
#include <QStringListModel>
#include <QTreeView>
#include <QLabel>
#pragma execution_character_set("utf-8")


struct Header
{
	QString qstrSrcMac;
	QString qstrDestMac;

};

/*******************************************IP Start*************************************************/

struct  IPHeader
{
	QString qstrUpProtocal ;
	QString qstrNetLayer;
	QString qstrVersion;
	QString qstrHeaderLength;
	QString qstrServiceType ;
	QString qstrTotalLength ;
	QString qstrNetDef ;
	QString qstrMetFlag ;
	QString qstrPieceOffset;
	QString qstrAliveTime;
	QString qstrHeaderCheckSum;
	QString qstrSrcIp;
	QString qstrDestIp;
};

struct TCPHeader 
{
	Header header;
	IPHeader ip;
	QString qstrUpProtocal;
	QString qstrTransLayer;
	QString qstrSrcPort;
	QString qstrDestPort;
	QString qstrSequNum ;
	QString qstrAckNum ;
	QString qstrTcpHeaderLength;
	QString qstrCtrlBit;
	QString qstrUrgenURG;
	QString qstrAckConfirm;
	QString qstrPushPSH;
	QString qstrRestRst;
	QString qstrSyncSyn ;
	QString qstrOverFin;
	QString qstrWindows ;
	QString qstrCheckSum ;
	QString qstrUrgenPoint;
};

struct UDPHeader
{
	Header header;
	IPHeader ip;
	QString qstrUpProtocal;
	QString qstrTransLayer;
	QString qstrSrcPort;
	QString qstrDestPort;
	QString qstrUdpHeaderLength;
	QString qstrCheckSum;

};

struct DNSHeader
{
	UDPHeader udp;
	QString qstrAppWork;
	QString qstrNetDef;
	QString qstrNetFlag;
	QString qstrQuestionNum;
	QString qstrResourceNoteNum;
	QString qstrAuthResourceNoteNum;
	QString qstrExtraResourceNoteNum;
};

struct ICMPHeader
{
	Header header;
	IPHeader ip;
	QString qstrUpProtocal;
	QString qstrTransLayer ;

	QString qstrType;
	QString qstrCode ;
	QString qstrCheckSum ;
};

struct IGMPHeader
{
	Header header;
	IPHeader ip;
	QString qstrUpProtocal;

};

struct EGPHeader
{
	Header header;
	IPHeader ip;
	QString qstrUpProtocal;

};

struct IPv6Header
{
	Header header;
	IPHeader ip;
	QString qstrUpProtocal;

};

struct OSPFHeader
{
	Header header;
	IPHeader ip;
	QString qstrUpProtocal;

};

struct UnKnowIPHeader
{
	Header header;
	IPHeader ip;
	QString qstrUpProtocal;

};
/*******************************************IP End*************************************************/


struct ARPHeader
{
	Header header;
	QString qstrUpProtocal;
	QString qstrNetLayer;
	QString qstrHardType;
	QString qstrProtocalType;
	QString qstrHardLength ;
	QString qstrProtocalLength ;
	QString qstrOpType ;
	QString qstrSendMacAddr;
	QString qstrSendProtocalAddr;
	QString qstrReciveMacAddr ;
	QString qstrReciveProtocalAddr;
};

struct RARPHeader
{
	Header header;
	QString qstrUpProtocal;

};

struct UnKnowHeader
{
	Header header;
	QString qstrUpProtocal;

};

class NetWorkConfigWidget : public QWidget
{
	Q_OBJECT
public:
	NetWorkConfigWidget(QWidget *parent = 0);
	~NetWorkConfigWidget();
protected:
	void initUi();
	bool getNetInfo();
	void initTableViewConfig();
	pcap_if_t* getSelectDevice();
	void addDataToTableCap(const pcap_pkthdr *pkt_header, const u_char *pkt_data);
	QTableView *m_pNetDeviceTableView;
	QLineEdit *m_pLineEdit;
	QStandardItemModel *m_pNetDeviceModel;
	bool m_bIsBindStatus;
	bool m_bIsStartCap;
	QPushButton *m_pBindButton;
	QPushButton *m_pStartCapButton;
	pcap_if_t *d;
	pcap_if_t *alldevs; //所有设备指针

	QTableView *m_pCapTableView;
	TableCapModel *m_pCapModel;

	QTreeView *m_plistView;
	QStandardItemModel *m_pTreeModel;

	QList<pcap_pkthdr *> m_pktHeaders;
	QList<u_char *> m_pktDatas;
	int m_totalCount;

	QLabel *m_pTotalByte;

private:
	public slots :
		void onTableNetDeviceClicked(const QModelIndex &);
	void onTableNetCapClicked(const QModelIndex &);
	void slotBindNetWork();
	void slotStartCap();
	void slotUpDataCapTable();
};

#endif
