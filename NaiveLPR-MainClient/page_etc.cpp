#include "mainclient.h"
#include "ui_mainclient.h"

void MainClient::on_pushButton_showmap_clicked()
{
    QtMap *qtMap = new QtMap("http://kousz.top/repo/project2/src/camera.html");
    connect(this, SIGNAL(webReturn(QVector<QStringList>)),
            qtMap, SLOT(webInit(QVector<QStringList>)));
    emit webReturn(this->cameras);
    qtMap->exec();
}

void MainClient::on_pushButton_etcDisplay_clicked()
{
    QStringList msg;
    msg.append("etcDisplay");
    sendMessage(msg);
}

void MainClient::on_pushButton_etcClear_clicked()
{
    ui->label_name->setText("-");
    ui->label_type->setText("-");
    ui->label_balance->setText("-");

    ui->label_name_2->setText("-");
    ui->label_toll->setText("-");
    ui->label_distance->setText("-");
    ui->label_tel->setText("-");

    ui->checkBox_sms->setChecked(false);

    ui->label_plate->setText("-");
    ui->label_entrance->setText("-");
    ui->label_exit->setText("-");

    ui->lineEdit_recharge->setText("300");

    ui->comboBox_selectType_2->setCurrentIndex(0);

    ui->tableWidget_accounts->setRowCount(0);
    ui->tableWidget_highway->setRowCount(0);
    ui->tableWidget_payment->setRowCount(0);
    ui->tableWidget_toPay->setRowCount(0);

    ui->label_quantityToPay->setText("-");
    ui->label_quantityOngoing->setText("-");
}

void MainClient::on_comboBox_selectType_2_currentTextChanged(const QString &arg1)
{

    QString type = arg1.trimmed();

    ui->tableWidget_highway->setRowCount(0);

    if(type == "ALL"){
        QVector<QStringList> vlist;
        vlist = this->etcHighway;

        int count = 0;
        ui->tableWidget_highway->setRowCount(0);
        for(QStringList l : vlist){
            QApplication::processEvents();
            ui->tableWidget_highway->insertRow(count);
            ui->tableWidget_highway->setItem(count, 0, new QTableWidgetItem(l.at(0)));
            QStringList s = l.at(2).split(QRegExp("[A-Z]"));
            ui->tableWidget_highway->setItem(count, 1, new QTableWidgetItem(s.at(0) + " " + s.at(1)));
            if(l.at(6).trimmed() == ""){
                ui->tableWidget_highway->setItem(count, 2, new QTableWidgetItem(QString("-")));
                ui->tableWidget_highway->item(count, 2)->setTextAlignment(Qt::AlignCenter);
            }else{
                s = l.at(6).split(QRegExp("[A-Z]"));
                ui->tableWidget_highway->setItem(count, 2, new QTableWidgetItem(s.at(0) + " " + s.at(1)));
            }
            ui->tableWidget_highway->setItem(count, 3, new QTableWidgetItem(l.at(3)));
            QApplication::processEvents();
            count++;
        }
        ui->tableWidget_highway->setRowCount(count);

        progressBar_fast();
    }

    if(type == "ON"){
        QVector<QStringList> vlist;
        vlist = this->etcHighway;

        int count = 0;
        ui->tableWidget_highway->setRowCount(0);
        for(QStringList l : vlist){
            QApplication::processEvents();
            if(l.at(4) == "0"){
                ui->tableWidget_highway->insertRow(count);
                ui->tableWidget_highway->setItem(count, 0, new QTableWidgetItem(l.at(0)));
                QStringList s = l.at(2).split(QRegExp("[A-Z]"));
                ui->tableWidget_highway->setItem(count, 1, new QTableWidgetItem(s.at(0) + " " + s.at(1)));

                ui->tableWidget_highway->setItem(count, 2, new QTableWidgetItem(QString("-")));
                ui->tableWidget_highway->item(count, 2)->setTextAlignment(Qt::AlignCenter);

                ui->tableWidget_highway->setItem(count, 3, new QTableWidgetItem(l.at(3)));

                QApplication::processEvents();
                count++;
            }
        }
        ui->tableWidget_highway->setRowCount(count);

        progressBar_fast();
    }

    if(type == "OFF"){
        QVector<QStringList> vlist;
        vlist = this->etcHighway;

        int count = 0;
        ui->tableWidget_highway->setRowCount(0);
        for(QStringList l : vlist){
            QApplication::processEvents();
            if(l.at(4) == "1"){
                ui->tableWidget_highway->insertRow(count);
                ui->tableWidget_highway->setItem(count, 0, new QTableWidgetItem(l.at(0)));
                QStringList s = l.at(2).split(QRegExp("[A-Z]"));
                ui->tableWidget_highway->setItem(count, 1, new QTableWidgetItem(s.at(0) + " " + s.at(1)));
                if(l.at(6).trimmed() == ""){
                    ui->tableWidget_highway->setItem(count, 2, new QTableWidgetItem(QString("-")));
                    ui->tableWidget_highway->item(count, 2)->setTextAlignment(Qt::AlignCenter);
                }else{
                    s = l.at(6).split(QRegExp("[A-Z]"));
                    ui->tableWidget_highway->setItem(count, 2, new QTableWidgetItem(s.at(0) + " " + s.at(1)));
                }
                ui->tableWidget_highway->setItem(count, 3, new QTableWidgetItem(l.at(3)));
                QApplication::processEvents();
                count++;
            }
        }
        ui->tableWidget_highway->setRowCount(count);

        progressBar_fast();
    }

}

