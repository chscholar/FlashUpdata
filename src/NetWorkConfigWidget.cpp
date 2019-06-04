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

NetWorkConfigWidget::NetWorkConfigWidget(QWidget *parent)
	:QWidget(parent)
	, m_bIsBindStatus(false)
	, m_bIsStartCap(false)
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

	m_pStartCap = new QPushButton();
	m_pStartCap->setText("开始捕获");
	buttonLayout->addWidget(m_pStartCap);
	buttonLayout->addStretch();
	mainLayout->addLayout(buttonLayout);

	m_pCapTableView = new QTableView();
	mainLayout->addWidget(m_pCapTableView);

	mainLayout->addStretch(3);
	setLayout(mainLayout);

	connect(m_pBindButton, SIGNAL(clicked()), this, SLOT(slotBindNetWork()));
	connect(m_pStartCap, SIGNAL(clicked()), this, SLOT(slotStartCap()));

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
		m_pStartCap->setText("停止捕获");
	}
	else {
		m_pStartCap->setText("开始捕获");
	}

	m_bIsStartCap = !m_bIsStartCap;
		
	
}