#include "processor.h"
#include <QDateTime>

Processor::Processor(QTcpSocket* socket)
{
    this->m_socket = socket;
}

void Processor::work ()
{

    if(!SQLTool::connectionState()){
        SQLTool::connection();
    }
    QDataStream in(m_socket);
    in.setVersion (QDataStream::Qt_5_7);
    quint16 blocksize = 0;
    if (m_socket->bytesAvailable() < (int)sizeof(quint16)){
        return;

    }
    in >> blocksize;

    if(m_socket->bytesAvailable() < blocksize){
        return;
    }

    QStringList list;
    in >> list;

    QByteArray message;
    QDataStream out(&message,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);

    QString function = list.at(0);
    list.removeFirst();

    qDebug() << function << endl;

    out << (quint16) 0;

    /*
     * hzc
     *
     */

    if(function == "Default"){
        out << "Default";
    }

    if(function == "login"){
        QString username = list.at(0);
        QString password = list.at(1);
        int result = Login::login(username, password);
        out << function;
        out << result;

    }
    if(function == "info"){
        QStringList result = Login::info(list.at(0));
        out << function;
        out << result;

    }

    if(function == "send"){
        QString username = list.at(0);
        QString email = list.at(1);
        QString securityCode = list.at(2);
        int result = ForgetPassword::sendCode(username, email, securityCode);
        out << function;
        out << result;

    }

    if(function == "check"){
        QString username = list.at(0);
        QString pw1 = list.at(1);
        QString pw2 = list.at(2);
        QString code = list.at(3);
        QString securityCode = list.at(4);
        int result = ForgetPassword::checkCode(username, pw1, pw2, code, securityCode);
        out << function;
        out << result;

    }
    if(function == "transfer"){
        QSqlQuery query;
        SQLTool::search(query, "plates");

        int count0;
        int count1;

        while(query.next()){
            QStringList l;
            if(query.value(3).toString() == "USER"){
                count0++;
            }else{
                count1++;
            }
        }

        SQLTool::search(query, "cameras");
        QVector<QStringList> vlist;

        while(query.next()){
            QStringList l;
            l.append(query.value(0).toString());
            l.append(query.value(1).toString());
            l.append(query.value(2).toString());
            vlist.append(l);
        }
        out << function;
        out << count0;
        out << count1;
        out << vlist;
    }

    if(function == "LPR"){

        //init model

        string dir;

        QString model = "";

        QFile *file = new QFile(MODEL_DIR);
        if(file->exists()){
            if (file->open(QFile::ReadWrite | QFile::Text)){
               model = file->readLine().trimmed();
            }else{
                qDebug()<<"打开失败";
            }
        }
        file->close();


        if(model == "" || model == "DEFAULT"){
            dir = DIR_ANN_DEFAULT;
        }else{
            QString tmp;
            QFile *file = new QFile(CONFIG_DIR);
            if(file->exists()){
                if (file->open(QFile::ReadWrite | QFile::Text)){
                   tmp = file->readLine().trimmed();
                }else{
                    qDebug()<<"打开失败";
                }
            }
            file->close();
            if(tmp.trimmed() != ""){
                QFile *file = new QFile(tmp + "/ANN-Model-CH.xml");
                if(file->exists()){
                    dir = tmp.toStdString();
                }else{
                    dir = DIR_ANN_DEFAULT;
                }

            }else{
                dir = DIR_ANN_DEFAULT;
            }
        }


        QString filename = list.at(0);

        QString plateName = filename.split(".").at(0) + "-plate.jpg";

        QString plateRName = filename.split(".").at(0) + "-plateR.jpg";

        QStringList charName;
        for(int i = 0; i < 7; i++){
            charName.append(filename.split(".").at(0) + "-" + QString::number(i) + ".jpg");
        }

        Mat srcImage = imread((DIR + QString("/plates/") + filename).toStdString(), 1);

        Mat midImage;

        midImage = PlateDetection::process(srcImage);

        if(midImage.rows < 2 || midImage.cols < 2){
            out << function;
            out << QString("FAIL");
        }else{
            imwrite((DIR + QString("/plates-d/") + plateName).toStdString(), midImage);

            Mat result = PlateRecognition::process(midImage);

            imwrite((DIR + QString("/plates-r/") + plateRName).toStdString(), result);
            vector<Mat> chars = CharSegment::process(midImage, result);
            string plate = "";
            double rate = 0;

            for(int j = 0; j < 7; ++j){
                imwrite((DIR + QString("/chars/") + charName.at(j)).toStdString(), chars.at(j));
                if(j == 0){
                    vector<string> r = CharRecognition::process_ch(chars.at(j), dir);
                    plate += r.at(0);
                    rate += stod(r.at(1)) / 7;
                }else if(j == 1){
                    vector<string> r = CharRecognition::process_sp(chars.at(j), dir);
                    plate += r.at(0);
                    rate += stod(r.at(1)) / 7;
                }else{
                    vector<string> r = CharRecognition::process(chars.at(j), dir);
                    plate += r.at(0);
                    rate += stod(r.at(1)) / 7;
                }
            }

            QStringList list;
            list.append(filename.split(".").at(0));
            list.append(QString::fromStdString(plate));
            list.append(QString::number(rate));
            list.append("USER");
            SQLTool::insert("plates", list);

            out << function;
            out << plateName;
            out << QString::fromStdString(plate);
            out << rate;
        }
    }

    if(function == "lpr_history"){
        QSqlQuery query;
        SQLTool::search(query, "plates");
        QVector<QStringList> vlist;

        while(query.next()){
            QStringList l;
            l.append(query.value(0).toString());
            l.append(query.value(1).toString());
            l.append(query.value(2).toString());
            l.append(query.value(3).toString());
            vlist.append(l);
        }


        out << function;
        out << vlist;
    }

    if(function == "lpr_lowProb"){
        QSqlQuery query;
        SQLTool::search(query, "plates");
        QVector<QStringList> vlist;

        while(query.next()){
            if(query.value(2).toDouble() < 90 && query.value(2).toString() != "Checked"){
                QStringList l;
                l.append(query.value(0).toString());
                l.append(query.value(1).toString());
                l.append(query.value(2).toString());
                l.append(query.value(3).toString());
                vlist.append(l);
            }
        }


        out << function;
        out << vlist;
    }

    if(function == "manual"){
        QString fileName = list.at(0);
        QString number = list.at(1);
        QString formerNumber = list.at(2);

        SQLTool::update("plates", "text", number, "pic_name", fileName);

        SQLTool::update("plates", "probability", "Checked", "pic_name", fileName);

        out << function;
        if(formerNumber.trimmed() == number.trimmed()){
            out << QString("Checked");
        }else{
            out << QString("Edited");
        }
    }

    if(function == "setModel"){
        QString model_type = list.at(0);
        QFile *file = new QFile(MODEL_DIR);
        if (file->open(QFile::ReadWrite | QFile::Text)){
            QTextStream out(file);
            out << model_type << "\n";
        }else{
            qDebug()<<"打开失败";
        }
        file->close();

        out << function;
        out << QString("Done");
    }

    if(function == "trainModel"){
        QString dir = list.at(0);

        QFile *file = new QFile(CONFIG_DIR);
        if (file->open(QFile::ReadWrite | QFile::Text)){
            QTextStream out(file);
            out << dir << "\n";
        }else{
            qDebug()<<"打开失败";
        }
        file->close();

        out << function;
        out << QString("Done");
    }

    if(function == "del_entry"){
        QString pic_name = list.at(0);

        SQLTool::del("plates", "pic_name", pic_name);

        out << function;
        out << QString("Done");
    }

    if(function == "showUser"){
        QVector<QStringList> result;

        QSqlQuery query;
        QStringList list;
        SQLTool::search(query, "userdata");
        while (query.next()) {
            list.clear();
            list.append(query.value(0).toString());
            list.append(query.value(2).toString());
            list.append(query.value(3).toString());
            list.append(query.value(4).toString());
            list.append(query.value(5).toString());
            list.append(query.value(6).toString());
            result.append(list);
        }

        out << function;
        out << result;
    }

    if(function == "updateUser"){
        QString username = list.at(0);
        QString name = list.at(1);
        QString gender = list.at(2);
        QString position = list.at(3);
        QString email = list.at(4);

        SQLTool::update("userdata", "name", name, "username", username);
        SQLTool::update("userdata", "gender", gender, "username", username);
        SQLTool::update("userdata", "position", position, "username", username);
        SQLTool::update("userdata", "email", email, "username", username);

        out << function;
        out << QString("Done");
    }

    if(function == "pp2_anu"){
        QString username = list.at(0);
        QSqlQuery query;
        SQLTool::search(query, "username", "userdata", "username", username);
        out << function;
        out << query.next();
    }

    if(function == "pp2_ane"){
        QString email = list.at(0);
        QSqlQuery query;
        SQLTool::search(query, "email", "userdata", "email", email);
        out << function;
        out << query.next();
    }

    if(function == "addUser"){
        QString username = list.at(0);
        QString password = list.at(1);
        QString name = list.at(2);
        QString gender = list.at(3);
        QString position = list.at(4);
        QString email = list.at(5);
        QString userPic = list.at(6);
        QStringList staffInfo;
        staffInfo.append(username);
        staffInfo.append(password);
        staffInfo.append(name);
        staffInfo.append(gender);
        staffInfo.append(position);
        staffInfo.append(email);
        staffInfo.append(userPic);
        SQLTool::insert("userdata", staffInfo);

        out << function;
        out << QString("Done");
    }

    if(function == "displayCamera"){
        QSqlQuery query;
        SQLTool::search(query, "traffic_flow");
        QVector<QStringList> vlist;

        QSet<QString> ids;

        while(query.next()){
            QStringList l;
            ids.insert(query.value(0).toString());
            l.append(query.value(0).toString());
            l.append(query.value(1).toString());
            l.append(query.value(2).toString());
            vlist.append(l);
        }

        SQLTool::search(query, "redlight");
        QVector<QStringList> rlist;

        while(query.next()){
            QStringList l;
            l.append(query.value(0).toString());
            l.append(query.value(1).toString());
            l.append(query.value(2).toString());
            rlist.append(l);
        }

        SQLTool::search(query, "overspeed");
        QVector<QStringList> olist;

        while(query.next()){
            QStringList l;
            l.append(query.value(0).toString());
            l.append(query.value(1).toString());
            l.append(query.value(2).toString());
            olist.append(l);
        }

        out << function;
        out << ids;
        out << vlist;
        out << rlist;
        out << olist;
    }




    //main cient ETCpage display all ETC vehicles
       if(function == "mc_ETCp_diaplay_ETCVehcles"){
           QSqlQuery query;
           QVector<QStringList> result;
           SQLTool::search(query, "ETCplates");
           QStringList list;
           while (query.next()) {
               list.clear();
               list.append(query.value(0).toString());
               list.append(query.value(1).toString());
               result.append(list);
           }
           out << function;
           out << result;
       }
       //main client ETCpage display all on road vehicles
          if(function == "nc_ETCp_displayOnRoadVehicles"){
              QSqlQuery query;
              QVector<QStringList> result;
              QStringList list;
              SQLTool::search(query, "ETCplates", "on_highway", QString::number(1));
              while (query.next()) {
                  list.clear();
                  list.append(query.value(0).toString());
                  list.append(query.value(1).toString());
                  result.append(list);
              }
              out << function;
              out << result;
          }
          //main client ETCpage display pay history
              if(function == "m_ETCp_displayPayHistory"){
                  QSqlQuery query;
                  QVector<QStringList> result;
                  QStringList list;
                  SQLTool::search(query, "ETChistoryPay");
                  while (query.next()) {
                      list.clear();
                      list.append(query.value(0).toString());
                      list.append(query.value(1).toString());
                      list.append(query.value(2).toString());
                      result.append(list);
                  }
                  out << function;
                  out << result;
              }
              //ETCpage send enter_highway_pic
              if(function == "ETCp_sendEnterPic"){

                  //init model

                  string dir;

                  QString model = "";

                  QFile *file = new QFile(MODEL_DIR);
                  if(file->exists()){
                      if (file->open(QFile::ReadWrite | QFile::Text)){
                         model = file->readLine().trimmed();
                      }else{
                          qDebug()<<"打开失败";
                      }
                  }
                  file->close();


                  if(model == "" || model == "DEFAULT"){
                      dir = DIR_ANN_DEFAULT;
                  }else{
                      QString tmp;
                      QFile *file = new QFile(CONFIG_DIR);
                      if(file->exists()){
                          if (file->open(QFile::ReadWrite | QFile::Text)){
                             tmp = file->readLine().trimmed();
                          }else{
                              qDebug()<<"打开失败";
                          }
                      }
                      file->close();
                      if(tmp.trimmed() != ""){
                          QFile *file = new QFile(tmp + "/ANN-Model-CH.xml");
                          if(file->exists()){
                              dir = tmp.toStdString();
                          }else{
                              dir = DIR_ANN_DEFAULT;
                          }

                      }else{
                          dir = DIR_ANN_DEFAULT;
                      }
                  }






                  QString filename = list.at(0);

                  QString plateName = filename.split(".").at(0) + "-plate.jpg";

                  QString tempPath = DIR + QString("/plates/") + filename;

                  Mat srcImage = imread(tempPath.toStdString(), 1);

                  Mat midImage;

                  bool isPlateETCAvailable;

                  QSqlQuery query;

                  QString set_off_coordinate = list.at(1);

                  midImage = PlateDetection::process(srcImage);

                  if(midImage.rows < 2 || midImage.cols < 2){
                      out << function;
                      out << QString("FAIL");
                  }else{
                      imwrite(tempPath.toStdString(), midImage);

                      Mat result = PlateRecognition::process(midImage);
                      vector<Mat> chars = CharSegment::process(midImage, result);
                      string plate = "";
                      string vehicleType;
                      vehicleType = PlateRecognition::carType(midImage);
                      string type;
                      if(vehicleType == "blue"){
                          type = "car";
                      }else {
                          type = "van";
                      }




                      for(int j = 0 ; j < 7 ; j++){
                          if(j == 0){
                              vector<string> r = CharRecognition::process_ch(chars.at(j), dir);
                              plate += r.at(0);
                          }else if(j == 1){
                              vector<string> r = CharRecognition::process_sp(chars.at(j), dir);
                              plate += r.at(0);
                          }else {
                              vector<string> r = CharRecognition::process(chars.at(j), dir);
                              plate += r.at(0);
                          }
                      }

                      SQLTool::search(query, "plate", "ETCplates", "plate", QString::fromStdString(plate));
                      if(query.next()){
                          isPlateETCAvailable = true;
                      }else{
                          isPlateETCAvailable = false;
                      }

                      SQLTool::update(query, "ETCplates", "set_off_coordinate", set_off_coordinate,
                                      "plate", QString::fromStdString(plate));

                      SQLTool::update(query, "ETCplates", "on_highway", QString::number(1), "plate", QString::fromStdString(plate));
                      SQLTool::update(query, "ETCplates", "isCharged", QString::number(1), "plate", QString::fromStdString(plate));
                      SQLTool::update(query, "ETCplates", "type", QString::fromStdString(type), "plate", QString::fromStdString(plate));
                      // isCharged = 1时表示未收费

                      out << function;
                      out << plateName;
                      out << QString::fromStdString(plate);
                      out << isPlateETCAvailable;
                  }
              }
              //ETCpage send leave_highway_pic
              if(function == "ETCp_sendLeavePic"){

                  //init model

                  string dir;

                  QString model = "";

                  QFile *file = new QFile(MODEL_DIR);
                  if(file->exists()){
                      if (file->open(QFile::ReadWrite | QFile::Text)){
                         model = file->readLine().trimmed();
                      }else{
                          qDebug()<<"打开失败";
                      }
                  }
                  file->close();


                  if(model == "" || model == "DEFAULT"){
                      dir = DIR_ANN_DEFAULT;
                  }else{
                      QString tmp;
                      QFile *file = new QFile(CONFIG_DIR);
                      if(file->exists()){
                          if (file->open(QFile::ReadWrite | QFile::Text)){
                             tmp = file->readLine().trimmed();
                          }else{
                              qDebug()<<"打开失败";
                          }
                      }
                      file->close();
                      if(tmp.trimmed() != ""){
                          QFile *file = new QFile(tmp + "/ANN-Model-CH.xml");
                          if(file->exists()){
                              dir = tmp.toStdString();
                          }else{
                              dir = DIR_ANN_DEFAULT;
                          }

                      }else{
                          dir = DIR_ANN_DEFAULT;
                      }
                  }




                  QString filename = list.at(0);

                  QString plateName = filename.split(".").at(0) + "-plate.jpg";

                  QString tempPath = DIR + QString("/plates/") + filename;

                  Mat srcImage = imread(tempPath.toStdString(), 1);

                  Mat midImage;

                  QSqlQuery query;

                  QString destination_coordinate = list.at(1);

                  midImage = PlateDetection::process(srcImage);

                  if(midImage.rows < 2 || midImage.cols < 2){
                      out << function;
                      out << QString("FAIL");
                  }else {
                      imwrite(tempPath.toStdString(), midImage);

                      Mat result = PlateRecognition::process(midImage);
                      vector<Mat> chars = CharSegment::process(midImage, result);
                      string plate = "";

                      for(int j = 0 ; j < 7 ; j++){
                          if(j == 0){
                              vector<string> r = CharRecognition::process_ch(chars.at(j), dir);
                              plate += r.at(0);
                          }else if(j == 1){
                              vector<string> r = CharRecognition::process_sp(chars.at(j), dir);
                              plate += r.at(0);
                          }else {
                              vector<string> r = CharRecognition::process(chars.at(j), dir);
                              plate += r.at(0);
                          }
                      }

                      SQLTool::update(query, "ETCplates", "destination_coordinate", destination_coordinate,
                                      "plate", QString::fromStdString(plate));
                      SQLTool::update(query, "ETCplates", "on_highway", QString::number(0), "plate", QString::fromStdString(plate));

                      out << function;
                      out << plateName;
                      out << QString::fromStdString(plate);



                  }
              }
              //main client ETC page recharge
              if(function == "mc_recharge"){
                QString plate = list.at(0);
                QString tempRecharge = list.at(1);
                int recharge = tempRecharge.toInt();
                QSqlQuery query;
                SQLTool::search(query, "balance", "ETCplates", "plate", plate);
                int balance;
                if(query.next()){
                    balance = query.value(0).toInt();
                }
                balance += recharge;
                SQLTool::update("ETCplates", "balance", QString::number(balance), "plate", plate);
                out << function;
              }
              //main client ETC page dpsplay vehicles with arrears
              if(function == "mc_displayVehiclesWA"){
                  QSqlQuery query;
                  QVector<QStringList> result;
                  QStringList list;
                  SQLTool::search(query, "ETCplates", "isCharged", QString::number(1));
                  while (query.next()) {
                      list.clear();
                      list.append(query.value(0).toString());
                      list.append(query.value(1).toString());
                      result.append(list);
                  }
                  out << function;
                  out << result;
              }
              //main client ETCpage search plates
              if(function == "mc_ETCp_searchPlate"){
                  QString searchPlate = list.at(0);
                  QSqlQuery query;
                  QVector<QStringList> result;
                  QStringList list;
                  SQLTool::fuzzySearch(query, "ETCplates", "plate", searchPlate);
                  while (query.next()) {
                      list.clear();
                      list.append(query.value(0).toString());
                      list.append(query.value(1).toString());
                      result.append(list);
                  }
                  out << function;
                  out << result;
              }
              //main client delete plate
              if(function == "mc_DeletePlate"){
                  QString deletePlate = list.at(0);
                  SQLTool::del("ETCplates", "plate", deletePlate);
                  out << function;
              }



    out.device()->seek(0);
    out << (quint16) (message.size() - sizeof(quint16));
    m_socket->write(message);
    m_socket->flush();
}
