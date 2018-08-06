#include "socketthread.h"

SocketThread::SocketThread(int id,QObject *parent)
    :QThread(parent)
{
    socketDescriptor=id;


}
void SocketThread::run ()
{
    socket=new QTcpSocket;
    qDebug() << "Socket: " << socket->thread();
    Processor *cpu=new Processor(socket);
    connect(socket,SIGNAL(readyRead()),cpu, SLOT(work()));
    socket->setSocketDescriptor(socketDescriptor);
    QThread::run ();

}
