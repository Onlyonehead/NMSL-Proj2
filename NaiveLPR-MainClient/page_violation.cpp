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
    ui->dsb_x_3->setValue((double)pStart.x()/ui->label_paint->width());
    ui->dsb_y_3->setValue((double)pStart.y()/ui->label_paint->height());
}

void MainClient::getPointEnd(QPoint p){
    pEnd.setX(p.x());
    pEnd.setY(p.y());
    ui->dsb_x2_3->setValue((double)pEnd.x()/ui->label_paint->width());
    ui->dsb_y2_3->setValue((double)pEnd.y()/ui->label_paint->height());
    ui->dsb_width_3->setValue(qAbs((double)pStart.x()-(double)pEnd.x())/ui->label_paint->width());
    ui->dsb_height_3->setValue(qAbs((double)pStart.y()-(double)pEnd.y())/ui->label_paint->height());

    ui->label_selectArea->setText(QString::number(ui->dsb_width_3->value()*ui->dsb_width_3->value()*video_width*video_height, 10, 0));
}


void MainClient::on_pb_clearRect_clicked()
{

    emit clearP();
    ui->label_board->clear();
    ui->label_selectedVideo->setText("");
    ui->pb_sendRequest->setEnabled(false);

    ui->label_cars_num->setText("-");
    ui->label_velocity_num->setText("-");

    ui->label_selectArea->setText("-");


    if(!(ui->label_player_3->layout() == NULL)){
        int itemCount = ui->label_player_3->layout()->count(); // layout是你前一个布局
        for (int i = (itemCount - 1); i >= 0; --i) //从末尾开始是因为你删除会影响布局的顺序。例如你删掉第一个，后面的会往前移，第二就变成第一个，然后这时你要是++i的话，就是删掉原来布局里的第三个，这第二个被跳过了。
        {
            QLayoutItem *item = ui->label_player_3->layout()->takeAt(i);
            if (item != 0)
            {
                ui->label_player_3->layout()->removeWidget(item->widget());
                delete item->widget(); //（ps：如果是子控件不是QWidget，这里会出错，要注意）
            }
        }
        player->stop();
    }



//    ui->tw_videos->setRowCount(0);

    ui->dsb_x_3->setValue(0);
    ui->dsb_y_3->setValue(0);
    ui->dsb_x2_3->setValue(1);
    ui->dsb_y2_3->setValue(1);
    ui->dsb_width_3->setValue(1);
    ui->dsb_height_3->setValue(1);

    ui->horizontalSlider_up->setValue(100);
    ui->horizontalSlider_up->setEnabled(false);
    ui->horizontalSlider_down->setValue(0);
    ui->horizontalSlider_down->setEnabled(false);
    ui->label_scale->setText("00:00--00:00");

    ui->label_width->setText("-");
    ui->label_height->setText("-");
    ui->label_fps->setText("-");

    ui->label_backGround->setText("Background\n\nPreview");
    QWidget *widget = new QWidget;
    ui->scrollArea_carPics->setWidget(widget);
}

void MainClient::on_tw_videos_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    if(row>=0){
        QString localPath = qm_videos.value(ui->tw_videos->item(row, 0)->text());

        Mat mat;
        VideoDetect video_detect;
        video_detect.getFirstFrame(localPath.toStdString(), mat);

        video_width = mat.cols;
        video_height = mat.rows;

        ui->label_width->setText(QString::number(video_width));
        ui->label_height->setText(QString::number(video_height));

        VideoCapture capture(localPath.toStdString());

        long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);

        double rate = capture.get(CV_CAP_PROP_FPS);

        duration = totalFrameNumber / rate * 1000;

        Mat Rgb;
        QImage Img;
        if (mat.channels() == 3)//RGB Img
        {
            cv::cvtColor(mat, Rgb, CV_BGR2RGB);//颜色空间转换
            Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
        }
        else//Gray Img
        {
            Img = QImage((const uchar*)(mat.data), mat.cols, mat.rows, mat.cols*mat.channels(), QImage::Format_Indexed8);
        }
        ui->label_board->setPixmap(QPixmap::fromImage(Img));

        ui->label_fps->setText(QString::number(rate, 10, 2));



        ui->label_selectedVideo->setText(ui->tw_videos->item(row, 0)->text());
        ui->pb_sendRequest->setEnabled(true);
        ui->horizontalSlider_up->setEnabled(true);
        ui->horizontalSlider_down->setEnabled(true);

        ui->dsb_x_3->setValue(0);
        ui->dsb_y_3->setValue(0);
        ui->dsb_x2_3->setValue(1);
        ui->dsb_y2_3->setValue(1);
        ui->dsb_width_3->setValue(1);
        ui->dsb_height_3->setValue(1);
    }
}

