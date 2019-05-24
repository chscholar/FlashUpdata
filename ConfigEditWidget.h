#ifndef CONFIGEDITWIDGET_H
#define CONFIGEDITWIDGET_H
#include <QWidget>

class ConfigEditWidget : public QWidget
{
	Q_OBJECT
public:
	ConfigEditWidget(QWidget *parent = 0);
	~ConfigEditWidget();
protected:
	void initUi();
private:
};

#endif
