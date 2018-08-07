#include "mainclient.h"
#include "ui_mainclient.h"

void MainClient::on_pushButton_showmap_clicked()
{
    QtMap *qtMap = new QtMap("http://kousz.top/repo/project2/src/camera.html");
    connect(this, SIGNAL(webReturn(QVector<QStringList>)),
            qtMap, SLOT(webInit(QVector<QStringList>)));
    emit webReturn(this->cameras);
    qtMap->exec();
}

