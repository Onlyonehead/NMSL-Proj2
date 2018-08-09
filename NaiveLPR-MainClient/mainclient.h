#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#define DIR "/Users/Haibara/Documents/qt build files2/Clientfiles"

#define CONFIG_DIR "/Users/Haibara/Documents/qt build files2/Clientfiles/config.data"

#define MODEL_DIR "/Users/Haibara/Documents/qt build files2/Clientfiles/model.data"

#define V_DIR "/Users/Haibara/Documents/qt build files2/Clientfiles/videos"

#include <QMainWindow>

#include <QFont>
#include <QTabBar>

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QFontDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QUrl>
#include <QTime>
#include <QDateTime>
#include <QDate>
#include <QCompleter>
#include <QStringListModel>
#include <QtNetwork>
#include "login.h"
#include "CharRecognition.h"
#include <QDesktopWidget>
#include <QDragEnterEvent>
#include <QDebug>
#include <QBitmap>
#include <QPainter>
#include <QMessageBox>
#include <QFontDatabase>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QDateTime>
#include <QTime>
#include <QDate>
#include <QListView>
#include <QFileDialog>
#include <QtCharts>
#include <QTime>
#include <QMap>
#include <QScrollArea>
#include <QMediaPlayer>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <QVideoFrame>

#include <qtmap.h>
#include <hiredis/hiredis.h>
#include "VideoDetect.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"

using namespace cv;
using namespace std;
using namespace ml;
using namespace QtCharts;

namespace Ui {
class MainClient;
}

class MainClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainClient(QWidget *parent = 0);
    ~MainClient();
    void getRoundPixmap(QPixmap* src,QSize size);
    void download(const QString &url, const QString &filePath);
//    void transfer();
//    bool eventFilter(QObject *watched, QEvent *event);

public slots:
    void getPointStart(QPoint);
    void getPointEnd(QPoint);
    void onTimerOut();
    void slider_progress_clicked();
    void slider_progress_moved();
    void slider_progress_released();

private slots:

    void on_pushButton_B_clicked();

    void on_pushButton_C_clicked();

    void on_pushButton_D_clicked();

    void on_pushButton_A_clicked();

    void sendMessage(QStringList);

    void readMessage();

    void showString(QString s1, QString s2, QString s3, QString s4, QString s5, QString s6);

    void on_style_change_clicked();

    void replyFinished();

    void on_pushButton_switch_clicked();

    void on_pushButton_quit_clicked();

    void on_welcome_button_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clearHistory_clicked();

    void on_tableWidget_history_itemClicked(QTableWidgetItem *item);

    void on_tableWidget_history_itemDoubleClicked(QTableWidgetItem *item);

    void on_pushButton_lowProb_clicked();

    void on_comboBox_selectType_currentTextChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    bool eventFilter(QObject *watched, QEvent *event);

    void on_pushButton_E_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_generate_clicked();

    void on_pushButton_updatePageShowStaff_clicked();

    void on_pushButton_updatePageClearStaff_clicked();

    void on_pushButton_changeStaffInfo_clicked();

    void on_tableWidget_updateShowStaffInfo_cellClicked(int row, int column);

    void on_pushButton_confirmNewStaff_clicked();

    void on_pushButton_cancelNewStaff_clicked();

    void on_lineEdit_addNewUsername_editingFinished();

    void on_lineEdit_repeatPassword_editingFinished();

    void on_lineEdit_addNewEmail_editingFinished();

    void on_pushButton_displayCamera_clicked();

    void on_pushButton_clearCamera_clicked();

    void on_button_right_clicked();

    void on_button_left_clicked();

    void on_tableWidget_left_itemClicked(QTableWidgetItem *item);

    void on_tableWidget_right_itemClicked(QTableWidgetItem *item);

    void on_pushButton_showmap_clicked();

    void on_icon_search_clicked();

    void on_tableWidget_search_itemClicked(QTableWidgetItem *item);

    void on_pushButton_12_clicked();

    void on_pushButton_11_clicked();


    void on_pb_sendRequest_clicked();

    void on_pb_selectVideos_clicked();

    void on_pb_clearRect_clicked();

    void on_tw_videos_cellClicked(int row, int column);

    void on_pb_play_3_clicked();

    void on_pushButton_clicked();

    void on_horizontalSlider_up_valueChanged(int value);

    void on_horizontalSlider_down_valueChanged(int value);

    void on_pushButton_etcDisplay_clicked();

    void on_pushButton_etcClear_clicked();

    void on_comboBox_selectType_2_currentTextChanged(const QString &arg1);

    void on_tableWidget_accounts_itemClicked(QTableWidgetItem *item);

    void on_tableWidget_toPay_itemClicked(QTableWidgetItem *item);

    void on_pushButton_charge_clicked();

    void on_pushButton_sms_clicked();

    void finishedSlot(QNetworkReply*);

    void on_pushButton_etcCharge_clicked();

    void on_tableWidget_highway_itemClicked(QTableWidgetItem *item);

    void on_pushButton_generate_2_clicked();

    void on_pushButton_showRoute_clicked();

    void disconnected_method();

    void finishedSlot2(QNetworkReply* reply);

signals:
    void stringReturn(QString);

    void webReturn(QVector<QStringList>);

    void clearP();

private:
    void showSelectedTime();

    void SplitString(const string& s, vector<string>& v, const string& c);

    bool removeFolderContent(const QString &folderDir);

private:
    Ui::MainClient *ui;
    QNetworkAccessManager netManager;
    QString filepath;
    QTcpSocket *m_tcpsocket;
    QTcpSocket *m_socket;

    bool style;

    qint32 dataSize = 0;
    QString fileName;

    void progressBar_fast();
    void progressBar();

    QVector<QStringList> cameras;

    //for page_lpr1
    QVector<QStringList> lpr_history;

    int processPhase;
    QString fileNameForProcess;

    //for page_lpr2
    QString dirForImage;

    QVector<QStringList> lpr_search;

    //for settings
    QVector<QStringList> userdata_settings;

    //for page_statistics
    QVector<QStringList> traffic;
    QVector<QStringList> redlight;
    QVector<QStringList> overspeed;
    QString left;
    QString right;
    int set0_array[5];
    int set1_array[5];
    int set2_array[5];

    //for page_offence

    QMap<QString, QString> qm_videos;
    QPoint pStart, pEnd;
    QScrollArea *scrollArea;

    QVBoxLayout* layout_video;  //布局
    QMediaPlayer* player;   //播放器
    QVideoWidget* widget;   //视频播放控件
    QTimer * timer;
    int maxValue = 1000;
    bool play_state;    //播放状态，true为播放，false为暂停
    bool if_reload=false;   //是否重新载入视频

    QMediaPlayer *indexPlayer;//用于提取视频第一帧
    int video_width, video_height;
    bool if_index=false;
    qint64 duration;//视频时长

    // for page_etc
    QVector<QStringList> etcHighway;
    QVector<QStringList> etcAccount;
    QVector<QStringList> etcPayment;

    int success_count;
    int tel_count;
};

#endif // MAINCLIENT_H
