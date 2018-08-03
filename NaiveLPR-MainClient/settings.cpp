#include "mainclient.h"
#include "ui_mainclient.h"

void MainClient::on_pushButton_updatePageShowStaff_clicked()
{
    QStringList list;
    list.append("showUser");
    sendMessage(list);
}

void MainClient::on_pushButton_updatePageClearStaff_clicked()
{
    ui->tableWidget_updateShowStaffInfo->setRowCount(0);
    ui->label_updateShowUsername->clear();
    ui->lineEdit_updateStaffName->clear();
    ui->lineEdit_updateStaffEmail->clear();
    ui->label_showEditPortrait->setText("Preview");
    ui->label_showEditPortraitPath->clear();
}

void MainClient::on_tableWidget_updateShowStaffInfo_cellClicked(int row, int column)
{
    for(QStringList list : userdata_settings){
        if(list.at(1) == ui->tableWidget_updateShowStaffInfo->item(row, 0)->text()){
            ui->label_updateShowUsername->setText(list.at(0));
            ui->lineEdit_updateStaffName->setText(list.at(1));
            ui->comboBox_updateStaffGender->setCurrentText("       " + list.at(2));
            ui->comboBox_updateStaffPosition->setCurrentText("    " + list.at(3));
            ui->lineEdit_updateStaffEmail->setText(list.at(4));

            QImage tempPortrait(DIR + QString("/users/") + list.at(5));
            if (tempPortrait.isNull()){
                download("/users/" + list.at(5), DIR + QString("/users/") + list.at(5));

                QElapsedTimer t;
                t.start();
                while(t.elapsed()<500)
                    QCoreApplication::processEvents();

                tempPortrait = QImage(DIR + QString("/users/") + list.at(5));
            }

            if(tempPortrait.isNull()){
                tempPortrait = QImage(":/user.png");
            }


            QPixmap portrait = QPixmap::fromImage(tempPortrait.scaled(92, 92, Qt::KeepAspectRatio,
                                                                      Qt::SmoothTransformation));
            ui->label_showEditPortrait->setPixmap(portrait);
            ui->label_showEditPortrait->show();
            break;
        }
    }

    Q_UNUSED(column);
}

void MainClient::on_pushButton_changeStaffInfo_clicked()
{
    QString username = ui->label_updateShowUsername->text().trimmed();
    QString name = ui->lineEdit_updateStaffName->text().trimmed();
    QString gender = ui->comboBox_updateStaffGender->currentText().trimmed();
    QString position = ui->comboBox_updateStaffPosition->currentText().trimmed();
    QString email = ui->lineEdit_updateStaffEmail->text().trimmed();


    QString pic = username +".jpg";
    QString picPath = ui->label_showEditPortraitPath->text().trimmed();

    QBuffer buffer;
    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);
    QImage img;

    if(username == "" || name == "" || gender == "" ||
            position == "" || email == ""){
        QMessageBox::warning(this,"警告", "\n请输入全部信息！",QMessageBox::Close);
        return ;
    }

    if(QMessageBox::Yes==QMessageBox::question(this, "警告", "Confirm to edit this user ?",
                                               QMessageBox::Yes, QMessageBox::No)){
        QStringList list;
        list.append("updateUser");
        list.append(username);
        list.append(name);
        list.append(gender);
        list.append(position);
        list.append(email);
        sendMessage(list);

        out.setVersion(QDataStream::Qt_5_7);
        buffer.open(QIODevice::ReadWrite);
        img.load(picPath);
        img.save(&buffer,"JPG");
        out << qint32(buffer.size());
        out << QString("/users/" + pic);
        out << buffer.data();

        m_socket->write(message);
        m_socket->flush();

        QFile file(DIR + QString("/users/") + pic);
        if (file.exists()){
            file.remove();
        }
    }
}

void MainClient::on_pushButton_confirmNewStaff_clicked()
{
    QString username;
    QString password;
    QString name;
    QString gender;
    QString position;
    QString email;
    QString pic;
    username = ui->lineEdit_addNewUsername->text().trimmed();
    password = ui->lineEdit_addNewPassword->text().trimmed();
    name = ui->lineEdit_addNewName->text().trimmed();
    gender = ui->comboBox_addNewGender->currentText().trimmed();
    position = ui->comboBox_addNewPosition->currentText().trimmed();
    email = ui->lineEdit_addNewEmail->text().trimmed();
    pic = username +".jpg";
    QString picPath = ui->label_showNewPortraitPath->text().trimmed();
    QBuffer buffer;
    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);
    QImage img;

    if(username == "" || password == "" || name == "" || gender == "" ||
            position == "" || email == "" || picPath == ""){
        QMessageBox::warning(this,"警告", "\n请输入全部信息！",QMessageBox::Close);
        return ;
    }

    if(QMessageBox::Yes==QMessageBox::question(this, "警告", "Confirm to add this user ?",
                                               QMessageBox::Yes, QMessageBox::No)){
        QStringList list;
        list.append("addUser");

        list.append(username);
        list.append(password);
        list.append(name);
        list.append(gender);
        list.append(position);
        list.append(email);
        list.append(pic);
        sendMessage(list);


        out.setVersion(QDataStream::Qt_5_7);
        buffer.open(QIODevice::ReadWrite);
        img.load(picPath);
        img.save(&buffer,"JPG");
        out << qint32(buffer.size());
        out << QString("/users/" + pic);
        out << buffer.data();

        m_socket->write(message);
        m_socket->flush();
    }
}

void MainClient::on_pushButton_cancelNewStaff_clicked()
{
    ui->lineEdit_addNewUsername->clear();
    ui->lineEdit_addNewPassword->clear();
    ui->lineEdit_repeatPassword->clear();
    ui->lineEdit_addNewName->clear();
    ui->lineEdit_addNewEmail->clear();
    ui->label_showNewPortrait->setText("Preview");
    ui->label_showNewPortraitPath->clear();
    ui->label_repeatPasswordTip->setVisible(false);
    ui->label_newStaffUsernameTip->setVisible(false);
    ui->label_newStaffEmailTip->setVisible(false);
}



void MainClient::on_lineEdit_addNewUsername_editingFinished()
{
    QStringList list;
    list.append("pp2_anu");
    QString username = ui->lineEdit_addNewUsername->text();
    list.append(username);
    sendMessage(list);

}

void MainClient::on_lineEdit_repeatPassword_editingFinished()
{
    QString repeatPassword = ui->lineEdit_repeatPassword->text();
    QString password = ui->lineEdit_addNewPassword->text();
    if(repeatPassword != password){
        ui->label_repeatPasswordTip->setVisible(true);
        ui->lineEdit_addNewName->setEnabled(false);
        ui->lineEdit_addNewEmail->setEnabled(false);
        ui->pushButton_confirmNewStaff->setEnabled(false);
    }else {
        ui->label_repeatPasswordTip->setVisible(false);
        ui->lineEdit_addNewName->setEnabled(true);
        ui->lineEdit_addNewEmail->setEnabled(true);
        ui->pushButton_confirmNewStaff->setEnabled(true);
    }

}

void MainClient::on_lineEdit_addNewEmail_editingFinished()
{
    QStringList list;
    list.append("pp2_ane");
    QString email = ui->lineEdit_addNewEmail->text();
    list.append(email);
    sendMessage(list);

}


