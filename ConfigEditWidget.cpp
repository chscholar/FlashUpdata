#include "ConfigEditWidget.h"
#include <QTextBrowser>
#include <QHBoxLayout>

ConfigEditWidget::ConfigEditWidget(QWidget *parent) {
	initUi();
}

ConfigEditWidget::~ConfigEditWidget()
{

}

void ConfigEditWidget::initUi()
{
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addSpacing(10);

	QTextBrowser *browse = new QTextBrowser();
	mainLayout->addWidget(browse);

	mainLayout->addSpacing(10);
	mainLayout->setStretch(1, 8);
	setLayout(mainLayout);

}