#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_FlashUpdata.h"
#include "FileConfigWidget.h"
#include "SinTabWindow.h"
#include <QTextEdit>
#include <QProgressBar>

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
	QTextEdit *m_pLogEdit;
	QProgressBar *m_pProgressBar;
public slots:
	void slotSwitchRadio(int, bool);
	void slotUpDataLogEdit();
	void slotUpdataProgressBar(int value);
};
