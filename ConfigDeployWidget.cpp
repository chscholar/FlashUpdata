#include "ConfigDeployWidget.h"
#include <qgroupbox.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QScrollArea>
#include <QPushButton>
#include <QButtonGroup>
#include <FileConfigWidget.h>
#include <QMessageBox>

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
	QRadioButton *downloadRadio = new QRadioButton("����");
	radioLayout->addWidget(downloadRadio);

	radioLayout->addStretch(1);
	QRadioButton *uploadRadio = new QRadioButton("�ϴ�");
	radioLayout->addWidget(uploadRadio);


	//connect(radioGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(slotSwitchRadio(int, bool)));

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
	QPushButton *confirmButton = new QPushButton("ȷ��");
	buttonLayout->addWidget(confirmButton);
	connect(confirmButton, SIGNAL(clicked()), this, SLOT(slotConfirmTrans()));

	buttonLayout->addStretch(3);
	QPushButton *cancleButton = new QPushButton("ȡ��");
	buttonLayout->addWidget(cancleButton);
	buttonLayout->addStretch(1);
	buttonLayout->addSpacing(10);
	mainLayout->addLayout(buttonLayout);

	setLayout(mainLayout);
}


void ConfigDeployWidget::slotConfirmTrans()
{
	QStringList pathList = m_pFileConfigWidget->getAllSelectPath();
	if (pathList.size() <= 0)
	{
		QMessageBox::information(this, "��ѡ���ļ�", "��ѡ���ļ�", QMessageBox::Ok);
		return;
	}
}