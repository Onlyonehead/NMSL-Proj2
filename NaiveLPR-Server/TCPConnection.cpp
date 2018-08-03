#include "TCPConnection.h"

TCPConnection::TCPConnection(QWidget *parent)
    : QWidget(parent)
{
    server = new MyServer;
    server->listen(QHostAddress::Any,8848);//监听的端口号
}


TCPConnection::~TCPConnection(){
    server->deleteLater();
}
