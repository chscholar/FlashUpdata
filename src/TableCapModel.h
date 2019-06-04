#ifndef TABLECAPMODEL_H
#define TABLECAPMODEL_H
#include <QAbstractTableModel>
#include <QVector>

#pragma execution_character_set("utf-8")
class TableCapModel : public QAbstractTableModel
{
public:
	TableCapModel();
	~TableCapModel();
	//必须实现这3个
	int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
	void addItem(QString one, QString two, QString three);
protected:
	QVector<QVector<QString>> m_aryData;
private:
};

#endif
