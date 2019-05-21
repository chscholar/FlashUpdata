#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H
#include <QWidget>
#pragma execution_character_set("utf-8")
class NetWorkConfig : public QWidget
{
	Q_OBJECT
public:
	NetWorkConfig(QWidget *parent = 0);
	~NetWorkConfig();
protected:
	void initUi();
private:
};

#endif
