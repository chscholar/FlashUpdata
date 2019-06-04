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
	d = NULL; //清空以便其他函数使用

	return true;

}

void NetWorkConfigWidget::initUi()
{

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setMargin(10);

	mainLayout->addSpacing(10);
	m_pNetDeviceTableView = new QTableView();
	mainLayout->addWidget(m_pNetDeviceTableView);

	mainLayout->addSpacing(10);
	m_pLineEdit = new QLineEdit();
	mainLayout->addWidget(m_pLineEdit);
	
	mainLayout->addSpacing(10);
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch();

	m_pBindButton = new QPushButton();
	m_pBindButton->setText("绑定");
	buttonLayout->addWidget(m_pBindButton);
	buttonLayout->addStretch();

	m_pStartCapButton = new QPushButton();
	m_pStartCapButton->setText("开始捕获");
	buttonLayout->addWidget(m_pStartCapButton);
	buttonLayout->addStretch();
	mainLayout->addLayout(buttonLayout);

	m_pCapTableView = new QTableView();
	mainLayout->addWidget(m_pCapTableView);

	m_plistView = new QTreeView();
	mainLayout->addWidget(m_plistView);

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
	m_pNetDeviceModel->setHeaderData(0, Qt::Horizontal, "设备名");
	m_pNetDeviceModel->setHeaderData(1, Qt::Horizontal, "设备描述");

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
		m_pBindButton->setText("绑定");
	}
	else {
		m_pBindButton->setText("解绑");
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
		QMessageBox::information(this, "请选择绑定网卡", "请选择绑定网卡", QMessageBox::Ok);
		return;
	}

	if (!m_bIsStartCap)
	{
		
		m_pStartCapButton->setText("停止捕获");
		//startCap(currentDevice);
		SinNetWorkCapSingle::getInstance().startThread(currentDevice);

	}
	else {
		
		m_pStartCapButton->setText("开始捕获");
		SinNetWorkCapSingle::getInstance().stopThread();
	}

	m_bIsStartCap = !m_bIsStartCap;
}


void NetWorkConfigWidget::slotUpDataCapTable()
{
	CapData data = sinTaskQueueSingle::getInstance().popBackCapData();
	//if (data == NULL) return;
	
	m_pCapModel->addItem(data);
}

