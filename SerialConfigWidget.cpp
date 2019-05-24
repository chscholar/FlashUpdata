#include "SerialConfigWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "SinSerial.h"
#include <QStringList>
#include <QMessageBox>

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

void SerialItem::setTipText(QString str)
{
	m_pLabel->setText(str);
}

void SerialItem::setValuItems(QStringList strList)
{
	m_pCombox->addItems(strList);
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

SerialConfigWidget::SerialConfigWidget(QWidget*parent){
	initUi();
}

SerialConfigWidget::~SerialConfigWidget(){

}

void SerialConfigWidget::initUi()
{

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->addSpacing(30);

	m_pPortName = new SerialItem();
	m_pPortName->setTipText("���ڣ�");
	QStringList comList = sinserial::getInstance().getEnablePorts();
	m_pPortName->setValuItems(comList);
	mainLayout->addWidget(m_pPortName);

	m_pRate = new SerialItem();
	m_pRate->setTipText("���ʣ�");
	QStringList rateList = sinserial::getInstance().getEnableRates();
	m_pRate->setValuItems(rateList);
	mainLayout->addWidget(m_pRate);


	m_pFlow = new SerialItem();
	m_pFlow->setTipText("���أ�");
	QStringList flowList = sinserial::getInstance().getFlowControl();
	m_pFlow->setValuItems(flowList);
	mainLayout->addWidget(m_pFlow);


	m_pParity = new SerialItem();
	m_pParity->setTipText("У�飺");
	QStringList parityList = sinserial::getInstance().getParity();
	m_pParity->setValuItems(parityList);
	mainLayout->addWidget(m_pParity);

	
	m_pDataBit = new SerialItem();
	m_pDataBit->setTipText("����λ��");
	QStringList dataList = sinserial::getInstance().getDataBits();
	m_pDataBit->setValuItems(dataList);
	mainLayout->addWidget(m_pDataBit);


	m_pStopDataBit = new SerialItem();
	m_pStopDataBit->setTipText("ֹͣλ��");
	QStringList stoopList = sinserial::getInstance().getStopBits();
	m_pStopDataBit->setValuItems(stoopList);
	mainLayout->addWidget(m_pStopDataBit);

	m_pStatus = new SerialItem(false);
	m_pStatus->setTipText("����״̬��");
	m_pStatus->setStatus(false);
	mainLayout->addWidget(m_pStatus);


	mainLayout->addSpacing(20);
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addStretch(2);

	m_pCloseOpenButton = new QPushButton();
	m_pCloseOpenButton->setText("�򿪴���");
	buttonLayout->addWidget(m_pCloseOpenButton);
	buttonLayout->addStretch(2);
	mainLayout->addLayout(buttonLayout);

	mainLayout->addStretch(3);
	setLayout(mainLayout);

	connect(m_pCloseOpenButton, SIGNAL(clicked()), this, SLOT(slotOpenCloseCom()));
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
		QMessageBox::information(this, "����", "��ѡ��˿�,����,���أ�����λ��У��λ��ֹͣλ", QMessageBox::Ok);
		return;
	}


	bool isOpen = sinserial::getInstance().isOPen();
	if (isOpen)
	{
		sinserial::getInstance().closeCom();
		m_pStatus->setStatus(false);
		m_pCloseOpenButton->setText("�򿪴���");
	}
	else {
		int nResult = sinserial::getInstance().openCom(portIndex, rateIndex, flowIndex, dataIndex, stopIndex, parityIndex);
		if (nResult == 0)
		{
			m_pStatus->setStatus(true);
			m_pCloseOpenButton->setText("�رմ���");
		}
		else {
			m_pStatus->setStatus(false);
			m_pCloseOpenButton->setText("�򿪴���");
		}
	}

	
}