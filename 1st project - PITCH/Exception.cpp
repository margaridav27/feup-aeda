#include "Exception.h"
#include <sstream>

ClientDoesNotExist::ClientDoesNotExist(unsigned int nif) {
    this->nif = nif;
}
string ClientDoesNotExist::what() {
    stringstream reason;
    reason << "Client with NIF " << nif << " does not exist.\n";
    return reason.str();
}


EmployeeDoesNotExist::EmployeeDoesNotExist(unsigned int nif) {
    this->nif = nif;
}
string EmployeeDoesNotExist::what() {
    stringstream reason;
    reason << "Employee with NIF " << nif << " does not exist.";
    return reason.str();
}


LiftDoesNotExist::LiftDoesNotExist() {
    code = 0;
    addr = "not defined";
}
LiftDoesNotExist::LiftDoesNotExist(unsigned int code) {
    this->code = code;
    addr = "not defined";
}
LiftDoesNotExist::LiftDoesNotExist(string addr) {
    this->addr = addr;
}
string LiftDoesNotExist::what() {
    stringstream reason;
    if (code != 0 & addr == "not defined") {
        reason << "Lift with code " << code << " does not exist.";
        return reason.str();
    }
    else if (code == 0 & addr != "not defined") {
        reason << "Lift at " << addr << " does not exist.";
        return reason.str();
    }
    reason << "Lift does not exist.";
    return reason.str();
}


NotAnEntrepreneur::NotAnEntrepreneur(unsigned int nif) {
    this->nif = nif;
}
string NotAnEntrepreneur::what() {
    stringstream reason;
    reason << "This client with NIF " << nif << " is not an entrepreneur. Only entrepreneurs have an associated client.";
    return reason.str();
}

InvalidDate::InvalidDate(unsigned int day, unsigned int month, unsigned int year) {
    d = day;
    m = month;
    y = year;
}
string InvalidDate::what() {
    stringstream reason;
    reason << "Date: " << d << "/" << m << "/" << y << " is not valid.";
    return reason.str();
}


InvalidTime::InvalidTime(unsigned int hours, unsigned int minutes, unsigned int seconds) {
    h = hours;
    m = minutes;
    s = seconds;
}
string InvalidTime::what() {
    stringstream reason;
    reason << "Time: " << h << ":" << m << ":" << s << " is not valid.";
    return reason.str();
}
InvalidNumFloors::InvalidNumFloors(unsigned int n) {
    nFloors = n;
}
string InvalidNumFloors::what() {
    stringstream reason;
    reason << "There can't be a lift with " << nFloors << " floors.";
    return reason.str();
}
