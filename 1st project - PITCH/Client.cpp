#include "Client.h"
#include <fstream>
using namespace std;

/*CLIENT*/
Client::Client(string name, unsigned int nif, string date) {
    this->name = name;
    this->nif = nif;
    Date d(date);
    firstLift = d;
}
Client::Client(string name, unsigned int nif, Date date) {
    this->name = name;
    this->nif = nif;
    firstLift = date;
}

string Client::show() {
    stringstream info;
    info << "NIF: " << nif << " | NAME: " << name << " | NUMBER OF LIFTS: " << lifts.size();
    return info.str();
}
void Client::printToFile(ofstream &op) {
    op << name << '\n'
       << nif << '\n'
       << firstLift << '\n';
    op << "####################\n";
    for (vector<Lift*>::iterator it = lifts.begin(); it != lifts.end(); it++) {
        op << (*it)->getCode() << "\n";
    }
    op << "####################\n";
}

string Client::getName() const {
    return name;
}
unsigned int Client::getNif() const {
    return nif;
}
Date Client::getDateFirstLift() {
    return firstLift;
}
vector<Lift*> Client::getLifts() {
    return lifts;
}

void Client::purchaseNewLift(Lift *lift) {
    lifts.push_back(lift);
}
bool Client::hasLifts() const {
    return !lifts.empty();
}

bool Client::operator==(Client* comp) {
    return (name == comp->name && nif == comp->nif && firstLift == comp->firstLift);
}
bool Client::operator<(Client *comp) {
    return (firstLift < comp->firstLift);
}


/*PARTICULAR CLIENT*/
ParticularClient::ParticularClient(string name, unsigned int nif, string date): Client(name,nif,date){}
ParticularClient::ParticularClient(string name, unsigned int nif, Date date) : Client(name, nif, date) {}

string ParticularClient::show() {
    stringstream info;
    info << "NIF: " << nif << " | NAME: " << name << " | NUMBER OF LIFTS: " << lifts.size();
    return info.str();
}
void ParticularClient::printToFile(ofstream &op) {
    op << name << '\n'
       << nif << '\n'
       << firstLift << '\n';
    op << "####################\n";
    for (vector<Lift*>::iterator it = lifts.begin(); it != lifts.end(); it++) {
        op << (*it)->getCode() << "\n";
    }
    op << "####################\n";
}


/*ENTREPRENEUR*/
Entrepreneur::Entrepreneur(string name, unsigned int nif, Date date) : Client(name, nif, date) {
    employeeNif = 0;
}
Entrepreneur::Entrepreneur(string name, unsigned int nif, string date, unsigned int employeeNif) : Client(name,nif,date) {
    this->employeeNif = employeeNif;
}

string Entrepreneur::show() {
    stringstream info;
    info << "NIF: " << nif << " | NAME: " << name << " | NUMBER OF LIFTS: " << lifts.size() << " | EMPLOYEE'S NIF: " << employeeNif;
    return info.str();
}
void Entrepreneur::printToFile(ofstream &op) {
    op << name << '\n'
       << nif << '\n'
       <<firstLift << '\n';
    op << employeeNif << '\n';
    op << "####################\n";
    for (vector<Lift*>::iterator it = lifts.begin(); it != lifts.end(); it++) {
        op << (*it)->getCode() << "\n";
    }
    op << "####################\n";
}

unsigned int Entrepreneur::getEmployeeNif() const{
    return employeeNif;
}
void Entrepreneur::associateEmployee(unsigned int employeeNif) {
    this->employeeNif = employeeNif;
}


ostream& operator<<(ostream& out, Client* c){
    out << c->show();
    return out;
}

bool sortClientsByNumLiftsPurchasedOrByAge(Client* c1, Client* c2) {
    if (c2->getLifts().size() > c1->getLifts().size()) return true;
    else if (c2->getLifts().size() == c1->getLifts().size() & c2->getDateFirstLift() < c1->getDateFirstLift()) return true;
    return false;
}
