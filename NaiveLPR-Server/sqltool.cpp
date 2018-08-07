#include <sqltool.h>


/**
 * connection state
 *
 * @author hzc
 */

bool SQLTool::connectionState(){
    QSqlQuery query;
    SQLTool::search(query, "userdata", "username", "koushizon");
    return query.isActive();
}

/**
 * connect database
 *
 * @author hzc
 */
void SQLTool::connection(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("kousz.top");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("abcd1234");
    db.setDatabaseName("project2");
    if(!db.open()){
        qDebug() << "Unable to open database" << endl;
    }else{
        qDebug() << "Open database project1" << endl;
    }

//    QSqlDatabase db2 = QSqlDatabase::addDatabase("QMYSQL","db2");
//    db2.setHostName("kousz.top");
//    db2.setPort(3306);
//    db2.setUserName("root");
//    db2.setPassword("abcd1234");
//    db2.setDatabaseName("proj1_warehouse");
//    if(!db2.open()){
//        qDebug() << "Unable to open database" << endl;
//    }else{
//        qDebug() << "Open database proj1_warehouse" << endl;
//    }
}

/**
 * disconnect database
 *
 * @author hzc
 */
void SQLTool::disconnection(){
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    qDebug() << "Close database proj1_main" << endl;
}


/**
 * E.g. "select * from blacklist"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param s table name
 */
void SQLTool::search(QSqlQuery &query, QString s){

    QString sql = "select * from "+ s;
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }

    qDebug()  << sql << endl;

}

/**
 * E.g. "select id from blacklist"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param s1 field you want to select
 * @param s1 table name
 */
void SQLTool::search(QSqlQuery &query, QString s1, QString s2){

    QString sql = "select "+ s1 +" from "+ s2;
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }

    qDebug()  << sql << endl;

}


/**
 * E.g. "select username,password from blacklist"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param list fields you want to select
 * @param s table name
 */
void SQLTool::search(QSqlQuery &query, QStringList list, QString s){
    int n = list.size();
    QString sql;
    sql += "select ";
    for(int i = 0; i < n; i ++){
        sql += list.at(i);
        if(i != n-1){
            sql += ",";
        }else{
            sql += " from ";
            sql += s;
        }
    }
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }
    qDebug()  << sql << endl;
}


/**
 * E.g. "select username from blacklist where username='koushizon'"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param s1 field you want to select
 * @param s2 table name
 * @param s3 field name (for specific row you want to select)
 * @param s4 field value
 */
void SQLTool::search(QSqlQuery &query, QString s1, QString s2, QString s3, QString s4){
    QString sql;
    sql += "select " + s1 + " from " + s2 + " where "+ s3 +"='"+ s4 +"'";;
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }
    qDebug()  << sql << endl;
}


/**
 * E.g. "select username,password from blacklist where username='koushizon'"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param list fields you want to select
 * @param s1 table name
 * @param s2 field name (for specific row you want to select)
 * @param s3 field value
 */
void SQLTool::search(QSqlQuery &query, QStringList list, QString s1, QString s2, QString s3){
    int n = list.size();
    QString sql;
    sql += "select ";
    for(int i = 0; i < n; i ++){
        sql += list.at(i);
        if(i != n-1){
            sql += ",";
        }else{
            sql += " from " + s1 + " where "+ s2 +"='"+ s3 +"'";
        }
    }
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }
    qDebug()  << sql << endl;
}


/**
 * E.g. "select * from blacklist where username='koushizon'"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param s1 table name
 * @param s2 field name (for specific row you want to select)
 * @param s3 field value
 */
void SQLTool::search(QSqlQuery &query, QString s1, QString s2, QString s3){
    QString sql = "select * from "+ s1 +" where "+ s2 +"='"+ s3 +"'";
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }
    qDebug()  << sql << endl;
}


/**
 * E.g. "select * from blacklist where username='koushizon' and password='123'"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param s table name
 * @param list field name and field value
 */
void SQLTool::search(QSqlQuery &query, QString s, QStringList list){
    QString sql = "select * from "+ s +" where ";

    int n = list.size();
    while(n){
        sql += list.at(n-2) +"='"+ list.at(n-1);
        if(n != 2){
            sql +="' and ";
        }else{
            sql += "'";
        }
        n -= 2;
    }

    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }
    qDebug()  << sql << endl;
}

/**
 * @brief SQLTool::fuzzySearch
 * E.g. "select * from userdata where username like '%dong%'"
 */
void SQLTool::fuzzySearch(QSqlQuery &query, QString s1, QString s2, QString s3)
{
    QString sql = "select * from " + s1 + " where " + s2 + " like '" + "%" + s3 + "%'";
    query.exec(sql);

    if(!query.isActive()){
        qDebug() << "fail to exec:  ";
    }
    qDebug() << sql << endl;

}

/**
 * E.g. "select till_time from blacklist where username='koushizon' and password='123'"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param s1 field you want to select
 * @param s2 table name
 * @param list field name and field value
 */
void SQLTool::search(QSqlQuery &query, QString s1, QString s2, QStringList list){
    QString sql = "select "+ s1 +" from "+ s2 +" where ";

    int n = list.size();
    while(n){
        sql += list.at(n-2) +"='"+ list.at(n-1);
        if(n != 2){
            sql +="' and ";
        }else{
            sql += "'";
        }
        n -= 2;
    }
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }
    qDebug()  << sql << endl;
}

/**
 * E.g. "delete from blacklist where username='koushizon'"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param s1 table name
 * @param s2 field name (for specific row you want to delete)
 * @param s3 field value
 */