void MainClient::on_tableWidget_accounts_itemClicked(QTableWidgetItem *item){
    QString plate = item->text().trimmed();

    QVector<QStringList> vlist;
    vlist = this->etcPayment;

    int count = 0;
    ui->tableWidget_payment->setRowCount(0);
    for(QStringList l : vlist){
        QApplication::processEvents();
        if(l.at(0) == plate){
            ui->tableWidget_payment->insertRow(count);
            QStringList s = l.at(1).split(QRegExp("[A-Z]"));
            ui->tableWidget_payment->setItem(count, 0, new QTableWidgetItem(s.at(0) + " " + s.at(1)));
            ui->tableWidget_payment->setItem(count, 1, new QTableWidgetItem(l.at(2)));

            QApplication::processEvents();
            count++;
        }
    }
    ui->tableWidget_payment->setRowCount(count);

    for(QStringList l : this->etcAccount){
        if(l.at(0) == plate){
            ui->label_name->setText(l.at(1));
            ui->label_type->setText(l.at(3));
            ui->label_balance->setText(l.at(4));
            break;
        }
    }

    progressBar_fast();

}

void MainClient::on_tableWidget_toPay_itemClicked(QTableWidgetItem *item){

    QString plate = item->text().split("\n")[0];
    QString datetime = item->text().split("\n")[1];

    QVector<QStringList> vlist;
    vlist = this->etcPayment;

    ui->label_name_2->setText("-");
    ui->label_tel->setText("-");

    for(QStringList l : vlist){
        QApplication::processEvents();
        QStringList s = l.at(1).split(QRegExp("[A-Z]"));
        if(l.at(0) == plate && (s[0] + " " + s[1]) == datetime && l.at(4) == "0"){
            for(QStringList ll : this->etcAccount){
                if(ll.at(0) == plate){
                    ui->label_name_2->setText(ll.at(1));
                    ui->label_tel->setText(ll.at(2));
                    break;
                }
            }
            ui->label_toll->setText(l.at(2) + " CNY");

            ui->label_distance->setText(QString::number(l.at(3).toDouble() / 1000, 10, 2) + " KM");

            QApplication::processEvents();

            break;
        }
    }


    progressBar_fast();

}


void MainClient::on_pushButton_charge_clicked()
{

    if(ui->tableWidget_toPay->currentItem() == NULL){
        QMessageBox::warning(this,"警告", "\n请选中条目！",QMessageBox::Close);
        return ;
    }
    QString plate = ui->tableWidget_toPay->currentItem()->text().split("\n")[0];
    QString datetime = ui->tableWidget_toPay->currentItem()->text().split("\n")[1];

    QStringList msg;
    msg.append("charge");
    msg.append(plate);
    msg.append(datetime);
    sendMessage(msg);
}

void MainClient::on_pushButton_generate_2_clicked()
{
    if(ui->tableWidget_accounts->currentItem() == NULL){
        QMessageBox::warning(this,"警告", "\n请选中条目！",QMessageBox::Close);
        return ;
    }

    QString plate = ui->tableWidget_accounts->currentItem()->text();

    QString recharge = ui->lineEdit_recharge->text();

    QStringList msg;
    msg.append("recharge");
    msg.append(plate);
    msg.append(recharge);
    sendMessage(msg);

}

