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

	m_pXml = new SinXml();
	QFile file(configFileName);
	if (!file.exists())
	{
		file.open(QIODevice::ReadWrite | QIODevice::Text);
		m_pXml->initXml();
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