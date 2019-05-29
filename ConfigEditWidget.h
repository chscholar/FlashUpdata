#ifndef CONFIGEDITWIDGET_H
#define CONFIGEDITWIDGET_H
#include <QWidget>
#include <QPaintEvent>
#include <QTextEdit>
#include <QShowEvent>

#pragma execution_character_set("utf-8")
class ConfigEditWidget : public QWidget
{
	Q_OBJECT
public:
	ConfigEditWidget(QWidget *parent = 0);
	~ConfigEditWidget();
protected:
	void initUi();
	void updateEditContent();
	void showEvent(QShowEvent *e);
	QTextEdit *m_pConfigEdit;
	public slots:
	void slotUpdateEdit();
	void slotSaveEditContent();
private:
};

#endif
