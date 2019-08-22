#ifndef CONFIDEPLOYWIDGET_H
#define CONFIDEPLOYWIDGET_H
#include <QWidget>
#include "FileConfigWidget.h"
#include <QButtonGroup>
#include <QByteArray>

class ConfigDeployWidget : public QWidget
{
	Q_OBJECT
public:
	ConfigDeployWidget(QWidget *parent = 0);
	~ConfigDeployWidget();
	QButtonGroup *radioGroup;
	public slots:
	void slotConfirmTrans();
	void slotSwitchRadio(int, bool);
signals:
	void signalUpdateTransType(bool transType, QString upFilePath, QByteArray BinFileId);
protected:
	void initUi();
private:
	FileConfigWidget *m_pFileConfigWidget;
	bool m_bUpLoadTrans;
	QByteArray binFileId;
};

#endif
