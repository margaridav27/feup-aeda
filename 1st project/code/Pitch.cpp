#include "Pitch.h"
#include <iostream>
#include <algorithm>

#define maxEntrepeneurNif 600000000
#define minEntrepeneurNif 500000000
#define maxParticularNif  300000000
#define minParticularNif  200000000

using namespace std;

Pitch::Pitch(string clientsFile, string employeesFile, string liftsFile) {
    if (clientsFile.substr(clientsFile.length() - 4, 4) != ".txt") {
        clientsFile += ".txt";
    }
    this->clientsFile = clientsFile;

    if (employeesFile.substr(employeesFile.length() - 4, 4) != ".txt") {
        employeesFile += ".txt";
    }
    this->employeesFile = employeesFile;

    if (liftsFile.substr(liftsFile.length() - 4, 4) != ".txt") {
        liftsFile += ".txt";
    }
    this->liftsFile = liftsFile;


    loadLifts();
    loadClients();
    loadEmployees();
}

void Pitch::loadLifts() {
    ifstream inStream;
    inStream.open(liftsFile);
    if (inStream.fail()) {
        cerr << "Input file opening failed.\n";
        exit(1);
    }

    unsigned int nif, floors, secs;
    int cFloor, hFloor, lFloor;
    unsigned int capac;
    string building, addr, install, info, trash, empty;

    while (!inStream.eof()) {
        inStream >> nif;
        inStream.ignore(1000000, '\n');
        getline(inStream, building);
        getline(inStream, addr);
        getline(inStream, install);
        Date d(install);

        inStream >> capac >> floors >> secs;
        inStream.ignore(1000000, '\n');

        Lift *newLift = new Lift(building, addr, capac, nif, floors, d);

        inStream >> cFloor >> hFloor >> lFloor;
        inStream.ignore(1000000, '\n');

        newLift->getPrice(); //by reading the capacity and the number of floors we can calculate the price
        newLift->setCurrentFloor(cFloor);
        newLift->setHigherFloor(hFloor);
        newLift->setLowerFloor(lFloor);
        newLift->setRegularTime(secs);

        getline(inStream, trash); //'####################'

        while (getline(inStream, info)) { //movesHistory
            if (info == trash) break; //lift has nothing on movesHistory vector yet or there is no more moves to add

            string date, tInit;
            int from, to;

            stringstream moves(info);
            moves >> date >> from >> to;
            getline(inStream, tInit);
            Time tI(tInit);
            newLift->readMove(from, to, date, tI);
        }
        while (getline(inStream, info)) { //maintHistory
            if (info == trash)
                break; //lift has nothing on maintHistory vector yet or there is no more maintenances to add

            string date;
            unsigned int nif;

            stringstream maint(info);
            maint >> date >> nif;
            Date d(date);
            newLift->registerMaintenance(nif, date);
        }
        if (nif == 555555555) {
            ownedLifts.push_back(newLift);
        }
        lifts.push_back(newLift);

        getline(inStream, empty);
    }
    inStream.close();
}
void Pitch::loadClients() {
    ifstream inStream;
    inStream.open(clientsFile);
    if (inStream.fail()) {
        cerr << "Input file opening failed.\n";
        exit(1);
    }

    string name, first, info, empty;
    unsigned int cNif, eNif;
    Client *newClient;

    while (getline(inStream, name)) {
        inStream >> cNif;
        inStream.ignore(1000000, '\n');
        getline(inStream, first);
        inStream >> info;
        inStream.ignore(1000000, '\n');

        if (info != "####################") { //entrepreneur
            eNif = (unsigned int) stoi(info);
            newClient = new Entrepreneur(name, cNif, first, eNif);
            getline(inStream, info);
        } else { //particular client
            newClient = new ParticularClient(name, cNif, first);
        }

        while (getline(inStream, info)) {
            if (info == "####################")
                break; //client has nothing on lifts vector yet there is no more lifts to add

            newClient->purchaseNewLift(searchLift((unsigned int) stoi(info)));
        }
        clients.push_back(newClient);
        getline(inStream, empty);
    }

    inStream.close();
}
void Pitch::loadEmployees() {
    ifstream inStream;
    inStream.open(employeesFile);
    if (inStream.fail()) {
        cerr << "Input file opening failed.\n";
        exit(1);
    }

    string name, empty;
    unsigned int nif, age, nRev, eva;
    char identifier;
    double salary;

    while (getline(inStream, name)) {
        inStream >> nif;
        inStream.ignore(1000000, '\n');
        inStream >> age;
        inStream.ignore(1000000, '\n');
        inStream >> eva >> nRev;
        inStream.ignore(1000000, '\n');
        inStream >> identifier;
        inStream.ignore(1000000, '\n');

        if (identifier == '+') { //service provider
            double hours;

            inStream >> salary >> hours;
            inStream.ignore(1000000, '\n');

            Employee *newEmployee = new ServiceProvider(name, nif, age, salary);
            ServiceProvider *sp = dynamic_cast<ServiceProvider *>(newEmployee);
            sp->setOldReviews(eva, nRev);
            sp->setHoursWorked(hours);
            employees.push_back(sp);

            inStream >> identifier;
            inStream.ignore(1000000, '\n');

        } else if (identifier == 'x') { //contracted employee
            unsigned int cNif;

            inStream >> salary;
            inStream.ignore(1000000, '\n');

            Employee *newEmployee = new ContractedEmployee(name, nif, age, salary);
            ContractedEmployee *ce = dynamic_cast<ContractedEmployee *>(newEmployee);
            ce->setOldReviews(eva, nRev);

            string cnifs;
            getline(inStream, cnifs);
            istringstream instr(cnifs);
            instr >> cNif;
            if (cNif != 0) { //has clients associated
                ce->associateToClient(searchClient(cNif));
                while (instr >> cNif) {
                    ce->associateToClient(searchClient(cNif));
                }
            }
            employees.push_back(ce);

            inStream >> identifier;
            inStream.ignore(1000000, '\n');
        }
        getline(inStream, empty);
    }
    inStream.close();
}

