#include "Lift.h"
#include <fstream>

#define PRICE1 40000
#define PRICE2 42000
#define PRICE3 50000
#define PRICE4 45000
#define PRICE5 53000
#define PRICE6 60000
#define PRICE7 65000
#define PRICE8 69000
#define PRICE9 71000
#define PRICE10 75000

using namespace std;

void printMove(ofstream &op, const Moves &move) {
    op << move.date << " " << move.from << " " << move.to << '\n'
       << move.tInitial;
}

ostream &operator<<(ostream &out, Moves move) {
    out << "date: " << move.date << '\n'
        << "started: " << move.tInitial << '\n'
        << "from: " << move.from << "\tto: " << move.to << "\tfloor count: " << move.floorCount << "\nended: "
        << move.tFinal;

    return out;
}

void printMaint(ofstream &op, Maintenance &maint) {
    op << maint.date << " " << maint.nifEmployee;
}

ostream &operator<<(ostream &out, Maintenance maint) {
    out << "employee's NIF: " << maint.nifEmployee << "     date: " << maint.date;
    return out;
}

unsigned int Lift::nLifts = 0;


Lift::Lift() {
    buildingName = "not defined";
    this->address = "not defined";
    this->capacity = 0.0;
    code = nLifts + 100;
    install = Date(0, 0, 0);
    regularTime = 0;
    clientNif = 0;
    nFloors = 0;
    price = 0;
    higherFloor = 0;
    lowerFloor = 0;
    nLifts++;
}
Lift::Lift(string buildingName, string address, unsigned int capacity, unsigned int clientNif, unsigned int nFloors, Date &d) {
    this->buildingName = buildingName;
    this->address = address;
    this->capacity = capacity;
    this->clientNif = clientNif;
    this->nFloors = nFloors;
    if (nFloors < 1)
        throw InvalidNumFloors(nFloors);
    code = nLifts + 100;
    this->install = d;
    nLifts++;
}

string Lift::show() {
    stringstream info;
    info << "CODE: " << code << '\n'
         << "ADDRESS: " << address << '\n'
         << "BUILDING NAME: " << buildingName << "\n"
         << "INSTALLATION DATE: " <<install << "\n"
         << "MAINTENANCE HISTORY: " << '\n';
    for (vector<struct Maintenance>::iterator it = maintHistory.begin(); it != maintHistory.end(); it++) {
        info << (*it) << "\n";
    }
    info << "MOVES HISTORY: " << '\n';
    if (movesHistory.empty()) info << '\n';
    for (vector<Moves>::iterator it = movesHistory.begin(); it != movesHistory.end(); it++) {
        info << (*it) << "\n";
    }
    return info.str();
}
void Lift::printToFile(ofstream &op) {
    op << clientNif << '\n'
       << buildingName << '\n'
       << address << '\n'
       << install << '\n'
       << capacity << " " << nFloors << " " << regularTime << '\n'
       << currentFloor << " " << higherFloor << " " << lowerFloor << '\n';
    op << "####################\n";
    for (vector<Moves>::iterator it = movesHistory.begin(); it != movesHistory.end(); it++) {
        printMove(op, (*it));
        op << '\n';
    }
    op << "####################\n";
    for (vector<struct Maintenance>::iterator it = maintHistory.begin(); it != maintHistory.end(); it++) {
        printMaint(op, (*it));
        op << '\n';
    }
    op << "####################\n";
}

unsigned int Lift::getCode() const {
    return code;
}
unsigned int Lift::getClientNif() const {
    return clientNif;
}
string Lift::getName() const {
    return this->buildingName;
}
string Lift::getAddress() const {
    return address;
}
Date Lift::getInstallDate() const {
    return install;
}
unsigned int Lift::getPrice() const {
    return price;
}
int Lift::getCurrentFloor() const {
    return currentFloor;
}
int Lift::getHigherFloor() const {
    return higherFloor;
}
int Lift::getLowerFloor() const {
    return lowerFloor;
}
vector<Moves> Lift::getMoves() {
    return movesHistory;
}
Moves Lift::getLastMove() const {
    return movesHistory[movesHistory.size() - 1];
}
int Lift::getRegularTime() const {
    return regularTime;
}
Maintenance Lift::getLastMaintenance() const {
    return maintHistory[maintHistory.size() - 1];
}
int Lift::getPreviousDoorOpens() const {
    return previousDoorOpens;
}

