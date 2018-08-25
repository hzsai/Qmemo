#include "database.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDir>

DataBase::DataBase(QWidget *parent)
    :QWidget(parent)
{
    //qDebug() << QSqlDatabase::drivers();
    connectdb("/daily.db");
    doAfavour();
}

//TODO: 将数据接口同意移到database上来进行统一管理.,具体功能·在
//		把一些简单的slot函数实现。
void DataBase::connectdb(QString database)
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString strDbName = QApplication::applicationDirPath() + database;
    db.setDatabaseName(strDbName);
    if (!db.open()) {
        qFatal("Cannot open database");
        return;
    }

    QSqlQuery query;
    //建立基本表，若存在，则创建失败
    QString sql_create = "create table daily ("
                         "id integer primary key autoincrement,"
                         "flag integer default 0,"
                         "favour integer default 1,"
                         "note text,"
                         "time timestamp not null unique default ("
                         "date('now', 'localtime')));";
    query.prepare(sql_create);
    query.exec();
    // unique time index
    query.exec("create unique index index_time on daily (time);");
}

void DataBase::save()
{
}

QString DataBase::queryNote(const QString &date)
{
    QString list;
    QSqlQuery query;
    query.prepare("select note from daily where time = :time;");
    query.bindValue(":time", date);
    query.exec();
    while (query.next()) {
        list = query.value("note").toString();
    }
    return list;
}

void DataBase::insertInto(QString &note, QString &date)
{
    QSqlQuery query;
    query.prepare("replace into daily (note, time) values(:note, :date);");
    query.bindValue(":note", note);
    query.bindValue(":data", date);
    query.exec();
}

void DataBase::doAfavour()
{
    QDir imageFolder;
    if (!imageFolder.exists("images")) {
        imageFolder.mkdir("images");
    }
}