void MainClient::on_pb_play_3_clicked()
{
    if(play_state)
    {
        player->pause();
        ui->pb_play_3->setText("play");
    }
    else
    {
        player->play();
        ui->pb_play_3->setText("pause");
    }

    play_state = !play_state;
}

void MainClient::onTimerOut()
{
    ui->slider_progress_3->setValue(player->position()*maxValue/player->duration());
}

void MainClient::on_pushButton_clicked()
{
    ui->sb_minArea_3->setValue(350);
    ui->sb_minDetect_3->setValue(800);
    ui->sb_initFrame_3->setValue(30);
    ui->sb_fMax_3->setValue(10);
    ui->dsb_shadow_3->setValue(0.7);
    ui->sb_mCore_3->setValue(6);
}

void MainClient::slider_progress_clicked()
{
    player->setPosition(ui->slider_progress_3->value()*player->duration()/maxValue);
}

void MainClient::slider_progress_moved()
{
    //暂时停止计时器，在用户拖动过程中不修改slider的值
    timer->stop();
    player->setPosition(ui->slider_progress_3->value()*player->duration()/maxValue);
}

void MainClient::slider_progress_released()
{
    //用户释放滑块后，重启定时器
    timer->start();
}

void MainClient::SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

void MainClient::on_pb_sendRequest_clicked()
{

    int picCount = 0;

    double sx, sy, x ,y, x2, y2, width, height, shadow;
    int minarea, mindetect, initFrame, fMax, mCore;
    QString path;

    x = ui->dsb_x_3->value();
    y = ui->dsb_y_3->value();
    x2 = ui->dsb_x2_3->value();
    y2 = ui->dsb_y2_3->value();
    sx = x<x2 ? x : x2;
    sy = y<y2 ? y : y2;

    width = ui->dsb_width_3->value();
    height = ui->dsb_height_3->value();

    shadow = ui->dsb_shadow_3->value();
    minarea = ui->sb_minArea_3->value();
    mindetect = ui->sb_minDetect_3->value();
    initFrame = ui->sb_initFrame_3->value();
    fMax = ui->sb_fMax_3->value();
    mCore = ui->sb_mCore_3->value();

    path = qm_videos.value(ui->label_selectedVideo->text());


    string s = ui->label_selectedVideo->text().toStdString();
    vector<string> v;
    SplitString(s, v, ".");
    string fileName = v[0];

    string full_dir = qm_videos.value(QString::fromStdString(s)).toStdString();

    QDir dir;
    dir.cd(V_DIR);  //进入某文件夹



    double rect[4] = {sx, sy, width, height};//avi

    double per1 = ui->horizontalSlider_up->value();
    double per2 = ui->horizontalSlider_down->value();

    QString vName;

    VideoDetect vd;

    if(rect[2]*rect[3]!=0){

        string source_dir = V_DIR + string("/") + fileName + string("_flow/");

        if(!dir.exists(QString::fromStdString(fileName + "_flow/")))//判断需要创建的文件夹是否存在
        {
            dir.mkdir(QString::fromStdString(fileName + "_flow/")); //创建文件夹
        }else{
            removeFolderContent(QString::fromStdString(source_dir));
        }

        string path[5] = {
            full_dir,
            source_dir + fileName + string("_flow.mp4"),
            source_dir,
            source_dir + "file_flow.txt",
            source_dir + fileName + "_back.jpg"
        };

        vName = QString::fromStdString(source_dir + fileName + string("_flow.mp4"));

        picCount = vd.vOperate_flow(path, rect, minarea, mindetect, initFrame, fMax, shadow, mCore);

        QGridLayout  *gLayout = new QGridLayout();
        gLayout->setSpacing(15);
        gLayout->setMargin(15);
        for(int i = 0; i < picCount; ++i){
            QLabel *label = new QLabel(this);
            QImage image(QString::fromStdString(source_dir) + QString::number(i+1, 10) + QString(".jpg"));
            QPixmap pix = QPixmap::fromImage(image.scaled(80, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            label->setStyleSheet("border:none;border-radius:0px;background:none;");
            label->setPixmap(pix);
            gLayout->addWidget(label, i / 2, i % 2);
        }
        QWidget *widget = new QWidget;
        widget->setLayout(gLayout);
        ui->scrollArea_carPics->setWidget(widget);

        QPixmap *p = new QPixmap(QString::fromStdString(source_dir) + QString::fromStdString(fileName + "_back.jpg"));
        ui->label_backGround->setPixmap(*p);

        ui->label_cars_num->setText(QString::number(picCount));
        ui->label_velocity_num->setText("43.6km/h");

    }
    else{//宽或高有一个为零，则执行闯红灯检测

        long time[2];

        if(per1 > per2){
            time[0] = per2 / 100 * duration;
            time[1] = per1 / 100 * duration;
        }else{
            time[0] = per1 / 100 * duration;
            time[1] = per2 / 100 * duration;
        }

        string source_dir = V_DIR + string("/") + fileName + string("_redlight/");

        if(!dir.exists(QString::fromStdString(fileName + "_redlight/")))//判断需要创建的文件夹是否存在
        {
            dir.mkdir(QString::fromStdString(fileName + "_redlight/")); //创建文件夹
        }else{
            removeFolderContent(QString::fromStdString(source_dir));
        }

        string path[4] = {
            full_dir,
            source_dir + fileName + string("_peccancy.mp4"),
            source_dir,
            source_dir + fileName + "_back.jpg"
        };

        vName = QString::fromStdString(source_dir + fileName + string("_peccancy.mp4"));

        picCount = vd.vOperatePeccancy(path, rect, time, minarea, mindetect, initFrame, fMax, shadow, mCore);

        QGridLayout  *gLayout = new QGridLayout();
        gLayout->setSpacing(15);
        gLayout->setMargin(15);
        for(int i = 0; i < picCount; ++i){
            QLabel *label = new QLabel(this);
            QImage image(QString::fromStdString(source_dir) + QString::number(i+1, 10) + QString(".jpg"));
            QPixmap pix = QPixmap::fromImage(image.scaled(80, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            label->setStyleSheet("border:none;border-radius:0px;background:none;");
            label->setPixmap(pix);
            gLayout->addWidget(label, i / 2, i % 2);
        }
        QWidget *widget = new QWidget;
        widget->setLayout(gLayout);
        ui->scrollArea_carPics->setWidget(widget);

        QPixmap *p = new QPixmap(QString::fromStdString(source_dir) + QString::fromStdString(fileName + "_back.jpg"));
        ui->label_backGround->setPixmap(*p);

        ui->label_cars_num->setText(QString::number(picCount));
    }


    //视频播放
    QFile file(vName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(NULL, "Title", "Content", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if(if_reload)
    {//重新加载视频时，重置以下变量
        delete layout_video;
        delete player;
        delete timer;
    }
    if_reload = true;

    //实例化三个全局变量
    layout_video = new QVBoxLayout;
    player = new QMediaPlayer;
    QVideoWidget *widget = new QVideoWidget;
    //设置视频播放区域与Label的边距
    layout_video->setMargin(1);
    //根据label_player尺寸设置播放区域
    widget->resize(ui->label_player_3->size());
    layout_video->addWidget(widget);
    ui->label_player_3->setLayout(layout_video);
    player->setVideoOutput(widget);

    //设置播放器
    player->setMedia(QMediaContent(QUrl::fromLocalFile(vName)));



    //play_state为true表示播放，false表示暂停
    play_state = true;
    //启用播放/暂停按钮，并将其文本设置为“暂停”
    ui->pb_play_3->setEnabled(true);
    ui->pb_play_3->setText("pause");
    //播放器开启
    player->play();

    //启用进度条
    ui->slider_progress_3->setEnabled(true);
    ui->slider_progress_3->setRange(0,maxValue);

    timer = new QTimer();
    timer->setInterval(100);//如果想看起来流畅些，可以把时间间隔调小，如100ms
    timer->start();
    //将timer连接至onTimerOut槽函数
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
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

bool MainClient::removeFolderContent(const QString &folderDir)
{
    QDir dir(folderDir);
    QFileInfoList fileList;
    QFileInfo curFile;
    if(!dir.exists())  {return false;}//文件不存，则返回false
    fileList=dir.entryInfoList(QDir::Dirs|QDir::Files
                               |QDir::Readable|QDir::Writable
                               |QDir::Hidden|QDir::NoDotAndDotDot
                               ,QDir::Name);
    while(fileList.size()>0)
    {
        int infoNum=fileList.size();
        for(int i=infoNum-1;i>=0;i--)
        {
            curFile=fileList[i];
            if(curFile.isFile())//如果是文件，删除文件
            {
                QFile fileTemp(curFile.filePath());
                fileTemp.remove();
                fileList.removeAt(i);
            }
            if(curFile.isDir())//如果是文件夹
            {
                QDir dirTemp(curFile.filePath());
                QFileInfoList fileList1=dirTemp.entryInfoList(QDir::Dirs|QDir::Files
                                                              |QDir::Readable|QDir::Writable
                                                              |QDir::Hidden|QDir::NoDotAndDotDot
                                                              ,QDir::Name);
                if(fileList1.size()==0)//下层没有文件或文件夹
                {
                    dirTemp.rmdir(".");
                    fileList.removeAt(i);
                }
                else//下层有文件夹或文件
                {
                    for(int j=0;j<fileList1.size();j++)
                    {
                        if(!(fileList.contains(fileList1[j])))
                            fileList.append(fileList1[j]);
                    }
                }
            }
        }
    }
    return true;
}
