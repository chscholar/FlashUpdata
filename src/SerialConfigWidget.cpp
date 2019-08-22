#include "SerialConfigWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "SinSerial.h"
#include <QStringList>
#include <QMessageBox>
#include "SinSerialChoose.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <QtConcurrent>
#include <QDebug>

SerialItem::SerialItem(bool isCombox,  QWidget *parent):
QWidget(parent)
{
	m_bIsCombox = isCombox;
	initUi();
	m_green_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:green";
	m_grey_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:grey";
	m_combox_sheetStyle = "";
}

SerialItem ::~SerialItem()
{

}

void SerialItem::initUi()
{
	QHBoxLayout *itemLayout = new QHBoxLayout();
	itemLayout->addSpacing(20);
	m_pLabel = new QLabel();
	m_pLabel->setAlignment(Qt::AlignHCenter);
	m_pLabel->setText("");
	itemLayout->setStretch(1, 1);
	
	itemLayout->addWidget(m_pLabel);

	if (m_bIsCombox)
	{
		m_pCombox = new QComboBox();
		m_pCombox->addItem("   ");
		itemLayout->addWidget(m_pCombox);
		itemLayout->setStretch(2, 4);
	}
	else {
		m_pLabelStatus = new QLabel();
		m_pLabelStatus->setFixedSize(QSize(20, 20));
		itemLayout->addWidget(m_pLabelStatus);
	}
	itemLayout->addSpacing(20);
	itemLayout->addStretch(2);

	
	setLayout(itemLayout);
}

void SerialItem::setComandBoxEnable(bool isEnable){
	m_pCombox->setEnabled(isEnable);
}

void SerialItem::setTipText(QString str)
{
	m_pLabel->setText(str);
}

void SerialItem::setValuItems(QStringList strList)
{
	m_pCombox->addItems(strList);
}

QStringList SerialItem::getValues()
{
	QStringList strList;
	for (int i = 0; i < m_pCombox->count();i++)
	{
		strList.append(m_pCombox->itemText(i));
	}
	
	return strList;
}

QString SerialItem::getSelectValue()
{
	return m_pCombox->currentText();
}


void SerialItem::setStatus(bool isOpen)
{
	if (m_bIsCombox) return;
	
	if (isOpen)
	{
		m_pLabelStatus->setStyleSheet(m_green_SheetStyle);
	}
	else {
		m_pLabelStatus->setStyleSheet(m_grey_SheetStyle);
	}
}

int SerialItem::getSelectIndex()
{
	if (!m_bIsCombox) return -1;
	
	return m_pCombox->currentIndex();
	
}

void SerialItem::setDefaultIndex(int defaultIndex)
{
	m_pCombox->setCurrentIndex(defaultIndex);
}

SerialConfigWidget::SerialConfigWidget(QWidget*parent){
	initUi();
	m_pChoose = new SinSerialChoose();
	connect(m_pChoose, SIGNAL(signalsHandOkSerial(QString)), this, SLOT(slotSetSinSerialSignal(QString)));
}

SerialConfigWidget::~SerialConfigWidget(){

}

void SerialConfigWidget::initUi()
{

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addSpacing(30);

	m_pPortName = new SerialItem();
	m_pPortName->setTipText("串口：");
	QStringList comList = sinserialSingle::getInstance().getEnablePorts();
	m_pPortName->setValuItems(comList);
	//m_pPortName->setComandBoxEnable(false); //不能选择 自动搜索串口
	mainLayout->addWidget(m_pPortName);

	m_pRate = new SerialItem();
	m_pRate->setTipText("速率：");
	QStringList rateList = sinserialSingle::getInstance().getEnableRates();
	m_pRate->setValuItems(rateList);
	m_pRate->setDefaultIndex(10);
	mainLayout->addWidget(m_pRate);


	m_pFlow = new SerialItem();
	m_pFlow->setTipText("流控：");
	QStringList flowList = sinserialSingle::getInstance().getFlowControl();
	m_pFlow->setValuItems(flowList);
	m_pFlow->setDefaultIndex(1);
	mainLayout->addWidget(m_pFlow);


	m_pParity = new SerialItem();
	m_pParity->setTipText("校验：");
	QStringList parityList = sinserialSingle::getInstance().getParity();
	m_pParity->setValuItems(parityList);
	m_pParity->setDefaultIndex(1);
	mainLayout->addWidget(m_pParity);

	
	m_pDataBit = new SerialItem();
	m_pDataBit->setTipText("数据位：");
	QStringList dataList = sinserialSingle::getInstance().getDataBits();
	m_pDataBit->setValuItems(dataList);
	m_pDataBit->setDefaultIndex(4);
	mainLayout->addWidget(m_pDataBit);


	m_pStopDataBit = new SerialItem();
	m_pStopDataBit->setTipText("停止位：");
	QStringList stoopList = sinserialSingle::getInstance().getStopBits();
	m_pStopDataBit->setValuItems(stoopList);
	m_pStopDataBit->setDefaultIndex(1);
	mainLayout->addWidget(m_pStopDataBit);

	m_pStatus = new SerialItem(false);
	m_pStatus->setTipText("串口状态：");
	m_pStatus->setStatus(false);
	mainLayout->addWidget(m_pStatus);


	mainLayout->addSpacing(20);
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch(2);

	m_pCloseOpenButton = new QPushButton();
	m_pCloseOpenButton->setText("打开串口");
	buttonLayout->addWidget(m_pCloseOpenButton);
	buttonLayout->addStretch(2);
	mainLayout->addLayout(buttonLayout);

	

	mainLayout->addStretch(3);
	setLayout(mainLayout);

	//connect(m_pCloseOpenButton, SIGNAL(clicked()), this, SLOT(slotOpenCloseCom()));
	connect(m_pCloseOpenButton, SIGNAL(clicked()), this, SLOT(slotChooseCom()));
}