void Pitch::saveLifts() {
    ofstream opStream;
    opStream.open(liftsFile);
    if (opStream.fail()) {
        cerr << "Input file opening failed.\n";
        exit(1);
    }
    for (vector<Lift *>::iterator it = lifts.begin(); it != lifts.end(); it++) {
        (*it)->printToFile(opStream);
        if (it != lifts.end() - 1)
            opStream << endl;
    }
    opStream.close();
}
void Pitch::saveClients() {
    ofstream opStream;
    opStream.open(clientsFile);
    if (opStream.fail()) {
        cerr << "Input file opening failed.\n";
        exit(1);
    }

    for (vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
        (*it)->printToFile(opStream);
        //why this if?
        if (it != clients.end() - 1)
            opStream << endl;
    }
    opStream.close();
}
void Pitch::saveEmployees() {
    ofstream opStream;
    opStream.open(employeesFile);
    if (opStream.fail()) {
        cerr << "Input file opening failed.\n";
        exit(1);
    }
    for (vector<Employee *>::iterator it = employees.begin(); it != employees.end(); it++) {
        (*it)->printToFile(opStream);
        if (it != employees.end() - 1)
            opStream << endl;
    }
    opStream.close();
}
void Pitch::saveData() {
    saveClients();
    saveEmployees();
    saveLifts();
}

Client *Pitch::searchClient(unsigned int searchedNIF) {
    for (unsigned int i = 0; i < clients.size(); i++) {
        if (clients[i]->getNif() == searchedNIF) {
            return clients[i];
        }
    }
    throw ClientDoesNotExist(searchedNIF);
}
Employee *Pitch::searchClientsEmployee(unsigned int clientNIF) {
    try {
        Entrepreneur *cl = dynamic_cast<Entrepreneur *>(searchClient(clientNIF));
        if (cl != NULL) {
            try {
                return searchEmployee(cl->getEmployeeNif());
            }
            catch (EmployeeDoesNotExist &e) {
                e.what();
            }
        } else {
            throw NotAnEntrepreneur(clientNIF);
        }
    }
    catch (NotAnEntrepreneur &e) {
        cout << e.what() << endl;
    }
    catch (ClientDoesNotExist &exc) {
        cout << exc.what() << endl;
    }

}
Employee *Pitch::searchEmployee(unsigned int searchedNIF) {
    for (unsigned int i = 0; i < employees.size(); i++) {
        if (employees[i]->getNif() == searchedNIF) {
            return employees[i];
        }
    }
    throw EmployeeDoesNotExist(searchedNIF);
}
Lift *Pitch::searchLift(unsigned int searchedCode) {
    for (unsigned int i = 0; i < lifts.size(); i++) {
        if (lifts[i]->getCode() == searchedCode) {
            return lifts[i];
        }
    }
    throw LiftDoesNotExist(searchedCode);
}
Lift *Pitch::searchLift(string searchedAddr) {
    for (unsigned int i = 0; i < lifts.size(); i++) {
        if (lifts[i]->getAddress() == searchedAddr) {
            return lifts[i];
        }
    }
    throw LiftDoesNotExist(searchedAddr);
}

