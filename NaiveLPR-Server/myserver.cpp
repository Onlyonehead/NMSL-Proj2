#include "myserver.h"

MyServer::MyServer()
{

}

/**
 * override
 *
 * @author hzc
 * @param socketDescriptor
 */
void MyServer::incomingConnection(qintptr socketDescriptor)
{
    SocketThread *thread = new SocketThread(socketDescriptor,this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
