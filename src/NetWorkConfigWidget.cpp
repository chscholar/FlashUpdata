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

NetWorkConfigWidget::NetWorkConfigWidget(QWidget *parent)
	:QWidget(parent)
	, m_bIsBindStatus(false)
	, m_bIsStartCap(false)
	, m_totalCount(0)
{
	initUi();
	getNetInfo();
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

	connect(m_pNetDeviceTableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));


	m_pCapModel = new TableCapModel();
	m_pCapTableView->setModel(m_pCapModel);

	//m_pCapModel->setColumnCount(2);


	m_pCapTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	//m_pCapTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_pCapTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_pCapTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_pCapTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pCapTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_pCapTableView->setSelectionMode(QAbstractItemView::SingleSelection);

	CapData itemData;
	itemData.strIndex = "1";
	itemData.strSrcIP = "2";
	itemData.strDestIp = "3";
	itemData.strProtocal = "4";
	itemData.strDate = "5";
	itemData.strLength = "6";

	m_pCapModel->addItem(itemData);
	

	//connect(m_pNetDeviceTableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));
}

void NetWorkConfigWidget::onTableClicked(const QModelIndex &index)
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
		SinNetWorkCapSingle::getInstance().startThread();

	}
	else {
		
		m_pStartCapButton->setText("开始捕获");
		stopCap();
	}

	m_bIsStartCap = !m_bIsStartCap;
}

int NetWorkConfigWidget::startCap(pcap_if_t *device)
{
	//CSnifferDlg *pDlg = (CSnifferDlg *)lpParam;
	pcap_t *pCap;
	char	strErrorBuf[PCAP_ERRBUF_SIZE];
	int res;
	struct pcap_pkthdr *pkt_header;
	const u_char *pkt_data;
	u_int netmask;
	struct bpf_program fcode;
	char* filterName = "(tcp and ip) or(udp and ip) or arp or rarp or(icmp and ip) or(igmp and ip)";

	if ((pCap = pcap_open_live(device->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, strErrorBuf)) == NULL)
	{
		return -1;
	}

	if (device->addresses != NULL)
		/* 获得接口第一个地址的掩码 */
		netmask = ((struct sockaddr_in *)(device->addresses->netmask))->sin_addr.S_un.S_addr;
	else
		/* 如果接口没有地址，那么我们假设一个C类的掩码 */
		netmask = 0xffffff;
	//编译过滤器
	if (pcap_compile(pCap, &fcode, filterName, 1, netmask) < 0)
	{
		//AfxMessageBox(_T("请设置过滤规则"));
		return -1;
	}
	//设置过滤器
	if (pcap_setfilter(pCap, &fcode) < 0)
		return -1;

	while ((res = pcap_next_ex(pCap, &pkt_header, &pkt_data)) >= 0)
	{

		if (res == 0)
			continue;
		if (m_bIsStartCap)
			break;

		addDataToTableCap(pkt_header, pkt_data);
	}

	pcap_close(pCap);
}

void NetWorkConfigWidget::stopCap()
{

}

