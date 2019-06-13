#ifndef CONFIDEPLOYWIDGET_H
#define CONFIDEPLOYWIDGET_H
#include <QWidget>
#include "FileConfigWidget.h"

class ConfigDeployWidget : public QWidget
{
	Q_OBJECT
public:
	ConfigDeployWidget(QWidget *parent = 0);
	~ConfigDeployWidget();
	public slots:
	void slotConfirmTrans();
	void slotSwitchRadio(int, bool);
protected:
	void initUi();
private:
	FileConfigWidget *m_pFileConfigWidget;
	bool m_bUpLoadTrans;
};

#endif
