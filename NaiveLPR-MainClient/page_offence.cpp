#include "mainclient.h"
#include "ui_mainclient.h"
#include "videosurface.h"

#include <QEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileDialog>
#include <QDebug>
#include <QPalette>
#include <QTime>


void MainClient::on_pb_selectVideos_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this,tr("选择视频"),
                "c:/users/cc/desktop",
                tr("视频文件(*mp4 *avi)"));

    if(path != ""){
        QStringList splits = path.split("/");
        QString fileName = splits.at(splits.size()-1);

        int i;
        for(i=0; i<ui->tw_videos->rowCount(); ++i){
            if(ui->tw_videos->item(i, 0)->text() == fileName)
                break;
        }

        if(ui->tw_videos->rowCount()==0||i==ui->tw_videos->rowCount()){
            qm_videos.insert(fileName, path);

            ui->tw_videos->setRowCount(ui->tw_videos->rowCount()+1);
            ui->tw_videos->setItem(ui->tw_videos->rowCount()-1, 0, new QTableWidgetItem(fileName));
        }
    }
}

void MainClient::getPointStart(QPoint p){
    pStart.setX(p.x());
    pStart.setY(p.y());
    ui->dsb_x->setValue((double)pStart.x()/ui->label_paint->width());
    ui->dsb_y->setValue((double)pStart.y()/ui->label_paint->height());
}

void MainClient::getPointEnd(QPoint p){
    pEnd.setX(p.x());
    pEnd.setY(p.y());
    ui->dsb_x2->setValue((double)pEnd.x()/ui->label_paint->width());
    ui->dsb_y2->setValue((double)pEnd.y()/ui->label_paint->height());
    ui->dsb_width->setValue(qAbs((double)pStart.x()-(double)pEnd.x())/ui->label_paint->width());
    ui->dsb_height->setValue(qAbs((double)pStart.y()-(double)pEnd.y())/ui->label_paint->height());

    ui->label_selectArea->setText("Selected Area:"+QString::number(ui->dsb_width->value()*ui->dsb_width->value()*video_width*video_height, 10, 0));
}


void MainClient::on_pb_clearRect_clicked()
{
    emit clearP();
    ui->dsb_x->setValue(0);
    ui->dsb_y->setValue(0);
    ui->dsb_x2->setValue(1);
    ui->dsb_y2->setValue(1);
    ui->dsb_width->setValue(1);
    ui->dsb_height->setValue(1);
}

void MainClient::on_tw_videos_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    if(row>=0){
        if(if_index)
            delete indexPlayer;
        if_index = true;
        indexPlayer = new QMediaPlayer;
        VideoSurface *videoSurface = new VideoSurface;
        connect(videoSurface, SIGNAL(frameAvailable(QVideoFrame &)), this, SLOT(ProcessFrame(QVideoFrame &)));
        indexPlayer->setVideoOutput(videoSurface);
        QString localPath = qm_videos.value(ui->tw_videos->item(row, 0)->text());
        indexPlayer->setMedia(QUrl::fromLocalFile(localPath));
        indexPlayer->play();

        ui->label_selectedVideo->setText(ui->tw_videos->item(row, 0)->text());
        ui->pb_sendRequest->setEnabled(true);
        ui->horizontalSlider_up->setEnabled(true);
        ui->horizontalSlider_down->setEnabled(true);

        ui->dsb_x->setValue(0);
        ui->dsb_y->setValue(0);
        ui->dsb_x2->setValue(1);
        ui->dsb_y2->setValue(1);
        ui->dsb_width->setValue(1);
        ui->dsb_height->setValue(1);
    }
}

void MainClient::on_pb_open_clicked()
{
    //选择视频文件
    QString filename = QFileDialog::getOpenFileName(this,tr("选择视频文件"),"c:/users/cc/desktop",tr("视频格式(*.avi *.mp4 *.flv *.mkv)"));
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL, "Title", "Content", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if(if_reload)
    {//重新加载视频时，重置以下变量
        delete layout_video;
        delete player;
        delete widget;
        delete timer;
    }
    if_reload = true;

    //实例化三个全局变量
    layout_video = new QVBoxLayout;
    player = new QMediaPlayer;
    widget = new QVideoWidget;
    //设置视频播放区域与Label的边距
    layout_video->setMargin(1);
    //根据label_player尺寸设置播放区域
    widget->resize(ui->label_player->size());
    layout_video->addWidget(widget);
    ui->label_player->setLayout(layout_video);
    player->setVideoOutput(widget);

    //设置播放器
    player->setMedia(QUrl::fromLocalFile(filename));
    //play_state为true表示播放，false表示暂停
    play_state = true;
    //启用播放/暂停按钮，并将其文本设置为“暂停”
    ui->pb_play->setEnabled(true);
    ui->pb_play->setText("pause");
    //播放器开启
    player->play();

    //启用进度条
    ui->slider_progress->setEnabled(true);
    ui->slider_progress->setRange(0,maxValue);

    timer = new QTimer();
    timer->setInterval(100);//如果想看起来流畅些，可以把时间间隔调小，如100ms
    timer->start();
    //将timer连接至onTimerOut槽函数
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
}

