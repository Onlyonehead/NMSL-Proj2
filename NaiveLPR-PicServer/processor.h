#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include "socketthread.h"

class Processor : public QObject
{
    Q_OBJECT
public:
    explicit Processor(QTcpSocket*, QObject *parent = 0);

signals:

public slots:
    void work();

private:
    QTcpSocket *m_socket;
    qint32 dataSize = 0;
    QString fileName;

};

#endif // PROCESSOR_H
