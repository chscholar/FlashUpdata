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


FlashUpdata::FlashUpdata(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initUi();

	connect(&sinTaskQueueSingle::getInstance(), SIGNAL(signalReadData()), this, SLOT(slotUpDataLogEdit()));
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
	m_pLogEdit->append("this is log info");
	workLayout->addWidget(m_pLogEdit);


	QHBoxLayout *progrressLayout = new QHBoxLayout();
	QLabel *progressLabel = new QLabel();
	progressLabel->setText("传输进度");
	progrressLayout->addWidget(progressLabel);

	QProgressBar *progressBar = new QProgressBar();
	progrressLayout->addWidget(progressBar);

	QLabel *progressValue = new QLabel();
	progressValue->setText("0%");
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
	QByteArray data = sinTaskQueueSingle::getInstance().popBackReadData();

	QString qstrText = m_pLogEdit->toPlainText();

	QString newText = qstrText + data;

	m_pLogEdit->setText(newText);
}