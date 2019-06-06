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
#include <QVariant>
#include <QMap>

#pragma execution_character_set("utf-8")


struct LinkHeader
{
	QString qstrLinkLayer;
	QString qstrSrcMac;
	QString qstrDestMac;
	bool isEmpty()
	{
		return this->qstrLinkLayer.isEmpty();
	}

};
Q_DECLARE_METATYPE(LinkHeader)

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

	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}
};
Q_DECLARE_METATYPE(IPHeader)

struct TCPHeader 
{
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

	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}
};
Q_DECLARE_METATYPE(TCPHeader)

struct UDPHeader
{
	QString qstrUpProtocal;
	QString qstrTransLayer;
	QString qstrSrcPort;
	QString qstrDestPort;
	QString qstrUdpHeaderLength;
	QString qstrCheckSum;

	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}
};
Q_DECLARE_METATYPE(UDPHeader)

struct DNSHeader
{
	QString qstrAppWork;
	QString qstrNetDef;
	QString qstrNetFlag;
	QString qstrQuestionNum;
	QString qstrResourceNoteNum;
	QString qstrAuthResourceNoteNum;
	QString qstrExtraResourceNoteNum;
	bool isEmpty()
	{
		return this->qstrAppWork.isEmpty();
	}
};
Q_DECLARE_METATYPE(DNSHeader)

struct ICMPHeader
{
	QString qstrUpProtocal;
	QString qstrTransLayer ;

	QString qstrType;
	QString qstrCode ;
	QString qstrCheckSum ;
	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}
};
Q_DECLARE_METATYPE(ICMPHeader)

struct IGMPHeader
{
	QString qstrUpProtocal;
	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}

};
Q_DECLARE_METATYPE(IGMPHeader)

struct EGPHeader
{
	QString qstrUpProtocal;
	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}
};
Q_DECLARE_METATYPE(EGPHeader)

struct IPv6Header
{
	QString qstrUpProtocal;
	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}
};
Q_DECLARE_METATYPE(IPv6Header)

struct OSPFHeader
{
	QString qstrUpProtocal;
	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}

};
Q_DECLARE_METATYPE(OSPFHeader)

struct UnKnowIPHeader
{
	QString qstrUpProtocal;
	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}

};
Q_DECLARE_METATYPE(UnKnowIPHeader)
/*******************************************IP End*************************************************/


struct ARPHeader
{
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
	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}
};
Q_DECLARE_METATYPE(ARPHeader)

struct RARPHeader
{
	QString qstrUpProtocal;
	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}

};
Q_DECLARE_METATYPE(RARPHeader)

struct UnKnowHeader
{
	QString qstrUpProtocal;
	bool isEmpty()
	{
		return this->qstrUpProtocal.isEmpty();
	}

};
Q_DECLARE_METATYPE(UnKnowHeader)

enum protocalType
{
	PROTOCAL_LINK,
	PROTOCAL_IP,
	PROTOCAL_TCP,
	PROTOCAL_UDP,
	PROTOCAL_DNS,
	PROTOCAL_ICMP,
	PROTOCAL_IGMP,
	PROTOCAL_OSPF,
	PROTOCAL_RARP,
	PROTOCAL_ARP,
	PROTOCAL_UNKNOW,
	PROTOCAL_UNKNOWIP,
	PROTOCAL_IPV6,
	PROTOCAL_EGP,
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
