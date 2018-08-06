#include "server.h"

Server::Server()
{

}
void Server::incomingConnection (qintptr socketDescriptor)
{
    SocketThread *thread = new SocketThread(socketDescriptor,this);

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

}
