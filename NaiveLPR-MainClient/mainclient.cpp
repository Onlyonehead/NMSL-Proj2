#include "mainclient.h"
#include "ui_mainclient.h"

MainClient::MainClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainClient)
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

    move(120,60);
    on_welcome_button_clicked();

//    struct timeval timeout={2,0};

//    redisContext* pRedisContext=(redisContext*)redisConnectWithTimeout("127.0.0.1",6379,timeout);

//    const char* command("get key");

//    redisReply* reply=(redisReply*)redisCommand(pRedisContext,command);

//    QString str=reply->str;

//    ui->textEdit->append(str);

}

MainClient::~MainClient()
{
    delete ui;
}

/**
 * on_pushButton_clicked
 *
 * @author hzc
 */
void MainClient::on_pushButton_quit_clicked()
{
    this->close();
}

/**
 * slot method
 *
 * @author hzc
 * @param s1 username
 * @param s2 name
 * @param s3 position
 * @param s4 gender
 * @param s5 email
 */
void MainClient::showString(QString s1, QString s2, QString s3, QString s4, QString s5, QString s6)
{
    this->show();
    ui->pushButton_switch->setEnabled(false);


    /*
     * slow start
     *
     */

    this->processPhase = 0;

    ui->label_platePreview->installEventFilter(this);

    ui->label_player_3->installEventFilter(this);

    ui->label_showEditPortrait->installEventFilter(this);
    ui->label_showEditPortrait->setAcceptDrops(true);
    ui->label_showEditPortraitPath->setVisible(false);

    ui->label_showNewPortrait->installEventFilter(this);
    ui->label_showNewPortrait->setAcceptDrops(true);
    ui->label_showNewPortraitPath->setVisible(false);

    ui->label_repeatPasswordTip->setVisible(false);
    ui->label_newStaffUsernameTip->setVisible(false);
    ui->label_newStaffEmailTip->setVisible(false);


    int fontId = QFontDatabase::addApplicationFont(":/font/fa-solid-900.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);

    QFont icon_search;
    icon_search.setFamily(fontFamilies.at(0));
    icon_search.setPointSize(24);

    ui->style_change->setFont(icon_search);
    ui->style_change->setText(QChar(0xf204));
    ui->style_change->setStyleSheet("QPushButton{border: 0px; color: white;} "
                                    "QPushButton:hover{border: 0px; color: rgba(15, 128, 255, 190);} ");


    ui->label_2->setText(s1);
    ui->label_4->setText(s2);
    ui->label_8->setText(s3);
    ui->label_12->setText(s4);
    ui->label_14->setText(s5);


    QPixmap *pixmap = new QPixmap(DIR + QString("/users/") + s6);
    if (pixmap->isNull()){
        download("/users/" + s6, DIR + QString("/users/") + s6);

        QElapsedTimer t;
        t.start();
        while(t.elapsed()<500)
            QCoreApplication::processEvents();

        pixmap = new QPixmap(DIR + QString("/users/") + s6);
    }

    if(pixmap->isNull()){
        pixmap = new QPixmap(":/user.png");
    }

    QApplication::processEvents();

    getRoundPixmap(pixmap, QSize(140, 140));
    ui->label_5->setScaledContents(true);
    ui->label_5->setPixmap(*pixmap);
    delete pixmap;

    QApplication::processEvents();

    //initialize tiny icon

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

    ui->welcome_button->setFont(font);
    ui->welcome_button->setText(QChar(0xf111));
    ui->welcome_button->setStyleSheet("QPushButton{border: 0px; color: rgb(200, 200, 200);background:none;}"
                                      "QPushButton:hover{border: 0px; color: rgba(15, 128, 255, 190);}"
                                      "QPushButton:checked{border: 0px; color: rgba(15, 128, 255, 190);}");

    ui->label_20->setFont(font);
    ui->label_20->setText(QChar(0xf138));
    ui->label_20->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_28->setFont(font);
    ui->label_28->setText(QChar(0xf1b8));
    ui->label_28->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_134->setFont(font);
    ui->label_134->setText(QChar(0xf2c2));
    ui->label_134->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_135->setFont(font);
    ui->label_135->setText(QChar(0xf5c2));
    ui->label_135->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_136->setFont(font);
    ui->label_136->setText(QChar(0xf1ae));
    ui->label_136->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_139->setFont(font);
    ui->label_139->setText(QChar(0xf2b6));
    ui->label_139->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_144->setFont(font);
    ui->label_144->setText(QChar(0xf05a));
    ui->label_144->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_145->setFont(font);
    ui->label_145->setText(QChar(0xf53c));
    ui->label_145->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_146->setFont(font);
    ui->label_146->setText(QChar(0xf248));
    ui->label_146->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_147->setFont(font);
    ui->label_147->setText(QChar(0xf599));
    ui->label_147->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_148->setFont(font);
    ui->label_148->setText(QChar(0xf1ae));
    ui->label_148->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_149->setFont(font);
    ui->label_149->setText(QChar(0xf0e0));
    ui->label_149->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_150->setFont(font);
    ui->label_150->setText(QChar(0xf1de));
    ui->label_150->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_151->setFont(font);
    ui->label_151->setText(QChar(0xf06a));
    ui->label_151->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_124->setFont(font);
    ui->label_124->setText(QChar(0xf058));
    ui->label_124->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_125->setFont(font);
    ui->label_125->setText(QChar(0xf017));
    ui->label_125->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_126->setFont(font);
    ui->label_126->setText(QChar(0xf461));
    ui->label_126->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_16->setFont(font);
    ui->label_16->setText(QChar(0xf008));
    ui->label_16->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_22->setFont(font);
    ui->label_22->setText(QChar(0xf101));
    ui->label_22->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_31->setFont(font);
    ui->label_31->setText(QChar(0xf192));
    ui->label_31->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_27->setFont(font);
    ui->label_27->setText(QChar(0xf058));
    ui->label_27->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_39->setFont(font);
    ui->label_39->setText(QChar(0xf2bd));
    ui->label_39->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_26->setFont(font);
    ui->label_26->setText(QChar(0xf1b9));
    ui->label_26->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_44->setFont(font);
    ui->label_44->setText(QChar(0xf53d));
    ui->label_44->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_40->setFont(font);
    ui->label_40->setText(QChar(0xf155));
    ui->label_40->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");


    ui->label_49->setFont(font);
    ui->label_49->setText(QChar(0xf22d));
    ui->label_49->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_53->setFont(font);
    ui->label_53->setText(QChar(0xf22d));
    ui->label_53->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_82->setFont(font);
    ui->label_82->setText(QChar(0xf22d));
    ui->label_82->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_83->setFont(font);
    ui->label_83->setText(QChar(0xf22d));
    ui->label_83->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_69->setFont(font);
    ui->label_69->setText(QChar(0xf122));
    ui->label_69->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_72->setFont(font);
    ui->label_72->setText(QChar(0xf43c));
    ui->label_72->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_74->setFont(font);
    ui->label_74->setText(QChar(0xf138));
    ui->label_74->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_77->setFont(font);
    ui->label_77->setText(QChar(0xf138));
    ui->label_77->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    ui->label_130->setFont(font);
    ui->label_130->setText(QChar(0xf571));
    ui->label_130->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    QApplication::processEvents();

    ui->label_131->setFont(font);
    ui->label_131->setText(QChar(0xf5a0));
    ui->label_131->setStyleSheet("border: 0px; color: rgb(106, 106, 106);background:none;");

    //network status
    ui->label_33->setFont(font);
    ui->label_33->setText(QChar(0xf111));
    ui->label_33->setStyleSheet("border: 0px; color: rgb(33, 233, 121);background:none;"
                                "border-image:none;");

    ui->icon_search->setFont(icon_search);
    ui->icon_search->setText(QChar(0xf35a));
    ui->icon_search->setStyleSheet("QPushButton{border: 0px; color: rgb(127, 127, 127);} "
                                   "QPushButton:hover{border: 0px; color: rgb(15, 128, 255);} ");

    ui->button_left->setFont(icon_search);
    ui->button_left->setText(QChar(0xf359));
    ui->button_left->setStyleSheet("QPushButton{border: 0px; color: rgb(106, 106, 106);background:none;}"
                                    "QPushButton:hover{border: 0px; color: rgba(15, 128, 255, 190);} ");

    ui->button_right->setFont(icon_search);
    ui->button_right->setText(QChar(0xf35a));
    ui->button_right->setStyleSheet("QPushButton{border: 0px; color: rgb(106, 106, 106);background:none;}"
                                    "QPushButton:hover{border: 0px; color: rgba(15, 128, 255, 190);} ");


    connect(ui->search_A, SIGNAL(returnPressed()), ui->icon_search, SIGNAL(clicked()), Qt::UniqueConnection);

    style = false;
    QFile *file = new QFile(CONFIG_DIR);
    if(file->exists()){
        if (file->open(QFile::ReadWrite | QFile::Text)){
           QString status;
           status = file->readLine().trimmed();
           if(status == "1"){
               style = !style;
               int fontId = QFontDatabase::addApplicationFont(":/font/fa-solid-900.ttf");
               QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
               QFont icon_search;
               icon_search.setFamily(fontFamilies.at(0));
               icon_search.setPointSize(24);
               ui->frame->setStyleSheet("QFrame{background: rgba(255, 255, 255, 0);"
                                        "border-image:url(:/bg5.png);}");
               ui->style_change->setFont(icon_search);
               ui->style_change->setText(QChar(0xf205));
               ui->style_change->setStyleSheet("QPushButton{border: 0px; color: white;} "
                                               "QPushButton:hover{border: 0px; color: rgba(15, 128, 255, 190);} ");
           }
        }else{
            qDebug()<<"打开失败";
        }
    }
    file->close();





    QApplication::processEvents();



    ui->progressBar->setVisible(true);

    progressBar();

    ui->progressBar->setVisible(false);


    ui->comboBox_selectType->setView(new QListView());
    ui->comboBox_p0->setView(new QListView());
    ui->comboBox_p1->setView(new QListView());
    ui->comboBox_p2->setView(new QListView());
    ui->comboBox_p3->setView(new QListView());
    ui->comboBox_p4->setView(new QListView());
    ui->comboBox_p5->setView(new QListView());
    ui->comboBox_p6->setView(new QListView());
    ui->comboBox_model->setView(new QListView());
    ui->comboBox_updateStaffGender->setView(new QListView());
    ui->comboBox_updateStaffPosition->setView(new QListView());
    ui->comboBox_addNewGender->setView(new QListView());
    ui->comboBox_addNewPosition->setView(new QListView());
    ui->comboBox_selectType_2->setView(new QListView());


    ui->pushButton_switch->setEnabled(true);

    ui->tableWidget_history->horizontalHeader()->setDefaultSectionSize(90);
    ui->tableWidget_history->setColumnWidth(0, 190);
    ui->tableWidget_history->setColumnWidth(3, 20);
    ui->tableWidget_history->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_history->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_history->setAlternatingRowColors(true);


    ui->tableWidget_updateShowStaffInfo->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_updateShowStaffInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_updateShowStaffInfo->setAlternatingRowColors(true);

    ui->tableWidget_left->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_left->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_left->setAlternatingRowColors(true);

    ui->tableWidget_right->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_right->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_right->setAlternatingRowColors(true);

    ui->tableWidget_search->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_search->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_search->setAlternatingRowColors(true);

    ui->tw_videos->installEventFilter(this);
    ui->tw_videos->setAcceptDrops(true);
    ui->tw_videos->horizontalHeader()->setDefaultSectionSize(10);
    ui->tw_videos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tw_videos->horizontalHeader()->setStretchLastSection(true);
    ui->tw_videos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tw_videos->setAlternatingRowColors(true);

    ui->tableWidget_highway->horizontalHeader()->setDefaultSectionSize(10);
    ui->tableWidget_highway->setColumnWidth(0, 90);
    ui->tableWidget_highway->setColumnWidth(1, 180);
    ui->tableWidget_highway->setColumnWidth(2, 180);
    ui->tableWidget_highway->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_highway->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_highway->setAlternatingRowColors(true);

    ui->tableWidget_accounts->setColumnWidth(0, 90);
    ui->tableWidget_accounts->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_accounts->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_accounts->setAlternatingRowColors(true);

    ui->tableWidget_payment->horizontalHeader()->setDefaultSectionSize(20);
    ui->tableWidget_payment->setColumnWidth(0, 180);
    ui->tableWidget_payment->setColumnWidth(1, 20);
    ui->tableWidget_payment->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_payment->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_payment->setAlternatingRowColors(true);


    ui->tableWidget_toPay->horizontalHeader()->setDefaultSectionSize(180);
    ui->tableWidget_toPay->verticalHeader()->setDefaultSectionSize(50);
    ui->tableWidget_toPay->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_toPay->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_toPay->setAlternatingRowColors(true);

    file = new QFile(MODEL_DIR);
    if(file->exists()){
        if (file->open(QFile::ReadWrite | QFile::Text)){
            QString m = file->readLine().trimmed();
            if(m == "DEFAULT" || m == ""){
                ui->comboBox_model->setCurrentIndex(0);
            }else{
                ui->comboBox_model->setCurrentIndex(1);
            }
        }else{
            qDebug()<<"打开失败";
        }
    }
    file->close();

    ui->progressBar->setVisible(true);
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(10);

    QLineSeries *series=new QLineSeries;

    QChart *chart=new QChart;
    chart->setTitle("Traffic");
    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->createDefaultAxes();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 10);
    axisX->setLabelFormat("%u");
    axisX->setGridLineVisible(true);
    axisX->setTickCount(10);
    axisX->setMinorTickCount(3);

    chart->setAxisX(axisX, series);

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    ui->progressBar->setValue(20);

    QBarSet *set0 = new QBarSet("Traffic");
    QBarSet *set1 = new QBarSet("Redlight");
    QBarSet *set2 = new QBarSet("Overspeed");

    QPercentBarSeries *series2 = new QPercentBarSeries();
    series2->append(set0);
    series2->append(set1);
    series2->append(set2);

    QChart *chart2 = new QChart();
    chart2->addSeries(series2);
    chart2->setTitle("Offence Percentage");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "1" << "2" << "3" << "4" << "5";
    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories);
    chart2->createDefaultAxes();
    chart2->setAxisX(axis2, series2);

    chart2->legend()->setVisible(true);
    chart2->legend()->setAlignment(Qt::AlignBottom);

    ui->chartView_2->setChart(chart2);
    ui->chartView_2->setRenderHint(QPainter::Antialiasing);

    ui->progressBar->setValue(30);


    QStringList msg;
    msg.append("transfer");
    sendMessage(msg);


    ui->pb_sendRequest->setEnabled(false);
    ui->horizontalSlider_up->setEnabled(false);
    ui->horizontalSlider_down->setEnabled(false);

    connect(ui->label_paint, SIGNAL(setPointStart(QPoint)), this, SLOT(getPointStart(QPoint)));
    connect(ui->label_paint, SIGNAL(setPointEnd(QPoint)), this, SLOT(getPointEnd(QPoint)));
    connect(this, SIGNAL(clearP()), ui->label_paint, SLOT(painterClear()));

    ui->pb_play_3->setEnabled(false);
    ui->slider_progress_3->setEnabled(false);
    connect(ui->slider_progress_3,&MySlider::mySliderClicked,this,&MainClient::slider_progress_clicked);
    connect(ui->slider_progress_3,&MySlider::sliderMoved,this,&MainClient::slider_progress_moved);
    connect(ui->slider_progress_3,&MySlider::sliderReleased,this,&MainClient::slider_progress_released);
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
void MainClient::getRoundPixmap(QPixmap* src,QSize size)

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
 * on_pushButton_A_clicked slot
 *
 * @author hzc
 */
