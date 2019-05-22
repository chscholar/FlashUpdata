#include "SerialConfig.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "SinSerial.h"
#include <QStringList>
#include <QMessageBox>

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
	m_pComBox = new QComboBox();
	m_pComBox->setFixedWidth(60);
	m_pComBox->addItem("  ");
	QStringList comList = sinserial::getInstance().getEnablePorts();
	m_pComBox->addItems(comList);

	comLayout->addWidget(m_pComBox);
	comLayout->addStretch(1);
	mainLayout->addLayout(comLayout);

	mainLayout->addSpacing(10);
	QHBoxLayout *rateLayout = new QHBoxLayout();
	rateLayout->addSpacing(30);
	QLabel *rateLabel = new QLabel();
	rateLabel->setText("速率:");
	rateLayout->addWidget(rateLabel);
	
	rateLayout->addSpacing(20);
	m_pRateBox = new QComboBox();
	m_pRateBox->addItem("   ");
	QStringList rateList = sinserial::getInstance().getEnableRates();
	m_pRateBox->addItems(rateList);
	rateLayout->addWidget(m_pRateBox);
	rateLayout->addStretch(1);
	mainLayout->addLayout(rateLayout);


	QHBoxLayout *statusLayout = new QHBoxLayout();
	statusLayout->addSpacing(30);
	QLabel *statusLabel = new QLabel();
	statusLabel->setText("串口状态：");
	statusLayout->addWidget(statusLabel);
	
	m_pStatusLabel = new QLabel();
	m_pStatusLabel->setFixedSize(QSize(20, 20));
	m_pStatusLabel->setStyleSheet(m_red_SheetStyle);
	statusLayout->addWidget(m_pStatusLabel);
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

	connect(openCloseCom, SIGNAL(clicked()), this, SLOT(slotOpenCloseCom()));
}

void SerialConfigWidget::slotOpenCloseCom()
{
	const QString m_red_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:red";
	const QString m_green_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:green";
	const QString m_grey_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:grey";
	const QString m_yellow_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:yellow";

	QString strPortName = m_pComBox->currentText();
	QString strRate = m_pRateBox->currentText();

	strPortName.remove(QRegExp("\\s"));
	strRate.remove(QRegExp("\\s"));
	
	if (strPortName.isEmpty() || strRate.isEmpty() || strPortName.isNull() || strRate.isNull())
	{
		QMessageBox::information(NULL, "错误", "请选择端口或速率", QMessageBox::Ok);
		return;
	}

	bool isOpen = sinserial::getInstance().isOPen();
	if (isOpen)
	{
		sinserial::getInstance().closeCom();
		m_pStatusLabel->setStyleSheet(m_red_SheetStyle);
	}
	else {
		int nResult = sinserial::getInstance().openCom(strPortName, strRate);
		if (nResult == 0)
		{
			m_pStatusLabel->setStyleSheet(m_green_SheetStyle);
		}else {
			m_pStatusLabel->setStyleSheet(m_red_SheetStyle);
		}
	}
	
}