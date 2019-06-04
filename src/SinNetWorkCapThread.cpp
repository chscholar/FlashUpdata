#include "SinNetWorkCapThread.h"
#include "SinTaskQueue.h"
#include "head.h"

SinNetWorkCapThread:: SinNetWorkCapThread(QObject *parent)
	:QObject(parent)
	, m_pDevice(NULL)
	, m_bIsStartCap(false)
{
	m_pThread = new QThread();
	connect(m_pThread, SIGNAL(started()), this, SLOT(startCap()));
	connect(m_pThread, SIGNAL(finished()), this, SLOT(deleteLater()));
	this->moveToThread(m_pThread);


}

SinNetWorkCapThread::~SinNetWorkCapThread()
{

}


void SinNetWorkCapThread::startThread(pcap_if_t *device)
{
	m_pDevice = device;
	m_bIsStartCap = true;
	m_pThread->start();
}

void SinNetWorkCapThread::stopThread()
{
	m_bIsStartCap = false;
	m_pThread->quit();
}

int SinNetWorkCapThread::startCap()
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

	if ((pCap = pcap_open_live(m_pDevice->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, strErrorBuf)) == NULL)
	{
		return -1;
	}

	if (m_pDevice->addresses != NULL)
		/* 获得接口第一个地址的掩码 */
		netmask = ((struct sockaddr_in *)(m_pDevice->addresses->netmask))->sin_addr.S_un.S_addr;
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
		if (!m_bIsStartCap)
			break;

		addDataToTableCap(pkt_header, pkt_data);
	
	
	}

	pcap_close(pCap);
}


void SinNetWorkCapThread::addDataToTableCap(const pcap_pkthdr *pkt_header, const u_char *pkt_data)
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


	//显示序列号
	nIndex = sinTaskQueueSingle::getInstance().getCapDataSize() - 1;
	itemData.strIndex = QString::number(nIndex);

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

	sinTaskQueueSingle::getInstance().pushBackCapData(itemData);
	//m_pCapModel->addItem(itemData);
	//m_pCapTableView->update();
}
