#include "picserver.h"

PicServer::PicServer(QWidget *parent)
    : QWidget(parent)
{
    Server *s = new Server;
    s->listen(QHostAddress::Any,7777);//监听的端口号
}

PicServer::~PicServer()
{

}
