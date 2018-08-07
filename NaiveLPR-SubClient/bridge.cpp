#include "bridge.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>

Bridge::Bridge(QObject *parent){

}


void Bridge::showMsgBox()
{
    QMessageBox::aboutQt(0, tr("Qt"));
}
