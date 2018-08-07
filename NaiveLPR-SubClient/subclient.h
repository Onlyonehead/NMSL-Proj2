#ifndef SUBCLIENT_H
#define SUBCLIENT_H

#include <QMainWindow>

#include <QFont>
#include <QTabBar>

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QFontDatabase>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtNetwork>
#include <QObject>
#include <QUrl>
#include <QTime>
#include <QDateTime>
#include <QDate>
#include <QCompleter>
#include <QStringListModel>
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
#include <QFileDialog>
#include <QWebChannel>
#include "bridge.h"

#define DIR "/Users/Haibara/Documents/qt build files2/Clientfiles"

#define LOGIN_DIR "/Users/Haibara/Documents/qt build files2/Clientfiles/pwd_sub.data"

namespace Ui {
class SubClient;
}

class SubClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubClient(QWidget *parent = 0);
    ~SubClient();
    void getRoundPixmap(QPixmap* src,QSize size);
    void download(const QString &url, const QString &filePath);

private slots:
    void on_pushButton_quit_2_clicked();

    void replyFinished();

    void on_pushButton_login_clicked();

    void sendMessage(QStringList);

    void readMessage();

    void on_pushButton_switch_clicked();

    void on_pushButton_quit_clicked();

    void on_pushButton_addNewPortrait_clicked();

    void on_pushButton_confirmNewStaff_clicked();

    bool eventFilter(QObject *watched, QEvent *event);

    void on_pushButton_confirmNewStaff_2_clicked();

    void on_pushButton_B_clicked();

    void on_pushButton_A_clicked();

    void on_pushButton_switch_3_clicked();

    void on_pushButton_quit_3_clicked();

    void on_pushButton_A_2_clicked();

    void on_pushButton_B_2_clicked();

    void slotSendDataToH5(bool ok);

    void on_icon_search_clicked();

    void on_tableWidget_search_itemClicked(QTableWidgetItem *item);

    void on_pushButton_set1_clicked();

    void on_pushButton_set2_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_confirm_clicked();

private:
    Ui::SubClient *ui;

    Bridge bridge;

    QString url;

    QStringList position;

    QVector<QStringList> camera_search;

    QNetworkAccessManager netManager;
    QString filepath;
    QTcpSocket *m_tcpsocket;
    QTcpSocket *m_socket;

    qint32 dataSize = 0;
    QString fileName;

    void progressBar_fast();
    void progressBar();

    //check pwd.data and auto save password to file
    int checkpass(QString, QString, bool);

    //tool method
    void sendSignalForLogin(QString);
};

#endif // SUBCLIENT_H