void MainClient::on_pb_play_clicked()
{
    if(play_state)
    {
        player->pause();
        ui->pb_play->setText("play");
    }
    else
    {
        player->play();
        ui->pb_play->setText("pause");
    }

    play_state = !play_state;
}

void MainClient::onTimerOut()
{
    ui->slider_progress->setValue(player->position()*maxValue/player->duration());
}

void MainClient::on_pb_deselectVideo_clicked()
{
    ui->label_board->clear();
    ui->label_selectedVideo->setText("");
    ui->pb_sendRequest->setEnabled(false);

    ui->dsb_x->setValue(0);
    ui->dsb_y->setValue(0);
    ui->dsb_x2->setValue(1);
    ui->dsb_y2->setValue(1);
    ui->dsb_width->setValue(1);
    ui->dsb_height->setValue(1);

    ui->horizontalSlider_up->setValue(100);
    ui->horizontalSlider_up->setEnabled(false);
    ui->horizontalSlider_down->setValue(0);
    ui->horizontalSlider_down->setEnabled(false);
    ui->label_scale->setText("");

    ui->label_width->setText("");
    ui->label_height->setText("");
    ui->label_fps->setText("");

}

void MainClient::on_pushButton_clicked()
{
    ui->sb_minArea->setValue(350);
    ui->sb_minDetect->setValue(800);
    ui->sb_initFrame->setValue(30);
    ui->sb_fMax->setValue(10);
    ui->dsb_shadow->setValue(0.7);
    ui->sb_mCore->setValue(6);
}

void MainClient::slider_progress_clicked()
{
    player->setPosition(ui->slider_progress->value()*player->duration()/maxValue);
}

void MainClient::slider_progress_moved()
{
    //暂时停止计时器，在用户拖动过程中不修改slider的值
    timer->stop();
    player->setPosition(ui->slider_progress->value()*player->duration()/maxValue);
}

void MainClient::slider_progress_released()
{
    //用户释放滑块后，重启定时器
    timer->start();
}


void MainClient::on_pb_sendRequest_clicked()
{

//    QGridLayout  *gLayout = new QGridLayout();
//    gLayout->setSpacing(15);
//    gLayout->setMargin(15);
//    for(int i=0; i<45; ++i){
//        QLabel *label = new QLabel(this);
//        QImage image(QString("C:/Users/CC/Desktop/videos/car highwayII/")+QString::number(i+1, 10)+QString(".jpg"));
//        QPixmap pix = QPixmap::fromImage(image.scaled(105, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//        label->setPixmap(pix);
//        gLayout->addWidget(label, i/2, i%2);
//    }
//    QWidget *widget = new QWidget;
//    widget->setLayout(gLayout);
//    ui->scrollArea_carPics->setWidget(widget);
//    ui->scrollArea_carPics->setWidgetResizable(true);

//    QPixmap *p = new QPixmap(QString("C:/Users/CC/Desktop/videos/car highwayII/")+QString("highwayII_raw_back.jpg"));
//    ui->label_backGround->setPixmap(*p);


    double sx, sy, x ,y, x2, y2, width, height, shadow;
    int minArea, minDetect, initFrame, fMax, mCore;
    QString path;

    x = ui->dsb_x->value();
    y = ui->dsb_y->value();
    x2 = ui->dsb_x2->value();
    y2 = ui->dsb_y2->value();
    sx = x<x2 ? x : x2;
    sy = y<y2 ? y : y2;

    width = ui->dsb_width->value();
    height = ui->dsb_height->value();

    shadow = ui->dsb_shadow->value();
    minArea = ui->sb_minArea->value();
    minDetect = ui->sb_minDetect->value();
    initFrame = ui->sb_initFrame->value();
    fMax = ui->sb_fMax->value();
    mCore = ui->sb_mCore->value();

    path = qm_videos.value(ui->label_selectedVideo->text());


    QString filename;
    if(width*height!=0){//车流量监测
        filename = "C://Users//CC//Desktop//videos//result//car highwayII//highwayII_trace.avi";

        QGridLayout  *gLayout = new QGridLayout();
        gLayout->setSpacing(10);
        gLayout->setMargin(10);

        QLabel *label_cars = new QLabel(this);
        QLabel *label_cars_num = new QLabel(this);
        QLabel *label_velocity = new QLabel(this);
        QLabel *label_velocity_num = new QLabel(this);
        label_cars->setText("cars:");
        label_cars_num->setText("45");
        label_velocity->setText("velocity:");
        label_velocity_num->setText("30km/h");
        gLayout->addWidget(label_cars, 0, 0);
        gLayout->addWidget(label_cars_num, 0, 1);
        gLayout->addWidget(label_velocity, 1, 0);
        gLayout->addWidget(label_velocity_num, 1, 1);

        QWidget *widget = new QWidget;
        widget->setLayout(gLayout);
        ui->scrollArea_operateResult->setWidget(widget);
        ui->scrollArea_operateResult->setWidgetResizable(true);
    }else {//闯红灯检测
        filename = "C://Users//CC//Desktop//videos//result//car picsII//highwayII_raw_trace.avi";

        double up, down;
        long time[2];
        up = ui->horizontalSlider_up->value();
        down = ui->horizontalSlider_down->value();
        time[0] = duration*(up<down?up:down)/100;
        time[1] = duration*(down>=up?down:up)/100;


        //hanshu

        QGridLayout  *gLayout = new QGridLayout();
        gLayout->setSpacing(10);
        gLayout->setMargin(10);

        QLabel *label_peccancy = new QLabel(this);
        QLabel *label_peccancy_num = new QLabel(this);
        label_peccancy->setText("peccancy:");
        label_peccancy_num->setText("8");
        gLayout->addWidget(label_peccancy, 0, 0);
        gLayout->addWidget(label_peccancy_num, 0, 1);

        QWidget *widget = new QWidget;
        widget->setLayout(gLayout);
        ui->scrollArea_operateResult->setWidget(widget);
        ui->scrollArea_operateResult->setWidgetResizable(true);
    }

    //视频播放
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL, "Title", "Content", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if(if_reload)
    {//重新加载视频时，重置以下变量
        delete layout_video;
        delete player;
        delete widget;
        delete timer;
    }
    if_reload = true;

    //实例化三个全局变量
    layout_video = new QVBoxLayout;
    player = new QMediaPlayer;
    widget = new QVideoWidget;
    //设置视频播放区域与Label的边距
    layout_video->setMargin(1);
    //根据label_player尺寸设置播放区域
    widget->resize(ui->label_player->size());
    layout_video->addWidget(widget);
    ui->label_player->setLayout(layout_video);
    player->setVideoOutput(widget);

    //设置播放器
    player->setMedia(QUrl::fromLocalFile(filename));
    //play_state为true表示播放，false表示暂停
    play_state = true;
    //启用播放/暂停按钮，并将其文本设置为“暂停”
    ui->pb_play->setEnabled(true);
    ui->pb_play->setText("pause");
    //播放器开启
    player->play();

    //启用进度条
    ui->slider_progress->setEnabled(true);
    ui->slider_progress->setRange(0,maxValue);

    timer = new QTimer();
    timer->setInterval(100);//如果想看起来流畅些，可以把时间间隔调小，如100ms
    timer->start();
    //将timer连接至onTimerOut槽函数
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
}