void Pitch::addClient(Client *toAdd) {
    clients.push_back(toAdd);
}
void Pitch::addEmployee(string name, unsigned int nif, unsigned int age, double salary) {
    if (salary >= 365) {
        Employee *newContractedEmployee = new ContractedEmployee(name, nif, age, salary);
        employees.push_back(newContractedEmployee);
    } else {
        Employee *newServiceProvider = new ServiceProvider(name, nif, age, salary);
        employees.push_back(newServiceProvider);
    }
}
void Pitch::addLift(Lift *toAdd) {
    lifts.push_back(toAdd);
}

unsigned int Pitch::getNif() const {
    return nif;
}
unsigned int Pitch::getNumEmployees() const {
    return employees.size();
}
unsigned int Pitch::getNumLift() const {
    return lifts.size();
}
vector<Lift *> Pitch::getLifts() {
    return lifts;
}

ServiceProvider *Pitch::getServiceProvider() {
    vector<ServiceProvider *> onlySP;
    for (vector<Employee *>::iterator it = employees.begin(); it != employees.end(); it++) {
        ServiceProvider *newSP = dynamic_cast<ServiceProvider *>((*it));
        if (newSP != NULL) {
            onlySP.push_back(newSP);
        }
    }
    unsigned int randIx = rand() % onlySP.size();
    return onlySP[randIx];
}
Employee *Pitch::getMostReviewedEmployee() {
    vector<Employee *> copy = employees;
    sort(copy.begin(), copy.end(), sortEmployeesByNumReviews);
    return copy[copy.size() - 1];
}
Employee *Pitch::getLeastReviewedEmployee() {
    vector<Employee *> copy = employees;
    sort(copy.begin(), copy.end(), sortEmployeesByNumReviews);
    return copy[0];
}
Employee *Pitch::getBestEvaluatedEmployee() {
    vector<Employee *> copy = employees;
    sort(copy.begin(), copy.end(), sortEmployeesByEvaluation);
    return copy[copy.size() - 1];
}
Employee *Pitch::getWorstEvaluatedEmployee() {
    vector<Employee *> copy = employees;
    sort(copy.begin(), copy.end(), sortEmployeesByEvaluation);
    return copy[0];
}
Employee *Pitch::getEmployeeWithLessClientsAssociated() {
    vector<Employee *> onlyContracted;
    for (vector<Employee *>::iterator it = employees.begin(); it != employees.end(); it++) {
        ContractedEmployee *newCE = dynamic_cast<ContractedEmployee *>((*it));
        if (newCE != NULL) {
            onlyContracted.push_back(newCE);
        }
    }
    sort(onlyContracted.begin(), onlyContracted.end(), sortEmployeesByNumClientsAssociated);
    return onlyContracted[0];
}
ContractedEmployee *Pitch::getEmployeeAssociatedToEntrepreneur(Entrepreneur *e) {
    return dynamic_cast<ContractedEmployee *>(searchClientsEmployee(e->getNif()));
}
Client *Pitch::getOldestClient() {
    return clients[0];
}
Client *Pitch::getMostRecentClient() {
    return clients[clients.size() - 1];
}
Client *Pitch::getClientWithMostLiftsPurchased() {
    vector<Client *> copy = clients;
    sort(copy.begin(), copy.end(), sortClientsByNumLiftsPurchasedOrByAge);
    return copy[copy.size() - 1];
}
Client *Pitch::getClientAssociatedToLift(Lift *l) {
    unsigned int searchedNIF = l->getClientNif();
    try {
        return searchClient(searchedNIF);
    }
    catch (ClientDoesNotExist &exc) {
        cout << exc.what();
    }
}