void MainClient::on_pushButton_A_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}

/**
 * on_pushButton_B_clicked slot
 *
 * @author hzc
 */
void MainClient::on_pushButton_B_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

}

/**
 * on_pushButton_C_clicked slot
 *
 * @author hzc
 */
void MainClient::on_pushButton_C_clicked()
{

    ui->stackedWidget->setCurrentIndex(3);
}

/**
 * on_pushButton_D_clicked slot
 *
 * @author hzc
 */
void MainClient::on_pushButton_D_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

/**
 * on_pushButton_E_clicked slot
 *
 * @author hzc
 */
void MainClient::on_pushButton_E_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


/**
 * on_welcome_button_clicked slot
 *
 * @author hzc
 */
void MainClient::on_welcome_button_clicked()
{
    ui->welcome_button->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}


/**
 * progress bar
 *
 * @author hzc
 */
void MainClient::progressBar()
{
    ui->progressBar->setVisible(true);
    ui->progressBar->setRange(0, 100);
    for (int i = 0; i <= 100; i++){
        for(int j = 0; j < 1000000; j++){
            i = i + 1;
            i = i - 1;
        }
        ui->progressBar->setValue(i);
        QApplication::processEvents();
    }
    ui->progressBar->setVisible(false);
}

/**
 * fast version progress bar
 *
 * @author hzc
 */
