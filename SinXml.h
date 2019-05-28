#ifndef SINXML_H
#define SINXML_H
#include <QObject>
#include <QDomNode>

class SinXml : public QObject
{
public:
	SinXml(QObject *parent = 0);
	~SinXml();
	void initXml();
	void addUpLoadFile(bool isUpLoad, QString fileId, QString fileCheck, QString filePath);
	void updateUpLoadFile(bool isUpLoad, QString fileId, QString fileCheck, QString filePath);
	void deleUpLoadFile(bool isUpLoad, QString fileId);
protected:
	QDomNode findNodByName(QDomElement elemtnt, QString nodeName);
	void appendChilds(QDomDocument doc,QDomElement element, QString fileId, QString fileCheck, QString filePath);
	void deleChild(QDomElement element, QString fileId);
private:
};

#endif
