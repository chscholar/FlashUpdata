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
	QList<QList<TransInterFace>> getDataFramFromFilePath(QStringList pathList);
	QList<TransInterFace> toDecode(QByteArray byteData);
private:
	TransInterFace toTransFace(QByteArray byteData);
};

#endif
