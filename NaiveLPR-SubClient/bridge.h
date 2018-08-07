#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class Bridge : public QObject
{
    Q_OBJECT
public:
    explicit Bridge(QObject *parent = 0);

public slots:
    void showMsgBox();

signals:
    void signalToWeb(const QJsonArray &json);


};

#endif // BRIDGE_H
