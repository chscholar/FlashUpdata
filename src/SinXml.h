#ifndef SINXML_H
#define SINXML_H
#include <QObject>
#include <QDomNode>
#include <QDomDocument>
#include <QList>
#include <QFileSystemWatcher>
#include <QByteArray>
#include "CSingleTon.h"


struct FileConfigItem
{
	QString fileConfigId; 
	QString fileConfigChecked;
	QString fileConfigPath;
};

class SinXml : public QObject
{
	Q_OBJECT
public:
	SinXml(QObject *parent = 0);
	~SinXml();
	void initXml();
	void addUpLoadFile(bool isUpLoad, QString fileId, QString fileCheck, QString filePath);
	void updateUpLoadFile(bool isUpLoad, QString fileId, QString fileCheck, QString filePath);
	void deleUpLoadFile(bool isUpLoad, QString fileId);
	void reload(QString qstrContent);
	QList<FileConfigItem> getFileConfigItemFromXmlConfig(bool isUpLoad);
	QByteArray getConfigFileContent();
protected:
	QDomNode findNodByName(QDomElement elemtnt, QString nodeName);
	void appendChilds(QDomDocument doc,QDomElement element, QString fileId, QString fileCheck, QString filePath);
	void deleChild(QDomElement element, QString fileId);
	void updateChild(QDomElement element, QString fileId, QString fileCheck, QString filePath);
	QDomDocument readConfigFile();
	void writeConfigFile(QDomDocument doc);
	QString getElementName(bool isUpLoad);
	QFileSystemWatcher *m_pFileWatcher;
public slots:
	void configFileChange(QString strPath);
signals:
	void signalsConfigChange();
private:
};

typedef CSingleton<SinXml> sinXmlSingle;
#endif
