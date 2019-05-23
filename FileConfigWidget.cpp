#include "FileConfigWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

FileConfigItem::FileConfigItem(int itemId,QWidget *parent)
	:QWidget(parent)
{
	initUi();
	m_iItemId = itemId;
}

FileConfigItem::~FileConfigItem()
{

}

void FileConfigItem::initUi()
{
	QHBoxLayout *mainLayout = new QHBoxLayout();
	//mainLayout->addSpacing(20);

	m_pAddressLabel = new QLabel("上传文件路径：");
	mainLayout->addWidget(m_pAddressLabel);

	//mainLayout->addSpacing(10);
	QLineEdit *addressEdit = new QLineEdit();
	mainLayout->addWidget(addressEdit);

	//mainLayout->addSpacing(10);
	QPushButton *browButton = new QPushButton("浏览...");
	mainLayout->addWidget(browButton);
	
	//mainLayout->addSpacing(10);

	addButton = new QPushButton();
	addButton->setStyleSheet("QPushButton{background-image: url(./images/file_add.png)}");
	addButton->setFixedSize(QSize(20, 20));
	mainLayout->addWidget(addButton);

	delButton = new QPushButton();
	delButton->setStyleSheet("QPushButton{background-image: url(./images/file_del.png)}");
	delButton->setFixedSize(QSize(20, 20));
	mainLayout->addWidget(delButton);

	//mainLayout->addSpacing(10);
	mainLayout->setStretch(0, 1);
	mainLayout->setStretch(1, 10);
	mainLayout->setStretch(2, 1);
	mainLayout->setStretch(3, 1);
	mainLayout->setStretch(4, 1);
	
	setLayout(mainLayout);

	connect(addButton, SIGNAL(clicked()), this, SIGNAL(signalAddFileConfig()));
	connect(delButton, SIGNAL(clicked()), this, SLOT(slotDel()));

}

void FileConfigItem::slotDel()
{
	emit signalDelFileConfig(m_iItemId);
}

int FileConfigItem::findItemById(int itemId) {
	return m_iItemId;
}

void FileConfigItem::setFirst()
{
	addButton->show();
	delButton->hide();
}

void FileConfigItem::setEnd()
{
	addButton->show();
	delButton->show();
}

void FileConfigItem::setNiddle()
{
	addButton->hide();
	delButton->show();
}

void FileConfigItem::setDownload()
{
	addButton->hide();
	m_pAddressLabel->setText("保存文件路径：");
	delButton->hide();
}

FileConfigWidget::FileConfigWidget(QWidget *parent) :
QWidget(parent)
{
	m_iItemId = 0;
	initUi();
}

FileConfigWidget::~FileConfigWidget()
{

}

void FileConfigWidget::initUi()
{

	
	
		mainUpLoadLayout = new QVBoxLayout(this);
		mainUpLoadLayout->setMargin(10);
		FileConfigItem *item = new FileConfigItem(++m_iItemId);
		item->setFirst();
		connect(item, SIGNAL(signalAddFileConfig()), this, SLOT(slotAddFileConfig()));
		connect(item, SIGNAL(signalDelFileConfig(int)), this, SLOT(slotDelFileConfig(int)));
		mainUpLoadLayout->addWidget(item);

		fileConfigVec.push_back(item);
		setLayout(mainUpLoadLayout);
	
		mainDownLoadLayout = new QVBoxLayout(this);
		mainDownLoadLayout->setMargin(10);
		FileConfigItem *itemdown = new FileConfigItem(++m_iItemId);
		itemdown->setDownload();
		mainDownLoadLayout->addWidget(itemdown);
		//setLayout(mainDownLoadLayout);
	
}

void FileConfigWidget::switchWidget(bool isUpLoad)
{
	if (isUpLoad)
	{
		this->setLayout(mainUpLoadLayout);
	}
	else {
		this->setLayout(mainDownLoadLayout);
	}
}

void FileConfigWidget::flushWidget()
{
	delAllWidgetFromLayout();
	for (int i = 0; i < fileConfigVec.size();i++)
	{
		FileConfigItem *item = fileConfigVec[i];
		if (i == 0)
		{
			if (fileConfigVec.size() > 1)
			{
				item->setNiddle();
			}
			else {
				item->setFirst();
			}
		}
		else if (i == fileConfigVec.size() - 1){
			item->setEnd();
		}
		else {
			item->setNiddle();
		}

		mainUpLoadLayout->addWidget(fileConfigVec[i]);
	}
}

void FileConfigWidget::slotAddFileConfig()
{
	FileConfigItem *item = new FileConfigItem(++m_iItemId);
	item->setEnd();
	connect(item, SIGNAL(signalAddFileConfig()), this, SLOT(slotAddFileConfig()));
	connect(item, SIGNAL(signalDelFileConfig(int)), this, SLOT(slotDelFileConfig(int)));

	fileConfigVec.push_back(item);
	flushWidget();
}

void FileConfigWidget::delAllWidgetFromLayout()
{

	for (int i = 0; i < fileConfigVec.size(); i++)
	{
		FileConfigItem *item = fileConfigVec[i];
		mainUpLoadLayout->removeWidget(item);
		
	}
}


void FileConfigWidget::slotDelFileConfig(int itemId)
{
	for (int i = 0; i < fileConfigVec.size(); i++)
	{
		FileConfigItem *item = fileConfigVec[i];
		if (item->findItemById(itemId) == itemId)
		{
			item->deleteLater();
			fileConfigVec.removeAt(i);
		}

	}
	flushWidget();
	
}