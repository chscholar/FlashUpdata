#include "SerialConfig.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

SerialConfigWidget::SerialConfigWidget(QWidget*parent){
	initUi();
}

SerialConfigWidget::~SerialConfigWidget(){

}

void SerialConfigWidget::initUi()
{

	const QString m_red_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:red";
	const QString m_green_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:green";
	const QString m_grey_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:grey";
	const QString m_yellow_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:yellow";


	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addSpacing(30);


	QHBoxLayout *comLayout = new QHBoxLayout();
	comLayout->addSpacing(30);
	QLabel *comLabel = new QLabel();
	comLabel->setText("COM口:");
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
	rateLabel->setText("速率:");
	rateLayout->addWidget(rateLabel);
	
	rateLayout->addSpacing(20);
	QComboBox *rateBox = new QComboBox();
	rateBox->addItem("   ");
	rateBox->addItem("9500");
	rateBox->addItem("115200");
	rateLayout->addWidget(rateBox);
	rateLayout->addStretch(1);
	mainLayout->addLayout(rateLayout);


	QHBoxLayout *statusLayout = new QHBoxLayout();
	statusLayout->addSpacing(30);
	QLabel *statusLabel = new QLabel();
	statusLabel->setText("串口状态：");
	statusLayout->addWidget(statusLabel);
	
	QLabel *status = new QLabel();
	status->setFixedSize(QSize(20, 20));
	status->setStyleSheet(m_red_SheetStyle);
	statusLayout->addWidget(status);
	statusLayout->addStretch(1);
	mainLayout->addLayout(statusLayout);


	mainLayout->addSpacing(20);
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch(2);

	QPushButton *openCloseCom = new QPushButton();
	openCloseCom->setText("打开串口");
	buttonLayout->addWidget(openCloseCom);
	buttonLayout->addStretch(2);
	mainLayout->addLayout(buttonLayout);

	mainLayout->addStretch(3);
	setLayout(mainLayout);
}