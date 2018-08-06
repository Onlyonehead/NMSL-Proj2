#include "mainclient.h"
#include "ui_mainclient.h"

void MainClient::sendMessage(QStringList list)
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
}

void MainClient::readMessage()
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
    qDebug() << "From: " << from << endl;

    if(from == "transfer"){
        int count0;
        int count1;
        in >> count0;
        in >> count1;

        QVector<QStringList> c;
        in >> c;
        this->cameras = c;

        QPieSeries *series3 = new QPieSeries();
        series3->setHoleSize(0.45);
        series3->append("User", count0);
        series3->append("Camera", count1);

        QChart *chart3 = new QChart;
        chart3->setTitle("Resource Type");
        chart3->addSeries(series3);
        chart3->legend()->setAlignment(Qt::AlignBottom);
        chart3->legend()->setFont(QFont("Helvetica", 13));
        chart3->setTheme(QChart::ChartThemeBrownSand);
        ui->chartView_3->setChart(chart3);
        ui->chartView_3->setRenderHint(QPainter::Antialiasing);
    }

    if(from == "lpr_history"){
        on_pushButton_clearHistory_clicked();
        QVector<QStringList> vlist;
        in >> vlist;
        this->lpr_history = vlist;

        int count = 0;
        for(QStringList l : vlist){
            QApplication::processEvents();
            ui->tableWidget_history->insertRow(count);
            ui->tableWidget_history->setItem(count, 0, new QTableWidgetItem(l.at(0)));
            ui->tableWidget_history->setItem(count, 1, new QTableWidgetItem(l.at(1)));
            ui->tableWidget_history->setItem(count, 2, new QTableWidgetItem(l.at(2)));
            ui->tableWidget_history->setItem(count, 3, new QTableWidgetItem(l.at(3)));
            QApplication::processEvents();
            count++;
        }
        ui->tableWidget_history->setRowCount(count);

        progressBar();
    }

    if(from == "lpr_lowProb"){
        on_pushButton_clearHistory_clicked();
        QVector<QStringList> vlist;
        in >> vlist;
        this->lpr_history = vlist;

        int count = 0;
        for(QStringList l : vlist){
            QApplication::processEvents();
            ui->tableWidget_history->insertRow(count);
            ui->tableWidget_history->setItem(count, 0, new QTableWidgetItem(l.at(0)));
            ui->tableWidget_history->setItem(count, 1, new QTableWidgetItem(l.at(1)));
            ui->tableWidget_history->setItem(count, 2, new QTableWidgetItem(l.at(2)));
            ui->tableWidget_history->setItem(count, 3, new QTableWidgetItem(l.at(3)));
            QApplication::processEvents();
            count++;
        }
        ui->tableWidget_history->setRowCount(count);

        progressBar();
    }

    if(from == "manual"){
        QString msg;
        in >> msg;
        if(msg == "Edited"){
            QMessageBox::information(this,"提示", "\n修改成功！",QMessageBox::Ok);
            on_pushButton_3_clicked();
            ui->tableWidget_history->setCurrentCell(-1, -1);
        }
        if(msg == "Checked"){
            QMessageBox::information(this,"提示", "\n已人工确认！",QMessageBox::Ok);
            on_pushButton_3_clicked();
            ui->tableWidget_history->setCurrentCell(-1, -1);
        }
    }

    if(from == "setModel"){
        QString msg;
        in >> msg;
        if(msg == "Done"){
            QMessageBox::information(this,"提示", "\n修改模型成功！",QMessageBox::Ok);
        }
    }

    if(from == "trainModel"){
        QString msg;
        in >> msg;
        if(msg == "Done"){
            QMessageBox::information(this,"提示", "\n模型训练成功！",QMessageBox::Ok);
        }
    }

    if(from == "del_entry"){
        QString msg;
        in >> msg;
        if(msg == "Done"){
            QMessageBox::information(this,"提示", "\n删除条目成功！",QMessageBox::Ok);
            on_pushButton_3_clicked();
            ui->tableWidget_history->setCurrentCell(-1, -1);
        }
    }

    if(from == "showUser"){
        on_pushButton_updatePageClearStaff_clicked();
        int i = 0;
        QVector<QStringList> result;
        in >> result;
        this->userdata_settings = result;
        for(QStringList list : result){
            if(list.at(0) == "camera"){
                continue;
            }
            ui->tableWidget_updateShowStaffInfo->insertRow(i);

            ui->tableWidget_updateShowStaffInfo->setItem(i, 0, new QTableWidgetItem(list.at(1)));
            ui->tableWidget_updateShowStaffInfo->item(i, 0)->setTextAlignment(Qt::AlignCenter);

            i++;
        }
        ui->tableWidget_updateShowStaffInfo->setRowCount(i);
        progressBar();
    }

    if(from == "updateUser"){
        QString msg;
        in >> msg;
        if(msg == "Done"){
            QMessageBox::information(this,"提示", "\n用户信息修改成功！",QMessageBox::Ok);
            on_pushButton_updatePageShowStaff_clicked();
        }

    }

    if(from == "pp2_anu"){
        bool isExisted;
        in >> isExisted;
        if(isExisted == true){
            ui->label_newStaffUsernameTip->setVisible(true);
            ui->lineEdit_addNewPassword->setEnabled(false);
            ui->lineEdit_repeatPassword->setEnabled(false);
            ui->lineEdit_addNewName->setEnabled(false);
            ui->lineEdit_addNewEmail->setEnabled(false);
            ui->pushButton_confirmNewStaff->setEnabled(false);
        }else {
            ui->label_newStaffUsernameTip->setVisible(false);
            ui->lineEdit_addNewPassword->setEnabled(true);
            ui->lineEdit_repeatPassword->setEnabled(true);
            ui->lineEdit_addNewName->setEnabled(true);
            ui->lineEdit_addNewEmail->setEnabled(true);
            ui->pushButton_confirmNewStaff->setEnabled(true);
        }
    }

    if(from == "pp2_ane"){
        bool isExisted;
        in >> isExisted;
        if(isExisted){
            ui->label_newStaffEmailTip->setVisible(true);
            ui->pushButton_confirmNewStaff->setEnabled(false);
        }else {
            ui->label_newStaffEmailTip->setVisible(false);
            ui->pushButton_confirmNewStaff->setEnabled(true);
        }
    }

    if(from == "addUser"){
        QString msg;
        in >> msg;
        if(msg == "Done"){
            QMessageBox::information(this,"提示", "\n添加用户成功！",QMessageBox::Ok);
            ui->lineEdit_addNewUsername->clear();
            ui->lineEdit_addNewPassword->clear();
            ui->lineEdit_repeatPassword->clear();
            ui->lineEdit_addNewName->clear();
            ui->lineEdit_addNewEmail->clear();
            ui->label_showNewPortrait->clear();
            ui->label_showNewPortraitPath->clear();
        }
    }

    if(from == "displayCamera"){
        on_pushButton_clearCamera_clicked();

        QSet<QString> ids;
        in >> ids;
        QVector<QStringList> vlist;
        in >> vlist;
        this->traffic = vlist;

        QVector<QStringList> rlist;
        in >> rlist;
        this->redlight = rlist;

        QVector<QStringList> olist;
        in >> olist;
        this->overspeed = olist;

        int count = 0;

        for(QString s : ids){
            ui->tableWidget_left->insertRow(count);
            ui->tableWidget_left->setItem(count, 0, new QTableWidgetItem("Camera ID: " + s));

            count++;
        }

        ui->tableWidget_left->setRowCount(count);
    }

    //main client ETC page display all on road vehicles
       if(from == "mc_ETCp_displayOnRoadVehicles"){
           ui->tableWidget_displayETCvehicleInfo->clear();
           QVector<QStringList> result;
           in >> result;
           int i = 0;
           for(QStringList list : result){
               ui->tableWidget_displayETCvehicleInfo->insertRow(i);
               ui->tableWidget_displayETCvehicleInfo->setItem(i, 0, new QTableWidgetItem(list.at(0)));
               ui->tableWidget_displayETCvehicleInfo->setItem(i, 1, new QTableWidgetItem(list.at(1)));
               i++;
           }
           ui->tableWidget_displayETCvehicleInfo->setRowCount(i);
           progressBar();
       }
       //main client display all pay history
           if(from == "mc_ETCp_displayPayHistory"){
               ui->tableWidget_displayETCPayHistory->clear();
               QVector<QStringList> result;
               in >> result;
               int i = 0;
               for(QStringList list : result){
                   ui->tableWidget_displayETCPayHistory->insertRow(i);
                   ui->tableWidget_displayETCPayHistory->setItem(i, 0, new QTableWidgetItem(list.at(0)));
                   ui->tableWidget_displayETCPayHistory->setItem(i, 1, new QTableWidgetItem(list.at(1)));
                   ui->tableWidget_displayETCPayHistory->setItem(i, 2, new QTableWidgetItem(list.at(2)));
                   i++;
               }
               ui->tableWidget_displayETCPayHistory->setRowCount(i);
               progressBar();
           }
           //main client ETC page recharge
           if(from == "mc_recharge"){
               QMessageBox::information(this, tr("information"), tr("\n充值成功！"), QMessageBox::Yes, QMessageBox::Yes);
           }
           //main client ETC page display vehicle wit arrears
           if(from == "mc_displayVehiclesWA"){
               ui->tableWidget_displayETCvehicleInfo->clear();
               QVector<QStringList> result;
               in >> result;
               int i = 0;
               for(QStringList list : result){
                   ui->tableWidget_displayETCvehicleInfo->insertRow(i);
                   ui->tableWidget_displayETCvehicleInfo->setItem(i, 0, new QTableWidgetItem(list.at(0)));
                   ui->tableWidget_displayETCvehicleInfo->setItem(i, 1, new QTableWidgetItem(list.at(1)));
                   i++;
               }
               ui->tableWidget_displayETCvehicleInfo->setRowCount(i);
               progressBar();
           }
           //main client search plates
           if(from == "mc_ETCp_searchPlate"){
               ui->tableWidget_displayETCvehicleInfo->clear();
               QVector<QStringList> result;
               in >> result;
               int i = 0;
               for(QStringList list : result){
                   ui->tableWidget_displayETCvehicleInfo->insertRow(i);
                   ui->tableWidget_displayETCvehicleInfo->setItem(i, 0, new QTableWidgetItem(list.at(0)));
                   ui->tableWidget_displayETCvehicleInfo->setItem(i, 0, new QTableWidgetItem(list.at(1)));
                   i++;
               }
               ui->tableWidget_displayETCvehicleInfo->setRowCount(i);
               progressBar();
           }
           //main client delete plate
           if(from == "mc_DeletePlate"){
               QMessageBox::information(this, tr("information"), tr("\n删除成功"), QMessageBox::Close, QMessageBox::Close);
           }


}