void SerialConfigWidget::slotSetSinSerialSignal(QString portName)
{
	//slotOpenCloseCom();
	int a = 1;

}

//遍历选择打开串口
void SerialConfigWidget::slotChooseCom()
{
	QStringList portNameList = m_pPortName->getValues();
	QString rateValue = m_pRate->getSelectValue();
	QString flowValue = m_pFlow->getSelectValue();
	QString dataValue = m_pDataBit->getSelectValue();
	QString stopValue = m_pStopDataBit->getSelectValue();
	QString parityValue = m_pParity->getSelectValue();

	if (rateValue.isEmpty()|| flowValue.isEmpty() || dataValue.isEmpty()|| stopValue.isEmpty()|| parityValue.isEmpty())
	{
		QMessageBox::information(this, "错误", "速率,流控，数据位，校验位，停止位", QMessageBox::Ok);
		return;
	}

	m_vSerialConfigList.clear();
	for (int portIndex = 1; portIndex< portNameList.count(); portIndex++)
	{
		QString portName = portNameList.at(portIndex);
		
		SerialConfig config;
		config.portName = portName;
		config.rateValue = rateValue;
		config.flowValue = flowValue;
		config.dataValue = dataValue;
		config.stopValue = stopValue;
		config.parityValue = parityValue;

		m_vSerialConfigList.append(config);
	}

	m_pChoose->setSerialConfig(m_vSerialConfigList);
	m_pChoose->start();
	
	/*QFuture<QSerialPort*> ft = QtConcurrent::run(this,&SerialConfigWidget::chooseSerial);
	ft.waitForFinished();
	QSerialPort *serial = ft.result();*/
	
}

QSerialPort* SerialConfigWidget::chooseSerial()
{
	/*for (int i = 0; i < m_vSerialPortList.count(); i++)
	{
	SerialChooseOperation *serial = m_vSerialPortList.at(i);
	bool isHandOk =  serial->chooseSerial();
	if (isHandOk)
	{
	return serial->getSerialPort();
	qDebug() << "握手成功，找寻到串口";
	}

	if (i == m_vSerialPortList.count() - 1)
	{
	i = 0;
	}

	qDebug() << "正在搜索串口信息";
	}*/

	return NULL;
}

void SerialConfigWidget::slotOpenCloseCom()
{

	int portIndex = m_pPortName->getSelectIndex();
	int rateIndex = m_pRate->getSelectIndex();
	int flowIndex = m_pFlow->getSelectIndex();
	int dataIndex = m_pDataBit->getSelectIndex();
	int stopIndex = m_pStopDataBit->getSelectIndex();
	int parityIndex = m_pParity->getSelectIndex();

	if (portIndex <= 0 || rateIndex <= 0 || flowIndex <= 0 || dataIndex <= 0 || stopIndex <= 0 || parityIndex <= 0)
	{
		QMessageBox::information(this, "错误", "请选择端口,速率,流控，数据位，校验位，停止位", QMessageBox::Ok);
		return;
	}


	bool isOpen = sinserialSingle::getInstance().isOPen();
	if (isOpen)
	{
		sinserialSingle::getInstance().closeCom();
		m_pStatus->setStatus(false);
		m_pCloseOpenButton->setText("打开串口");
	}
	else {
		int nResult = sinserialSingle::getInstance().openCom(portIndex, rateIndex, flowIndex, dataIndex, stopIndex, parityIndex);
		if (nResult == 0)
		{
			m_pStatus->setStatus(true);
			m_pCloseOpenButton->setText("关闭串口");
		}
		else {
			m_pStatus->setStatus(false);
			m_pCloseOpenButton->setText("打开串口");
		}
	}

	
}