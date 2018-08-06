#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>
#include <QtNetwork>
#include <QThread>
#include <QDebug>
#include <QBitmap>
#include <QPainter>
#include <QMessageBox>
#include <QSqlQuery>
#include <QFontDatabase>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QDateTime>
#include <QTime>
#include <QDate>

#include "myserver.h"
#include "login.h"
#include "forgetpassword.h"
#include "sqltool.h"
#include "tool.h"
#include "CharRecognition.h"
#include "CharSegment.h"
#include "PlateDetection.h"
#include "PlateRecognition.h"

#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"

#define DIR "/Users/Haibara/Documents/qt build files2/Serverfiles"

#define DIR_ANN_DEFAULT "/Users/Haibara/Documents/GitHub/NMSL-Proj2/NaiveLPR-Server"

#define CONFIG_DIR "/Users/Haibara/Documents/qt build files2/Serverfiles/config.data"

#define MODEL_DIR "/Users/Haibara/Documents/qt build files2/Serverfiles/model.data"

using namespace cv;
using namespace ml;
//using namespace std;

class Processor : public QObject
{
    Q_OBJECT
public:
    explicit Processor(QTcpSocket*);

signals:

public slots:
    void work ();

private:
    QTcpSocket *m_socket;
    quint16 blocksize;
    QStringList list;

};

#endif // PROCESSOR_H
