#include "ConfigDeployWidget.h"
#include <qgroupbox.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QScrollArea>
#include <QPushButton>
#include <QButtonGroup>
#include "FileConfigWidget.h"
#include <QMessageBox>
#include "FileUtil.h"
#include "SinSerial.h"
#include "SinSerialThreadManager.h"
#include <QFile>

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
	QVBoxLayout *mainLayout = new QVBoxLayout();

	QButtonGroup *radioGroup = new QButtonGroup();
	QHBoxLayout *radioLayout = new QHBoxLayout();
	radioLayout->addStretch(1);

	QRadioButton *downloadRadio = new QRadioButton("下载");
	radioLayout->addWidget(downloadRadio);
	radioLayout->addStretch(1);

	QRadioButton *uploadRadio = new QRadioButton("上传");
	radioLayout->addWidget(uploadRadio);

	connect(radioGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(slotSwitchRadio(int, bool)));

	downloadRadio->setChecked(true);
	radioGroup->addButton(uploadRadio);
	radioGroup->setId(uploadRadio, 0);
	radioGroup->addButton(downloadRadio);
	radioGroup->setId(downloadRadio, 1);
	radioLayout->addStretch(1);
	mainLayout->addLayout(radioLayout);

	QScrollArea *fileConfigArea = new QScrollArea();
	m_pFileConfigWidget = new FileConfigWidget();
	fileConfigArea->setWidget(m_pFileConfigWidget);
	fileConfigArea->setWidgetResizable(true);
	fileConfigArea->setFrameShape(QFrame::NoFrame);
	mainLayout->addWidget(fileConfigArea);

	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addSpacing(10);
	buttonLayout->addStretch(1);
	QPushButton *confirmButton = new QPushButton("确认");
	buttonLayout->addWidget(confirmButton);
	connect(confirmButton, SIGNAL(clicked()), this, SLOT(slotConfirmTrans()));
	buttonLayout->addStretch(1);
	buttonLayout->addSpacing(10);
	mainLayout->addLayout(buttonLayout);
	setLayout(mainLayout);
}

void ConfigDeployWidget::slotSwitchRadio(int index, bool isChecked)
{
	if (isChecked)
	{
		if (index == 0) //upload
		{
			m_pFileConfigWidget->switchWidget(true);
			m_bUpLoadTrans = true;
		}
		else {
			m_pFileConfigWidget->switchWidget(false);
			m_bUpLoadTrans = false;
		}
	}

}


void ConfigDeployWidget::slotConfirmTrans()
{
	QStringList pathList = m_pFileConfigWidget->getAllSelectPath();
	

	if (pathList.size() <= 0)
	{
		QMessageBox::information(this, "请选择文件", "请选择文件", QMessageBox::Ok);
		return;
	};

	for (int i = 0; i < pathList.size(); i++)
	{
		QString strPath = pathList.at(i);
		QFile file(strPath);
		if (!file.exists())
		{
			QMessageBox::information(this, "请选择文件", "文件不存在 请重新选择你这", QMessageBox::Ok);
			return;
		}
		
	}

	FileUtil *futil = new FileUtil();
	QList<QList<QByteArray>> fileListData =  futil->getDataFramFromFilePath(pathList);
	sinserialSingle::getInstance().setTransTypeWriteData(m_bUpLoadTrans,fileListData);
	//sinSerialThreadManagerSingle::getInstance().start();

}