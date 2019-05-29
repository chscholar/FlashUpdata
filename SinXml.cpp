#include "SinXml.h"
#include <QFile>
#include <QDomDocument>
#include <QTextStream>

const QString configFileName = "config.xml";

SinXml::SinXml(QObject *parent)
	:QObject(parent){

	m_pFileWatcher = new QFileSystemWatcher();
	bool isWather =  m_pFileWatcher->addPath(configFileName);
	connect(m_pFileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChange(QString)));
}

SinXml::~SinXml()
{

}

void SinXml::initXml()
{
	QFile file(configFileName);
	if (!file.open(QFile::WriteOnly | QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
		return;


	QDomDocument doc;
	QDomProcessingInstruction instruction;
	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);


	QDomElement root = doc.createElement("config");
	doc.appendChild(root);
	QDomElement uploadConfig = doc.createElement("uploadConfig");
	QDomElement File = doc.createElement("File");

	QDomAttr fileId = doc.createAttribute("id");
	fileId.setValue("0");
	File.setAttributeNode(fileId);

	QDomAttr check = doc.createAttribute("checked");
	check.setValue("check");
	File.setAttributeNode(check);

	QDomAttr path = doc.createAttribute("path");
	path.setValue("./upload/test.txt");
	File.setAttributeNode(path);

	uploadConfig.appendChild(File);
	root.appendChild(uploadConfig);

	QDomElement downloadConfig = doc.createElement("downLoadConfig");
	File = doc.createElement("File");
	fileId = doc.createAttribute("id");
	fileId.setValue("0");
	File.setAttributeNode(fileId);

	check = doc.createAttribute("checked");
	check.setValue("uncheck");
	File.setAttributeNode(check);

	path = doc.createAttribute("path");
	path.setValue("./download/test.txt");
	File.setAttributeNode(path);

	downloadConfig.appendChild(File);
	root.appendChild(downloadConfig);

	QTextStream out_stream(&file);
	doc.save(out_stream, 4); //缩进4格
	file.close();

}

QDomNode SinXml::findNodByName(QDomElement elemtnt, QString nodeName)
{
	QDomNode temp;
	if (temp.nodeName() == nodeName)
	{
		return temp;
	}

	QDomNodeList childs = elemtnt.childNodes();
	for (int i = 0; i < childs.size(); i++)
	{
		temp = childs.at(i);
		QString qstrNodeName = temp.nodeName();
		if (qstrNodeName == nodeName)
		{
			return temp;
		}
		if (temp.hasChildNodes())
		{
			findNodByName(temp.toElement(), nodeName);

		}
	}

	if (temp.nodeName() != nodeName)
	{
		QDomNode temp1;
		return temp1;
	}

}

void SinXml::appendChilds(QDomDocument doc,QDomElement element, QString fileId, QString fileCheck, QString filePath)
{

	QDomElement FileElement = doc.createElement("File");

	QDomAttr fileIdAttr = doc.createAttribute("id");
	fileIdAttr.setValue(fileId);
	FileElement.setAttributeNode(fileIdAttr);

	QDomAttr checkAttr = doc.createAttribute("checked");
	checkAttr.setValue(fileCheck);
	FileElement.setAttributeNode(checkAttr);

	QDomAttr pathcAttr = doc.createAttribute("path");
	pathcAttr.setValue(filePath);
	FileElement.setAttributeNode(pathcAttr);
	
	element.appendChild(FileElement);
}

void SinXml::deleChild(QDomElement element, QString fileId){

	QDomNode temp;
	QDomNodeList childs = element.childNodes();
	for (int i = 0; i < childs.size(); i++)
	{
		temp = childs.at(i);
		QString qstrNodeName = temp.nodeName();
		QString strfileId =  temp.toElement().attribute("id");
		if (strfileId == fileId)
		{
			element.removeChild(temp);
		}
	}
}

void SinXml::updateChild(QDomElement element, QString fileId, QString fileCheck, QString filePath)
{
	QDomNode temp;
	QDomNodeList childs = element.childNodes();
	for (int i = 0; i < childs.size(); i++)
	{
		temp = childs.at(i);
		QString qstrNodeName = temp.nodeName();
		QString strfileId = temp.toElement().attribute("id");
		if (strfileId == fileId)
		{
			temp.toElement().setAttribute("checked", fileCheck);
			temp.toElement().setAttribute("path", filePath);
		}
	}

}

void SinXml::addUpLoadFile(bool isUpLoad, QString fileId, QString fileCheck, QString filePath)
{
	

	QDomDocument doc = readConfigFile();

	QDomElement root = doc.documentElement();
	QString elementName = getElementName(isUpLoad);;

	QDomNode node =  findNodByName(root,elementName);
	QString nodeName = node.nodeName();
	appendChilds(doc,node.toElement(), fileId, fileCheck, filePath);

	writeConfigFile(doc);

}

void SinXml::deleUpLoadFile(bool isUpLoad,QString fileId)
{
	QDomDocument doc = readConfigFile();
	QDomElement root = doc.documentElement();

	QString elementName = getElementName(isUpLoad);;

	QDomNode node = findNodByName(root, elementName);
	QString nodeName = node.nodeName();
	deleChild(node.toElement(), fileId);

	writeConfigFile(doc);

}

void SinXml::updateUpLoadFile(bool isUpLoad, QString fileId, QString fileCheck, QString filePath)
{
	QDomDocument doc = readConfigFile();

	QDomElement root = doc.documentElement();

	QString elementName = getElementName(isUpLoad);

	QDomNode node = findNodByName(root, elementName);
	QString nodeName = node.nodeName();
	updateChild(node.toElement(), fileId, fileCheck, filePath);

	writeConfigFile(doc);

}

void SinXml::reload(QString qstrContent)
{
	QDomDocument doc = readConfigFile();

	QFile file(configFileName);
	const QString &content = qstrContent;
	doc.setContent(content);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
		return;
	QTextStream out_stream(&file);
	doc.save(out_stream, 4); //缩进4格
	file.close();

}

void SinXml::fileChange(QString strFilePath)
{
	int a = 1;
}

QDomDocument SinXml::readConfigFile()
{
	QDomDocument doc;
	QFile file(configFileName);
	if (!file.open(QFile::ReadOnly))
		return doc;

	
	if (!doc.setContent(&file))
	{
		file.close();
		return doc;
	}
	file.close();
	return doc;
}

void SinXml::writeConfigFile(QDomDocument doc)
{
	QFile file(configFileName);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
		return;
	QTextStream out_stream(&file);
	doc.save(out_stream, 4); //缩进4格
	file.close();
}

QString SinXml::getElementName(bool isUpLoad)
{
	QString elementName;
	if (isUpLoad)
	{
		return elementName = "uploadConfig";
	}
	else {
		return elementName = "downLoadConfig";
	}
}

QList<FileConfigItem> SinXml::getFileConfigItemFromXmlConfig(bool isUpLoad)
{
	QList<FileConfigItem> result;
	QDomDocument doc = readConfigFile();
	return result;
}