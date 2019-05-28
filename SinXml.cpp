#include "SinXml.h"
#include <QFile>
#include <QDomDocument>
#include <QTextStream>

const QString configFileName = "config.xml";

SinXml::SinXml(QObject *parent)
	:QObject(parent){

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
	QDomText text;
	text = doc.createTextNode("./upload/test.txt");
	uploadConfig.appendChild(File);
	File.appendChild(text);
	root.appendChild(uploadConfig);

	QDomElement downloadConfig = doc.createElement("downLoadConfig");
	File = doc.createElement("File");
	fileId = doc.createAttribute("id");
	fileId.setValue("0");
	File.setAttributeNode(fileId);
	check = doc.createAttribute("checked");
	check.setValue("uncheck");
	File.setAttributeNode(check);
	text = doc.createTextNode("./download/test.txt");
	downloadConfig.appendChild(File);
	File.appendChild(text);
	root.appendChild(downloadConfig);

	QTextStream out_stream(&file);
	doc.save(out_stream, 4); //缩进4格
	file.close();

}

void SinXml::addUpLoadFile(bool isUpLoad, QString fileId, QString fileCheck, QString filePath)
{
	QFile file(configFileName);
	if (!file.open(QFile::ReadOnly))
		return;

	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();

	QDomElement root = doc.documentElement();

	QDomNodeList childs = doc.childNodes();


	QDomElement book = doc.createElement("book");
	book.setAttribute("id", 3);
	book.setAttribute("time", "1813/1/27");
	QDomElement title = doc.createElement("title");
	QDomText text;
	text = doc.createTextNode("Pride and Prejudice");
	title.appendChild(text);
	book.appendChild(title);
	QDomElement author = doc.createElement("author");
	text = doc.createTextNode("Jane Austen");
	author.appendChild(text);
	book.appendChild(author);
	root.appendChild(book);

	if (!file.open(QFile::WriteOnly | QFile::Truncate))
		return;
	QTextStream out_stream(&file);
	doc.save(out_stream, 4); //缩进4格
	file.close();

}

void SinXml::updateUpLoadFile(bool isUpLoad, QString fileId, QString fileCheck, QString filePath)
{

}

void SinXml::deleUpLoadFile(bool isUpLoad, int fileId)
{

}