#include "socketthread.h"

SocketThread::SocketThread(int id,QObject *parent):QThread()
{
    Q_UNUSED(parent);
    socketDescriptor=id;
}
void SocketThread::run ()
{
    qDebug() << "Thread" << QThread::currentThread() << endl;
    socket=new QTcpSocket;
    Processor *cpu=new Processor(socket);
    connect(socket,SIGNAL(readyRead()),cpu,SLOT(work()));
    socket->setSocketDescriptor(socketDescriptor);
    QThread::run ();
}
