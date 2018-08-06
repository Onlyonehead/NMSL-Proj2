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

            ui->stackedWidget->setCurrentIndex(0);


            ui->label_2->setText(list.at(0));
            ui->label_4->setText(list.at(1));
            ui->label_8->setText(list.at(2));
            ui->label_12->setText(list.at(3));
            ui->label_14->setText(list.at(4));

            ui->frame->setStyleSheet("QFrame{background: rgba(0, 0, 0, 100);}");

            ui->frame_2->setStyleSheet("QFrame{"
                                       "background: rgba(248, 248, 248, 220);"
                                       "border-image:none;"
                                       "border: 2px solid rgb(200, 200, 200);}");


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
    //ETCpage send enter_highway_image
    if(from == "ETCp_sendEnterPic"){
        QString plateName;
        in >> plateName;

        if(plateName == "FAIL"){
            QMessageBox::warning(this, "warning", "\n未检测到车牌", QMessageBox::Close);
            ui->label_vehicleFromPic->clear();
            ui->label_vehicleFromPic->setVisible(false);
            return;
        }else {
            QMessageBox::information(this, "information", "\n上传成功！", QMessageBox::Ok);
        }

        QString plate;
        in >> plate;

        bool isPLateETCAvailable;
        in >> isPLateETCAvailable;

        if(isPLateETCAvailable == false){
            QMessageBox::warning(this, "Warning", "\n此车辆未办理ETC服务，请走人工通道", QMessageBox::Close);
        }
    }
    //ETC page send leave_highway_pic
    if(from == "ETCp_sendLeavePic"){
        QString plateName;
        in >> plateName;

        if(plateName == "FAIL"){
            QMessageBox::warning(this, "warning", "\n未检测到车牌", QMessageBox::Close);
            ui->label_vehicleTarPic->clear();
            ui->label_vehicleTarPic->setVisible(false);
            return;
        }else {
            QMessageBox::information(this, "information", "\n上传成功!", QMessageBox::Ok);
        }

        QString plate;
        in >> plate;


    }
    //ETCpage send enter_highway_image
    if(from == "ETCp_sendEnterPic"){
        QString plateName;
        in >> plateName;

        if(plateName == "FAIL"){
            QMessageBox::warning(this, "warning", "\n未检测到车牌", QMessageBox::Close);
            ui->label_vehicleFromPic->clear();
            ui->label_vehicleFromPic->setVisible(false);
            return;
        }else {
            QMessageBox::information(this, "information", "\n上传成功！", QMessageBox::Ok);
        }

        QString plate;
        in >> plate;

        bool isPLateETCAvailable;
        in >> isPLateETCAvailable;

        if(isPLateETCAvailable == false){
            QMessageBox::warning(this, "Warning", "\n此车辆未办理ETC服务，请走人工通道", QMessageBox::Close);
        }
    }
    //ETC page send leave_highway_pic
    if(from == "ETCp_sendLeavePic"){
        QString plateName;
        in >> plateName;

        if(plateName == "FAIL"){
            QMessageBox::warning(this, "warning", "\n未检测到车牌", QMessageBox::Close);
            ui->label_vehicleTarPic->clear();
            ui->label_vehicleTarPic->setVisible(false);
            return;
        }else {
            QMessageBox::information(this, "information", "\n上传成功!", QMessageBox::Ok);
        }

        QString plate;
        in >> plate;


    }




//    m_tcpsocket->disconnectFromHost();
}
