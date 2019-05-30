#include "FileConfigWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QDomDocument>
#include "SinXml.h"


FileConfigItemWidget::FileConfigItemWidget(bool isUpLoad, int itemId ,QString fileCheck,QString filePath,QWidget *parent)
	:QWidget(parent)
{
	m_iItemId = itemId;
	m_sFileChecked = fileCheck;
	m_sFilePath = filePath;
	m_bisUpLoad = isUpLoad;
	initUi();
}

FileConfigItemWidget::~FileConfigItemWidget()
{

}

void FileConfigItemWidget::initUi()
{
	QHBoxLayout *mainLayout = new QHBoxLayout();
	//mainLayout->addSpacing(20);

	m_pFileCheckbox = new QCheckBox();
	if (m_sFileChecked == "check")
	{
		m_pFileCheckbox->setChecked(true);
	}
	else {
		m_pFileCheckbox->setChecked(false);
	}
	
	mainLayout->addWidget(m_pFileCheckbox);
	m_pAddressLabel = new QLabel("上传文件路径：");
	mainLayout->addWidget(m_pAddressLabel);

	//mainLayout->addSpacing(10);
	m_pAddressEdit = new QLineEdit();
	m_pAddressEdit->setText(m_sFilePath);
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
	connect(m_pFileCheckbox, SIGNAL(stateChanged(int)), this, SLOT(slotChecboxStateChange(int)));;
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
	if (!path.isEmpty())
	{
		return path;
	}
	return "";
}

void FileConfigItemWidget::slotAdd()
{
	sinXmlSingle::getInstance().addUpLoadFile(m_bisUpLoad, QString::number(++m_iItemId), getCheckedStatus(), "");
}

void FileConfigItemWidget::slotDel()
{
	sinXmlSingle::getInstance().deleUpLoadFile(m_bisUpLoad, QString::number(m_iItemId));
}

void FileConfigItemWidget::slotBrowFile()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open File"), "./", tr("Text File(*.*)"));
	if (!path.isEmpty())
	{
		m_pAddressEdit->setText(path);
		sinXmlSingle::getInstance().updateUpLoadFile(m_bisUpLoad, QString::number(m_iItemId), getCheckedStatus(), path);
	}
}

void FileConfigItemWidget::slotChecboxStateChange(int state)
{
	QString qstrFileChecked;
	if (state == Qt::Checked)
	{
		qstrFileChecked = "check";
	} else if (state == Qt::Unchecked)
	{
		qstrFileChecked = "uncheck";
	}
	QString qstrfilePath = getFilePath();
	sinXmlSingle::getInstance().updateUpLoadFile(true, QString::number(m_iItemId), qstrFileChecked, qstrfilePath);
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

void FileConfigItemWidget::setDownLoadStatus()
{
	delButton->hide();
	addButton->hide();
	m_pAddressLabel->setText("保存文件路径:");
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
	m_bUpLoadChecked = true;
	connect(&sinXmlSingle::getInstance(), SIGNAL(signalsConfigChange()), this, SLOT(slotConfigFileChange()));
	initUi();
}

FileConfigWidget::~FileConfigWidget()
{

}

void FileConfigWidget::initUi()
{
		mainUpLoadLayout = new QVBoxLayout(this);
		setLayout(mainUpLoadLayout);
}

void FileConfigWidget::switchWidget(bool isUpLoad)
{
	m_bUpLoadChecked = isUpLoad;
	flushWidget();
}

void FileConfigWidget::fillItemVecFromConfig(bool isUpLoad)
{
	fileConfigVec.clear();
	m_iItemId = 0;
	QList<FileConfigItem> itemList = sinXmlSingle::getInstance().getFileConfigItemFromXmlConfig(isUpLoad);
	for (int i = 0; i < itemList.size();i++)
	{
		QString qstrId = itemList.at(i).fileConfigId;
		QString qstrChecked = itemList.at(i).fileConfigChecked;
		QString qstrPath = itemList.at(i).fileConfigPath;

		if (m_iItemId <= qstrId.toInt())
		{
			m_iItemId = qstrId.toInt();
		}

		FileConfigItemWidget *itemWidget = new FileConfigItemWidget(isUpLoad,qstrId.toInt(), qstrChecked, qstrPath);
		fileConfigVec.push_back(itemWidget);
	}

}

void FileConfigWidget::flushWidget()
{
	bool isUpLoad = m_bUpLoadChecked;;
	delAllWidgetFromLayout();
	fileConfigVec.clear();
	fillItemVecFromConfig(isUpLoad);
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

		if (!isUpLoad)
		{
			item->setDownLoadStatus();
		}
		mainUpLoadLayout->addWidget(fileConfigVec[i]);
	}
	mainUpLoadLayout->addStretch(5);
}

void FileConfigWidget::delAllWidgetFromLayout()
{

	for (int i = 0; i < fileConfigVec.size(); i++)
	{
		FileConfigItemWidget *item = fileConfigVec[i];
		item->deleteLater();
		mainUpLoadLayout->removeWidget(item);
		
	}

	for (int i = 0; i < mainUpLoadLayout->count();i++)
	{
		QLayoutItem *item = mainUpLoadLayout->itemAt(i);
		item->widget()->deleteLater();
		mainUpLoadLayout->removeItem(item);
	}
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

void FileConfigWidget::showEvent(QShowEvent *e)
{
	flushWidget();
	this->update();
}

void FileConfigWidget::slotConfigFileChange()
{
	QShowEvent *e = new QShowEvent();
	showEvent(e);
}