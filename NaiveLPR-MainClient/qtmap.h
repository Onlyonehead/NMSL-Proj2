#ifndef QTMAP_H
#define QTMAP_H

#include <QWidget>
#include "bridge.h"
#include "mainclient.h"

namespace Ui {
class QtMap;
}

class QtMap : public QDialog
{
    Q_OBJECT

public:
    QtMap(QString surl);
    ~QtMap();

private slots:
    void slotSendDataToH5(bool ok);
    void webInit(QVector<QStringList> v);

    void on_pushButton_clicked();

private:
    Ui::QtMap *ui;
    Bridge bridge;
    QVector<QStringList> camera;
};

#endif // QTMAP_H
