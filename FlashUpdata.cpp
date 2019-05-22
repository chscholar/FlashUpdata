#include "FlashUpdata.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QTextEdit>
#include "NetWorkConfig.h"
#include "SerialConfig.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QProgressBar>

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

	uploadRadio->setChecked(true);
	radioGroup->addButton(uploadRadio);
	radioGroup->addButton(downloadRadio);
	radioLayout->addStretch(1);
	configLayout->addLayout(radioLayout);


	QLabel *fileConfigLabel = new QLabel(configBox);
	fileConfigLabel->setText("保存位置");
	configLayout->addWidget(fileConfigLabel);

	QLabel *fileLabel = new QLabel(configBox);
	fileLabel->setText("文件");
	configLayout->addWidget(fileLabel);

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
	connectTab->insertTab(1, new NetWorkConfig(), "网口设置");

	connectLayout->addWidget(connectTab);

	mainLayout->addLayout(connectLayout);

	mainLayout->setStretch(0, 7);
	mainLayout->setStretch(1, 3);

	this->centralWidget()->setLayout(mainLayout);
	//setLayout(mainLayout);

}
