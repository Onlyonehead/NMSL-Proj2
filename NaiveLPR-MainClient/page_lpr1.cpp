#include "mainclient.h"
#include "ui_mainclient.h"

void MainClient::on_pushButton_3_clicked()
{
    QStringList msg;
    msg.append("lpr_history");
    sendMessage(msg);

}


void MainClient::on_pushButton_clearHistory_clicked()
{
    ui->lineEdit_Hlayer->setText("65");
    ui->lineEdit_iter->setText("5000");
    ui->lineEdit_threshold->setText("1e-4");
    ui->tableWidget_search->setRowCount(0);
    ui->scrollArea->setWidget(new QWidget());
    ui->tableWidget_history->setRowCount(0);
    ui->search_A->clear();
    ui->label_platePreview->setText("Preview");
    ui->comboBox_selectType->setCurrentIndex(0);
    this->processPhase = 0;
    this->fileNameForProcess = "";
    ui->label_PlateDetection->clear();
    ui->label_PlateRecognition->clear();
    ui->label_char0->clear();
    ui->label_char1->clear();
    ui->label_char2->clear();
    ui->label_char3->clear();
    ui->label_char4->clear();
    ui->label_char5->clear();
    ui->label_char6->clear();
    ui->label_Result->clear();
    ui->comboBox_p0->setCurrentIndex(0);
    ui->comboBox_p1->setCurrentIndex(0);
    ui->comboBox_p2->setCurrentIndex(0);
    ui->comboBox_p3->setCurrentIndex(0);
    ui->comboBox_p4->setCurrentIndex(0);
    ui->comboBox_p5->setCurrentIndex(0);
    ui->comboBox_p6->setCurrentIndex(0);
}

void MainClient::on_tableWidget_history_itemDoubleClicked(QTableWidgetItem *item){
    if(QMessageBox::Yes==QMessageBox::question(this, "警告", "Confirm to delete this entry ?",
                                               QMessageBox::Yes, QMessageBox::No)){
        QString pic_name = ui->tableWidget_history->item(item->row(), 0)->text();
        QStringList msg;
        msg.append("del_entry");
        msg.append(pic_name);
        sendMessage(msg);
    }
}

void MainClient::on_tableWidget_history_itemClicked(QTableWidgetItem *item){

    this->processPhase = 0;
    this->fileNameForProcess = "";
    ui->label_PlateDetection->clear();
    ui->label_PlateRecognition->clear();
    ui->label_char0->clear();
    ui->label_char1->clear();
    ui->label_char2->clear();
    ui->label_char3->clear();
    ui->label_char4->clear();
    ui->label_char5->clear();
    ui->label_char6->clear();
    ui->label_Result->clear();
    ui->comboBox_p0->setCurrentIndex(0);
    ui->comboBox_p1->setCurrentIndex(0);
    ui->comboBox_p2->setCurrentIndex(0);
    ui->comboBox_p3->setCurrentIndex(0);
    ui->comboBox_p4->setCurrentIndex(0);
    ui->comboBox_p5->setCurrentIndex(0);
    ui->comboBox_p6->setCurrentIndex(0);

    QString fileName = ui->tableWidget_history->item(item->row(), 0)->text() + ".png";

    QImage tempPortrait(DIR + QString("/plates/") + fileName);

    if(tempPortrait.isNull()){
        download("/plates/" + fileName, "/plates/" + fileName);

        QElapsedTimer t;
        t.start();
        while(t.elapsed()<500)
            QCoreApplication::processEvents();

        tempPortrait = QImage(DIR + QString("/plates/") + fileName);
    }

    QPixmap portrait = QPixmap::fromImage(tempPortrait.scaled(192, 142, Qt::KeepAspectRatio,
                                                              Qt::SmoothTransformation));
    ui->label_platePreview->setPixmap(portrait);
    ui->label_platePreview->show();

    QString number = ui->tableWidget_history->item(item->row(), 1)->text();
    ui->comboBox_p0->setCurrentText("   " + QString(number.at(0)));
    ui->comboBox_p1->setCurrentText("    " + QString(number.at(1)));
    ui->comboBox_p2->setCurrentText("    " + QString(number.at(2)));
    ui->comboBox_p3->setCurrentText("    " + QString(number.at(3)));
    ui->comboBox_p4->setCurrentText("    " + QString(number.at(4)));
    ui->comboBox_p5->setCurrentText("    " + QString(number.at(5)));
    ui->comboBox_p6->setCurrentText("    " + QString(number.at(6)));

    progressBar_fast();
}


void MainClient::on_pushButton_lowProb_clicked()
{
    QStringList msg;
    msg.append("lpr_lowProb");
    sendMessage(msg);
}

