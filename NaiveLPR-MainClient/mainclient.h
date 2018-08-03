#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#define DIR "/Users/Haibara/Documents/qt build files2/Clientfiles"

#define CONFIG_DIR "/Users/Haibara/Documents/qt build files2/Clientfiles/config.data"

#define MODEL_DIR "/Users/Haibara/Documents/qt build files2/Clientfiles/model.data"

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

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"

using namespace cv;
using namespace std;
using namespace ml;

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

signals:
    void stringReturn(QString);

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

    //for page_lpr1
    QVector<QStringList> lpr_history;

    int processPhase;
    QString fileNameForProcess;

    //for page_lpr2
    QString dirForImage;

    //for settings
    QVector<QStringList> userdata_settings;

};

#endif // MAINCLIENT_H
