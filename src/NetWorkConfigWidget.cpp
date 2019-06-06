#include "NetWorkConfigWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QNetworkInterface>
#include "pcap.h"
#include "remote-ext.h"
#include <QStandardItem>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include "head.h"
#include <WinSock2.h>
#include "SinNetWorkCapThread.h"
#include "SinTaskQueue.h"
static int TotalIndex = 0;
static int TcpIndex = 0;
static int UdpIndex = 0;

NetWorkConfigWidget::NetWorkConfigWidget(QWidget *parent)
	:QWidget(parent)
	, m_bIsBindStatus(false)
	, m_bIsStartCap(false)
	, m_totalCount(0)
{
	initUi();
	getNetInfo();
	//signalCapData
	connect(&sinTaskQueueSingle::getInstance(), SIGNAL(signalCapData()), this, SLOT(slotUpDataCapTable()));
}

NetWorkConfigWidget::~NetWorkConfigWidget(){

}

bool NetWorkConfigWidget::getNetInfo()
{
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
		return FALSE;
	int i = 0;
	for (d = alldevs; d; d = d->next)
	{
		m_pNetDeviceModel->setItem(i, 0, new QStandardItem((QString)d->name));
		m_pNetDeviceModel->setItem(i, 1, new QStandardItem((QString)d->description));
		i++;
	
	}
	d = NULL; //����Ա���������ʹ��

	return true;

}

void NetWorkConfigWidget::initUi()
{

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setMargin(10);

	//mainLayout->addSpacing(10);
	m_pNetDeviceTableView = new QTableView();
	mainLayout->addWidget(m_pNetDeviceTableView);

	//mainLayout->addSpacing(10);
	m_pLineEdit = new QLineEdit();
	mainLayout->addWidget(m_pLineEdit);
	
	//mainLayout->addSpacing(10);
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch();

	m_pBindButton = new QPushButton();
	m_pBindButton->setText("��");
	buttonLayout->addWidget(m_pBindButton);
	buttonLayout->addStretch();

	m_pStartCapButton = new QPushButton();
	m_pStartCapButton->setText("��ʼ����");
	buttonLayout->addWidget(m_pStartCapButton);
	buttonLayout->addStretch();
	mainLayout->addLayout(buttonLayout);

	m_pCapTableView = new QTableView();
	mainLayout->addWidget(m_pCapTableView);

	m_plistView = new QTreeView();
	mainLayout->addWidget(m_plistView);


	m_pTotalByte = new QLabel("����:0");
	mainLayout->addWidget(m_pTotalByte);

	mainLayout->addStretch(3);
	setLayout(mainLayout);

	connect(m_pBindButton, SIGNAL(clicked()), this, SLOT(slotBindNetWork()));
	connect(m_pStartCapButton, SIGNAL(clicked()), this, SLOT(slotStartCap()));

	initTableViewConfig();
}

void NetWorkConfigWidget::initTableViewConfig()
{
	m_pNetDeviceModel = new QStandardItemModel();
	m_pNetDeviceTableView->setModel(m_pNetDeviceModel);

	m_pNetDeviceModel->setColumnCount(2);
	m_pNetDeviceModel->setHeaderData(0, Qt::Horizontal, "�豸��");
	m_pNetDeviceModel->setHeaderData(1, Qt::Horizontal, "�豸����");

	m_pNetDeviceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//m_pTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_pNetDeviceTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_pNetDeviceTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_pNetDeviceTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pNetDeviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pNetDeviceTableView->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(m_pNetDeviceTableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableNetDeviceClicked(const QModelIndex &)));


	m_pCapModel = new TableCapModel();
	m_pCapTableView->setModel(m_pCapModel);

	m_pCapTableView->setColumnWidth(0, 40);
	m_pCapTableView->setColumnWidth(5, 40);
	//m_pCapModel->setColumnCount(2);


	//m_pCapTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//m_pCapTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_pCapTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_pCapTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_pCapTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pCapTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pCapTableView->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(m_pCapTableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableNetCapClicked(const QModelIndex &)));

	m_pTreeModel = new QStandardItemModel();
	m_plistView->setModel(m_pTreeModel);
	m_pTreeModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("������"));

}

void NetWorkConfigWidget::onTableNetDeviceClicked(const QModelIndex &index)
{
	int row = index.row();
	int colou = index.column();
	QModelIndex rowIndex = m_pNetDeviceModel->index(row, 0);
	QModelIndex colouIndex = m_pNetDeviceModel->index(row, 1);

	QString cellText = m_pNetDeviceModel->data(rowIndex).toString();// +m_pModel->data(colouIndex).toString();
	if (!m_bIsBindStatus)
	{
		m_pLineEdit->setText(cellText);
	}
	
}

