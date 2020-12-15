#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>
#include <iomanip>
#include "Exception.h"

using namespace std;

/**
 * @class Date
 */
class Date {
public:
    unsigned int day, month, year;
    Date();
    Date(unsigned int d, unsigned int m, unsigned int y);
    /**
     * Constructor
     * @param d string with format day/month/year
     */
    Date(string d);
    Date(const Date& toCopy);
    Date& operator=(const Date& toAssign);
    bool operator< (const Date& d);
    bool operator== (const Date& d);
};
/** output operator for objects of the class Date */
ostream& operator<<(ostream& out, Date d);

/**
 * @class Time
 */
class Time {
public:
    unsigned int hours, minutes, seconds;
    Time();
    Time(unsigned int h, unsigned int m, unsigned int s);
    /**
     * Constructor
     * @param t string with format hours:minutes:seconds
     */
    Time(string t);
    Time(const Time& toCopy);
    Time& operator=(const Time& toAssign);
    Time& operator+(Time& t2);
    bool operator<(const Time& t);
    bool operator==(const Time& t);
};
Time& operator*(Time& t, int x);
/** output operator for objects of the class Time */
ostream& operator<<(ostream& out, Time t);

template <class T>
int SequentialSearch(const vector<T> &v, T x) {
    for (unsigned int i = 0; i < v.size(); i++)
        if (v[i] == x)
            return i;
    return -1;
}
template <class T>
int binarySearch(const vector<T> &v, T x)
{
    int left = 0, right = v.size() - 1;
    while (left <= right)
    {
        int middle = (left + right) / 2;
        if (x == v[middle]) return middle; // encontrou
        else if (x < v[middle]) right = middle - 1;
        else left = middle + 1;
    }
    return -1; // não encontrou
}


#endif
