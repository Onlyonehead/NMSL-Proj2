#include "subclient.h"
#include "ui_subclient.h"

SubClient::SubClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SubClient)
{
    ui->setupUi(this);

    m_tcpsocket = new QTcpSocket;
    m_tcpsocket->connectToHost(QHostAddress::LocalHost,8848);//设置客户端的端口号
    connect(m_tcpsocket,SIGNAL(readyRead()),
            this,SLOT(readMessage()));//用于接受数据

    m_socket = new QTcpSocket;
    m_socket->connectToHost(QHostAddress::LocalHost, 7777);
    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(replyFinished()));

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("QFrame{border-radius:10px;}");

    move(550,130);

    ui->lineEdit_4->setFocus();

    QFile file(LOGIN_DIR);
    if(file.open(QIODevice::ReadOnly)){
        QString tmp_username, tmp_password;
        tmp_username = file.readLine().trimmed();
        tmp_password = file.readLine().trimmed();
        ui->lineEdit_4->setText(tmp_username);
        ui->lineEdit_3->setText("******");
        ui->lineEdit_3->setTextMargins(13,0,10,0);
        ui->label_pass->setText(tmp_password);
        ui->checkBox_2->setChecked(true);
        ui->lineEdit_3->setFocus();
    }
    file.close();


    QPixmap *pixmap = new QPixmap(":/user.png");

    getRoundPixmap(pixmap, QSize(140, 140));
    ui->label_5->setScaledContents(true);
    ui->label_5->setPixmap(*pixmap);
    delete pixmap;

    QApplication::processEvents();

    int fontId = QFontDatabase::addApplicationFont(":/font/fa-solid-900.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    QFont font;
    font.setFamily(fontFamilies.at(0));
    font.setPointSize(15);
    ui->label->setFont(font);
    ui->label->setText(QChar(0xf007));
    ui->label->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");
    ui->label_9->setFont(font);
    ui->label_9->setText(QChar(0xf530));
    ui->label_9->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");
    ui->label_10->setFont(font);
    ui->label_10->setText(QChar(0xf182));
    ui->label_10->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");
    ui->label_13->setFont(font);
    ui->label_13->setText(QChar(0xf2b6));
    ui->label_13->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_19->setFont(font);
    ui->label_19->setText(QChar(0xf5ab));
    ui->label_19->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");
    ui->label_20->setFont(font);
    ui->label_20->setText(QChar(0xf577));
    ui->label_20->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_21->setFont(font);
    ui->label_21->setText(QChar(0xf03e));
    ui->label_21->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_23->setFont(font);
    ui->label_23->setText(QChar(0xf02f));
    ui->label_23->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_25->setFont(font);
    ui->label_25->setText(QChar(0xf200));
    ui->label_25->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->stackedWidget->setCurrentIndex(1);

    connect(ui->lineEdit_3, SIGNAL(returnPressed()), ui->pushButton_switch_2, SIGNAL(clicked()), Qt::UniqueConnection);

    ui->stackedWidget_2->setVisible(false);
    ui->label_showNewPortraitPath->setVisible(false);

    ui->label_showNewPortrait->installEventFilter(this);
    ui->label_showNewPortrait->setAcceptDrops(true);

    ui->progressBar_2->setVisible(false);
}

SubClient::~SubClient()
{
    delete ui;
}

void SubClient::on_pushButton_quit_2_clicked()
{
    this->close();
}

/**
 * shape picture to circle
 *
 * @author hzc
 * @param s1 username
 * @param s2 name
 * @param s3 position
 * @param s4 gender
 * @param s5 email
 */
void SubClient::getRoundPixmap(QPixmap* src,QSize size)

