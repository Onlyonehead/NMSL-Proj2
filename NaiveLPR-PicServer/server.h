#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include "socketthread.h"
#include "processor.h"

class Server : public QTcpServer
{
public:
    Server();
private:
    void incomingConnection (qintptr socketDescriptor);
};

#endif // SERVER_H
