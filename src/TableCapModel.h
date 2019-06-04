#ifndef TABLECAPMODEL_H
#define TABLECAPMODEL_H
#include <QAbstractTableModel>
#include <QList>

struct CapData 
{
	QString strIndex;
	QString strSrcIP;
	QString strDestIp;
	QString strProtocal;
	QString strDate;
	QString strLength;
};

#pragma execution_character_set("utf-8")
class TableCapModel : public QAbstractTableModel
{
public:
	TableCapModel();
	~TableCapModel();
	//����ʵ����3��
	int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
	void addItem(CapData itemData);
protected:
	QList<CapData> m_aryData;
private:
};

#endif