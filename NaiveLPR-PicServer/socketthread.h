#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include "processor.h"

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(int socketDescriptor,QObject* parent);
    int socketDescriptor;
    QTcpSocket *socket;
    void run();
};

#endif // SOCKETTHREAD_H
