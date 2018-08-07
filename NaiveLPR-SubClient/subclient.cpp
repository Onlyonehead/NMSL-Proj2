#include "subclient.h"
#include "ui_subclient.h"

SubClient::SubClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SubClient)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_3->setCurrentIndex(1);

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

    QFont icon_search;
    icon_search.setFamily(fontFamilies.at(0));
    icon_search.setPointSize(24);

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

    ui->label_139->setFont(font);
    ui->label_139->setText(QChar(0xf083));
    ui->label_139->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_140->setFont(font);
    ui->label_140->setText(QChar(0xf5a0));
    ui->label_140->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->icon_search->setFont(icon_search);
    ui->icon_search->setText(QChar(0xf35a));
    ui->icon_search->setStyleSheet("QPushButton{border: 0px; color: rgb(127, 127, 127);} "
                                   "QPushButton:hover{border: 0px; color: rgb(15, 128, 255);} ");

    connect(ui->lineEdit_3, SIGNAL(returnPressed()), ui->pushButton_login, SIGNAL(clicked()), Qt::UniqueConnection);
    connect(ui->lineEdit_4, SIGNAL(returnPressed()), ui->pushButton_login, SIGNAL(clicked()), Qt::UniqueConnection);
    connect(ui->search_A, SIGNAL(returnPressed()), ui->icon_search, SIGNAL(clicked()), Qt::UniqueConnection);

    ui->stackedWidget_2->setVisible(false);
    ui->label_showNewPortraitPath->setVisible(false);

    ui->label_showNewPortrait->installEventFilter(this);
    ui->label_showNewPortrait->setAcceptDrops(true);

    ui->label_preview1->installEventFilter(this);
    ui->label_preview1->setAcceptDrops(true);

    ui->label_previewPath1->setVisible(false);

    ui->progressBar_2->setVisible(false);

    ui->tableWidget_search->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_search->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_search->setAlternatingRowColors(true);


    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("bridge", &bridge);
    url = "http://kousz.top/repo/project2/src/camera_sub.html";

    position.append("");
    position.append("116.416357");
    position.append("39.928353");

    connect(ui->widget_web, SIGNAL(loadFinished(bool)), this, SLOT(slotSendDataToH5(bool)));
    ui->widget_web->page()->setWebChannel(channel);
    ui->widget_web->setUrl(QUrl(url));

    QDateTime current_datetime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(current_datetime);
}

SubClient::~SubClient()
{
    delete ui;
}

void SubClient::on_pushButton_quit_2_clicked()
{
    this->close();
}

void SubClient::on_pushButton_quit_3_clicked()
{
    this->close();
}

void SubClient::slotSendDataToH5(bool ok)
{
    if (ok){
        QJsonArray *array = new QJsonArray();
        QJsonObject o;
        o.insert("name", position.at(0));
        QJsonArray a;
        a.append(position.at(1));
        a.append(position.at(2));
        o.insert("position", QJsonValue(a));
        array->push_back(o);

        emit bridge.signalToWeb(*array);
    }
    else
        qDebug()<<"load failed! ok="<<ok;
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


void SubClient::on_pushButton_login_clicked()
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

void SubClient::on_pushButton_switch_3_clicked()
{
    on_pushButton_switch_clicked();
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_3->setCurrentIndex(1);
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

     move(550,130);

    QPixmap *pixmap = new QPixmap(":/user.png");

    getRoundPixmap(pixmap, QSize(140, 140));
    ui->label_5->setScaledContents(true);
    ui->label_5->setPixmap(*pixmap);
    delete pixmap;

    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_3->setCurrentIndex(1);

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
    if (watched == ui->label_preview1) {
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
                image = image.scaled(212, 212,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
                ui->label_preview1->setPixmap(QPixmap::fromImage(image));
                ui->label_previewPath1->setText(path);
            }

            return true;
        }

        if (event->type() == QEvent::MouseButtonDblClick) {
            QStringList picture = QFileDialog::getOpenFileNames(this, tr("open file"),
                                                                tr("图片文件(*png *jpg)"));
            if(picture.isEmpty()){
                ui->label_preview1->setText("Preview");
                return true;
            }

            QImage tempPortrait(picture.at(0));
            QPixmap portrait = QPixmap::fromImage(tempPortrait.scaled(212, 212, Qt::KeepAspectRatio,
                                                                      Qt::SmoothTransformation));
            ui->label_previewPath1->setText(picture.at(0));
            ui->label_preview1->setPixmap(portrait);
            ui->label_preview1->show();

            return true;
        }
    }

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

void SubClient::on_pushButton_B_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}

