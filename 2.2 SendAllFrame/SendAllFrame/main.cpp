#include "SendAllFrame.h"


#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SendAllFrame w;
    w.show();
    return a.exec();
}