void Pitch::removeEmployee(unsigned int nif) {
    try {
        ContractedEmployee *toRm = dynamic_cast<ContractedEmployee *>(searchEmployee(nif));
        if (toRm != NULL) { //the employee we are trying to remove is a contracted employee
            vector<Client *> copy = toRm->getClientsAssociated();
            for (unsigned int i = 0; i < copy.size(); i++) {
                ContractedEmployee *ce = dynamic_cast<ContractedEmployee *>(getEmployeeWithLessClientsAssociated());
                ce->associateToClient(
                        copy[i]); //therefore each one of his clients need to be associated to other contracted employee
                Entrepreneur *ent = dynamic_cast<Entrepreneur *>(copy[i]); //the client associated to the employee to be removed was indeed an entrepreneur
                if (ent != NULL) {
                    ent->associateEmployee(
                            ce->getNif()); //therefore we also need to set the new associated to the entrepreneur employee's nif
                }
            }
        }
        //there is nothing to limit a service provider removal

        if (binarySearch(employees, searchEmployee(nif)) != -1) {
            employees.erase(employees.begin() + SequentialSearch(employees, searchEmployee(nif)));
        } //there is no need to worry about an else condition because if employee does not exist, that exception will be catched
    }
    catch (EmployeeDoesNotExist &exc) {
        cout << exc.what();
    }
}
void Pitch::removeClient(unsigned int nif) {
    try {
        searchClient(nif);
    }
    catch (ClientDoesNotExist &exc) {
        exc.what();
    }
    vector<Lift *> thisClientLifts = searchClient(nif)->getLifts();
    for (vector<Lift *>::iterator it = thisClientLifts.begin(); it != thisClientLifts.end(); it++) {
        (*it)->setOwner(this->nif);
        ownedLifts.push_back((*it));

    }

    Client *ent = (searchClient(nif));
    if (ent->getNif() > minEntrepeneurNif) {
        Entrepreneur *e = dynamic_cast<Entrepreneur *>(ent);
        ContractedEmployee *ce = dynamic_cast<ContractedEmployee *>(searchEmployee(e->getEmployeeNif()));
        ce->removeClient(searchClient(nif));
    }

    for (vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
        if ((*it)->getNif() == nif) {
            clients.erase(it);
            break;
        }
    }
}

void Pitch::associateEmployeeToClient(Client *c) {
    Entrepreneur *newEntrep = dynamic_cast<Entrepreneur *>(c);
    if (newEntrep != NULL) {
        Employee *e = getEmployeeWithLessClientsAssociated();
        ContractedEmployee *ce = dynamic_cast<ContractedEmployee *>(e);

        newEntrep->associateEmployee(ce->getNif());
        ce->associateToClient(c);
    } else {
        throw NotAnEntrepreneur(c->getNif());
    }
}
void Pitch::reviewMaintenance(ServiceProvider *reviewed, unsigned int eva) {
    reviewed->review(eva);
}
void Pitch::reviewContractedEmployee(ContractedEmployee *reviewed, unsigned int eva) {
    reviewed->review(eva);
}

void Pitch::showLift(Lift *toShow) {
    cout << toShow;
}
void Pitch::showAllClients() {
    for (vector<Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
        cout << (*it) << "\n\n";
    }
}
void Pitch::showAllEmployees() {
    for (vector<Employee *>::iterator it = employees.begin(); it != employees.end(); it++) {
        cout << (*it) << "\n\n";
    }
}
void Pitch::showAllLifts() {
    for (vector<Lift *>::iterator it = lifts.begin(); it != lifts.end(); it++) {
        cout << (*it) << "\n";
        cout << "----------------------------------------------------------------------\n\n";
    }
}

void Pitch::showMostReviewedEmployee() {
    cout << getMostReviewedEmployee();
}
void Pitch::showLeastReviewedEmployee() {
    cout << getLeastReviewedEmployee();
}
void Pitch::showBestEvaluatedEmployee() {
    cout << getBestEvaluatedEmployee();
}
void Pitch::showWorstEvaluatedEmployee() {
    cout << getWorstEvaluatedEmployee();
}

void Pitch::showOldestClient() {
    cout << getOldestClient();
}
void Pitch::showMostRecentClient() {
    cout << getMostRecentClient();
}
void Pitch::showClientWithMostLiftsPurchased() {
    cout << getClientWithMostLiftsPurchased();
}
void Pitch::showClientAssociatedToLift(Lift *l) {
    cout << getClientAssociatedToLift(l);
}

void Pitch::showLiftInstallDate(Lift *l) {
    cout << l->getInstallDate();
}
void Pitch::showLiftLastMove(Lift *l) {
    cout << l->getLastMove();
}
void Pitch::showLiftLastMaintenance(Lift *l) {
    l->printLastMaintenance();
}
void Pitch::showLiftMaintenanceHist(Lift *l) {
    l->printMaintenanceHistory();
}

int Pitch::maintenance(Employee &employee, Lift &lift, Date today) {
    lift.Maintenance(employee.getNif(), today);
    return 0;
}
int Pitch::registerMove(Lift &lift, int op2, Date today, Time now) {
    lift.readMove(lift.getCurrentFloor(), op2, today, now);
    return 0;
}

bool Pitch::employeeExists(unsigned int nif) {
    try {
        searchEmployee(nif);
    }
    catch (EmployeeDoesNotExist &e) {
        return false;
    }
    return true;
}



