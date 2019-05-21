#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FlashUpdata.h"
#pragma execution_character_set("utf-8")
class FlashUpdata : public QMainWindow
{
	Q_OBJECT

public:
	FlashUpdata(QWidget *parent = Q_NULLPTR);

private:
	Ui::FlashUpdataClass ui;
	void initUi();
};
