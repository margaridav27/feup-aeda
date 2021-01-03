#include "Utils.h"
#include <sstream>

using namespace std;

Date::Date() {
    day = 0;
    month = 0;
    year = 0;
}
Date::Date(unsigned int d, unsigned int m, unsigned int y) {
    day = d;
    month = m;
    year = y;
}
Date::Date(const Date& toCopy) {
    day = toCopy.day;
    month = toCopy.month;
    year = toCopy.year;
}
Date::Date(string dat) {
    char trash;
    unsigned int d, m, y;
    istringstream dateStrStream(dat);
    bool validFormat = false;

    if (dateStrStream >> d >> trash >> m >> trash >> y) {
        validFormat = true;
        if (m > 12 || m < 1) {
            throw InvalidDate(d, m, y);
        } else if ((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) && (d < 1 || d > 31)) {
            throw InvalidDate(d, m, y);
        } else if ((m == 4 || m == 6 || m == 9 || m == 11) && (d < 1 || d > 31)) {
            throw InvalidDate(d, m, y);
        } else if (((y % 4 == 0) && (y % 100 != 0) || ((y % 4 == 0) && (y % 100 == 0) && (y % 400 == 0)))) {
            //leap year
            if ((m == 2) && (d < 1 || d > 29)) {
                throw InvalidDate(d, m, y);
            }
        } else if ((m == 2) && (d < 1 || d > 28)) {
            throw InvalidDate(d, m, y);
        }
        this->year = y;
        this->month = m;
        this->day = d;
    }
    else {
        validFormat = false;
        cout << "Not a valid Date format.\n";
    }
}
Date &Date::operator=(const Date &toAssign) {
    day = toAssign.day;
    month = toAssign.month;
    year = toAssign.year;
    return *this;
}
bool Date::operator<(const Date& d) {
    if (year < d.year) return true;
    else if (year == d.year && month < d.month) return true;
    else if (year == d.year && month == d.month && day < d.day) return true;
    else return false;
}
bool Date::operator==(const Date& d) {
    return (year == d.year && month == d.month && year == d.year);
}

ostream& operator<<(ostream& out, Date d) {
    out << d.day << "/" << d.month << "/" << d.year;
    return out;
}

Time::Time() {
    hours = 0;
    minutes = 0;
    seconds = 0;
}
Time::Time(unsigned int h, unsigned int m, unsigned int s) {
    hours = h;
    minutes = m;
    seconds = s;
}
Time::Time(string t) {
    char trash;
    unsigned int h, m, s;
    istringstream timeStrStream(t);
    bool validFormat = false;
    if (timeStrStream >> h >> trash >> m >> trash >> s) {
        validFormat = true;
        if (h < 0 || h > 24 || m < 0 || m > 59 || s < 0 || s > 59) {
            throw InvalidTime(h,m,s);
        }

        this->hours = h;
        this->minutes = m;
        this->seconds = s;
    }
    else {
        validFormat = false;
        cout << "Not a valid Time format.\n";
    }
}
Time::Time(const Time &toCopy) {
    hours = toCopy.hours;
    minutes = toCopy.minutes;
    seconds = toCopy.seconds;
}
Time& Time::operator=(const Time &toAssign) {
    hours = toAssign.hours;
    minutes = toAssign.minutes;
    seconds = toAssign.seconds;
    return *this;
}
Time& Time::operator+(Time& t2){
    seconds += t2.seconds;
    minutes = minutes + (seconds/60);
    hours = hours + (minutes / 60);
    minutes = minutes %60;
    seconds = seconds % 60;
    return *this;
}

bool Time::operator<(const Time& t) {
    if (hours < t.hours) return true;
    else if (hours == t.hours && minutes < t.minutes) return true;
    else if (hours == t.hours && minutes == t.minutes && seconds < t.seconds) return true;
    else return false;
}
bool Time::operator==(const Time& t) {
    return (hours == t.hours && minutes == t.minutes && seconds == t.seconds);
}

ostream& operator<<(ostream& out, Time t) {
    out << t.hours << ":" << t.minutes << ":" << t.seconds;
    return out;
}

Time &operator*(Time &t, int x) {
    Time aux = t;
    for(int i = 0; i<x-1; i++)
        t = t + aux;
    return t;
}
