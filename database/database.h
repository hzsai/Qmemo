#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QWidget>
#include <QSqlDatabase>

class DataBase : public QWidget
{
    Q_OBJECT
public:
    explicit DataBase(QWidget *parent = 0);

    QSqlDatabase db;

    //static void initdb();
    static void insertInto(QString &, QString &);
    static void connectdb(QString database);
    static void save();
    static bool updateDb(const QString &conten, const QString &dataid);
    static QString queryNote(const QString &date);

private:


signals:

public slots:

};

#endif // DATABASE_H
