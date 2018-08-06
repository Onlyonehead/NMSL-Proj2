#include "processor.h"
#include <QDataStream>
#include <QImage>
#include <QBuffer>

#define DIR "/Users/Haibara/Documents/qt build files2/Serverfiles"

Processor::Processor(QTcpSocket* socket, QObject *parent) :
    QObject(parent)
{
    m_socket = socket;
}

void Processor::work ()
{


    //发送方代码

    /*

    QBuffer buffer;
    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    buffer.open(QIODevice::ReadWrite);
    QImage img;
    img.load("/Users/Haibara/Desktop/a.jpg");
    img.save(&buffer,"JPG");
    out << qint32(buffer.size());
    out << QString("a.jpg");
    out << buffer.data();

    m_socket->write(message);
    m_socket->flush();
    qDebug("sendpic");


    */



    QDataStream in(m_socket);
    in.setVersion (QDataStream::Qt_5_7);

    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    if(dataSize == 0)
    {
       if(m_socket->bytesAvailable() < (sizeof(quint32)+sizeof(QString)) )
       {
            return;
       }
       in >> dataSize;
       in >> fileName;

       qDebug() << fileName;
    }

    if(dataSize == sizeof(QString)){


        QBuffer buffer;
        QByteArray message;
        QDataStream out(&message,QIODevice::WriteOnly);
        QImage img;

        out.setVersion(QDataStream::Qt_5_7);
        buffer.open(QIODevice::ReadWrite);
        img.load(DIR + fileName);
        img.save(&buffer,"JPG");
        out << qint32(buffer.size());
        out << QString(fileName);
        out << buffer.data();

        m_socket->write(message);
        m_socket->flush();
        qDebug() << "Downloaded: " + fileName;


    }else{
        if(dataSize > qint32(m_socket->bytesAvailable()))
        {
           return;
        }

        QByteArray data;

        in>>data;

        QImage img;
        img.loadFromData(data);

        img.save(DIR + fileName);

        qDebug() << "Uploaded: " + fileName;

    }
    dataSize = 0;
    fileName.clear();

}
