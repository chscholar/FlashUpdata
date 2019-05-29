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
#include "SinXml.h"

ConfigEditWidget::ConfigEditWidget(QWidget *parent) {

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
	QByteArray configFileContent = sinXmlSingle::getInstance().getConfigFileContent();
	SinXmlHighlighter *highlighter = new SinXmlHighlighter(m_pConfigEdit);
	m_pConfigEdit->setPlainText(configFileContent);
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
	sinXmlSingle::getInstance().reload(qstrContent);

	updateEditContent();
	m_pConfigEdit->update();
}