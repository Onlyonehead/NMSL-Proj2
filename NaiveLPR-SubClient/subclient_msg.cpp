#include "subclient.h"
#include "ui_subclient.h"

void SubClient::sendMessage(QStringList list)
{

    if(!(m_tcpsocket->state() == QAbstractSocket::ConnectedState)){
        qDebug() << "Reconnecting......";
        m_tcpsocket = new QTcpSocket(this);
        m_tcpsocket->abort();
        m_tcpsocket->connectToHost(QHostAddress::LocalHost,8848);//设置客户端的端口号
        connect(m_tcpsocket,SIGNAL(readyRead()),
                this,SLOT(readMessage()));//用于接受数据
        m_tcpsocket->waitForConnected();
    }

    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << (quint16) 0;
    out << list;
    out.device()->seek(0);
    out << (quint16) (message.size() - sizeof(quint16));
    m_tcpsocket->write(message);
    m_tcpsocket->flush();
}

void SubClient::readMessage()
{
    QDataStream in(m_tcpsocket);
    in.setVersion(QDataStream::Qt_5_7);
    quint16 blocksize = 0;
    if (m_tcpsocket->bytesAvailable() < (int)sizeof(quint16)){
        return;

    }
    in >> blocksize;

    if(m_tcpsocket->bytesAvailable() < blocksize){
        return;
    }
    QString from;
    in >> from;
    qDebug() << from << endl;

    if(from == "login"){

        int status;
        in >> status;


        QString username = ui->lineEdit_4->text().trimmed();
        QString password = (ui->lineEdit_3->text() == "******") ?
                    ui->label_pass->text() : ui->lineEdit_3->text();
        bool isChecked = ui->checkBox_2->isChecked();

        if(status == 0){
            status = checkpass(username, password, isChecked);
        }

        switch(status){
            case -1:
                QMessageBox::warning(this,"警告", "\n保存密码失败！",QMessageBox::Close);
                break;
            case -2:
                QMessageBox::warning(this,"警告", "\n请输入用户名！",QMessageBox::Close);
                break;
            case -3:
                QMessageBox::warning(this,"警告", "\n请输入密码！",QMessageBox::Close);
                break;
            case -4:
                QMessageBox::warning(this,"警告", "\n请稍后再试。", QMessageBox::Close);
                break;
            case -5:
                QMessageBox::warning(this,"警告", "\n密码错误！\n剩余次数：0 次。\n请5分钟后再试。",QMessageBox::Close);
                break;
            case -6:
                QMessageBox::warning(this,"警告", "\n密码错误！\n剩余次数：1 次。",QMessageBox::Close);
                break;
            case -7:
                QMessageBox::warning(this,"警告", "\n密码错误！\n剩余次数：2 次。",QMessageBox::Close);
                break;
            case -8:
                QMessageBox::warning(this,"警告", "\n找不到该用户！",QMessageBox::Close);
                break;
            case 0:
                sendSignalForLogin(username);
                break;
        }
    }
    if(from == "info"){

        QStringList list;
        in >> list;

        qDebug() << list.at(2);

        if(list.at(2) == "用户"){

            QApplication::processEvents();
            QApplication::processEvents();
            for(int i = 550; i >= 280; i-=5){
                QApplication::processEvents();
                move(i,130);
                QApplication::processEvents();
            }

            for(int i = 280; i >= 260; i-=2){
                QApplication::processEvents();
                move(i,130);
                QApplication::processEvents();
            }

            for(int i = 260; i >= 250; i-=1){
                QApplication::processEvents();
                move(i,130);
                QApplication::processEvents();
            }

            ui->stackedWidget->setCurrentIndex(0);
            ui->stackedWidget_3->setCurrentIndex(0);

            ui->label_2->setText(list.at(0));
            ui->label_4->setText(list.at(1));
            ui->label_8->setText(list.at(2));
            ui->label_12->setText(list.at(3));
            ui->label_14->setText(list.at(4));

            ui->frame->setStyleSheet("QFrame{background: rgba(255, 255, 255, 100);"
                                     "border: 2px solid rgb(160, 160, 160);}");

            ui->frame_2->setStyleSheet("QFrame{"
                                       "background: rgba(248, 248, 248, 220);"
                                       "border-image:none;"
                                       "border: 2px solid rgb(220, 220, 220);}");


            QPixmap *pixmap = new QPixmap(DIR + QString("/users/") + list.at(5));
            if (pixmap->isNull()){
                download("/users/" + list.at(5), DIR + QString("/users/") + list.at(5));

                QElapsedTimer t;
                t.start();
                while(t.elapsed()<500)
                    QCoreApplication::processEvents();

                pixmap = new QPixmap(DIR + QString("/users/") + list.at(5));
            }

            if(pixmap->isNull()){
                pixmap = new QPixmap(":/user.png");
            }

            QApplication::processEvents();

            getRoundPixmap(pixmap, QSize(140, 140));
            ui->label_5->setScaledContents(true);
            ui->label_5->setPixmap(*pixmap);
            delete pixmap;

            ui->stackedWidget_2->setVisible(true);
            ui->stackedWidget_2->setCurrentIndex(0);


            QApplication::processEvents();
            progressBar();
        }
        else if(list.at(2) == "摄像头")
        {
            QApplication::processEvents();
            QApplication::processEvents();
            for(int i = 550; i >= 280; i-=5){
                QApplication::processEvents();
                move(i,130);
                QApplication::processEvents();
            }

            for(int i = 280; i >= 260; i-=2){
                QApplication::processEvents();
                move(i,130);
                QApplication::processEvents();
            }

            for(int i = 260; i >= 250; i-=1){
                QApplication::processEvents();
                move(i,130);
                QApplication::processEvents();
            }

            ui->stackedWidget->setCurrentIndex(1);


            ui->label_2->setText(list.at(0));
            ui->label_4->setText(list.at(1));
            ui->label_8->setText(list.at(2));
            ui->label_12->setText(list.at(3));
            ui->label_14->setText(list.at(4));

            ui->frame->setStyleSheet("QFrame{background: rgba(255, 255, 255, 100);"
                                     "border: 2px solid rgb(160, 160, 160);}");

            ui->frame_2->setStyleSheet("QFrame{"
                                       "background: rgba(248, 248, 248, 220);"
                                       "border-image:none;"
                                       "border: 2px solid rgb(220, 220, 220);}");


            QPixmap *pixmap = new QPixmap(DIR + QString("/users/") + list.at(5));
            if (pixmap->isNull()){
                download("/users/" + list.at(5), DIR + QString("/users/") + list.at(5));
            }

            if(pixmap->isNull()){
                pixmap = new QPixmap(":/user.png");
            }

            QApplication::processEvents();

            getRoundPixmap(pixmap, QSize(140, 140));
            ui->label_5->setScaledContents(true);
            ui->label_5->setPixmap(*pixmap);
            delete pixmap;

            ui->stackedWidget_2->setVisible(true);
            ui->stackedWidget_2->setCurrentIndex(0);

            QApplication::processEvents();

            progressBar();
        }
        else
        {
            QMessageBox::warning(this,"警告", "\n非用户账号",QMessageBox::Close);
            return;
        }

    }

    if(from == "LPR"){
        QString plateName;
        in >> plateName;

        if(plateName == "FAIL"){
            QMessageBox::warning(this,"警告", "\n未检测到车牌",QMessageBox::Close);
            on_pushButton_confirmNewStaff_2_clicked();
            return ;
        }else{
            QMessageBox::information(this,"提示", "\n上传成功！",QMessageBox::Ok);
        }

        QString plate;
        in >> plate;

        double rate;
        in >> rate;

        ui->label_showNumber->setText(plate);
        ui->label_showProbability->setText(QString::number(rate));

        download("/plates-d/" + plateName, "/plates-d/" + plateName);

        QElapsedTimer t;
        t.start();
        while(t.elapsed()<500)
            QCoreApplication::processEvents();

        QImage tempPortrait(DIR + QString("/plates-d/") + plateName);
        QPixmap portrait = QPixmap::fromImage(tempPortrait.scaled(122, 42, Qt::KeepAspectRatio,
                                                                  Qt::SmoothTransformation));
        ui->label_showPlate->setPixmap(portrait);
        ui->label_showPlate->show();
        progressBar();
    }

    if(from == "camera_search"){
        QSet<QString> set;
        in >> set;

        QVector<QStringList> vlist;
        in >> vlist;
        this->camera_search = vlist;

        int count = 0;
        for(QString s : set){
            ui->tableWidget_search->insertRow(count);
            ui->tableWidget_search->setItem(count, 0, new QTableWidgetItem("Camera ID: " + s));
            count++;
        }
        ui->tableWidget_search->setRowCount(count);

    }

    if(from == "ETC"){

        QString msg;
        in >> msg;
        if(msg == "Done"){
            QMessageBox::information(this,"提示", "\n上传成功！",QMessageBox::Ok);
            on_pushButton_clear_clicked();
        }else{



            QString position_from;
            QString position_to;

            QString car_type;

            in >> this->plate;
            in >> car_type;
            in >> position_from;
            in >> position_to;

            QNetworkAccessManager *m_accessManager = new QNetworkAccessManager(this);
            QObject::connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlotForAuto(QNetworkReply*)));
            QUrl url;
            QNetworkRequest request;

            qDebug() << car_type;
            if(car_type == "car"){
                url = QUrl("https://restapi.amap.com/v3/direction/driving?origin="
                            + position_from +
                            "&destination="
                            + position_to +
                            "&key=be6f38b0c3801dd73bcba0e07165b068"
                            "&extensions=base");
            }else{
//                url = QUrl("https://restapi.amap.com/v4/direction/truck?origin="
//                            + position_from +
//                            "&destination="
//                            + position_to +
//                            "&key=be6f38b0c3801dd73bcba0e07165b068"
//                            "&size=2"
//                            "&extensions=base");

                url = QUrl("https://restapi.amap.com/v3/direction/driving?origin="
                            + position_from +
                            "&destination="
                            + position_to +
                            "&key=be6f38b0c3801dd73bcba0e07165b068"
                            "&extensions=base");
            }


            qDebug() << url;


            request.setUrl(url);
            m_accessManager->get(request);

            QMessageBox::information(this,"提示", "\n上传成功！",QMessageBox::Ok);
            on_pushButton_clear_clicked();

        }
    }

}

