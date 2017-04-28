#include "date.h"

//example from Internet

int monthAdd[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};

Date::Date(QObject *parent)
    :QObject(parent)
{
}

// firstDayOfWeek
int Date::getFirstDayOfWeek(int year, int month)
{
    int week = 0;
    week = (year + (year-1) / 4 -(year-1)/100 + (year-1) / 400) % 7;
    week += getTotalMonthDays(year, month);
    return week % 7;
}

// totalMonthDays
int Date::getTotalMonthDays(int year, int month)
{
    int nDay = 0;
    int nLeapDay = isLeapYear(year) ? 1 : 0;
    switch (month) {
    case 1: nDay =	0;	break;
    case 2: nDay = 31;	break;
    case 3: nDay = 59 + nLeapDay;	break;
    case 4: nDay = 90 + nLeapDay;	break;
    case 5: nDay = 120 + nLeapDay;	break;
    case 6: nDay = 151 + nLeapDay;	break;
    case 7: nDay = 181 + nLeapDay;	break;
    case 8: nDay = 212 + nLeapDay;	break;
    case 9: nDay = 243 + nLeapDay;	break;
    case 10: nDay = 273 + nLeapDay;	break;
    case 11: nDay = 304 + nLeapDay;	break;
    case 12: nDay = 334 + nLeapDay;	break;
    default: nDay = 0; break;
    }
    return nDay;
}

// MonthDays
int Date::getMonthDays(int year, int month)
{
    int nDay = 0;
    int nLeapDay = isLeapYear(year);
    switch (month) {
    case 1: nDay = 31; break;
    case 2: nDay = 28 + nLeapDay; break;
    case 3: nDay = 31; break;
    case 4: nDay = 30; break;
    case 5: nDay = 31; break;
    case 6: nDay = 30; break;
    case 7: nDay = 31; break;
    case 8: nDay = 31; break;
    case 9: nDay = 30; break;
    case 10: nDay = 31; break;
    case 11: nDay = 30; break;
    case 12: nDay = 31; break;
    }

    return nDay;
}

//leapYear
bool Date::isLeapYear(int year)
{
    return (((0 == (year %4) && (0 != (year % 100))) || (0 == (year % 400))));
}

