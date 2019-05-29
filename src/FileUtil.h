#ifndef FILEUTIL_H
#define  FILEUTIL_H
#include <QObject>
#include <QList>
#include "SinByte.h"

class FileUtil : public QObject
{
	Q_OBJECT
public:
	FileUtil(QObject *parent = 0);
	~FileUtil();
	QList<QList<QByteArray>> getDataFramFromFilePath(QStringList pathList);
private:
	QList<QByteArray> toFileDataDecode(QByteArray byteData);
	ReqInterrFace toTransFace(QByteArray byteData);
	void fillStructItem();
};

#endif
