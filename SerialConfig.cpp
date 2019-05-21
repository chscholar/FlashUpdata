#include "SerialConfig.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
SerialConfigWidget::SerialConfigWidget(QWidget*parent){
	initUi();
}

SerialConfigWidget::~SerialConfigWidget(){

}

void SerialConfigWidget::initUi()
{

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addSpacing(30);


	QHBoxLayout *comLayout = new QHBoxLayout();
	comLayout->addSpacing(30);
	QLabel *comLabel = new QLabel();
	comLabel->setText("COM¿Ú:");
	comLayout->addWidget(comLabel);

	comLayout->addSpacing(20);
	QComboBox *comBox = new QComboBox();
	comBox->setFixedWidth(60);
	comBox->addItem("  ");
	comBox->addItem("COM1");
	comBox->addItem("COM2");
	comBox->addItem("COM2");
	comBox->addItem("COM4");
	comLayout->addWidget(comBox);
	comLayout->addStretch(1);
	mainLayout->addLayout(comLayout);

	mainLayout->addSpacing(10);
	QHBoxLayout *rateLayout = new QHBoxLayout();
	rateLayout->addSpacing(30);
	QLabel *rateLabel = new QLabel();
	rateLabel->setText("ËÙÂÊ:");
	rateLayout->addWidget(rateLabel);
	
	rateLayout->addSpacing(20);
	QComboBox *rateBox = new QComboBox();
	rateBox->addItem("   ");
	rateBox->addItem("9500");
	rateBox->addItem("115200");
	rateLayout->addWidget(rateBox);
	rateLayout->addStretch(1);
	mainLayout->addLayout(rateLayout);


	mainLayout->addStretch(3);
	setLayout(mainLayout);
}