#ifndef SINXML_H
#define SINXML_H
#include <QObject>

class SinXml : public QObject
{
public:
	SinXml(QObject *parent = 0);
	~SinXml();
	void initXml();
	void addUpLoadFile(bool isUpLoad, QString fileId, QString fileCheck, QString filePath);
	void updateUpLoadFile(bool isUpLoad, QString fileId, QString fileCheck, QString filePath);
	void deleUpLoadFile(bool isUpLoad, int fileId);
protected:
private:
};

#endif