void SQLTool::del(QSqlQuery &query, QString s1, QString s2, QString s3){
    QString sql = "delete from "+ s1 +" where "+ s2 +"='"+ s3 +"'";
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }

    qDebug()  << sql << endl;
}


/**
 * E.g. "update userdata set password='123' where username='koushizon'"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param s1 table name
 * @param s2 field you want to update
 * @param s3 field value
 * @param s4 field name (for specific row you want to update)
 * @param s5 field value
 */
void SQLTool::update(QSqlQuery &query, QString s1, QString s2, QString s3, QString s4, QString s5){
     QString sql = "UPDATE "+ s1 +" SET "+ s2 +" = '" + s3 +
             "' WHERE "+ s4 +" = '" + s5 + "' ";
     query.exec(sql);

     if(!query.isActive()){
         qDebug()  << "fail to exec:  ";
     }

     qDebug()  << sql << endl;
}


/**
 * E.g. "insert into userdata valuses ('koushizon','123','杭咨存','男','管理员','koushizon@outlook.com')"
 *
 * @author hzc
 * @param query QSqlQuery
 * @param s table name
 * @param list values
 */
void SQLTool::insert(QSqlQuery &query, QString s, QStringList list){
    int n = list.size();
    QString sql;
    sql += "INSERT INTO "+ s +" VALUES ('";
    for(int i = 0; i < n; i++){
        sql += list.at(i);
        if(i != n-1){
            sql += "', '";
        }else{
            sql += "')";
        }
    }
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }
    qDebug()  << sql << endl;
}

/**
 * E.g. "delete from blacklist where username='koushizon'"
 *
 * @author hzc
 * @param s1 table name
 * @param s2 field name (for specific row you want to delete)
 * @param s3 field value
 */
void SQLTool::del(QString s1, QString s2, QString s3){
    QSqlQuery query;
    QString sql = "delete from "+ s1 +" where "+ s2 +"='"+ s3 +"'";
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }

    qDebug()  << sql << endl;
}

/**
 * E.g. "delete from blacklist where username='koushizon' and password='123'"
 *
 * @author hzc
 * @param s table name
 * @param list field name and field value
 */
void SQLTool::del(QString s, QStringList list){
    QSqlQuery query;
    QString sql = "delete from "+ s +" where ";
    int n = list.size();
    while(n){
        sql += list.at(n-2) +"='"+ list.at(n-1);
        if(n != 2){
            sql +="' and ";
        }else{
            sql += "'";
        }
        n -= 2;
    }

    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }

    qDebug()  << sql << endl;
}

/**
 * E.g. "update userdata set password='123' where username='koushizon'"
 *
 * @author hzc
 * @param s1 table name
 * @param s2 field you want to update
 * @param s3 field value
 * @param s4 field name (for specific row you want to update)
 * @param s5 field value
 */
void SQLTool::update(QString s1, QString s2, QString s3, QString s4, QString s5){
     QSqlQuery query;
     QString sql = "UPDATE "+ s1 +" SET "+ s2 +" = '" + s3 +
             "' WHERE "+ s4 +" = '" + s5 + "' ";

     query.exec(sql);

     if(!query.isActive()){
         qDebug()  << "fail to exec:  ";
     }

     qDebug()  << sql << endl;
}


/**
 * E.g. "update userdata set password='123' where username='koushizon' and gender='男'"
 *
 * @author hzc
 * @param s1 table name
 * @param s2 field you want to update
 * @param s3 field value
 * @param list field name and field value
 */
void SQLTool::update(QString s1, QString s2, QString s3, QStringList list){
     QSqlQuery query;

     QString sql = "UPDATE "+ s1 +" SET "+ s2 +" = '" + s3 +
             "' WHERE ";
     int n = list.size();
     while(n){
         sql += list.at(n-2) +"='"+ list.at(n-1);
         if(n != 2){
             sql +="' and ";
         }else{
             sql += "'";
         }
         n -= 2;
     }

     query.exec(sql);

     if(!query.isActive()){
         qDebug()  << "fail to exec:  ";
     }

     qDebug()  << sql << endl;
}

/**
 * E.g. "insert into userdata valuses ('koushizon','123','杭咨存','男','管理员','koushizon@outlook.com')"
 *
 * @author hzc
 * @param s table name
 * @param list values
 */
void SQLTool::insert(QString s, QStringList list, int nullcount){
    QSqlQuery query;
    int n = list.size();
    QString sql;
    sql += "INSERT INTO "+ s +" VALUES ('";
    for(int i = 0; i < n; i++){
        sql += list.at(i);
        if(i != n-1){
            sql += "', '";
        }else{
            if(nullcount == 0){
                sql += "')";
            }else{
                sql += "', ";
            }
        }
    }
    for(int i = 0; i < nullcount-1; i++){
        sql += "null,";
    }
    if(nullcount > 0){
        sql += "null)";
    }
    query.exec(sql);

    if(!query.isActive()){
        qDebug()  << "fail to exec:  ";
    }
    qDebug()  << sql << endl;
}

/**
* E.g "ALTER TABLE clothes AUTO_INCREMENT = 6 ('clothes', '6')"
* action:   reset self-increase attribute as resetNum
*
* @author Yihan Dong
* @param s table name
* @param resetNum reset number
*/
void SQLTool::reset(QString s, int resetNum)
{
    QSqlQuery query;
    QString sql = "ALTER TABLE" + s + "AUTO_INCREMENT = " + QString::number(resetNum);
    query.exec(sql);

    if(!query.isActive()){
        qDebug() << "fail to exec:  ";
    }
    qDebug() << sql << endl;
}
