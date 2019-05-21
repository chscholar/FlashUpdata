#ifndef SERIALCONFIG_H
#define  SERIALCONFIG_H
#include <QWidget>
#pragma execution_character_set("utf-8")
class SerialConfigWidget : public QWidget
{
	Q_OBJECT
public:
	SerialConfigWidget(QWidget *parent = 0);
	~SerialConfigWidget();
protected:
	void initUi();
private:
};

#endif