{

    QImage resultImage(size,QImage::Format_ARGB32_Premultiplied);
    QPixmap head_mask(":/mask.png");
    QPainter painter(&resultImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(resultImage.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(0, 0, head_mask);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOut);
    painter.drawPixmap(0, 0, src->scaled(size));
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.end();
    *src = QPixmap::fromImage(resultImage);
}


/**
 * download picture
 *
 * @author hzc
 * @param surl pic url
 * @param filePath local file path
 */
void SubClient::download(const QString &url, const QString &filePath)
{

    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out << (quint32) (sizeof(QString));
    out << url;
    out.device()->seek(0);
    m_socket->write(message);

    this->filepath = filePath;

}

/**
 * save picture
 *
 * @author hzc
 * @param reply NetworkReply
 */
void SubClient::replyFinished()
{
    QDataStream in(m_socket);
    in.setVersion (QDataStream::Qt_5_7);

    if(dataSize == 0)
    {
       if(m_socket->bytesAvailable() < (qint32)(sizeof(quint32)+sizeof(QString)) )
       {
            return;
       }
       in >> dataSize;
       in >> fileName;
    }

    if(dataSize > qint32(m_socket->bytesAvailable()))
    {
       return;
    }

    QByteArray data;

    in>>data;

    QImage img;
    img.loadFromData(data);

    img.save(DIR + fileName);

    dataSize = 0;
    fileName.clear();
}


void SubClient::on_pushButton_switch_2_clicked()
{
    QString username = ui->lineEdit_4->text().trimmed();
    QString password = (ui->lineEdit_3->text() == "******") ?
                ui->label_pass->text() : ui->lineEdit_3->text();
    QFile file(LOGIN_DIR);
    if(file.open(QIODevice::ReadOnly)){
        QString tmp_username, tmp_password;
        tmp_username = file.readLine().trimmed();
        tmp_password = file.readLine().trimmed();
        if(tmp_username != username){
            file.remove();
            if(tmp_password == password){
                ui->lineEdit_3->clear();
                QMessageBox::warning(this,"警告", "\n请重新输入密码！",QMessageBox::Close);
                return ;
            }
        }

    }
    file.close();

    QStringList list;
    list.append("login");
    list.append(username);
    list.append(password);
    sendMessage(list);
}


/**
 * check pwd.data and auto save password to file
 *
 * @author hzc
 * @param username username
 * @param passwordEntered password entered by user
 * @param status check status of checkbox
 */
int SubClient::checkpass(QString username, QString passwordEntered, bool status){
    if(status){
        QFile *file = new QFile(LOGIN_DIR);
        if(!file->exists()){
            if (file->open(QFile::ReadWrite | QFile::Text)){
                QByteArray bytePwd = passwordEntered.toLatin1();
                QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
                QString strPwdMd5 = bytePwdMd5.toHex();
                QTextStream out(file);
                out << username << "\n";
                out << strPwdMd5;
            }else{
                qDebug()<<"打开失败";
                return -1;
            }
        }
        file->close();
    }else{
        QFile file(LOGIN_DIR);
        if (file.exists()){
            file.remove();
        }
        file.close();
    }
    return 0;

}


/**
 * tool method
 *
 * @author hzc
 * @param username username
 */
void SubClient::sendSignalForLogin(QString username){
    QStringList list;
    list.append("info");
    list.append(username);
    sendMessage(list);
}

void SubClient::on_pushButton_switch_clicked()
{

    ui->frame->setStyleSheet("QFrame{background: rgba(255, 255, 255, 0);}");
    ui->stackedWidget_2->setVisible(false);

    QApplication::processEvents();
    QApplication::processEvents();
//    for(int i = 250; i <= 260; i+=1){
//        QApplication::processEvents();
//        move(i,130);
//        QApplication::processEvents();
//    }

//    for(int i = 260; i <= 280; i+=2){
//        QApplication::processEvents();
//        move(i,130);
//        QApplication::processEvents();
//    }

//    for(int i = 280; i <= 550; i+=5){
//        QApplication::processEvents();
//        move(i,130);
//        QApplication::processEvents();
//    }

     move(550,130);

    QPixmap *pixmap = new QPixmap(":/user.png");

    getRoundPixmap(pixmap, QSize(140, 140));
    ui->label_5->setScaledContents(true);
    ui->label_5->setPixmap(*pixmap);
    delete pixmap;

    ui->stackedWidget->setCurrentIndex(1);

    ui->label_2->clear();
    ui->label_4->clear();
    ui->label_8->clear();
    ui->label_12->clear();
    ui->label_14->clear();

    ui->label_2->setText("Please Login");

    ui->frame_2->setStyleSheet("QFrame{"
                               "background: rgba(248, 248, 248, 245);"
                               "border-image:none;"
                               "border: 2px solid rgb(200, 200, 200);}");

}

void SubClient::on_pushButton_quit_clicked()
{
    this->close();
}

void SubClient::on_pushButton_addNewPortrait_clicked()
{
    QStringList picture = QFileDialog::getOpenFileNames(this, tr("open file"), "/Users/Haibara/Desktop",
                                                        tr("图片文件(*png *jpg)"));
    if(picture.isEmpty()){
        return ;
    }

    QImage tempPortrait(picture.at(0));
    QPixmap portrait = QPixmap::fromImage(tempPortrait.scaled(432, 392, Qt::KeepAspectRatio,
                                                              Qt::SmoothTransformation));
    ui->label_showNewPortraitPath->setText(picture.at(0));
    ui->label_showNewPortrait->setPixmap(portrait);
    ui->label_showNewPortrait->show();
}

void SubClient::on_pushButton_confirmNewStaff_clicked()
{

    if(ui->label_showNewPortraitPath->text().trimmed() == ""){
        QMessageBox::warning(this,"警告", "\n请添加图片",QMessageBox::Close);
        return ;
    }
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString time = current_date_time.toString("yyyy-MM-dd hh-mm-ss");

    QString picName = time + ".png";

    QString picPath = ui->label_showNewPortraitPath->text();

    QBuffer buffer;
    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    buffer.open(QIODevice::ReadWrite);
    QImage img;
    img.load(picPath);
    img.save(&buffer,"png");
    out << qint32(buffer.size());
    out << QString("/plates/" + picName.split(".").at(0) + ".png");
    out << buffer.data();

    m_socket->write(message);
    m_socket->flush();

    QElapsedTimer t;
    t.start();
    while(t.elapsed()<500)
        QCoreApplication::processEvents();

    QStringList list;

    list.append("LPR");
    list.append(picName);
    sendMessage(list);


}


bool SubClient::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->label_showNewPortrait) {
        if (event->type() == QEvent::DragEnter) {

            QDragEnterEvent *dee = dynamic_cast<QDragEnterEvent *>(event);
            dee->acceptProposedAction();
            return true;
        } else if (event->type() == QEvent::Drop) {

            QDropEvent *de = dynamic_cast<QDropEvent *>(event);
            QList<QUrl> urls = de->mimeData()->urls();

            if (urls.isEmpty()) { return true; }
            QString path = urls.first().toLocalFile();

            QImage image(path);
            if (!image.isNull()) {
                image = image.scaled(432, 392,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
                ui->label_showNewPortrait->setPixmap(QPixmap::fromImage(image));
                ui->label_showNewPortraitPath->setText(path);
            }

            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void SubClient::on_pushButton_confirmNewStaff_2_clicked()
{
    ui->label_showNewPortrait->setText("Preview");
    ui->label_showNewPortraitPath->clear();
    ui->label_showNumber->clear();
    ui->label_showPlate->clear();
    ui->label_showProbability->clear();
}

/**
 * progress bar
 *
 * @author hzc
 */
void SubClient::progressBar()
{
    ui->progressBar_2->setVisible(true);
    ui->progressBar_2->setRange(0, 100);
    for (int i = 0; i <= 100; i++){
        for(int j = 0; j < 1000000; j++){
            i = i + 1;
            i = i - 1;
        }
        ui->progressBar_2->setValue(i);
        QApplication::processEvents();
    }
    ui->progressBar_2->setVisible(false);
}

/**
 * fast version progress bar
 *
 * @author hzc
 */
void SubClient::progressBar_fast()
{
    ui->progressBar_2->setVisible(true);
    ui->progressBar_2->setRange(0, 100);
    for (int i = 0; i <= 100; i++){
        for(int j = 0; j < 300000; j++){
            i = i + 1;
            i = i - 1;
        }
        ui->progressBar_2->setValue(i);
        QApplication::processEvents();
    }
    ui->progressBar_2->setVisible(false);
}


void SubClient::on_pushButton_EnterhighwayPic_clicked()
{
    QStringList enterHighwayPic = QFileDialog::getOpenFileNames(this, tr("open file"), "/Users/Haibara/Desktop",
                                                                   tr("图片文件(*png *jpg)"));

       if(enterHighwayPic.isEmpty()){
           return;
       }

       ui->label_EnterPicPath->clear();
       ui->label_vehicleFromPic->clear();
       QImage tempEnterPic(enterHighwayPic.at(0));
       QPixmap EnterPic = QPixmap::fromImage(tempEnterPic.scaled(200, 200, Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation));
       ui->label_EnterPicPath->setText(enterHighwayPic.at(0));
       ui->label_vehicleFromPic->setPixmap(EnterPic);
       ui->label_vehicleFromPic->show();

}

void SubClient::on_pushButton_leaveHighwayPic_clicked()
{
    QStringList leaveHighwayPic = QFileDialog::getOpenFileNames(this, tr("open file"), "/Users/Haibara/Desktop",
                                                                tr("图片文件(*png *jpg)"));
    if(leaveHighwayPic.isEmpty()){
        return;
    }

    ui->label_LeavePicPath->clear();
    ui->label_vehicleTarPic->clear();
    QImage tempLeavePic(leaveHighwayPic.at(0));
    QPixmap LeavePic = QPixmap::fromImage(tempLeavePic.scaled(200, 200, Qt::KeepAspectRatio,
                                                              Qt::SmoothTransformation));
    ui->label_LeavePicPath->setText(leaveHighwayPic.at(0));
    ui->label_vehicleTarPic->setPixmap(LeavePic);
    ui->label_vehicleTarPic->show();

}

void SubClient::on_pushButton_confirmEnterHighwayPic_clicked()
{
    if(ui->label_EnterPicPath->text().isEmpty()){
        QMessageBox::warning(this, tr("warning"), tr("\n未选择图片！"), QMessageBox::Close, QMessageBox::Close);
    }

    QString set_off_coordinate = ui->label_enterPicCoordinate->text();
    QString enter_highway_datetime = ui->dateTimeEdit_vehicleFromDatetime->dateTime().toString();
    QString enterHighwayPicName = enter_highway_datetime + ".png";

    QString EnterPicPath = ui->label_EnterPicPath->text();

    QBuffer buffer;
    QByteArray message;
    QDataStream out(&message, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    buffer.open(QIODevice::ReadWrite);
    QImage img;
    img.load(EnterPicPath);
    img.save(&buffer, "png");
    out << qint32(buffer.size());
    out << QString("/plates/" + enterHighwayPicName.split(".").at(0) + ".png");
    out << buffer.data();

    m_socket->write(message);
    m_socket->flush();

    QElapsedTimer t;
    t.start();
    while (t.elapsed() < 500) {
        QCoreApplication::processEvents();
    }
    QStringList list;
    list.append("ETCp_sendEnterPic");
    list.append(enterHighwayPicName);
    list.append(set_off_coordinate);
    sendMessage(list);

}

void SubClient::on_pushButton_confirmLeaveHighwayPic_clicked()
{
    if(ui->label_LeavePicPath->text().isEmpty()){
        QMessageBox::warning(this, tr("warning"), tr("\n未选择图片！"), QMessageBox::Close, QMessageBox::Close);
    }
    QString leave_highway_datetime = ui->dateTimeEdit_vehicleTarDateTime->dateTime().toString();
    QString leaveHighwayPicName = leave_highway_datetime + ".png";

    QString LeavePicPath = ui->label_LeavePicPath->text();
    QString destination_coordinate = ui->label_leavePicCoordinate->text();

    QBuffer buffer;
    QByteArray message;
    QDataStream out(&message, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    buffer.open(QIODevice::ReadWrite);
    QImage img;
    img.load(LeavePicPath);
    img.save(&buffer, "png");
    out << qint32(buffer.size());
    out << QString("/plates/" + leaveHighwayPicName.split(".").at(0) + "png");
    out << buffer.data();

    m_socket->write(message);
    m_socket->flush();

    QElapsedTimer t;
    t.start();
    while (t.elapsed() < 500) {
        QCoreApplication::processEvents();
    }
    QStringList list;
    list.append("ETCp_sendLeavePic");
    list.append(leaveHighwayPicName);
    list.append(destination_coordinate);
    sendMessage(list);

}
