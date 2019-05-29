#include "ConfigEditWidget.h"
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QTextEdit>
#include <QPainter>
#include <QPushButton>
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
	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addSpacing(10);

	m_pConfigEdit = new QTextEdit();
	mainLayout->addWidget(m_pConfigEdit);
	m_pConfigEdit->setReadOnly(true);
	m_pConfigEdit->setEnabled(false);
	updateEditContent();

	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch(2);
	QPushButton *editButton = new QPushButton();
	editButton->setText("±à¼­");
	buttonLayout->addWidget(editButton);
	connect(editButton, SIGNAL(clicked()), this, SLOT(slotUpdateEdit()));

	buttonLayout->addStretch(1);
	QPushButton *saveButton = new QPushButton();
	saveButton->setText("±£´æ");
	buttonLayout->addWidget(saveButton);
	buttonLayout->addStretch(2);
	mainLayout->addLayout(buttonLayout);
	connect(saveButton, SIGNAL(clicked()), this, SLOT(slotSaveEditContent()));



	mainLayout->addSpacing(10);
	mainLayout->setStretch(0, 8);
	mainLayout->setStretch(1, 2);
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

void ConfigEditWidget::slotUpdateEdit()
{
	m_pConfigEdit->setEnabled(true);
	m_pConfigEdit->setReadOnly(false);
}

void ConfigEditWidget::slotSaveEditContent()
{
	m_pConfigEdit->setEnabled(false);
	m_pConfigEdit->setReadOnly(true);
	
	QString qstrContent = m_pConfigEdit->toPlainText();
	m_pXml->reload(qstrContent);

	updateEditContent();
	m_pConfigEdit->update();
}