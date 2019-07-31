#include "FlashUpdata.h"
#include <QtWidgets/QApplication>
#include <QDir>
#include "QdebugToLog.h"

int main(int argc, char *argv[])
{
	//UpData::useTxtLog();
	QApplication a(argc, argv);

	QString dir = QApplication::applicationDirPath();
	QDir::setCurrent(dir);
	//QApplication::addLibraryPath("./plugins");
	QApplication::addLibraryPath("./images");
	a.setWindowIcon(QIcon("./images/logo.ico"));

	int windowXPos, windowYPos, windowWidth, windowHeight;
	windowXPos = 100;
	windowYPos = 100;
	windowWidth = 1024;
	windowHeight = 768;

	FlashUpdata w;
	w.setGeometry(windowXPos, windowYPos, windowWidth, windowHeight);
	w.show();

	return a.exec();
}