void NetWorkConfigWidget::slotBindNetWork()
{
	QString strNetName = m_pLineEdit->text();
	if (strNetName.isEmpty()) return;
	

	if (m_bIsBindStatus)
	{
		m_pBindButton->setText("��");
	}
	else {
		m_pBindButton->setText("���");
	}
	bool editEnable = m_pLineEdit->isEnabled();
	m_pLineEdit->setEnabled(!m_pLineEdit->isEnabled());
	m_bIsBindStatus = !m_bIsBindStatus;
}

pcap_if_t* NetWorkConfigWidget::getSelectDevice()
{
	QString strNetName = m_pLineEdit->text();
	if (!strNetName.isEmpty())
	{
		for (d = alldevs; d; d = d->next)
			if (d->name == strNetName)
				return d;
	}
	return NULL;
}


void NetWorkConfigWidget::slotStartCap()
{
	pcap_if_t * currentDevice = getSelectDevice();

	if (currentDevice == NULL || !m_bIsBindStatus)
	{
		QMessageBox::information(this, "��ѡ�������", "��ѡ�������", QMessageBox::Ok);
		return;
	}

	if (!m_bIsStartCap)
	{
		
		m_pStartCapButton->setText("ֹͣ����");
		//startCap(currentDevice);
		TotalIndex = 0;
		TcpIndex = 0;
		UdpIndex = 0;
		SinNetWorkCapSingle::getInstance().startThread(currentDevice);

	}
	else {
		
		m_pStartCapButton->setText("��ʼ����");
		SinNetWorkCapSingle::getInstance().stopThread();
	}

	m_bIsStartCap = !m_bIsStartCap;
}



void NetWorkConfigWidget::slotUpDataCapTable()
{
	CapData data = sinTaskQueueSingle::getInstance().popBackCapData();
	//if (data == NULL) return;
	QString qstrTotalText = "������:" + QString::number(TotalIndex) + "              TCP:" + QString::number(TcpIndex) + "              UDP:" + QString::number(UdpIndex);
	m_pTotalByte->setText(qstrTotalText);

	if (data.strProtocal == "TCP")
	{
		TcpIndex++;
	}
	else if (data.strProtocal == "UDP")
	{
		UdpIndex++;
	}
	
	TotalIndex++;
	
	m_pCapModel->addItem(data);
}

