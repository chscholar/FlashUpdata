#include "FlashUpdata.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QTextEdit>
#include "NetWorkConfigWidget.h"
#include "SerialConfigWidget.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QProgressBar>
#include <QScrollArea>
#include "FileConfigWidget.h"
#include "SinTabWindow.h"
#include "ConfigDeployWidget.h"
#include "ConfigEditWidget.h"
#include "SinTaskQueue.h"
#include "pcap.h"
#include "SinSerial.h"


FlashUpdata::FlashUpdata(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initUi();

	connect(&sinTaskQueueSingle::getInstance(), SIGNAL(signalReadData()), this, SLOT(slotUpDataLogEdit()));
	connect(&sinserialSingle::getInstance(), SIGNAL(signalUpDateProgressBar(int)), this, SLOT(slotUpdataProgressBar(int)));
}

void FlashUpdata::initUi()
{   
	QHBoxLayout *mainLayout = new QHBoxLayout(this);

	QVBoxLayout  *workLayout = new QVBoxLayout(this);
	workLayout->addSpacing(10);

	m_pTabWindow = new SinTabWindow();
	workLayout->addWidget(m_pTabWindow);

	ConfigDeployWidget *configDeploy = new ConfigDeployWidget();
	m_pTabWindow->addView(configDeploy, "属性配置");

	ConfigEditWidget *configEdit = new ConfigEditWidget();
	m_pTabWindow->addView(configEdit, "属性编辑");

	m_pLogEdit = new QTextEdit();
	m_pLogEdit->setPlaceholderText("this is log info");
	m_pLogEdit->setAcceptRichText(true);
	workLayout->addWidget(m_pLogEdit);


	QHBoxLayout *progrressLayout = new QHBoxLayout();
	QLabel *progressLabel = new QLabel();
	progressLabel->setText("传输进度");
	progrressLayout->addWidget(progressLabel);

	m_pProgressBar= new QProgressBar();
	m_pProgressBar->setMinimum(0);
	m_pProgressBar->setMaximum(100);
	m_pProgressBar->setRange(0, 100);
	m_pProgressBar->setOrientation(Qt::Horizontal);
	progrressLayout->addWidget(m_pProgressBar);

	QLabel *progressValue = new QLabel();
	//progressValue->setText("0%");
	progrressLayout->addWidget(progressValue);
	progrressLayout->setStretch(0, 1);
	progrressLayout->setStretch(1, 8);
	progrressLayout->setStretch(2, 1);
	workLayout->addLayout(progrressLayout);
	
	workLayout->setStretch(0, 5);
	workLayout->setStretch(1, 5);
	workLayout->setStretch(2, 1);

	mainLayout->addLayout(workLayout);

	QVBoxLayout *connectLayout = new QVBoxLayout(this);
	connectLayout->addSpacing(10);
	QTabWidget *connectTab = new QTabWidget(this);
	connectTab->insertTab(0, new SerialConfigWidget(), "串口设置");
	connectTab->insertTab(1, new NetWorkConfigWidget(), "网口设置");

	connectLayout->addWidget(connectTab);

	mainLayout->addLayout(connectLayout);

	mainLayout->setStretch(0, 6);
	mainLayout->setStretch(1, 4);

	this->centralWidget()->setLayout(mainLayout);
	//setLayout(mainLayout);

}

void FlashUpdata::slotSwitchRadio(int radioID, bool bCheck) 
{
	if (bCheck && radioID == RADIO_DOWNLOAD)
	{
		m_pFileConfigWidget->switchWidget(true);
	}

	if (bCheck && radioID == RADIO_DOWNLOAD)
	{
		m_pFileConfigWidget->switchWidget(false);
	}
}

void FlashUpdata::slotUpDataLogEdit()
{
	//更新界面日志
	QString data = sinTaskQueueSingle::getInstance().popBackReadData();

	QString qstrText = m_pLogEdit->toPlainText() +"\r ";

	QString  qstrPlainText =  m_pLogEdit->toPlainText();
	QString qstrTextHtml = m_pLogEdit->toHtml();

	QString  qstrPlainTextL = m_pLogEdit->toPlainText().toLocal8Bit();
	QString qstrTextHtmlL = m_pLogEdit->toHtml().toLocal8Bit();

	QString newText = qstrText + data;

	m_pLogEdit->setPlainText(newText);
}

void FlashUpdata::slotUpdataProgressBar(int value)
{
	m_pProgressBar->setValue(value);
}