void MainClient::on_pushButton_sms_clicked()
{
    if(ui->tableWidget_toPay->currentItem() == NULL){
        QMessageBox::warning(this,"警告", "\n请选中条目！",QMessageBox::Close);
        return ;
    }
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int random = 111111 + qrand() % (999999 - 111111);

    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timestrap = time.toTime_t();   //将当前时间转为时间戳

    QByteArray byte_array;
    byte_array.append("appkey=35a4af1e1aba05887576d1515153d5f4&random="
                      + QString::number(random) +
                      "&time="
                      + QString::number(timestrap) +
                      "&mobile="
                      + ui->label_tel->text());
    QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Sha256);
    QString sig = hash_byte_array.toHex();

    QJsonObject json_in;
    QJsonObject json;
    QJsonDocument docum;
    QByteArray data_json;
    json.insert("msg", "您本次高速公路的花费为: " + ui->label_toll->text() +
                "，如果未缴费，请及时缴费。");
    json.insert("sig", sig);
    json_in.insert("mobile", ui->label_tel->text());
    json_in.insert("nationcode", "86");
    json.insert("tel",json_in);
    json.insert("time", timestrap);
    json.insert("type", 0);


    docum.setObject(json);
    data_json=docum.toJson(QJsonDocument::Compact);

    QString addr = "https://yun.tim.qq.com/v5/tlssmssvr/sendsms?"
                   "sdkappid=1400118824&"
                   "random=" + QString::number(random);
    QUrl url(addr);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    QNetworkAccessManager *m_accessManager;
    m_accessManager= new QNetworkAccessManager(this);
    m_accessManager->post(request,data_json);

    QObject::connect(m_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

}

void MainClient::finishedSlot(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError){
        QMessageBox::information(this,"提示", "\n短信发送成功！",QMessageBox::Ok);
    }else{
        QMessageBox::warning(this,"警告", "\n短信发送失败！",QMessageBox::Close);
    }
}

void MainClient::on_pushButton_etcCharge_clicked()
{
    int isChecked = ui->checkBox_sms->isChecked();
    QStringList msg;
    msg.append("AutoCharge");
    msg.append(QString::number(isChecked));
    sendMessage(msg);
}

void MainClient::on_tableWidget_highway_itemClicked(QTableWidgetItem *item){

    QString plate = ui->tableWidget_highway->item(item->row(), 0)->text();
    QString departure_time = ui->tableWidget_highway->item(item->row(), 1)->text();

    QVector<QStringList> vlist;
    vlist = this->etcHighway;

    for(QStringList l : vlist){
        QApplication::processEvents();
        QString tmp_plate = l.at(0);
        QStringList s = l.at(2).split(QRegExp("[A-Z]"));
        QString tmp_time = s.at(0) + " " + s.at(1);

        if(plate == tmp_plate && departure_time == tmp_time){
            ui->label_plate->setText(plate);
            ui->label_entrance->setText(l.at(1));
            if(l.at(5) == ""){
                ui->label_exit->setText("-");
            }else{
                ui->label_exit->setText(l.at(5));
            }
        }
    }
}

void MainClient::on_pushButton_showRoute_clicked()
{
    QString from = ui->label_entrance->text();
    QString to = ui->label_exit->text();

    if(from == "-"){
        QMessageBox::warning(this,"警告", "\n请选中条目！",QMessageBox::Close);
        return ;
    }

    if(to == "-"){
        QtMap *qtMap = new QtMap("http://kousz.top/repo/project2/src/camera.html");
        connect(this, SIGNAL(webReturn(QVector<QStringList>)),
                qtMap, SLOT(webInit(QVector<QStringList>)));
        QVector<QStringList> sites;
        QStringList l;

        l.append("Entrance");
        l.append(from.split(",")[0]);
        l.append(from.split(",")[1]);
        sites.append(l);
        emit webReturn(sites);
        qtMap->exec();

    }else{
        QtMap *qtMap = new QtMap("http://kousz.top/repo/project2/src/route.html");
        connect(this, SIGNAL(webReturn(QVector<QStringList>)),
                qtMap, SLOT(webInit(QVector<QStringList>)));
        QVector<QStringList> sites;
        QStringList l;

        l.append("Entrance");
        l.append(from.split(",")[0]);
        l.append(from.split(",")[1]);
        sites.append(l);

        l.clear();
        l.append("Exit");
        l.append(to.split(",")[0]);
        l.append(to.split(",")[1]);
        sites.append(l);

        emit webReturn(sites);

        qtMap->exec();
    }

}
