#include "ConfigEditWidget.h"
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextEdit>
#include <QPainter>
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

	m_pConfigEdit = new QTextEdit();
	mainLayout->addWidget(m_pConfigEdit);

	updateEditContent();

	mainLayout->addSpacing(10);
	mainLayout->setStretch(1, 8);
	setLayout(mainLayout);
}

void ConfigEditWidget::updateEditContent()
{
	QFile file(configFileName);
	file.open(QIODevice::ReadOnly);
	QByteArray configXml = file.readAll();
	file.close();

	SinXmlHighlighter *highlighter = new SinXmlHighlighter(m_pConfigEdit);
	m_pConfigEdit->setPlainText(configXml);
}

void ConfigEditWidget::showEvent(QShowEvent *e)
{
	updateEditContent();
	m_pConfigEdit->update();
}