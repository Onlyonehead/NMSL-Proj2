#ifndef SQLTOOL_H
#define SQLTOOL_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class SQLTool
{

public:

    //connect database
    static void connection();

    //connection state
    static bool connectionState();

    //disconnect database
    static void disconnection();

    //E.g : "select * from blacklist"
    static void search(QSqlQuery&, QString);

    //E.g : "select username from blacklist"
    static void search(QSqlQuery&, QString , QString);

    //E.g : "select username,password from blacklist"
    static void search(QSqlQuery&, QStringList, QString);

    //E.g : "select username from blacklist where username='koushizon'"
    static void search(QSqlQuery &, QString, QString, QString, QString);

    //E.g : "select username,password from blacklist where username='koushizon'"
    static void search(QSqlQuery&, QStringList, QString, QString, QString);

    //E.g : "select * from blacklist where username='koushizon'"
    static void search(QSqlQuery&, QString, QString, QString);

    //E.g. "select till_time from blacklist where username='koushizon' and password='123'"
    static void search(QSqlQuery &, QString , QString , QStringList);

    //E.g. "select * from blacklist where username='koushizon' and password='123'"
    static void search(QSqlQuery &, QString, QStringList);

    //E.g. "select * from staff where username like '%dong%'"
    static void fuzzySearch(QSqlQuery &, QString, QString, QString);

    //E.g : "delete from blacklist where username='koushizon'"
    static void del(QSqlQuery&, QString, QString, QString);

    //E.g : "update userdata set password='123' where username='koushizon'"
    static void update(QSqlQuery&, QString, QString, QString, QString, QString);

    //E.g : "insert into userdata valuses ('koushizon','123','杭咨存','男','管理员','koushizon@outlook.com')"
    static void insert(QSqlQuery&, QString, QStringList);

    //E.g : "delete from blacklist where username='koushizon'"
    static void del(QString, QString, QString);


    //E.g. "delete from blacklist where username='koushizon' and password='123'"
    static void del(QString, QStringList);

    //E.g : "update userdata set password='123' where username='koushizon'"
    static void update(QString, QString, QString, QString, QString);

    //E.g. "update userdata set password='123' where username='koushizon' and gender='男'"
    static void update(QString, QString, QString, QStringList);

    //E.g : "insert into userdata valuses ('koushizon','123','杭咨存','男','管理员','koushizon@outlook.com')"
    static void insert(QString, QStringList);

    //E.g : "reset self-increase attributes in TABLE_NAME as n"
    static void reset(QString, int);


private:

};


#endif // SQLTOOL_H
