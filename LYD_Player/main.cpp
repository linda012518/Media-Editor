#include "LYD_Player.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LYD_Player w;
	//w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
