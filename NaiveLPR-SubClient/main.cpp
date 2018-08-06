#include "subclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SubClient *w = new SubClient;
    w->show();

    return a.exec();
}