void Lift::setOwner(unsigned int nif) {
    clientNif = nif;
}
void Lift::setPrice() {
    if (nFloors < 3) {
        if (capacity < 200) { //até 2/3 users
            price = PRICE1;
        } else if (capacity < 500) { //até 6/7 users
            price = PRICE2;
        } else { //mais que 7 users
            price = PRICE3;
        }
    } else if (nFloors < 7) {
        if (capacity < 200) { //até 2/3 users
            price = PRICE4;
        } else if (capacity < 500) { //até 6/7 users
            price = PRICE5;
        } else { //mais que 7 users
            price = PRICE6;
        }
    } else if (nFloors < 50) {
        if (capacity < 500) { //até 6/7 users
            price = PRICE7;
        } else { //mais que 7 users
            price = PRICE8;
        }
    } else {
        if (capacity < 500) { //até 6/7 users
            price = PRICE9;
        } else { //mais q 7 users
            price = PRICE10;
        }
    }
}
void Lift::setRegularTime() {
    regularTime = rand() % 5 + 7;
}
void Lift::setRegularTime(int secs) {
    regularTime = secs;
}
void Lift::setCurrentFloor(int floor) {
    currentFloor = floor;
}
void Lift::setHigherFloor(int floor) {
    higherFloor = floor;
}
void Lift::setLowerFloor(int floor) {
    lowerFloor = floor;
}

void Lift::readMove(int from, int to, Date d, Time t) {
    Moves move;
    Time regTime(0, 0, regularTime);
    move.from = from;
    move.to = to;
    move.floorCount = abs(to - from);
    move.date = d;
    move.tInitial = t;
    move.tFinal = t + regTime * (to - from);
    movesHistory.push_back(move);
    currentFloor = to;
    doorOpens += 2;
}
void Lift::registerMaintenance(int employeeNif, Date date) {
    struct Maintenance m;
    m.nifEmployee = employeeNif;
    m.date = date;
    maintHistory.push_back(m);
}
int Lift::checkMaintenanceNeeded(Date today) {
    if (this->doorOpens > 18000)
        return 0;
    if (maintHistory.size() == 0) {
        if (today.year - install.year > 2)
            return 0;
        else
            return 1;
    }
    if (today.year == maintHistory[maintHistory.size()].date.year) {
        return 1;
    } else if (today.year - maintHistory[maintHistory.size()].date.year > 2) { return 0; }

    else if (today.year - maintHistory[maintHistory.size()].date.year == 1) {
        if (today.month - maintHistory[maintHistory.size()].date.month > 0)
            return 0;
        else
            return 1;
    }
    return 1;

}
void Lift::printMaintenanceHistory() {
    for (int i = 0; i < maintHistory.size(); i++) {
        cout << "Employee's nif: " << maintHistory[i].nifEmployee << " | Date: " << maintHistory[i].date;
    }
}
void Lift::printLastMaintenance() {
    cout << "Employee's nif: " << maintHistory[maintHistory.size() - 1].nifEmployee << " | Date: "
         << maintHistory[maintHistory.size() - 1].date;
}
void Lift::Maintenance(unsigned int nifEmployee, Date today) {
    previousDoorOpens = doorOpens;
    doorOpens = 0;
    struct Maintenance m;
    m.date = today;
    m.nifEmployee = nifEmployee;
    maintHistory.push_back(m);
}
bool Lift::hasHadMaintenance() const {
    if (maintHistory.size() > 0)
        return true;
    return false;
}

bool Lift::operator==(Lift *comp) const {
    return (code == comp->code);
}

ostream &operator<<(ostream &out, Lift *l) {
    out << l->show();
    return out;
}














