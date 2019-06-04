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

#pragma execution_character_set("utf-8")
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

private:
	public slots :
		void onTableNetDeviceClicked(const QModelIndex &);
	void onTableNetCapClicked(const QModelIndex &);
	void slotBindNetWork();
	void slotStartCap();
	void slotUpDataCapTable();
};

#endif
