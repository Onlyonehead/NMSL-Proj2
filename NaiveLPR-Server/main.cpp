#include "TCPConnection.h"
#include <QApplication>



#include "processor.h"

#define PICNUM 41

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SQLTool::connection();
    TCPConnection w;

//    for(int i = 1; i < 15; i++){
//        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//        QDate current_date = QDate::currentDate();
//        for(int j = 0; j < 13; j++){
//            int num = 0 + qrand() % (10 - 0);
//            QString date = current_date.addDays(-j).toString("yyyy-MM-dd");
//            QStringList list;
//            list.append(QString::number(i));
//            list.append(date);
//            list.append(QString::number(num));
//            SQLTool::insert("overspeed", list);
//        }
//    }

//    CharRecognition::ANN_Train("/Users/Haibara/Desktop/LPCharSamples", 65, 5000, 0.0001);
//    CharRecognition::ANN_Train_SP("/Users/Haibara/Desktop/LPCharSamples", 65, 5000, 0.0001);
//    CharRecognition::ANN_Train_CH("/Users/Haibara/Desktop/LPCharSamples", 65, 5000, 0.0001);

//    int failCount = 0;
//    for(int i = 1; i <= PICNUM; i++) {
//        string filename = "/Users/Haibara/Desktop/carPicture/P" + to_string(i) + ".jpg";//打开的文件
//        Mat srcImage = imread(filename, 1);
//        Mat midImage;
//        midImage = PlateDetection::process(srcImage);
//        Mat result = PlateRecognition::process(midImage);
//        vector<Mat> chars = CharSegment::process(midImage, result);
//        string plate = "";
//        double rate = 0;

//        for(int j = 0; j < 7; ++j){
//            if(j == 0){
//                vector<string> r = CharRecognition::process_ch(chars.at(j));
//                plate += r.at(0);
//                rate += stod(r.at(1)) / 7;
//            }else if(j == 1){
//                vector<string> r = CharRecognition::process_sp(chars.at(j));
//                plate += r.at(0);
//                rate += stod(r.at(1)) / 7;
//            }else{
//                vector<string> r = CharRecognition::process(chars.at(j));
//                plate += r.at(0);
//                rate += stod(r.at(1)) / 7;
//            }
//        }
//        cout << "Plate:  " << plate << endl;
//        cout << "Probability:  " << rate << "%" << endl;
//        if(rate < 90){
//            failCount++;
//        }
//    }
//    cout << endl <<  "Total accuracy:  " << (double)(PICNUM - failCount) * 100 / PICNUM << "%" << endl;

    return a.exec();
}