void MainClient::progressBar_fast()
{
    ui->progressBar->setVisible(true);
    ui->progressBar->setRange(0, 100);
    for (int i = 0; i <= 100; i++){
        for(int j = 0; j < 300000; j++){
            i = i + 1;
            i = i - 1;
        }
        ui->progressBar->setValue(i);
        QApplication::processEvents();
    }
    ui->progressBar->setVisible(false);
}


/**
 * on_pushButton_2_clicked slot
 *
 * @author hzc
 */
void MainClient::on_pushButton_switch_clicked()
{
    QApplication::processEvents();
    Login *lg = new Login;
    connect(this, SIGNAL(stringReturn(QString)), lg, SLOT(showString(QString)));
    emit stringReturn(this->ui->label_2->text());
    this->close();
}

/**
 * download picture
 *
 * @author hzc
 * @param surl pic url
 * @param filePath local file path
 */
void MainClient::download(const QString &url, const QString &filePath)
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
void MainClient::replyFinished()
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

void MainClient::on_style_change_clicked()
{
    int fontId = QFontDatabase::addApplicationFont(":/font/fa-solid-900.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    QFont icon_search;
    icon_search.setFamily(fontFamilies.at(0));
    icon_search.setPointSize(24);


    if(style){
        ui->frame->setStyleSheet("QFrame{background:rgba(255, 255, 255, 255);"
                                 "border-image:url(:/bg4.png);}");
        ui->style_change->setFont(icon_search);
        ui->style_change->setText(QChar(0xf204));
        ui->style_change->setStyleSheet("QPushButton{border: 0px; color: white;} "
                                        "QPushButton:hover{border: 0px; color: rgba(15, 128, 255, 190);} ");
    }else{
        ui->frame->setStyleSheet("QFrame{background: rgba(255, 255, 255, 0);"
                                 "border-image:url(:/bg5.png);}");
        ui->style_change->setFont(icon_search);
        ui->style_change->setText(QChar(0xf205));
        ui->style_change->setStyleSheet("QPushButton{border: 0px; color: white;} "
                                        "QPushButton:hover{border: 0px; color: rgba(15, 128, 255, 190);} ");
    }
    style = !style;

    QFile *file = new QFile(CONFIG_DIR);
    if (file->open(QFile::ReadWrite | QFile::Text)){
        QTextStream out(file);
        out << style << "\n";
    }else{
        qDebug()<<"打开失败";
    }
    file->close();
}



bool MainClient::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->label_platePreview) {
        if (event->type() == QEvent::MouseButtonPress) {

            if(ui->tableWidget_history->currentItem() == NULL){
                QMessageBox::warning(this,"警告", "\n请选中条目！",QMessageBox::Close);
                return false;
            }

            QString tmp = ui->tableWidget_history->item(
                        ui->tableWidget_history->currentItem()->row(), 0)->text() + ".png";


            QString fileName = DIR + QString("/plates/") + tmp;

            Mat m = imread(fileName.toStdString(), 1);
            namedWindow("Source Image");
            resizeWindow("Source Image", 600, 450);
            moveWindow("Source Image", 425, 120);
            imshow("Source Image", m);
            return true;
        }

        if(event->type() == QEvent::MouseButtonRelease){
            destroyWindow("Source Image");
        }
    }

    if (watched == ui->label_showEditPortrait) {
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
                image = image.scaled(92, 92,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
                ui->label_showEditPortrait->setPixmap(QPixmap::fromImage(image));
                ui->label_showEditPortraitPath->setText(path);
            }

            return true;
        }

        if (event->type() == QEvent::MouseButtonDblClick) {
            QStringList picture = QFileDialog::getOpenFileNames(this, tr("open file"),
                                                                tr("图片文件(*png *jpg)"));
            if(picture.isEmpty()){
                ui->label_showEditPortrait->setText("Preview");
                return true;
            }

            QImage tempPortrait(picture.at(0));
            QPixmap portrait = QPixmap::fromImage(tempPortrait.scaled(92, 92, Qt::KeepAspectRatio,
                                                                      Qt::SmoothTransformation));
            ui->label_showEditPortraitPath->setText(picture.at(0));
            ui->label_showEditPortrait->setPixmap(portrait);
            ui->label_showEditPortrait->show();

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
                image = image.scaled(152, 152,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);
                ui->label_showNewPortrait->setPixmap(QPixmap::fromImage(image));
                ui->label_showNewPortraitPath->setText(path);
            }

            return true;
        }

        if (event->type() == QEvent::MouseButtonDblClick) {
            QStringList picture = QFileDialog::getOpenFileNames(this, tr("open file"),
                                                                tr("图片文件(*png *jpg)"));
            if(picture.isEmpty()){
                ui->label_showNewPortrait->setText("Preview");
                return true;
            }

            QImage tempPortrait(picture.at(0));
            QPixmap portrait = QPixmap::fromImage(tempPortrait.scaled(152, 152, Qt::KeepAspectRatio,
                                                                      Qt::SmoothTransformation));
            ui->label_showNewPortraitPath->setText(picture.at(0));
            ui->label_showNewPortrait->setPixmap(portrait);
            ui->label_showNewPortrait->show();

            return true;
        }
    }

    if(watched == ui->tw_videos){
        if(event->type() == QEvent::DragEnter){
            QDragEnterEvent *dEE = static_cast<QDragEnterEvent*>(event);

            if(!dEE->mimeData()->urls()[0].toLocalFile().right(3).compare("mp4",Qt::CaseInsensitive)
                    ||!dEE->mimeData()->urls()[0].toLocalFile().right(3).compare("avi",Qt::CaseInsensitive)
                ||!dEE->mimeData()->urls()[0].toLocalFile().right(3).compare("mov",Qt::CaseInsensitive))
            {
                dEE->acceptProposedAction();//接受鼠标拖入事件
            }
            else
            {
                dEE->ignore();//否则不接受鼠标事件
            }
            return true;
        }

        if(event->type() == QEvent::Drop){
            QDropEvent *dE = static_cast<QDropEvent*>(event);

            //窗口部件放下一个对象时,调用该函数
            const QMimeData *qm=dE->mimeData();//获取MIMEData
            QString path = qm->urls()[0].toLocalFile();

            QStringList splits = path.split("/");
            QString fileName = splits.at(splits.size()-1);

            int i;
            for(i=0; i<ui->tw_videos->rowCount(); ++i){
                qDebug()<<ui->tw_videos->item(i, 0)->text()<<" "<<fileName;
                if(ui->tw_videos->item(i, 0)->text() == fileName)
                    break;
            }
            qDebug()<<i<<" "<<ui->tw_videos->rowCount();
            if(ui->tw_videos->rowCount()==0||i==ui->tw_videos->rowCount()){
                qm_videos.insert(fileName, path);

                ui->tw_videos->setRowCount(ui->tw_videos->rowCount()+1);
                ui->tw_videos->setItem(ui->tw_videos->rowCount()-1, 0, new QTableWidgetItem(fileName));
            }
            return true;
        }

    }

    if(watched == ui->label_player_3){
        if(event->type() == QEvent::MouseButtonDblClick){
            //选择视频文件
            QString filename = QFileDialog::getOpenFileName(this,tr("选择视频文件"),"/Users/Haibara/Desktop",tr("视频格式(*.mp4)"));
            qDebug() << filename;
            QFile file(filename);
            if(!file.open(QIODevice::ReadOnly))
            {
                return true;
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
            player->setMedia(QMediaContent(QUrl::fromLocalFile(filename)));



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

            return true;
        }

    }
    return QWidget::eventFilter(watched, event);
}

