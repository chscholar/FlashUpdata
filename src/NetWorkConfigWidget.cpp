#include "NetWorkConfigWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

NetWorkConfigWidget::NetWorkConfigWidget(QWidget *parent){
	initUi();
}

NetWorkConfigWidget::~NetWorkConfigWidget(){

}

void NetWorkConfigWidget::initUi(){

	QVBoxLayout *mainLayout = new QVBoxLayout();

	QLabel *test = new QLabel();
	test->setText("network");
	mainLayout->addWidget(test);

	mainLayout->addStretch(3);
	setLayout(mainLayout);
}