void SubClient::finishedSlotForAuto(QNetworkReply* reply){

    bool flag = false;
    if (reply->error() == QNetworkReply::NoError){
        QByteArray ba = reply->readAll();
            QJsonParseError jsonpe;
            QJsonDocument json = QJsonDocument::fromJson(ba, &jsonpe);
            if (jsonpe.error == QJsonParseError::NoError){
                if (json.isObject()){
                    QJsonObject obj = json.object();
                    if (obj.contains("error")){
                        qDebug() << "error:" << obj["error"];
                    }else{
                        QJsonObject o = obj.value("route").toObject();

                        QString origin = o.value("origin").toString();
                        QString destination = o.value("destination").toString();

                        QJsonArray jarray = o.value("paths").toArray();



                        QString tolls = jarray[0].toObject().value("tolls").toString();

                        QString toll_distance = jarray[0].toObject().value("toll_distance").toString();



                        QStringList msg;
                        msg.append("setCost");
                        msg.append(this->plate);
                        msg.append(tolls);
                        msg.append(toll_distance);
                        sendMessage(msg);

                        flag = true;
                    }
                }else{
                    qDebug() << "error, should be json object";
                }
            }else{
                qDebug() << "error:" << jsonpe.errorString();
            }
    }else{
        qDebug() << "error:" << reply->errorString();
    }
    if(!flag){
        QMessageBox::warning(this," 警告", "\n计算失败",QMessageBox::Close);
        return ;
    }
}
