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
	QTableView *m_pTableView;
	QLineEdit *m_pLineEdit;
	QStandardItemModel *m_pModel;
	bool m_bIsBindStatus;
	bool m_bIsStartCap;
	QPushButton *m_pBindButton;
	QPushButton *m_pStartCap;
	pcap_if_t *d;
	pcap_if_t *alldevs; //所有设备指针

private:
	public slots :
		void onTableClicked(const QModelIndex &);
	void slotBindNetWork();
	void slotStartCap();
};

#endif
