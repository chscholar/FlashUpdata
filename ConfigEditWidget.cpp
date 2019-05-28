#include "ConfigEditWidget.h"
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextEdit>
#include "SinXmlHighlighter.h"

const QString configFileName = "config.xml";

ConfigEditWidget::ConfigEditWidget(QWidget *parent) {
	QFile file(configFileName);
	if (!file.exists())
	{
		file.open(QIODevice::ReadWrite | QIODevice::Text);
		initXml();
	}
	file.close();
		
	initUi();
}

ConfigEditWidget::~ConfigEditWidget()
{

}

void ConfigEditWidget::initUi()
{
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addSpacing(10);

	QTextEdit *configEdit = new QTextEdit();
	mainLayout->addWidget(configEdit);

	QFile file(configFileName);
	file.open(QIODevice::ReadOnly);
	QByteArray configXml = file.readAll();
	file.close();

	SinXmlHighlighter *highlighter = new SinXmlHighlighter(configEdit);
	configEdit->setPlainText(configXml);
	
	mainLayout->addSpacing(10);
	mainLayout->setStretch(1, 8);
	setLayout(mainLayout);

}

void ConfigEditWidget::initXml()
{
	QFile file(configFileName);
	if (!file.open(QFile::WriteOnly | QFile::Truncate)) //可以用QIODevice，Truncate表示清空原来的内容
		return;


	QDomDocument doc;
	//写入xml头部
	QDomProcessingInstruction instruction; //添加处理命令
	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);


	QDomElement root = doc.createElement("config");
	doc.appendChild(root);
	//添加第一个子节点及其子元素
	QDomElement book = doc.createElement("uploadConfig");
	book.setAttribute("id", 1); //方式一：创建属性  其中键值对的值可以是各种类型
	QDomAttr time = doc.createAttribute("time"); //方式二：创建属性 值必须是字符串
	time.setValue("2013/6/13");
	book.setAttributeNode(time);
	QDomElement title = doc.createElement("title"); //创建子元素
	QDomText text; //设置括号标签中间的值
	text = doc.createTextNode("C++ primer");
	book.appendChild(title);
	title.appendChild(text);
	QDomElement author = doc.createElement("author"); //创建子元素
	text = doc.createTextNode("Stanley Lippman");
	author.appendChild(text);
	book.appendChild(author);
	root.appendChild(book);

	//添加第二个子节点及其子元素，部分变量只需重新赋值
	book = doc.createElement("downLoadConfig");
	book.setAttribute("id", 2);
	time = doc.createAttribute("time");
	time.setValue("2007/5/25");
	book.setAttributeNode(time);
	title = doc.createElement("title");
	text = doc.createTextNode("Thinking in Java");
	book.appendChild(title);
	title.appendChild(text);
	author = doc.createElement("author");
	text = doc.createTextNode("Bruce Eckel");
	author.appendChild(text);
	book.appendChild(author);
	root.appendChild(book);

	//输出到文件
	QTextStream out_stream(&file);
	doc.save(out_stream, 4); //缩进4格
	file.close();

}