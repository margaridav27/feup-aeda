#include "Employee.h"
#include <fstream>

#define MAINTDURATION 1.5

using namespace std;

/*EMPLOYEE*/
Employee::Employee(string name, unsigned int nif, unsigned int age) {
    this->name = name;
    this->nif = nif;
    this->age = age;
    function = "general employee";
    evaluation = 0;
    numReviews = 0;
}

string Employee::show() {
    stringstream info;
    info << "NIF: " << nif << '\n'
         << "NAME: " << name << '\n'
         << "FUNCTION: " << function << '\n'
         << "EVALUATION: " << evaluation << "   NUMBER OF REVIEWS: " << numReviews;
    return info.str();
}
void Employee::printToFile(ofstream &op) {
    op << name << '\n'
       << nif << '\n'
       << age << '\n'
       << evaluation << " " << numReviews << '\n';
}

unsigned int Employee::getNif() const {
    return nif;
}
unsigned int Employee::getEvaluation() const {
    return evaluation;
}
unsigned int Employee::getNumOfReviews() const {
    return numReviews;
}

void Employee::setOldReviews(unsigned int eva, unsigned int nR) {
    if (eva < 0)
        eva = 0;
    if (eva > 5)
        eva = 5;
    this->evaluation = eva;
    this->numReviews = nR;
}

void Employee::review(unsigned int punctuation) {
    evaluation = (evaluation * numReviews) + punctuation;
    numReviews++;
    evaluation = evaluation / numReviews;
}

bool Employee::operator==(Employee *comp) {
    return (name == comp->name && nif == comp->nif && age == comp->age && function == comp->function);
}


/*SERVICE PROVIDER*/
ServiceProvider::ServiceProvider(string name, unsigned int nif, unsigned int age, double perHourSalary) : Employee(name, nif, age) {
    function = "maintenance";
    hoursWorked = 0;
    this->perHourSalary = perHourSalary;
}

string ServiceProvider::show() {
    stringstream info;
    info << "NIF: " << nif << '\n'
         << "NAME: " << name << '\n'
         << "FUNCTION: " << function << '\n'
         << "EVALUATION: " << evaluation << "   NUMBER OF REVIEWS: " << numReviews << '\n'
         << "PER HOUR: " << perHourSalary << "   HOURS: " << hoursWorked << "   TOTAL TO RECEIVE: " << getSalary();
    return info.str();
}
void ServiceProvider::printToFile(ofstream &op) {
    op << name << '\n'
       << nif << '\n'
       << age << '\n'
       << evaluation << " " << numReviews << '\n'
       << "+\n"
       << setprecision(3) << perHourSalary << " " << setprecision(2) << hoursWorked << '\n'
       << "+\n";
}

double ServiceProvider::getSalary() const {
    return (perHourSalary * hoursWorked);
}

void ServiceProvider::setHoursWorked(double h) {
    hoursWorked = h;
}

void ServiceProvider::executeMaintenance() {
    hoursWorked += MAINTDURATION;
}

bool ServiceProvider::operator==(Employee *comp) {
    ServiceProvider *sp = dynamic_cast<ServiceProvider *>(comp);
    if (comp != NULL)
        return (name == sp->name && nif == sp->nif && age == sp->age && function == sp->function &&
                perHourSalary == sp->perHourSalary);
    else
        return false;
}


/*CONTRACTED EMPLOYEE*/
ContractedEmployee::ContractedEmployee(string name, unsigned int nif, unsigned int age, double perMonthSalary) : Employee(name, nif, age) {
    function = "lift manager";
    this->perMonthSalary = perMonthSalary;
}

string ContractedEmployee::show() {
    stringstream info;
    info << "NIF: " << nif << '\n'
         << "NAME: " << name << '\n'
         << "FUNCTION: " << function << '\n'
         << "EVALUATION: " << evaluation << "   NUMBER OF REVIEWS: " << numReviews << '\n'
         << "SALARY: " << perMonthSalary << '\n'
         << "NUMBER OF CLIENTS ASSOCIATED: " << getNumClientsAssociated() << '\n';
    return info.str();
}
void ContractedEmployee::printToFile(ofstream &op) {
    op << name << '\n'
       << nif << '\n'
       << age << '\n'
       << evaluation << " " << numReviews << '\n'
       << "x\n"
       << setprecision(4) << perMonthSalary << "\n";
    if (clientsAssociated.size() == 0) {
        op << 0;
    } else {
        for (vector<Client *>::iterator it = clientsAssociated.begin(); it != clientsAssociated.end(); it++) {
            op << (*it)->getNif() << " ";
        }
    }
    op << '\n'
       << "x\n";
}

unsigned int ContractedEmployee::getNumClientsAssociated() const {
    return clientsAssociated.size();
}
vector<Client *> ContractedEmployee::getClientsAssociated() {
    return clientsAssociated;
}

void ContractedEmployee::associateToClient(Client *c) {
    clientsAssociated.push_back(c);
}

void ContractedEmployee::removeClient(Client *rm) {
    for (vector<Client *>::iterator it = clientsAssociated.begin(); it != clientsAssociated.end(); it++) {
        if (*it == rm) {
            clientsAssociated.erase(it);
            break;
        }
    }
}

bool ContractedEmployee::operator==(Employee *comp) {
    ContractedEmployee *ce = dynamic_cast<ContractedEmployee *>(comp);
    if (comp != NULL)
        return (name == ce->name && nif == ce->nif && age == ce->age && function == ce->function && perMonthSalary == ce->perMonthSalary);
    else
        return false;
}


ostream &operator<<(ostream &out, Employee *e) {
    out << e->show();
    return out;
}

bool sortEmployeesByNumReviews(Employee *e1, Employee *e2) {
    if (e2->getNumOfReviews() > e1->getNumOfReviews()) return true;
    else return false;
}

bool sortEmployeesByEvaluation(Employee *e1, Employee *e2) {
    if (e2->getEvaluation() > e1->getEvaluation()) return true;
    else return false;
}

bool sortEmployeesByNumClientsAssociated(Employee *e1, Employee *e2) {
    ContractedEmployee *ce1 = dynamic_cast<ContractedEmployee *>(e1);
    ContractedEmployee *ce2 = dynamic_cast<ContractedEmployee *>(e2);
    if (ce1 != NULL & ce2 != NULL) {
        if (ce2->getNumClientsAssociated() > ce1->getNumClientsAssociated()) return true;
        else return false;
    }
    return false;
}

