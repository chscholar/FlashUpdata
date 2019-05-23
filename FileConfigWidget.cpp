#include "FileConfigWidget.h"
#include <QHBoxLayout>
#include <QLabel>

FileConfigWidget::FileConfigWidget(QWidget *parent) :
QWidget(parent)
{
	initUi();
}

FileConfigWidget::~FileConfigWidget()
{

}

void FileConfigWidget::initUi()
{

	QHBoxLayout *mainLayout = new QHBoxLayout();
	QLabel *label = new QLabel();
	label->setText("file config");
	mainLayout->addWidget(label);

	setLayout(mainLayout);
}