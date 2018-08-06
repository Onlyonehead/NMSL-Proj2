#include "login.h"
#include <QApplication>
#include <QtCharts>
using namespace QtCharts;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login *w = new Login;
    w->show();

    return a.exec();
}
