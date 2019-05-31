#include "NetWorkConfigWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QNetworkInterface>

NetWorkConfigWidget::NetWorkConfigWidget(QWidget *parent){
	initUi();
}

NetWorkConfigWidget::~NetWorkConfigWidget(){

}

QStringList NetWorkConfigWidget::getNetInfo()
{
	QStringList nameList;;
	
	QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
	foreach(QNetworkInterface netInterface, list) {
	
		nameList.push_back(netInterface.humanReadableName());
	
	}
	return nameList;
}

void NetWorkConfigWidget::initUi()
{
	QStringList nameList =  getNetInfo();

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setMargin(10);

	for (int i = 0; i < nameList.size(); i++)
	{
		QString strIp = nameList.at(i);
		QLabel *lable = new QLabel(strIp);
		mainLayout->addWidget(lable);
	}
	
	mainLayout->addStretch(3);
	setLayout(mainLayout);
}