void MainClient::on_comboBox_selectType_currentTextChanged(const QString &arg1)
{
    this->processPhase = 0;
    this->fileNameForProcess = "";
    ui->label_PlateDetection->clear();
    ui->label_PlateRecognition->clear();
    ui->label_char0->clear();
    ui->label_char1->clear();
    ui->label_char2->clear();
    ui->label_char3->clear();
    ui->label_char4->clear();
    ui->label_char5->clear();
    ui->label_char6->clear();
    ui->label_Result->clear();

    ui->comboBox_p0->setCurrentIndex(0);
    ui->comboBox_p1->setCurrentIndex(0);
    ui->comboBox_p2->setCurrentIndex(0);
    ui->comboBox_p3->setCurrentIndex(0);
    ui->comboBox_p4->setCurrentIndex(0);
    ui->comboBox_p5->setCurrentIndex(0);
    ui->comboBox_p6->setCurrentIndex(0);

    QString type = arg1.trimmed();

    ui->tableWidget_history->setRowCount(0);

    if(type == "ALL"){
        QVector<QStringList> vlist;
        vlist = this->lpr_history;

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

        progressBar_fast();
    }

    if(type == "USER"){
        QVector<QStringList> vlist;
        vlist = this->lpr_history;

        int count = 0;
        for(QStringList l : vlist){
            if(l.at(3) == "USER"){
                QApplication::processEvents();
                ui->tableWidget_history->insertRow(count);
                ui->tableWidget_history->setItem(count, 0, new QTableWidgetItem(l.at(0)));
                ui->tableWidget_history->setItem(count, 1, new QTableWidgetItem(l.at(1)));
                ui->tableWidget_history->setItem(count, 2, new QTableWidgetItem(l.at(2)));
                ui->tableWidget_history->setItem(count, 3, new QTableWidgetItem(l.at(3)));
                QApplication::processEvents();
                count++;
            }
        }
        ui->tableWidget_history->setRowCount(count);

        progressBar_fast();
    }

    if(type == "CAMERA"){
        QVector<QStringList> vlist;
        vlist = this->lpr_history;

        int count = 0;
        for(QStringList l : vlist){
            if(l.at(3) == "CAMERA"){
                QApplication::processEvents();
                ui->tableWidget_history->insertRow(count);
                ui->tableWidget_history->setItem(count, 0, new QTableWidgetItem(l.at(0)));
                ui->tableWidget_history->setItem(count, 1, new QTableWidgetItem(l.at(1)));
                ui->tableWidget_history->setItem(count, 2, new QTableWidgetItem(l.at(2)));
                ui->tableWidget_history->setItem(count, 3, new QTableWidgetItem(l.at(3)));
                QApplication::processEvents();
                count++;
            }
        }
        ui->tableWidget_history->setRowCount(count);

        progressBar_fast();
    }

}

