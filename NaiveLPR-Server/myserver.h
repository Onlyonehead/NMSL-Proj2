#ifndef MYSERVER_H
#define MYSERVER_H
#include <QtNetwork>
#include "socketthread.h"
#include "processor.h"

class MyServer : public QTcpServer
{
public:
    MyServer();

private:
    void incomingConnection(qintptr socketDescriptor);
};

#endif // MYSERVER_H
