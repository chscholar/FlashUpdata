#include "FileConfigWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

FileConfigItem::FileConfigItem(bool isFrist, bool isEnd,QWidget *parent)
	:QWidget(parent)
{
	initUi(isFrist,isEnd);
}

FileConfigItem::~FileConfigItem()
{

}

void FileConfigItem::initUi(bool isFrist,bool isEnd)
{
	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addStretch(1);
	mainLayout->addSpacing(20);

	QLabel *addressLabel = new QLabel("ÎÄ¼þÂ·¾¶£º");
	mainLayout->addWidget(addressLabel);

	mainLayout->addSpacing(10);
	QLineEdit *addressEdit = new QLineEdit();
	mainLayout->addWidget(addressEdit);

	mainLayout->addSpacing(10);
	QPushButton *browButton = new QPushButton("ä¯ÀÀ...");
	mainLayout->addWidget(browButton);
	
	mainLayout->addSpacing(10);

	QPushButton *addButton = new QPushButton();
	addButton->setStyleSheet("QPushButton{background-image: url(./images/file_add.png)}");
	addButton->setFixedSize(QSize(20, 20));
	mainLayout->addWidget(addButton);

	QPushButton *minButton = new QPushButton();
	minButton->setStyleSheet("QPushButton{background-image: url(./images/file_del.png)}");
	minButton->setFixedSize(QSize(20, 20));
	mainLayout->addWidget(minButton);

	//if (isFrist)
	//{
	//	minButton->hide();
	//}
	//
	//if (isEnd)
	//{
	//	addButton->show();
	//}

	mainLayout->addSpacing(10);


	mainLayout->addStretch(1);
	mainLayout->setStretch(0, 1);
	mainLayout->setStretch(1, 8);
	mainLayout->setStretch(2, 1);
	setLayout(mainLayout);

	connect(addButton, SIGNAL(clicked()), this, SIGNAL(signalAddFileConfig()));
	connect(minButton, SIGNAL(clicked()), this, SIGNAL(signalDelFileConfig()));

}


FileConfigWidget::FileConfigWidget(QWidget *parent) :
QWidget(parent)
{
	initUi();
}

FileConfigWidget::~FileConfigWidget()
{

}

void FileConfigWidget::initUi()
{

	mainLayout = new QVBoxLayout(this);
	
	FileConfigItem *item = new FileConfigItem(true);
	connect(item, SIGNAL(signalAddFileConfig()), this, SLOT(slotAddFileConfig()));
	connect(item, SIGNAL(signalDelFileConfig()), this, SLOT(slotDelFileConfig()));
	mainLayout->addWidget(item);

	fileConfigVec.push_back(item);
	setLayout(mainLayout);
}

void FileConfigWidget::flushWidget()
{
	delAllWidgetFromLayout();
	for (int i = 0; i < fileConfigVec.size();i++)
	{
		mainLayout->addWidget(fileConfigVec[i]);
	}
}

void FileConfigWidget::slotAddFileConfig()
{
	FileConfigItem *item = new FileConfigItem();
	connect(item, SIGNAL(signalAddFileConfig()), this, SLOT(slotAddFileConfig()));
	connect(item, SIGNAL(signalDelFileConfig()), this, SLOT(slotDelFileConfig()));

	fileConfigVec.push_back(item);
	flushWidget();
}

void FileConfigWidget::delAllWidgetFromLayout()
{

	for (int i = 0; i < fileConfigVec.size(); i++)
	{
		FileConfigItem *item = fileConfigVec[i];
		mainLayout->removeWidget(item);
		//item->deleteLater();
	}
}


void FileConfigWidget::slotDelFileConfig()
{
}