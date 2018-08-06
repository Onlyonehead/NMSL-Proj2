#ifndef PICSERVER_H
#define PICSERVER_H

#include <QWidget>
#include "server.h"

class PicServer : public QWidget
{
    Q_OBJECT

public:
    PicServer(QWidget *parent = 0);
    ~PicServer();
};

#endif // PICSERVER_H
