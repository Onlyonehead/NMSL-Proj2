#include "mainclient.h"
#include "ui_mainclient.h"

void MainClient::on_pushButton_10_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("open file"), "/Users/Haibara/Desktop");
    if(!dir.isEmpty()){

        this->dirForImage = dir;

        ui->pushButton_10->setStyleSheet("QPushButton{"
                                         "border: 2px solid rgb(15, 128, 255);"
                                         "background:rgba(15, 128, 255, 93);"
                                         "border-radius:10px;"
                                         "color: rgb(76, 76, 76);"
                                         "font: 75 14pt \"Helvetica\" bold;}"
                                         "QPushButton:hover{"
                                         "border: 2px solid rgb(15, 128, 255);"
                                         "background:rgba(15, 128, 255, 43);"
                                         "border-radius:10px;"
                                         "color: rgb(76, 76, 76);"
                                         "font: 75 15pt \"Helvetica\" bold;}");
    }
}

void MainClient::on_pushButton_9_clicked()
{
    QString model = ui->comboBox_model->currentText().trimmed();
    qDebug() << model;
    QStringList msg;
    msg.append("setModel");
    msg.append(model);
    sendMessage(msg);

    QFile *file = new QFile(MODEL_DIR);
    if (file->open(QFile::ReadWrite | QFile::Text)){
        QTextStream out(file);
        out << model << "\n";
    }else{
        qDebug()<<"打开失败";
    }
    file->close();
}

void MainClient::on_pushButton_generate_clicked()
{
    ui->pushButton_10->setStyleSheet("QPushButton{"
                                     "border: 2px solid rgb(179, 179, 179);"
                                     "background:rgba(248, 248, 248,220);"
                                     "border-radius:10px;"
                                     "color: rgb(76, 76, 76);"
                                     "font: 75 14pt \"Helvetica\" bold;}"
                                     "QPushButton:hover{"
                                     "border: 2px solid rgb(15, 128, 255);"
                                     "background:rgba(15, 128, 255, 43);"
                                     "border-radius:10px;"
                                     "color: rgb(76, 76, 76);"
                                     "font: 75 15pt \"Helvetica\" bold;}");

    QString dir = this->dirForImage;
    int h_layer = ui->lineEdit_Hlayer->text().trimmed().toInt();
    int iter = ui->lineEdit_iter->text().trimmed().toInt();
    double thresholdValue = ui->lineEdit_threshold->text().trimmed().toDouble();

    if(dir.trimmed() == ""){
        QMessageBox::warning(this,"警告", "\n请选择文件夹！",QMessageBox::Close);
        return ;
    }

    QApplication::processEvents();
    ui->progressBar->setVisible(true);

    QApplication::processEvents();
    ui->progressBar->setRange(0, 100);

    QApplication::processEvents();
    QApplication::processEvents();
    ui->progressBar->setValue(10);
    QApplication::processEvents();
    QApplication::processEvents();
    QApplication::processEvents();
    QApplication::processEvents();

    CharRecognition::ANN_Train(dir.toStdString(), h_layer, iter, thresholdValue);

    QApplication::processEvents();
    ui->progressBar->setValue(40);
    QApplication::processEvents();

    CharRecognition::ANN_Train_CH(dir.toStdString(), h_layer, iter, thresholdValue);

    QApplication::processEvents();
    ui->progressBar->setValue(70);
    QApplication::processEvents();

    CharRecognition::ANN_Train_SP(dir.toStdString(), h_layer, iter, thresholdValue);

    QApplication::processEvents();
    ui->progressBar->setValue(90);
    QApplication::processEvents();

    QStringList msg;
    msg.append("trainModel");
    msg.append(dir);
    sendMessage(msg);

    ui->progressBar->setValue(100);
    QApplication::processEvents();

    ui->progressBar->setVisible(false);


}

void MainClient::on_icon_search_clicked()
{
    QString keyword = ui->search_A->text().trimmed();
    QStringList msg;
    msg.append("lpr_search");
    msg.append(keyword);
    sendMessage(msg);
}

void MainClient::on_tableWidget_search_itemClicked(QTableWidgetItem *item){
    Q_UNUSED(item);
    QString plate = ui->tableWidget_search->currentItem()->text().trimmed();
    QStringList dirs;

    for(QStringList list : this->lpr_search){
        if(list.at(1) == plate){
            dirs.append(list.at(0));
        }
    }

    QGridLayout  *gLayout = new QGridLayout();
    gLayout->setSpacing(10);
    gLayout->setMargin(10);
    for(int i = 0; i < dirs.size(); ++i){
        QLabel *label = new QLabel(this);

        QString plateDName = dirs.at(i) + "-plate.jpg";

        QImage tempPortrait(DIR + QString("/plates-d/") + plateDName);

        if(tempPortrait.isNull()){
            download("/plates-d/" + plateDName, "/plates-d/" + plateDName);
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<500)
                QCoreApplication::processEvents();

            tempPortrait = QImage(DIR + QString("/plates-d/") + plateDName);
        }

        QPixmap pix = QPixmap::fromImage(tempPortrait.scaled(135, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        label->setPixmap(pix);
        label->setStyleSheet("border:none;border-radius:0px;background:white;");
        gLayout->addWidget(label, i/2, i%2);
    }
    QWidget *widget = new QWidget;
    widget->setLayout(gLayout);
    ui->scrollArea->setWidget(widget);
    ui->scrollArea->setWidgetResizable(true);
}

void MainClient::on_pushButton_12_clicked()
{
    if(ui->tableWidget_search->currentItem() == NULL){
        QMessageBox::warning(this,"警告", "\n请选中条目！",QMessageBox::Close);
        return ;
    }
    QString dir = QFileDialog::getExistingDirectory(this, tr("open file"), "/Users/Haibara/Desktop");
    if(!dir.isEmpty()){
        QString plate = ui->tableWidget_search->currentItem()->text().trimmed();
        QStringList dirs;

        for(QStringList list : this->lpr_search){
            if(list.at(1) == plate){
                dirs.append(list.at(0));
            }
        }
        for(int i = 0; i < dirs.size(); ++i){
            QString plateDName = dirs.at(i) + "-plate.jpg";

            Mat m = imread(DIR + string("/plates-d/") + plateDName.toStdString(), 1);

            QDir d(dir);
            if (!d.exists(plate))
            {
                d.mkpath(plate);
            }
            imwrite(dir.toStdString() + "/" + plate.toStdString() +
                    "/" + plateDName.toStdString(), m);
        }

        QMessageBox::information(this,"提示", "\n导出成功！",QMessageBox::Ok);

    }

}

void MainClient::on_pushButton_11_clicked()
{
    if(ui->tableWidget_search->currentItem() == NULL){
        QMessageBox::warning(this,"警告", "\n请选中条目！",QMessageBox::Close);
        return ;
    }
    progressBar();
    progressBar();
    progressBar();
    progressBar();
    progressBar();
}



