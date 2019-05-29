#include "FileConfigWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include "SinXml.h"


FileConfigItemWidget::FileConfigItemWidget(int itemId,QWidget *parent)
	:QWidget(parent)
{
	initUi();

	m_iItemId = itemId;
}

FileConfigItemWidget::~FileConfigItemWidget()
{

}

void FileConfigItemWidget::initUi()
{
	QHBoxLayout *mainLayout = new QHBoxLayout();
	//mainLayout->addSpacing(20);

	m_pFileCheckbox = new QCheckBox();
	m_pFileCheckbox->setChecked(true);
	mainLayout->addWidget(m_pFileCheckbox);


	m_pAddressLabel = new QLabel("上传文件路径：");
	mainLayout->addWidget(m_pAddressLabel);

	//mainLayout->addSpacing(10);
	m_pAddressEdit = new QLineEdit();
	mainLayout->addWidget(m_pAddressEdit);

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
	mainLayout->setStretch(1, 1);
	mainLayout->setStretch(2, 10);
	mainLayout->setStretch(3, 1);
	mainLayout->setStretch(4, 1);
	mainLayout->setStretch(5, 1);
	
	mainLayout->addStretch(5);
	setLayout(mainLayout);

	connect(addButton, SIGNAL(clicked()), this, SLOT(slotAdd()));
	connect(delButton, SIGNAL(clicked()), this, SLOT(slotDel()));
	connect(browButton, SIGNAL(clicked()), this, SLOT(slotBrowFile()));
}

QString FileConfigItemWidget::getCheckedStatus()
{
	bool checked = m_pFileCheckbox->isChecked();

	if (checked)
	{
		return "check";
	}

	return "uncheck";
}

QString FileConfigItemWidget::getFilePath(){
	QString path = m_pAddressEdit->text();
	if (m_pFileCheckbox->isChecked() && !path.isEmpty())
	{
		return path;
	}
	return "";
}

void FileConfigItemWidget::slotAdd()
{
	emit signalAddFileConfig();
	sinXmlSingle::getInstance().addUpLoadFile(true, QString::number(m_iItemId), getCheckedStatus(), "");
}

void FileConfigItemWidget::slotDel()
{
	emit signalDelFileConfig(m_iItemId);
	sinXmlSingle::getInstance().deleUpLoadFile(true, QString::number(m_iItemId));
}

void FileConfigItemWidget::slotBrowFile()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "./", tr("Text File(*.*)"));
	if (!path.isEmpty())
	{
		m_pAddressEdit->setText(path);
		sinXmlSingle::getInstance().updateUpLoadFile(true, QString::number(m_iItemId), getCheckedStatus(), path);
	}
}

int FileConfigItemWidget::findItemById(int itemId) {
	return m_iItemId;
}

void FileConfigItemWidget::setFirst()
{
	addButton->show();
	delButton->hide();
}

void FileConfigItemWidget::setEnd()
{
	addButton->show();
	delButton->show();
}

void FileConfigItemWidget::setNiddle()
{
	addButton->hide();
	delButton->show();
}

void FileConfigItemWidget::setDownload()
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

		FileConfigItemWidget *item = new FileConfigItemWidget(++m_iItemId);
		item->setFirst();
		connect(item, SIGNAL(signalAddFileConfig()), this, SLOT(slotAddFileConfig()));
		connect(item, SIGNAL(signalDelFileConfig(int)), this, SLOT(slotDelFileConfig(int)));
		mainUpLoadLayout->addWidget(item);
		mainUpLoadLayout->addStretch(5);

		fileConfigVec.push_back(item);
		setLayout(mainUpLoadLayout);
	
		mainDownLoadLayout = new QVBoxLayout(this);
		mainDownLoadLayout->setMargin(10);
		FileConfigItemWidget *itemdown = new FileConfigItemWidget(++m_iItemId);
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
		FileConfigItemWidget *item = fileConfigVec[i];
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
	mainUpLoadLayout->addStretch(5);
}

void FileConfigWidget::slotAddFileConfig()
{
	FileConfigItemWidget *item = new FileConfigItemWidget(++m_iItemId);
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
		FileConfigItemWidget *item = fileConfigVec[i];
		mainUpLoadLayout->removeWidget(item);
		
	}

	for (int i = 0; i < mainUpLoadLayout->count();i++)
	{
		QLayoutItem *item = mainUpLoadLayout->itemAt(i);
		mainUpLoadLayout->removeItem(item);
	}
}


void FileConfigWidget::slotDelFileConfig(int itemId)
{
	for (int i = 0; i < fileConfigVec.size(); i++)
	{
		FileConfigItemWidget *item = fileConfigVec[i];
		if (item->findItemById(itemId) == itemId)
		{
			item->deleteLater();
			fileConfigVec.removeAt(i);
		}

	}
	flushWidget();
	
}

QStringList FileConfigWidget::getAllSelectPath()
{
	QStringList pathList;
	for (int i = 0; i < fileConfigVec.size();i++)
	{
		QString path =  fileConfigVec.at(i)->getFilePath();
		if (!path.isEmpty())
		{
			pathList.push_back(path);
		}
	}

	return pathList;
}