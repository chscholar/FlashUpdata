#include "ConfigDeployWidget.h"
#include <qgroupbox.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QScrollArea>
#include <QPushButton>
#include <QButtonGroup>
#include <FileConfigWidget.h>

ConfigDeployWidget::ConfigDeployWidget(QWidget *parent)
	:QWidget(parent)
{
	initUi();
}

ConfigDeployWidget::~ConfigDeployWidget()
{

}

void ConfigDeployWidget::initUi()
{
	//configDeploy->setStyleSheet("QMainWindow { background-color: red; }");
	//configEdit->setStyleSheet("QMainWindow { background-color: blue; }");
	QVBoxLayout *workLayout = new QVBoxLayout();

	QGroupBox *configBox = new QGroupBox(this);
	configBox->setTitle("属性配置");
	workLayout->addWidget(configBox);

	QVBoxLayout *configLayout = new QVBoxLayout(configBox);


	QButtonGroup *radioGroup = new QButtonGroup();
	QHBoxLayout *radioLayout = new QHBoxLayout();
	radioLayout->addStretch(1);
	QRadioButton *downloadRadio = new QRadioButton("下载");
	radioLayout->addWidget(downloadRadio);

	radioLayout->addStretch(1);
	QRadioButton *uploadRadio = new QRadioButton("上传");
	radioLayout->addWidget(uploadRadio);


	//connect(radioGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(slotSwitchRadio(int, bool)));

	downloadRadio->setChecked(true);
	radioGroup->addButton(uploadRadio);
	radioGroup->setId(uploadRadio, 0);
	radioGroup->addButton(downloadRadio);
	radioGroup->setId(downloadRadio, 1);
	radioLayout->addStretch(1);
	configLayout->addLayout(radioLayout);

	QScrollArea *fileConfigArea = new QScrollArea();
	FileConfigWidget *m_pFileConfigWidget = new FileConfigWidget();
	fileConfigArea->setWidget(m_pFileConfigWidget);
	fileConfigArea->setWidgetResizable(true);
	fileConfigArea->setFrameShape(QFrame::NoFrame);
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
	setLayout(workLayout);

}