void MainClient::on_pushButton_5_clicked()
{
    if(ui->tableWidget_history->currentItem() == NULL){
        QMessageBox::warning(this,"警告", "\n未选中条目",QMessageBox::Close);
        return;
    }

    if(processPhase == 0){
        fileNameForProcess = ui->tableWidget_history->item(
                    ui->tableWidget_history->currentItem()->row(), 0)->text();
        QString plateName = fileNameForProcess + "-plate.jpg";

        QImage tempPortrait(DIR + QString("/plates-d/") + plateName);

        if(tempPortrait.isNull()){
            download("/plates-d/" + plateName, "/plates-d/" + plateName);
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<500)
                QCoreApplication::processEvents();

            tempPortrait = QImage(DIR + QString("/plates-d/") + plateName);
        }

        QPixmap portrait = QPixmap::fromImage(tempPortrait.scaled(142, 42, Qt::KeepAspectRatio,
                                                                  Qt::SmoothTransformation));
        ui->label_PlateDetection->setPixmap(portrait);
        ui->label_PlateDetection->show();
        processPhase++;
    }

    QElapsedTimer t;
    t.start();
    while(t.elapsed()<300)
        QCoreApplication::processEvents();

    if(processPhase == 1){

        QString plateRName = fileNameForProcess + "-plateR.jpg";

        QImage tempPortrait(DIR + QString("/plates-r/") + plateRName);

        if(tempPortrait.isNull()){
            download("/plates-r/" + plateRName, "/plates-r/" + plateRName);
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<500)
                QCoreApplication::processEvents();

            tempPortrait = QImage(DIR + QString("/plates-r/") + plateRName);
        }

        QPixmap portrait = QPixmap::fromImage(tempPortrait.scaled(142, 42, Qt::KeepAspectRatio,
                                                                  Qt::SmoothTransformation));
        ui->label_PlateRecognition->setPixmap(portrait);
        ui->label_PlateRecognition->show();
        processPhase++;
    }

    t.start();
    while(t.elapsed()<300)
        QCoreApplication::processEvents();

    if(processPhase == 2){
        fileNameForProcess = ui->tableWidget_history->item(
                    ui->tableWidget_history->currentItem()->row(), 0)->text();
        QStringList charName;

        for(int i = 0; i < 7; i++){
            charName.append(fileNameForProcess + "-" + QString::number(i) + ".jpg");
        }

        //for char0

        QImage tempPortrait(DIR + QString("/chars/") + charName.at(0));

        if(tempPortrait.isNull()){
            download("/chars/" + charName.at(0), "/chars/" + charName.at(0));
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<500)
                QCoreApplication::processEvents();

            tempPortrait = QImage(DIR + QString("/chars/") + charName.at(0));
        }

        QPixmap portrait = QPixmap::fromImage(tempPortrait.scaled(28, 38, Qt::KeepAspectRatio,
                                                                  Qt::SmoothTransformation));
        ui->label_char0->setPixmap(portrait);
        ui->label_char0->show();

        //for char1

        tempPortrait = QImage(DIR + QString("/chars/") + charName.at(1));

        if(tempPortrait.isNull()){
            download("/chars/" + charName.at(1), "/chars/" + charName.at(1));
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<500)
                QCoreApplication::processEvents();

            tempPortrait = QImage(DIR + QString("/chars/") + charName.at(1));
        }
        portrait = QPixmap::fromImage(tempPortrait.scaled(28, 38, Qt::KeepAspectRatio,
                                                                  Qt::SmoothTransformation));
        ui->label_char1->setPixmap(portrait);
        ui->label_char1->show();

        //for char2

        tempPortrait = QImage(DIR + QString("/chars/") + charName.at(2));

        if(tempPortrait.isNull()){
            download("/chars/" + charName.at(2), "/chars/" + charName.at(2));
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<500)
                QCoreApplication::processEvents();

            tempPortrait = QImage(DIR + QString("/chars/") + charName.at(2));
        }

        portrait = QPixmap::fromImage(tempPortrait.scaled(28, 38, Qt::KeepAspectRatio,
                                                                  Qt::SmoothTransformation));
        ui->label_char2->setPixmap(portrait);
        ui->label_char2->show();

        //for char3

        tempPortrait = QImage(DIR + QString("/chars/") + charName.at(3));

        if(tempPortrait.isNull()){
            download("/chars/" + charName.at(3), "/chars/" + charName.at(3));
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<500)
                QCoreApplication::processEvents();

            tempPortrait = QImage(DIR + QString("/chars/") + charName.at(3));
        }

        portrait = QPixmap::fromImage(tempPortrait.scaled(28, 38, Qt::KeepAspectRatio,
                                                                  Qt::SmoothTransformation));
        ui->label_char3->setPixmap(portrait);
        ui->label_char3->show();

        //for char4

        tempPortrait = QImage(DIR + QString("/chars/") + charName.at(4));

        if(tempPortrait.isNull()){
            download("/chars/" + charName.at(4), "/chars/" + charName.at(4));
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<500)
                QCoreApplication::processEvents();

            tempPortrait = QImage(DIR + QString("/chars/") + charName.at(4));
        }

        portrait = QPixmap::fromImage(tempPortrait.scaled(28, 38, Qt::KeepAspectRatio,
                                                                  Qt::SmoothTransformation));
        ui->label_char4->setPixmap(portrait);
        ui->label_char4->show();

        //for char5

        tempPortrait = QImage(DIR + QString("/chars/") + charName.at(5));

        if(tempPortrait.isNull()){
            download("/chars/" + charName.at(5), "/chars/" + charName.at(5));
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<500)
                QCoreApplication::processEvents();

            tempPortrait = QImage(DIR + QString("/chars/") + charName.at(5));
        }

        portrait = QPixmap::fromImage(tempPortrait.scaled(28, 38, Qt::KeepAspectRatio,
                                                                  Qt::SmoothTransformation));
        ui->label_char5->setPixmap(portrait);
        ui->label_char5->show();

        //for char6

        tempPortrait = QImage(DIR + QString("/chars/") + charName.at(6));

        if(tempPortrait.isNull()){
            download("/chars/" + charName.at(6), "/chars/" + charName.at(6));
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<500)
                QCoreApplication::processEvents();

            tempPortrait = QImage(DIR + QString("/chars/") + charName.at(6));
        }

        portrait = QPixmap::fromImage(tempPortrait.scaled(28, 38, Qt::KeepAspectRatio,
                                                                  Qt::SmoothTransformation));
        ui->label_char6->setPixmap(portrait);
        ui->label_char6->show();

        processPhase++;
    }

    t.start();
    while(t.elapsed()<300)
        QCoreApplication::processEvents();

    if(processPhase == 3){

        ui->label_Result->setText(
                    ui->tableWidget_history->item(
                        ui->tableWidget_history->currentItem()->row(), 1)->text());

        processPhase = 0;
    }
    progressBar();
}

void MainClient::on_pushButton_8_clicked()
{
    QString number = "";
    number += ui->comboBox_p0->currentText().trimmed();
    number += ui->comboBox_p1->currentText().trimmed();
    number += ui->comboBox_p2->currentText().trimmed();
    number += ui->comboBox_p3->currentText().trimmed();
    number += ui->comboBox_p4->currentText().trimmed();
    number += ui->comboBox_p5->currentText().trimmed();
    number += ui->comboBox_p6->currentText().trimmed();

    QString fileName = ui->tableWidget_history->item(
                ui->tableWidget_history->currentItem()->row(), 0)->text();

    QStringList msg;
    msg.append("manual");
    msg.append(fileName);
    msg.append(number);
    msg.append(ui->tableWidget_history->item(
                   ui->tableWidget_history->currentItem()->row(), 1)->text());
    sendMessage(msg);
}

