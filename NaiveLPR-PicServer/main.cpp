#include "picserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PicServer w;

    return a.exec();
}
