#include "FlashUpdata.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

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
