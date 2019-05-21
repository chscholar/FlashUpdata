#include "NetWorkConfig.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

NetWorkConfig::NetWorkConfig(QWidget *parent){
	initUi();
}

NetWorkConfig::~NetWorkConfig(){

}

void NetWorkConfig::initUi(){

	QVBoxLayout *mainLayout = new QVBoxLayout();

	QLabel *test = new QLabel();
	test->setText("network");
	mainLayout->addWidget(test);

	mainLayout->addStretch(3);
	setLayout(mainLayout);
}