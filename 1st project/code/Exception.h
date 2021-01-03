#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <iostream>

using namespace std;

/**
 * @class ClientDoesNotExist
 */
class ClientDoesNotExist {
private:
    unsigned int nif;
public:
    ClientDoesNotExist(unsigned int nif);
    string what();
};

/**
 * @class EmployeeDoesNotExist
 */
class EmployeeDoesNotExist {
private:
    unsigned int nif;
public:
    EmployeeDoesNotExist(unsigned int nif);
    string what();
};

/**
 * @class LiftDoesNotExist
 */
class LiftDoesNotExist {
private:
    unsigned int code;
    string addr;
public:
    LiftDoesNotExist();
    LiftDoesNotExist(unsigned int code);
    LiftDoesNotExist(string addr);
    string what();
};

/**
 * @class NotAnEntrepreneur
 */
class NotAnEntrepreneur {
private:
    unsigned int nif;
public:
    NotAnEntrepreneur(unsigned int nif);
    string what();
};

/**
 * @class InvalidDate
 */
class InvalidDate {
private:
    unsigned int d, m, y;
public:
    InvalidDate(unsigned int day, unsigned int month, unsigned int year);
    string what();
};

/**
 * @class InvalidTime
 */
class InvalidTime {
private:
    unsigned int h, m, s;
public:
    InvalidTime(unsigned int hours, unsigned int minutes, unsigned int seconds);
    string what();
};

/**
 * @class InvalidNumFloors
 */
class InvalidNumFloors {
private:
    unsigned int nFloors;
public:
    InvalidNumFloors(unsigned int n);
    string what();
};

#endif
