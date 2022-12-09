#include "CController.h"


#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CController w;
    w.show();
    return a.exec();
}
