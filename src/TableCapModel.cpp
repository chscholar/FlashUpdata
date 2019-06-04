#include "TableCapModel.h"
#include <QDebug>

TableCapModel::TableCapModel() : QAbstractTableModel(NULL)
{

}

TableCapModel::~TableCapModel()
{

}

int TableCapModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
		return m_aryData.size();
}

int TableCapModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
		return 3;
}

QVariant TableCapModel::data(const QModelIndex &index, int role) const
{
	if (role == 0)
		return m_aryData[index.row()][index.column()];
	else
		return QVariant();
}

QVariant TableCapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal){

		if (section == 0)
			return QStringLiteral("���");
		else if (section == 1)
			return QStringList("ԴIP");
		else if (section == 2)
			return QStringLiteral("Ŀ��IP");
		else if (section == 3)
			return QStringLiteral("Э��");
		else if (section == 4)
			return QStringLiteral("ʱ��");
		else if (section == 5)
			return QStringLiteral("����");
	}

	return QVariant();
}

void TableCapModel::addItem(QString one, QString two, QString three)
{
	beginInsertRows(QModelIndex(), m_aryData.size(), m_aryData.size());
	QVector<QString> list;
	list << one << two << three;
	m_aryData << list;
	endInsertRows();

}