void MainClient::ProcessFrame(QVideoFrame &frame)
{
    duration = indexPlayer->duration();
    qDebug()<<"duration:"<<duration;

    qDebug() << "=============ProcessFrame===============";
    qDebug() << "width : " << frame.width() << " height : " << frame.height();
    qDebug() << "start time : " << frame.startTime()/1000 << "ms";
    qDebug() << "end time : " << frame.endTime()/1000 << "ms";
    qDebug() << "pixelFormat :" << frame.pixelFormat();

    video_width = frame.width();
    video_height = frame.height();
    ui->label_width->setText("width:"+QString::number(frame.width(), 10));
    ui->label_height->setText("height:"+QString::number(frame.height(), 10));
    ui->label_fps->setText("fps:"+QString::number((1000000/((double)frame.endTime()-(double)frame.startTime())), 10, 2));

    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage recvImage(frame.bits(), frame.width(), frame.height(), QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
    ui->label_board->clear();
    ui->label_board->setPixmap(QPixmap::fromImage(recvImage));
    qDebug() << "frame data size :" << frame.mappedBytes();
    frame.unmap();
    indexPlayer->stop();
}

void MainClient::on_horizontalSlider_up_valueChanged(int value)
{
    Q_UNUSED(value);

    showSelectedTime();
}

void MainClient::on_horizontalSlider_down_valueChanged(int value)
{
    Q_UNUSED(value);

    showSelectedTime();
}

void MainClient::showSelectedTime(){
    double up = ui->horizontalSlider_up->value();
    double down = ui->horizontalSlider_down->value();
    double maxValue = ui->horizontalSlider_up->maximum();

    qint64 time_l;
    qint64 time_s;
    (up>down)?(time_l = up/maxValue*duration/1000,time_s = down/maxValue*duration/1000):(time_l = down/maxValue*duration/1000,time_s = up/maxValue*duration/1000);
    QTime Time1((time_l/3600)%60, (time_l/60)%60, time_l%60, (time_l*1000)%1000);
    QTime Time2((time_s/3600)%60, (time_s/60)%60, time_s%60, (time_s*1000)%1000);
    QString format = "mm:ss";
    if (duration > 3600000)//到毫秒
        format = "hh:mm:ss";
    QString tStr = Time2.toString(format)+"--"+Time1.toString(format);
    ui->label_scale->setText(tStr);
}
