#include "login.h"



/**
 * login method
 *
 * @author hzc
 * @param username username
 * @param password password
 * @param isChecked true if checkbox is checked
 */
int Login::login(QString username, QString password){

    if(username == ""){
        return -2;
    }
    if(password == ""){

        return -3;
    }

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString till_time = current_date_time.addSecs(300).toString("yyyy-MM-dd hh:mm:ss");

    QSqlQuery query;
    SQLTool::search(query, "userdata", "username", username);
    if(query.next()){

        QSqlQuery query2;
         SQLTool::search(query2, "blacklist", "username", username);
        if(query2.next()){

            QString sql_till_time_string = query2.value(1).toString();
            qDebug() << sql_till_time_string << endl;
            QStringList s = sql_till_time_string.split(QRegExp("[-A-Z:]"));
            int year = s[0].toInt();
            int month = s[1].toInt();
            int day = s[2].toInt();
            int hour = s[3].toInt();
            int minute = s[4].toInt();
            int second = s[5].toInt();
            QTime time;
            time.setHMS(hour,minute,second);
            QDate date;
            date.setDate(year, month, day);
            QDateTime sql_till_time;
            sql_till_time.setDate(date);
            sql_till_time.setTime(time);


            if(sql_till_time > current_date_time){
                if(query2.value(2) == 3){

                    return -4;
                }
            }else{
                SQLTool::del("blacklist", "username", username);
            }
        }


        QString pwd = query.value(1).toString();
        QByteArray bytePwd = pwd.toLatin1();
        QByteArray bytePwdMd5 = QCryptographicHash::hash(bytePwd, QCryptographicHash::Md5);
        QString strPwdMd5 = bytePwdMd5.toHex();
        qDebug() << password << " " << strPwdMd5 << endl;


        if(password == pwd || password == strPwdMd5){
            QSqlQuery query5;
            SQLTool::search(query5, "blacklist", "username", username);
            if(query5.next()){
                SQLTool::del("blacklist", "username", username);
            }
            return 0;
        }else{

            QSqlQuery query3;
            SQLTool::search(query3, "blacklist", "username", username);
            if(query3.next()){
                SQLTool::update("blacklist", "error_count", QString::number((query3.value(2).toInt() + 1)),
                        "username", username);
                if(query3.value(2) == 2){
                    SQLTool::update("blacklist", "till_time", till_time,
                            "username", username);
                    return -5;
                }else{
                    return -6;
                }

            }else{

                QStringList list;
                list << username << till_time << "1";
                SQLTool::insert("blacklist", list);
                return -7;
            }
        }
    }else{

        return -8;
    }

}

/**
 * return infomation
 *
 * @author hzc
 * @param username username
 * @return infomation of user
 */
QStringList Login::info(QString username){
    QSqlQuery query;
    SQLTool::search(query, "userdata", "username", username);
    query.next();
    QStringList list;
    list.append(username);
    list.append(query.value(2).toString());
    list.append(query.value(4).toString());
    list.append(query.value(3).toString());
    list.append(query.value(5).toString());
    list.append(query.value(6).toString());
    return list;
}
