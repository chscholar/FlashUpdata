#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FlashUpdata.h"
#include <FileConfigWidget.h>
#include "SinTabWindow.h"
#pragma execution_character_set("utf-8")

enum RadioId
{
	RADIOID_UPLOAD,
	RADIO_DOWNLOAD,

};
class FlashUpdata : public QMainWindow
{
	Q_OBJECT

public:
	FlashUpdata(QWidget *parent = Q_NULLPTR);
	
private:
	Ui::FlashUpdataClass ui;
	void initUi();

	FileConfigWidget *m_pFileConfigWidget;
	SinTabWindow *m_pTabWindow;
public slots:
	void slotSwitchRadio(int, bool);
};
