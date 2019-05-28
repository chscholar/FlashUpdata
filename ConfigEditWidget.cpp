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
	if (!file.open(QFile::WriteOnly | QFile::Truncate)) //������QIODevice��Truncate��ʾ���ԭ��������
		return;


	QDomDocument doc;
	//д��xmlͷ��
	QDomProcessingInstruction instruction; //��Ӵ�������
	instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	doc.appendChild(instruction);


	QDomElement root = doc.createElement("config");
	doc.appendChild(root);
	//��ӵ�һ���ӽڵ㼰����Ԫ��
	QDomElement book = doc.createElement("uploadConfig");
	book.setAttribute("id", 1); //��ʽһ����������  ���м�ֵ�Ե�ֵ�����Ǹ�������
	QDomAttr time = doc.createAttribute("time"); //��ʽ������������ ֵ�������ַ���
	time.setValue("2013/6/13");
	book.setAttributeNode(time);
	QDomElement title = doc.createElement("title"); //������Ԫ��
	QDomText text; //�������ű�ǩ�м��ֵ
	text = doc.createTextNode("C++ primer");
	book.appendChild(title);
	title.appendChild(text);
	QDomElement author = doc.createElement("author"); //������Ԫ��
	text = doc.createTextNode("Stanley Lippman");
	author.appendChild(text);
	book.appendChild(author);
	root.appendChild(book);

	//��ӵڶ����ӽڵ㼰����Ԫ�أ����ֱ���ֻ�����¸�ֵ
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

	//������ļ�
	QTextStream out_stream(&file);
	doc.save(out_stream, 4); //����4��
	file.close();

}