void NetWorkConfigWidget::onTableNetCapClicked(const QModelIndex &index){
	
	int row = index.row();
	pcap_pkthdr *header = sinTaskQueueSingle::getInstance().popIndexPktHeaderData(row);
	u_char *data = sinTaskQueueSingle::getInstance().popIndexPktDataData(row);

	int a = 1;
	/*
	m_tree1.DeleteAllItems();
	CString str;
	str.Format(_T("数据包:%ld"),index);
	HTREEITEM hRoot;
	HTREEITEM hSubItem;
	HTREEITEM hItem;
	HTREEITEM hItem2;

	hRoot = m_tree1.InsertItem(str);
	hSubItem = m_tree1.InsertItem(_T("数据链路层"),hRoot);
	ethernet_header *eh;
	eh = (ethernet_header *)pkt_data;
	str.Format(_T("源MAC:%x:%x:%x:%x:%x:%x"),eh->saddr.byte1,eh->saddr.byte2,eh->saddr.byte3,eh->saddr.byte4,eh->saddr.byte5,eh->saddr.byte6);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("目的MAC:%x:%x:%x:%x:%x:%x"),eh->daddr.byte1,eh->daddr.byte2,eh->daddr.byte3,eh->daddr.byte4,eh->daddr.byte5,eh->daddr.byte6);
	hItem = m_tree1.InsertItem(str,hSubItem);
	switch(ntohs(eh->type))
	{
	case IP:
	{
	hItem = m_tree1.InsertItem(_T("上层协议:IP"),hSubItem);
	hSubItem = m_tree1.InsertItem(_T("网络层"),hRoot);
	ip_header *ih;
	const u_char *ip_data;
	ip_data=pkt_data+14;
	ih = (ip_header *)ip_data;
	str.Format(_T("版本：%d"),(ih->ver_ihl & 0xf0) / 0x10);
	hItem = m_tree1.InsertItem(str,hSubItem);
	u_int ip_len;//IP首部长度
	ip_len = (ih->ver_ihl & 0xf) * 4;
	str.Format(_T("首部长度：%d"),ip_len);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("服务类型：0x%x"),ih->tos);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("总长度：%d"),ntohs( ih->tlen ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("标识：0x%x"),ntohs( ih->identification ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("标志：0x%x"),ntohs( ih->flags_fo ) & 0xe000 / 0x2000);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("片偏移：%d"),ntohs( ih->flags_fo ) & 0x1fff);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("生存时间：%d"),ih->ttl);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("首部校验和：0x%x"),ntohs( ih->crc ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("源IP地址：%d.%d.%d.%d"),ih->saddr.byte1,ih->saddr.byte2,ih->saddr.byte3,ih->saddr.byte4);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("目的IP地址：%d.%d.%d.%d"),ih->saddr.byte1,ih->saddr.byte2,ih->saddr.byte3,ih->saddr.byte4);
	hItem = m_tree1.InsertItem(str,hSubItem);
	switch(ih->type)
	{
	case TCP:
	{
	hItem = m_tree1.InsertItem(_T("上层协议:TCP"),hSubItem);
	hSubItem = m_tree1.InsertItem(_T("传输层"),hRoot);
	tcp_header *th;
	const u_char *tcp_data;
	tcp_data = ip_data+ip_len;
	th = (tcp_header *)tcp_data;
	str.Format(_T("源端口号：%d"),ntohs( th->sport ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("目的口号：%d"),ntohs( th->dport ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("顺序号：%d"),ntohs( th->seq ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("确认号：%d"),ntohs( th->ack ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("TCP头长：%d"),(th->len & 0xf0) / 0x10 * 4);
	hItem = m_tree1.InsertItem(str,hSubItem);
	hItem = m_tree1.InsertItem(_T("控制位"),hSubItem);
	str.Format(_T("紧急URG:%d"),(th->flags & 0x20) / 0x20 );
	hItem2 = m_tree1.InsertItem(str,hItem);
	str.Format(_T("确认ACK:%d"),(th->flags & 0x10) / 0x10);
	hItem2 = m_tree1.InsertItem(str,hItem);
	str.Format(_T("推送PSH:%d"),(th->flags & 0x08) / 0x08);
	hItem2 = m_tree1.InsertItem(str,hItem);
	str.Format(_T("复位RSTG:%d"),(th->flags & 0x04) / 0x04);
	hItem2 = m_tree1.InsertItem(str,hItem);
	str.Format(_T("同步SYN:%d"),(th->flags & 0x02) / 0x02);
	hItem2 = m_tree1.InsertItem(str,hItem);
	str.Format(_T("结束FIN:%d"),(th->flags & 0x01) / 0x01);
	hItem2 = m_tree1.InsertItem(str,hItem);
	str.Format(_T("窗口：%d"),ntohs( th->win ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("校验和：0x%x"),ntohs( th->crc ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("紧急指针：0x%x"),ntohs( th->urp ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	break;
	}
	case UDP:
	{
	hItem = m_tree1.InsertItem(_T("上层协议:UDP"),hSubItem);
	hSubItem = m_tree1.InsertItem(_T("传输层"),hRoot);
	udp_header *uh;
	const u_char *udp_data;
	udp_data = ip_data+ip_len;
	uh = (udp_header *)udp_data;
	str.Format(_T("源端口号：%d"),ntohs( uh->sport ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("目的口号：%d"),ntohs( uh->dport ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("长度：%d"),ntohs( uh->len ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("校验和：0x%x"),ntohs( uh->crc ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	if(ntohs( uh->dport ) == DNS || ntohs( uh->sport ) == DNS)
	{
	hSubItem = m_tree1.InsertItem(_T("应用层"),hRoot);
	dns_header *dh;
	const u_char *dns_data;
	dns_data = udp_data+8;
	dh = (dns_header *)dns_data;
	str.Format(_T("标识：0x%x"),ntohs( dh->identification ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("标志：0x%x"),ntohs( dh->flags ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("问题数：%d"),ntohs( dh->questions_num ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("资源记录数：%d"),ntohs( dh->answers_num ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("授权资源记录数：%d"),ntohs( dh->authority_num ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("额外资源记录数：%d"),ntohs( dh->addition_num ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	}
	break;
	}
	case ICMP:
	{
	hItem = m_tree1.InsertItem(_T("上层协议:ICMP"),hSubItem);
	hSubItem = m_tree1.InsertItem(_T("传输层"),hRoot);
	icmp_header *icmph;
	const u_char *icmp_data;
	icmp_data = ip_data+ip_len;
	icmph = (icmp_header *)icmp_data;
	str.Format(_T("类型：%d"),icmph->type);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("代码：%d"),icmph->code);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("校验和：0x%x"),ntohs( icmph->checksum ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	break;
	}
	case IGMP:hItem = m_tree1.InsertItem(_T("上层协议:IGMP"),hSubItem);break;
	case EGP:hItem = m_tree1.InsertItem(_T("上层协议:EGP"),hSubItem);break;
	case IPv6:hItem = m_tree1.InsertItem(_T("上层协议:IPv6"),hSubItem);break;
	case OSPF:hItem = m_tree1.InsertItem(_T("上层协议:OSPF"),hSubItem);break;
	default:hItem = m_tree1.InsertItem(_T("上层协议:未知"),hSubItem);
	}
	break;
	}
	case ARP:
	{
	hItem = m_tree1.InsertItem(_T("上层协议:ARP"),hSubItem);
	hSubItem = m_tree1.InsertItem(_T("网络层"),hRoot);
	arp_header *ah;
	const u_char *arp_data;
	arp_data=pkt_data+14;
	ah = (arp_header *)arp_data;
	str.Format(_T("硬件类型：%d"),ntohs( ah->arp_hdr ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("协议类型：0x%x"),ntohs( ah->arp_pro ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("硬件长度：%d"),ah->arp_hln);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("协议长度：%d"),ah->apr_pln );
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("操作类型：%d"),ntohs( ah->arp_opt ));
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("发送端MAC地址：%x:%x:%x:%x:%x:%x"),ah->arp_smac.byte1,ah->arp_smac.byte2,ah->arp_smac.byte3,ah->arp_smac.byte4,ah->arp_smac.byte5,ah->arp_smac.byte6);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("发送端协议地址：%d.%d.%d.%d"),ah->arp_sip.byte1,ah->arp_sip.byte2,ah->arp_sip.byte3,ah->arp_sip.byte4);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("接收端MAC地址：%x:%x:%x:%x:%x:%x"),ah->arp_dmac.byte1,ah->arp_dmac.byte2,ah->arp_dmac.byte3,ah->arp_dmac.byte4,ah->arp_dmac.byte5,ah->arp_dmac.byte6);
	hItem = m_tree1.InsertItem(str,hSubItem);
	str.Format(_T("接收端协议地址：%d.%d.%d.%d"),ah->arp_dip.byte1,ah->arp_dip.byte2,ah->arp_dip.byte3,ah->arp_dip.byte4);
	hItem = m_tree1.InsertItem(str,hSubItem);
	break;
	}
	case RARP:
	{
	hItem = m_tree1.InsertItem(_T("上层协议:RARP"),hSubItem);
	break;
	}
	default:
	hItem = m_tree1.InsertItem(_T("上层协议:未知"),hSubItem);
	}

	m_tree1.Expand(hRoot,TVE_EXPAND);		//默认展开目录
	m_tree1.Expand(hSubItem,TVE_EXPAND);

	CString strHex;
	int nCount = 0;
	CString strText;
	for (unsigned short i = 0; i < pkt_header->caplen ; i++)
	{
	CString hex;
	if ( (i % 16) == 0)
	{
	hex.Format(_T("\x0d\x0a 0X%04x   "),nCount);
	nCount++;
	if( i != 0)
	{
	strHex +=_T("  ")+strText ;
	strText = _T("");
	}
	strHex += hex;
	}
	hex.Format(_T("%2.2x "),pkt_data[i-1]);
	strHex += hex;
	if(pkt_data[i-1] <= 127 && pkt_data[i-1] >= 0)
	hex.Format(_T("%c"),pkt_data[i-1]);
	else
	hex = _T(".");
	strText += hex;
	}
	if( strText !=_T(""))
	strHex += strText;
	m_edit1.SetWindowText(strHex);
	*/
}