void NetWorkConfigWidget::addDataToTableCap(const pcap_pkthdr *pkt_header, const u_char *pkt_data)
{
	CapData itemData;

	//用于存诸网络中的数据，并保存到CArray中,以备将来使用
	struct pcap_pkthdr *pHeader = new pcap_pkthdr;
	u_char *pData;
	unsigned char *pPosition = (unsigned char *)pkt_data;

	long nIndex = 0;//标识当前的数据包位置
	long nCount = 0;//标识后来

	pHeader->caplen = pkt_header->caplen;
	pHeader->len = pkt_header->len;

	pData = new unsigned char[pHeader->len];
	memcpy((void *)pData, pkt_data, pHeader->len);

	m_pktHeaders.push_back(pHeader);
	m_pktDatas.push_back(pData);

	//显示序列号
	nIndex = m_pktHeaders.size() - 1;
	itemData.strIndex = QString::number(nIndex);
	m_totalCount++;//总数据包加1

	/*显示时间*/
	struct tm *ltime;
	time_t local_tv_sec;
	local_tv_sec = pkt_header->ts.tv_sec;
	ltime = localtime(&local_tv_sec);
	QString strHour = QString::number(ltime->tm_hour);
	QString strMin = QString::number(ltime->tm_min);
	QString strSec = QString::number(ltime->tm_sec);
	QString strUsec = QString::number(pkt_header->ts.tv_usec);
	QString strTime = strHour + strMin + strSec + strUsec;
	itemData.strDate = strTime;


	/*处理链路层*/
	ethernet_header *eh;
	eh = (ethernet_header *)pkt_data;

	QString strSrc1 = QString::number(eh->saddr.byte1);
	QString strSrc2 = QString::number(eh->saddr.byte2);
	QString strSrc3 = QString::number(eh->saddr.byte3);
	QString strSrc4 = QString::number(eh->saddr.byte4);
	QString strSrc5 = QString::number(eh->saddr.byte5);
	QString strSrc6 = QString::number(eh->saddr.byte6);
	QString strSrcIp = strSrc1 + ":" + strSrc2 + ":" + strSrc3 + ":" + strSrc4 + ":" + strSrc5 + ":" + strSrc6;
	itemData.strSrcIP = strSrcIp;

	QString strDest1 = QString::number(eh->daddr.byte1);
	QString strDest2 = QString::number(eh->daddr.byte2);
	QString strDest3 = QString::number(eh->daddr.byte3);
	QString strDest4 = QString::number(eh->daddr.byte4);
	QString strDest5 = QString::number(eh->daddr.byte5);
	QString strDest6 = QString::number(eh->daddr.byte6);
	QString strDestIp = strDest1 + ":" + strDest2 + ":" + strDest3 + ":" + strDest4 + ":" + strDest5 + ":" + strDest6;
	itemData.strDestIp = strDestIp;

	QString strLength = QString::number(pHeader->len);
	itemData.strLength = strLength;

	QString qstrProtocal;
	switch (ntohs(eh->type))
	{
	case IP:
	{
		ip_header *ih;
		const u_char *ip_data;
		ip_data = pkt_data + 14;
		ih = (ip_header *)ip_data;
		u_int ip_len;//IP首部长度
		ip_len = (ih->ver_ihl & 0xf) * 4;
		/*处理传输层*/
		switch (ih->type)
		{
		case TCP:
		{
			tcp_header *th;
			const u_char *tcp_data;
			tcp_data = ip_data + ip_len;
			th = (tcp_header *)tcp_data;
			if (ntohs(th->sport) == HTTP)
			{
				qstrProtocal = "源HTTP";
			}
			else if (ntohs(th->dport) == HTTP)
			{
				qstrProtocal = "目的HTTP";
			}
			else if (ntohs(th->dport) == DNS || ntohs(th->sport) == DNS)
			{
				qstrProtocal = "DNS";
			}
			else
				qstrProtocal = "TCP";
			break;
		}
		case UDP:
		{
			udp_header *uh;
			const u_char *udp_data;
			udp_data = ip_data + ip_len;
			uh = (udp_header *)udp_data;
			if (ntohs(uh->dport) == DNS || ntohs(uh->sport) == DNS)
			{
				qstrProtocal = "DNS";
			}
			else
				qstrProtocal = "UDP";
			break;
		}
		case ICMP:qstrProtocal = "ICMP"; break;
		case IGMP:qstrProtocal = "IGMP"; break;
		case EGP:qstrProtocal = "EGP"; break;
		case IPv6:qstrProtocal = "IPV6"; break;
		case OSPF:qstrProtocal = "OSPF"; break;
		default:qstrProtocal = QString::number(ih->type);
		}
		break;
	}
	case ARP:
	{
		qstrProtocal = "ARP";
		break;
	}
	case RARP:
		qstrProtocal = "RARP";
		break;
	default:
		qstrProtocal = QString::number(eh->type);
	}
	itemData.strProtocal = qstrProtocal;
	m_pCapModel->addItem(itemData);
	m_pCapTableView->update();
}