void SubClient::on_pushButton_A_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void SubClient::on_pushButton_A_2_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void SubClient::on_pushButton_B_2_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}

void SubClient::on_icon_search_clicked()
{
    QString keyword = ui->search_A->text().trimmed();
    QStringList msg;
    msg.append("camera_search");
    msg.append(keyword);
    sendMessage(msg);
}

void SubClient::on_tableWidget_search_itemClicked(QTableWidgetItem *item){
    Q_UNUSED(item);
    QString id = ui->tableWidget_search->currentItem()->text().split(":")[1].trimmed();
    QString position_x, position_y;

    for(QStringList list : this->camera_search){
        if(list.at(0) == id){
            position_x = list.at(1);
            position_y = list.at(2);
        }
    }

    position.clear();
    position.append(id);
    position.append(position_x);
    position.append(position_y);
    ui->widget_web->setUrl(QUrl(url));

    ui->label_cameraid->setText(id);
    ui->label_position->setText(position_x + "," + position_y);

}

void SubClient::on_pushButton_set1_clicked()
{
    if(ui->label_cameraid->text() == "-"){
        QMessageBox::warning(this,"警告", "\n请选中摄像头",QMessageBox::Close);
        return ;
    }
    ui->lineEdit_checkpoint->setText("Camera ID: " + ui->label_cameraid->text());
}

void SubClient::on_pushButton_set2_clicked()
{

    ui->lineEdit_time->setText(ui->dateTimeEdit->dateTime().toString("yyyy/MM/dd hh:mm:ss"));
}

void SubClient::on_pushButton_clear_clicked()
{
    QDateTime current_datetime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(current_datetime);

    ui->label_preview1->setText("Preview");
    ui->label_previewPath1->clear();
    ui->label_cameraid->setText("-");
    ui->label_position->setText("-");
    ui->lineEdit_checkpoint->clear();
    ui->lineEdit_time->clear();
    ui->tableWidget_search->setRowCount(0);
    ui->search_A->clear();

    position.clear();
    position.append("");
    position.append("116.416357");
    position.append("39.928353");
    ui->widget_web->setUrl(QUrl(url));
}


void SubClient::on_pushButton_confirm_clicked()
{
    if(ui->label_previewPath1->text().trimmed() == ""){
        QMessageBox::warning(this,"警告", "\n请添加图片",QMessageBox::Close);
        return ;
    }


    if(ui->lineEdit_checkpoint->text().trimmed() == ""){
        QMessageBox::warning(this,"警告", "\n请添加出发点",QMessageBox::Close);
        return ;
    }

    if(ui->lineEdit_time->text().trimmed() == ""){
        QMessageBox::warning(this,"警告", "\n请添加到达点",QMessageBox::Close);
        return ;
    }

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString time = current_date_time.toString("yyyy-MM-dd hh-mm-ss");

    QString picName = time + ".png";

    QString picPath = ui->label_previewPath1->text();

    QBuffer buffer;
    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    buffer.open(QIODevice::ReadWrite);
    QImage img;
    img.load(picPath);
    img.save(&buffer,"png");
    out << qint32(buffer.size());
    out << QString("/plates/" + picName);
    out << buffer.data();

    m_socket->write(message);
    m_socket->flush();

    QElapsedTimer t;
    t.start();
    while(t.elapsed()<500)
        QCoreApplication::processEvents();


    QStringList list;

    list.append("ETC");
    list.append(picName);
    QString id = ui->lineEdit_checkpoint->text().split(":")[1].trimmed();
    for(QStringList l : camera_search){
        if(l.at(0) == id){
            list.append(l.at(1) + "," + l.at(2));
            break;
        }
    }
    QStringList tmp = ui->lineEdit_time->text().split(QRegExp("[/:]"));
    time = tmp.at(0) + "-" + tmp.at(1) + "-" + tmp.at(2) + "-" + tmp.at(3) + "-" + tmp.at(4);
    list.append(time);
    sendMessage(list);
}
