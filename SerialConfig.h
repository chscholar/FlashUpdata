#ifndef SERIALCONFIG_H
#define  SERIALCONFIG_H
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#pragma execution_character_set("utf-8")
class SerialConfigWidget : public QWidget
{
	Q_OBJECT
public:
	SerialConfigWidget(QWidget *parent = 0);
	~SerialConfigWidget();
public slots:
void slotOpenCloseCom();
protected:
	void initUi();
private:
	QComboBox *m_pComBox;
	QComboBox *m_pRateBox;
	QLabel *m_pStatusLabel;
};

#endif
