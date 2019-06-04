#ifndef NETWORKCONFIGWIDGET_H
#define NETWORKCONFIGWIDGET_H
#include <QWidget>
#include <QStringList>
#include <QTableView>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QPushButton>

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
	QTableView *m_pTableView;
	QLineEdit *m_pLineEdit;
	QStandardItemModel *m_pModel;
	bool m_bIsBindStatus;
	QPushButton *m_pBindButton;

private:
	public slots :
		void onTableClicked(const QModelIndex &);
	void slotBindNetWork();
};

#endif
