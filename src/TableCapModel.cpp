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
		return 6;
}

QVariant TableCapModel::data(const QModelIndex &index, int role) const
{
	int row = index.row();
	int col = index.column();
	if (role == 0)
	{
		switch (col)
		{
		case 0:
			return m_aryData.at(row).strIndex;
			break;
		case 1:
			return m_aryData.at(row).strSrcIP;
			break;
		case 2:
			return m_aryData.at(row).strDestIp;
			break;
		case 3:
			return m_aryData.at(row).strProtocal;
			break;
		case 4:
			return m_aryData.at(row).strDate;
			break;
		case 5:
			return m_aryData.at(row).strLength;
			break;
		default:return QVariant();
		}
	}
	else {
		return QVariant();
	}
	
}

QVariant TableCapModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal){

		if (section == 0)
			return QStringLiteral("序号");
		else if (section == 1)
			return QStringList("源IP");
		else if (section == 2)
			return QStringLiteral("目的IP");
		else if (section == 3)
			return QStringLiteral("协议");
		else if (section == 4)
			return QStringLiteral("时间");
		else if (section == 5)
			return QStringLiteral("长度");
	}

	return QVariant();
}

void TableCapModel::addItem(CapData itemData)
{
	beginInsertRows(QModelIndex(), m_aryData.size(), m_aryData.size());
	m_aryData << itemData;
	endInsertRows();

}