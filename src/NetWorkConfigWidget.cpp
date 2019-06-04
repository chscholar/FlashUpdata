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

NetWorkConfigWidget::NetWorkConfigWidget(QWidget *parent){
	initUi();
	getNetInfo();
}

NetWorkConfigWidget::~NetWorkConfigWidget(){

}

bool NetWorkConfigWidget::getNetInfo()
{
	pcap_if_t *d;
	pcap_if_t *alldevs; //所有设备指针
	char errbuf[PCAP_ERRBUF_SIZE];

	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
		return FALSE;
	int i = 0;
	for (d = alldevs; d; d = d->next)
	{
		//m_list1.InsertItem(0, (CString)d->name);		//d->name的类型是char *,需要强制转换为CString才能在InsertItem中显示
		//m_list1.SetItemText(0, 1, (CString)d->description);

		m_pModel->setItem(i, 0, new QStandardItem((QString)d->name));
		m_pModel->setItem(i, 1, new QStandardItem((QString)d->description));
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
	m_pTableView = new QTableView();
	mainLayout->addWidget(m_pTableView);

	mainLayout->addSpacing(10);
	m_pLineEdit = new QLineEdit();
	mainLayout->addWidget(m_pLineEdit);
	
	mainLayout->addStretch(3);
	setLayout(mainLayout);

	m_pModel = new QStandardItemModel();
	m_pTableView->setModel(m_pModel);

	m_pModel->setColumnCount(2);
	m_pModel->setHeaderData(0, Qt::Horizontal, "设备名");
	m_pModel->setHeaderData(1, Qt::Horizontal,"设备描述");
}