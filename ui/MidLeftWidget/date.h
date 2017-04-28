#ifndef DATE_H
#define DATE_H

#include <QObject>

class Date : public QObject
{
    Q_OBJECT
public:
    explicit Date(QObject *parent = 0);

signals:

public slots:

public:

    static int getFirstDayOfWeek(int year, int month);
    static int getTotalMonthDays(int year, int month);
    static int getMonthDays(int year, int month);
    static bool isLeapYear(int year);
private:

};

#endif // DATE_H