void NetWorkConfigWidget::onTableNetCapClicked(const QModelIndex &index){

	int row = index.row();
	pcap_pkthdr *header = sinTaskQueueSingle::getInstance().popIndexPktHeaderData(row);
	u_char *pkt_data = sinTaskQueueSingle::getInstance().popIndexPktDataData(row);

	int a = 1;

	QString qstrDataIndex = "���ݰ�" + QString::number(row);
	QString qstrEth = "������·��";

	

	ethernet_header *eh;
	eh = (ethernet_header *)pkt_data;
	//ԴMac
	QString qstrSrcMac = "ԴMac:" + QString::number(eh->saddr.byte1) + ":" + QString::number(eh->saddr.byte2) + ":" + QString::number(eh->saddr.byte3) + ":" + QString::number(eh->saddr.byte4) + ":" + QString::number(eh->saddr.byte5) + ":" + QString::number(eh->saddr.byte6);
	//Ŀ��Mac
	QString qstrDestMac = "Ŀ��Mac:" + QString::number(eh->daddr.byte1) + ":" + QString::number(eh->daddr.byte2) + ":" + QString::number(eh->daddr.byte3) + ":" + QString::number(eh->daddr.byte4) + ":" + QString::number(eh->daddr.byte5) + ":" + QString::number(eh->daddr.byte6);
	
	Header Ethheader;
	Ethheader.qstrSrcMac = qstrSrcMac;
	Ethheader.qstrDestMac = qstrDestMac;

	switch (ntohs(eh->type))
	{
	case IP:
	{
		QString qstrUpProtocal = "�ϲ�Э��:IP";
		QString qstrNetLayer = "�����";

		ip_header *ih;
		const u_char *ip_data;
		ip_data = pkt_data + 14;
		ih = (ip_header *)ip_data;
		QString qstrVersion = "�汾��:" + QString::number((ih->ver_ihl & 0xf0) / 0x10);

		u_int ip_len;//IP�ײ�����
		ip_len = (ih->ver_ihl & 0xf) * 4;
		QString qstrHeaderLength = "�ײ�����:" + QString::number(ip_len);
		QString qstrServiceType = "��������:" + QString::number(ih->tos);
		QString qstrTotalLength = "�ܳ���:" + QString::number(ih->tlen);
		QString qstrNetDef = "��ʶ:" + QString::number(ntohs(ih->identification));
		QString qstrMetFlag = "���:" + QString::number(ntohs(ih->flags_fo) & 0xe000 / 0x2000);
		QString qstrPieceOffset = "Ƭƫ��:" + QString::number(ntohs(ih->flags_fo) & 0x1fff);
		QString qstrAliveTime = "����ʱ��:" + QString::number(ih->ttl);
		QString qstrHeaderCheckSum = "�ײ�У��ͣ�0x" + QString::number(ntohs(ih->crc));

		QString qstrSrcIp = "ԴIP��" + QString::number(ih->saddr.byte1) + "." + QString::number(ih->saddr.byte2) + "." + QString::number(ih->saddr.byte3) + "." + QString::number(ih->saddr.byte4);
		QString qstrDestIp = "Ŀ��IP��" + QString::number(ih->daddr.byte1) + "." + QString::number(ih->daddr.byte2) + "." + QString::number(ih->daddr.byte3) + "." + QString::number(ih->daddr.byte4);

		IPHeader ipherader;
		ipherader.qstrUpProtocal = qstrUpProtocal;
		ipherader.qstrNetLayer = qstrNetLayer;
		ipherader.qstrVersion = qstrVersion;
		ipherader.qstrHeaderLength = qstrHeaderLength;
		ipherader.qstrTotalLength = qstrTotalLength;
		ipherader.qstrNetDef = qstrNetDef;
		ipherader.qstrMetFlag = qstrMetFlag;
		ipherader.qstrPieceOffset = qstrPieceOffset;
		ipherader.qstrAliveTime = qstrAliveTime;
		ipherader.qstrHeaderCheckSum = qstrHeaderCheckSum;
		ipherader.qstrSrcIp = qstrSrcIp;
		ipherader.qstrDestIp = qstrDestIp;



		switch (ih->type)
		{
		case TCP:
		{
			QString qstrUpProtocal = "�ϲ�Э��:TCP";
			QString qstrTransLayer = "�����";

			tcp_header *th;
			const u_char *tcp_data;
			tcp_data = ip_data + ip_len;
			th = (tcp_header *)tcp_data;

			QString qstrSrcPort = "Դ�˿ںţ�" + QString::number(ntohs(th->sport));
			QString qstrDestPort = "Ŀ��IP" + QString::number(ntohs(th->dport));
			QString qstrSequNum = "˳���:" + QString::number(ntohs(th->seq));
			QString qstrAckNum = "ȷ�Ϻ�:" + QString::number(ntohs(th->ack));
			QString qstrTcpHeaderLength = "TCPͷ��:" + QString::number((th->len & 0xf0) / 0x10 * 4);


			QString qstrCtrlBit = "����λ";
			QString qstrUrgenURG = "����λ:" + QString::number((th->flags & 0x20) / 0x20);

			QString qstrAckConfirm = "ȷ��ACK��" + QString::number((th->flags & 0x10) / 0x10);
			QString qstrPushPSH = "����PSH:" + QString::number((th->flags & 0x08) / 0x08);
			QString qstrRestRst = "��λRSTG:" + QString::number((th->flags & 0x04) / 0x04);
			QString qstrSyncSyn = "ͬ��:" + QString::number((th->flags & 0x02) / 0x02);
			QString qstrOverFin = "����FIN��" + QString::number((th->flags & 0x01) / 0x01);
			QString qstrWindows = "����:" + QString::number(ntohs(th->win));
			QString qstrCheckSum = "У���:" + QString::number(ntohs(th->crc));
			QString qstrUrgenPoint = "����ָ��:" + QString::number(ntohs(th->urp));

			TCPHeader tcpHeader;
			tcpHeader.header = Ethheader;
			tcpHeader.ip = ipherader;
			tcpHeader.qstrUpProtocal = qstrUpProtocal;
			tcpHeader.qstrTransLayer = qstrTransLayer;
			tcpHeader.qstrSrcPort = qstrSrcPort;
			tcpHeader.qstrDestPort = qstrDestPort;
			tcpHeader.qstrSequNum = qstrSequNum;
			tcpHeader.qstrAckNum = qstrAckNum;
			tcpHeader.qstrTcpHeaderLength = qstrTcpHeaderLength;
			tcpHeader.qstrCtrlBit = qstrCtrlBit;
			tcpHeader.qstrUrgenURG = qstrUrgenURG;
			tcpHeader.qstrAckConfirm = qstrAckConfirm;
			tcpHeader.qstrPushPSH = qstrPushPSH;
			tcpHeader.qstrRestRst = qstrRestRst;
			tcpHeader.qstrSyncSyn = qstrSyncSyn;
			tcpHeader.qstrOverFin = qstrOverFin;
			tcpHeader.qstrWindows = qstrWindows;
			tcpHeader.qstrCheckSum = qstrCheckSum;
			tcpHeader.qstrUrgenPoint = qstrUrgenPoint;

			break;
		}
		case UDP:
		{
			QString qstrUpProtocal = "�ϲ�Э��:UDP";
			QString qstrTransLayer = "�����";

			udp_header *uh;
			const u_char *udp_data;
			udp_data = ip_data + ip_len;
			uh = (udp_header *)udp_data;

			QString qstrSrcPort = "Դ�˿�:" + QString::number(ntohs(uh->sport));
			QString qstrDestPort = "Ŀ�Ķ˿�" + QString::number(ntohs(uh->dport));
			QString qstrUdpHeaderLength = "���ȣ�" + QString::number(ntohs(uh->len));
			QString qstrCheckSum = "У���:" + QString::number(ntohs(uh->crc));

			UDPHeader udpHeader;
			udpHeader.header = Ethheader;
			udpHeader.ip = ipherader;
			udpHeader.qstrUpProtocal = qstrUpProtocal;
			udpHeader.qstrTransLayer = qstrTransLayer;
			udpHeader.qstrSrcPort = qstrSrcPort;
			udpHeader.qstrDestPort = qstrDestPort;
			udpHeader.qstrUdpHeaderLength = qstrUdpHeaderLength;
			udpHeader.qstrCheckSum = qstrCheckSum;

			if (ntohs(uh->dport) == DNS || ntohs(uh->sport) == DNS)
			{
				QString qstrAppWork = "Ӧ�ò�";

				dns_header *dh;
				const u_char *dns_data;
				dns_data = udp_data + 8;
				dh = (dns_header *)dns_data;
				QString qstrNetDef = "��ʶ:" + QString::number(ntohs(dh->identification));
				QString qstrNetFlag = "��־:" + QString::number(ntohs(dh->flags));
				QString qstrQuestionNum = "������:" + QString::number(ntohs(dh->questions_num));
				QString qstrResourceNoteNum = "��Դ��¼��:" + QString::number(ntohs(dh->answers_num));
				QString qstrAuthResourceNoteNum = "��Ȩ��Դ��¼����" + QString::number(ntohs(dh->authority_num));
				QString qstrExtraResourceNoteNum = "������Դ��¼��" + QString::number(ntohs(dh->addition_num));

				DNSHeader dnsHeader;
				dnsHeader.udp = udpHeader;
				dnsHeader.qstrNetDef = qstrNetDef;
				dnsHeader.qstrNetFlag = qstrNetFlag;
				dnsHeader.qstrQuestionNum = qstrQuestionNum;
				dnsHeader.qstrResourceNoteNum = qstrResourceNoteNum;
				dnsHeader.qstrAuthResourceNoteNum = qstrAuthResourceNoteNum;
				dnsHeader.qstrExtraResourceNoteNum = qstrExtraResourceNoteNum;
			}
			break;
		}
		case ICMP:
		{
			QString qstrUpProtocal = "�ϲ�Э��:ICMP";
			QString qstrTransLayer = "�����";

			icmp_header *icmph;
			const u_char *icmp_data;
			icmp_data = ip_data + ip_len;
			icmph = (icmp_header *)icmp_data;

			QString qstrType = "����:" + QString::number(icmph->type);
			QString qstrCode = "����:" + QString::number(icmph->code);
			QString qstrCheckSum = "У���:" + QString::number(ntohs(icmph->checksum));

			ICMPHeader icmpHeader;
			icmpHeader.header = Ethheader;
			icmpHeader.ip = ipherader;
			icmpHeader.qstrUpProtocal = qstrUpProtocal;
			icmpHeader.qstrTransLayer = qstrTransLayer;
			icmpHeader.qstrType = qstrType;
			icmpHeader.qstrCode = qstrCode;
			icmpHeader.qstrCheckSum = qstrCheckSum;
			break;
		}
		case IGMP:
		{
			QString qstrUpProtocal = "�ϲ�Э��:IGMP";
			IGMPHeader igmpHeader;
			igmpHeader.header = Ethheader;
			igmpHeader.ip = ipherader;
			igmpHeader.qstrUpProtocal = qstrUpProtocal;

			break;
		}

		case EGP:
		{
			QString qstrUpProtocal = "�ϲ�Э��:EGP";
			EGPHeader egpHeader;
			egpHeader.header = Ethheader;
			egpHeader.ip = ipherader;
			egpHeader.qstrUpProtocal = qstrUpProtocal;
			break;
		}
		case IPv6:
		{
			QString qstrUpProtocal = "�ϲ�Э��:IPv6";
			IPv6Header ipv6Header;
			ipv6Header.header = Ethheader;
			ipv6Header.ip = ipherader;
			ipv6Header.qstrUpProtocal = qstrUpProtocal;
			break;
		}
		case OSPF:
		{
			QString qstrUpProtocal = "�ϲ�Э�� ";
			OSPFHeader ospfHeader;
			ospfHeader.header = Ethheader;
			ospfHeader.ip = ipherader;
			ospfHeader.qstrUpProtocal = qstrUpProtocal;
			break;
		}
		default:
		{
			QString qstrUpProtocal = "�ϲ�Э��:δ֪";
			UnKnowIPHeader unknowIPHeader;
			unknowIPHeader.header = Ethheader;
			unknowIPHeader.ip = ipherader;
			unknowIPHeader.qstrUpProtocal = qstrUpProtocal;
		}
		}
	}
	case ARP:
	{
		QString qstrUpProtocal = "�ϲ�Э��:ARP";
		QString qstrNetLayer = "�����";


		arp_header *ah;
		const u_char *arp_data;
		arp_data = pkt_data + 14;
		ah = (arp_header *)arp_data;

		QString qstrHardType = "Ӳ������:" + QString::number(ntohs(ah->arp_hdr));
		QString qstrProtocalType = "Э������:" + QString::number(ntohs(ah->arp_pro));
		QString qstrHardLength = "Ӳ������:" + QString::number(ah->arp_hln);
		QString qstrProtocalLength = "Э�鳤��:" + QString::number(ah->apr_pln);
		QString qstrOpType = "��������" + QString::number(ntohs(ah->arp_opt));

		QString qstrSendMacAddr = "���Ͷ�MAC��ַ:" + QString::number(ah->arp_smac.byte1) + ":" + QString::number(ah->arp_smac.byte2) + ":" + QString::number(ah->arp_smac.byte3) + ":" + QString::number(ah->arp_smac.byte4) + ":" + QString::number(ah->arp_smac.byte5) + ":" + QString::number(ah->arp_smac.byte6);
		QString qstrSendProtocalAddr = "���Ͷ�Э���ַ:" + QString::number(ah->arp_sip.byte1) + ":" + QString::number(ah->arp_sip.byte2) + ":" + QString::number(ah->arp_sip.byte3) + ":" + QString::number(ah->arp_sip.byte4);

		QString qstrReciveMacAddr = "���ն�MAC��ַ:" + QString::number(ah->arp_dmac.byte1) + ":" + QString::number(ah->arp_dmac.byte2) + ":" + QString::number(ah->arp_dmac.byte3) + ":" + QString::number(ah->arp_dmac.byte4) + ":" + QString::number(ah->arp_dmac.byte5) + ":" + QString::number(ah->arp_dmac.byte6);
		QString qstrReciveProtocalAddr = "���ն�Э���ַ:" + QString::number(ah->arp_dip.byte1) + ":" + QString::number(ah->arp_dip.byte2) + ":" + QString::number(ah->arp_dip.byte3) + ":" + QString::number(ah->arp_dip.byte4);

		ARPHeader arpHeader;
		arpHeader.header = Ethheader;
		arpHeader.qstrUpProtocal = qstrUpProtocal;
		arpHeader.qstrNetLayer = qstrNetLayer;
		arpHeader.qstrHardType = qstrHardType;
		arpHeader.qstrHardLength = qstrHardLength;
		arpHeader.qstrProtocalLength = qstrProtocalLength;
		arpHeader.qstrOpType = qstrOpType;
		arpHeader.qstrSendMacAddr = qstrSendMacAddr;
		arpHeader.qstrSendProtocalAddr = qstrSendProtocalAddr;
		arpHeader.qstrReciveMacAddr = qstrReciveMacAddr;
		arpHeader.qstrReciveProtocalAddr;

		break;
	}
	case RARP:
	{
		QString qstrUpProtocal = "�ϲ�Э��:RARP";
		RARPHeader rarpHeader;
		rarpHeader.header = Ethheader;
		rarpHeader.qstrUpProtocal = qstrUpProtocal;
		break;
	}
	default:

		QString qstrUpProtocal = "�ϲ�Э��:δ֪";
		UnKnowHeader unknowHeader;
		unknowHeader.header = Ethheader;
		unknowHeader.qstrUpProtocal = qstrUpProtocal;

	}
}