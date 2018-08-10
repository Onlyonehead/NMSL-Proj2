#include "TCPConnection.h"
#include <QApplication>



#include "processor.h"

#define PICNUM 41

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SQLTool::connection();
    TCPConnection w;

    //    QString chars1[30] = {"云", "京", "冀", "吉", "宁", "川", "新", "晋", "桂", "沪",
    //                          "津", "浙", "渝", "湘", "琼", "甘", "皖", "粤", "苏", "蒙",
    //                          "豫", "贵", "赣", "辽", "鄂", "闽", "陕", "青", "鲁", "黑"};
    //    QString chars2[14] = {"A", "B", "C", "D", "E", "F", "G", "H", "J", "K",
    //                          "L", "M", "N", "P"};
    //    QString chars3[34] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
    //                          "A", "B", "C", "D", "E", "F", "G", "H", "J", "K",
    //                          "L", "M", "N", "P", "Q", "R", "S", "T", "U", "V",
    //                          "W", "X", "Y", "Z"};
    //    QString chars4 = "赵钱孙李周吴郑王冯陈褚卫蒋沈韩杨朱秦尤许何吕施张孔曹严华金魏陶姜戚谢邹喻柏水窦章"
    //                     "云苏潘葛奚范彭鲁韦昌马苗凤花方俞任袁柳酆鲍史唐费廉岑薛雷贺倪汤滕殷罗毕郝邬安常乐"
    //                     "于时傅皮卞齐康伍余元卜顾孟平黄和穆萧尹姚邵湛汪祁毛禹狄米贝明臧计伏成戴谈宋茅庞熊"
    //                     "纪舒屈项祝董梁杜阮蓝闵席季麻强贾路娄危江童颜郭梅盛林刁钟徐邱骆高夏蔡田樊胡凌霍虞"
    //                     "万支柯昝管卢莫经房裘缪干解应宗丁宣贲邓郁单杭洪包诸左石崔吉钮龚程嵇邢滑裴陆荣翁荀"
    //                     "羊於惠甄曲家封芮羿储靳汲邴糜松井段富巫乌焦巴弓牧隗山谷车侯宓蓬全郗班仰秋仲伊宫宁"
    //                     "仇栾暴甘钭厉戎祖武符刘景詹束龙叶幸司韶郜黎蓟薄印宿白怀蒲邰从鄂索咸籍赖卓蔺屠蒙池"
    //                     "乔阴鬱胥能苍双闻莘党翟谭贡劳逄姬申扶堵冉宰郦雍卻璩桑桂濮牛寿通边扈燕冀郏浦尚农温"
    //                     "别庄晏柴瞿阎充慕连茹习宦艾鱼容向古易慎戈廖庾终暨居衡步都耿满弘匡国文寇广禄阙东欧"
    //                     "殳沃利蔚越夔隆师巩厍聂晁勾敖融冷訾辛阚那简饶空曾毋沙乜养鞠须丰巢关蒯相查后荆红游"
    //                     "竺权逯盖益桓公万俟司马上官欧阳夏侯诸葛闻人东方赫连皇甫尉迟公羊澹台公冶宗政濮阳淳"
    //                     "于单于太叔申屠公孙仲孙轩辕令狐钟离宇文长孙慕容鲜于闾丘司徒司空丌官司寇仉督子车颛"
    //                     "孙端木巫马公西漆雕乐正壤驷公良拓跋夹谷宰父谷梁晋楚闫法汝鄢涂钦段干百里东郭南门呼"
    //                     "延归海羊舌微生岳帅缑亢况郈有琴梁丘左丘东门西门商牟佘佴伯赏南宫墨哈谯笪年爱阳佟第五言福";
    //    QString chars5[27] = {"134", "135", "136", "137", "138", "139", "150", "151", "152",
    //                        "158", "159", "157", "182", "187", "188", "147", "130", "131",
    //                        "132", "155", "156", "185", "186", "133", "153", "180", "189"};

    //    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //    for(int i = 1; i < 200000; i++){
    //        int num = 0 + qrand() % (30 - 0);
    //        QStringList list;

    //        QString s;
    //        s += (chars1[num]);
    //        num = 0 + qrand() % (14 - 0);

    //        s += (chars2[num]);
    //        num = 0 + qrand() % (34 - 0);
    //        s += (chars3[num]);
    //        num = 0 + qrand() % (34 - 0);
    //        s += (chars3[num]);
    //        num = 0 + qrand() % (34 - 0);
    //        s += (chars3[num]);
    //        num = 0 + qrand() % (34 - 0);
    //        s += (chars3[num]);
    //        num = 0 + qrand() % (34 - 0);
    //        s += (chars3[num]);
    //        list.append(s);

    //        s.clear();
    //        num = 0 + qrand() % (563 - 0);
    //        s += chars4.at(num);
    //        num = 0 + qrand() % (10 - 0);
    //        if(num > 6){
    //            num = 0 + qrand() % (563 - 0);
    //            s += chars4.at(num);
    //        }
    //        num = 0 + qrand() % (563 - 0);
    //        s += chars4.at(num);
    //        list.append(s);

    //        s.clear();
    //        num = 0 + qrand() % (27 - 0);
    //        s += chars5[num];
    //        for(int i = 0; i < 8; i++){
    //            num = 0 + qrand() % (10 - 0);
    //            s += QString::number(num);
    //        }
    //        list.append(s);

    //        num = 0 + qrand() % (5 - 0);
    //        if(num > 3){
    //            list.append("truck");
    //        }else{
    //            list.append("car");
    //        }

    //        num = 99 + qrand() % (2000 - 99);

    //        list.append(QString::number(num));

    //        SQLTool::insert("ETCAccounts", list);
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
