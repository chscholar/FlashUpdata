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

FlashUpdata::FlashUpdata(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	initUi();
}

void FlashUpdata::initUi()
{
	QHBoxLayout *mainLayout = new QHBoxLayout(this);

	QVBoxLayout  *workLayout = new QVBoxLayout(this);
	workLayout->addSpacing(10);

	QGroupBox *configBox = new QGroupBox(this);
	configBox->setTitle("属性配置");
	workLayout->addWidget(configBox);

	QVBoxLayout *configLayout = new QVBoxLayout(configBox);


	QButtonGroup *radioGroup = new QButtonGroup();
	QHBoxLayout *radioLayout = new QHBoxLayout();
	radioLayout->addStretch(1);
	QRadioButton *uploadRadio = new QRadioButton("上传");
	radioLayout->addWidget(uploadRadio);
	radioLayout->addStretch(1);
	QRadioButton *downloadRadio = new QRadioButton("下载");
	radioLayout->addWidget(downloadRadio);

	connect(radioGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(slotSwitchRadio(int, bool)));

	uploadRadio->setChecked(true);
	radioGroup->addButton(uploadRadio);
	radioGroup->setId(uploadRadio, RADIOID_UPLOAD);
	radioGroup->addButton(downloadRadio);
	radioGroup->setId(downloadRadio, RADIO_DOWNLOAD);
	radioLayout->addStretch(1);
	configLayout->addLayout(radioLayout);

	QScrollArea *fileConfigArea = new QScrollArea();
	m_pFileConfigWidget = new FileConfigWidget();
	fileConfigArea->setWidget(m_pFileConfigWidget);
	fileConfigArea->setWidgetResizable(true);
	configLayout->addWidget(fileConfigArea);

	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addSpacing(10);
	buttonLayout->addStretch(1);
	QPushButton *confirmButton = new QPushButton("确认");
	buttonLayout->addWidget(confirmButton);

	buttonLayout->addStretch(3);
	QPushButton *cancleButton = new QPushButton("取消");
	buttonLayout->addWidget(cancleButton);
	buttonLayout->addStretch(1);
	buttonLayout->addSpacing(10);
	configLayout->addLayout(buttonLayout);

	configBox->setLayout(configLayout);

	QTextEdit *logEdit = new QTextEdit();
	logEdit->append("this is log info");
	workLayout->addWidget(logEdit);


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
	
	workLayout->setStretch(0, 4);
	workLayout->setStretch(1, 6);
	workLayout->setStretch(2, 1);

	mainLayout->addLayout(workLayout);

	QVBoxLayout *connectLayout = new QVBoxLayout(this);
	connectLayout->addSpacing(10);
	QTabWidget *connectTab = new QTabWidget(this);
	connectTab->insertTab(0, new SerialConfigWidget(), "串口设置");
	connectTab->insertTab(1, new NetWorkConfigWidget(), "网口设置");

	connectLayout->addWidget(connectTab);

	mainLayout->addLayout(connectLayout);

	mainLayout->setStretch(0, 7);
	mainLayout->setStretch(1, 3);

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