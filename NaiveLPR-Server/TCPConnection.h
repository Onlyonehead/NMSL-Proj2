#ifndef STATUS_H
#define STATUS_H

#include <QWidget>
#include <QtNetwork>
#include "sqltool.h"
#include "myserver.h"

class MyServer;

class TCPConnection : public QWidget
{
    Q_OBJECT

public:
    TCPConnection(QWidget *parent = 0);

    ~TCPConnection();

private:
    MyServer *server;

private slots:

};

#endif // STATUS_H
