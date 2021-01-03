#include "Pitch.h"
#include <iostream>
#include <algorithm>

#define maxEntrepeneurNif 600000000
#define minEntrepeneurNif 500000000
#define maxParticularNif  300000000
#define minParticularNif  200000000

using namespace std;

Pitch::Pitch(string clientsFile, string employeesFile, string liftsFile) : maintenances(MaintenanceRecord()) {
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

        newLift->setPrice(); //by reading the capacity and the number of floors we can calculate the price
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
            Date d(date);
            Time tI(tInit);
            newLift->readMove(from, to, d, tI);
        }

        while (getline(inStream, info)) { //maintHistory
            if (info == trash)
                break; //lift has nothing on maintHistory vector yet or there is no more maintenances to add

            string date, time;
            unsigned int nif;

            stringstream maints(info);
            maints >> date >> time >> nif;
            Date d(date);
            Time t(time);
            Maintenance *maintenance = new Maintenance(newLift->getCode(), d, t, nif, true);
            newLift->addMaintenanceToMaintHistory(maintenance);
            MaintenanceRecord m(maintenance);
            maintenances.insert(m);
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
        clients.insert(newClient);
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
            unsigned int numMaints;
            string trash, info;

            inStream >> salary >> hours >> numMaints;
            inStream.ignore(1000000, '\n');

            Employee *newEmployee = new ServiceProvider(name, nif, age, salary);
            ServiceProvider *sp;
            sp = dynamic_cast<ServiceProvider *>(newEmployee);
            ServiceProviderType spt(sp);
            sp->setOldReviews(eva, nRev);
            sp->setHoursWorked(hours);
            sp->setNumMaintenances(numMaints);

            getline(inStream, trash); //'--------------------'

            while (getline(inStream, info)) { //service provider schedule
                if (info == trash) break; //service provider has nothing on his schedule

                string date, time;
                unsigned int code;

                stringstream maints(info);
                maints >> code >> date >> time;
                Date d(date);
                Time t(time);
                Maintenance *m = new Maintenance(code, d, t, sp->getNif(), false);
                maintenances.insert(MaintenanceRecord(m));
                sp->scheduleMaintenance(m);
            }

            employees.push_back(sp);
            serviceProviders.push(spt);

            inStream >> identifier;
            inStream.ignore(1000000, '\n');

        } else if (identifier == 'x') { //contracted employee
            unsigned int cNif;

            inStream >> salary;
            inStream.ignore(1000000, '\n');

            Employee *newEmployee = new ContractedEmployee(name, nif, age, salary);
            ContractedEmployee *ce;
            ce = dynamic_cast<ContractedEmployee *>(newEmployee);
            ContractedEmployeeType cet(ce);
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
            contractedEmployees.push(cet);

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

    for (HashTableClient::iterator it = clients.begin(); it != clients.end(); it++) {
        (*it)->printToFile(opStream);
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
    for (HashTableClient::iterator it = clients.begin(); it != clients.end(); it++) {
        if ((*it)->getNif() == searchedNIF) {
            return *it;
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

void Pitch::addClient(Client *toAdd) {
    clients.insert(toAdd);
}

void Pitch::addServiceProvider(ServiceProviderType spt) {
    serviceProviders.push(spt);
    employees.push_back(spt.getServiceProvider());
}

void Pitch::addContractedEmployee(ContractedEmployeeType cet) {
    contractedEmployees.push(cet);
    employees.push_back(cet.getContractedEmployee());
}

void Pitch::addEmployee(string name, unsigned int nif, unsigned int age, double salary) {
    if (salary >= 365) {
        ContractedEmployee *newcontractedEmployee = new ContractedEmployee(name, nif, age, salary);
        Employee *newEmployee = dynamic_cast<Employee *>(newcontractedEmployee);
        employees.push_back(newEmployee);
        contractedEmployees.push(newcontractedEmployee);
    } else {
        ServiceProvider *newSp = new ServiceProvider(name, nif, age, salary);
        Employee *newEmployee = dynamic_cast<Employee *>(newSp);
        employees.push_back(newEmployee);
        serviceProviders.push(newSp);
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

void Pitch::automaticMaintenance(Lift *lift, Date today, Time time) {
    ServiceProviderType sp(serviceProviders.top());
    this->temporarilyRemoveEmployee(sp.getServiceProvider()->getNif());
    Maintenance *m = new Maintenance(lift->getCode(), today, time, sp.getServiceProvider()->getNif());
    MaintenanceRecord mr(m->getLiftCode(), m->getMaintenanceDate(), m->getMaintenanceSchedule(), m->getSpNIF());
    maintenances.insert(mr);
    sp.getServiceProvider()->scheduleMaintenance(m);
    sp.getServiceProvider()->executeMaintenance();
    this->addServiceProvider(sp);
    lift->addMaintenanceToMaintHistory(m);
}

bool Pitch::scheduleMaintenance(Lift *l, Time sched, Date appoint) {
    heapEmployeesSP aux = serviceProviders;
    ServiceProviderType sp(serviceProviders.top());
    heapMaint aux_maint = sp.getServiceProvider()->getSchedule();
    bool available = true;
    while (!aux.empty()) {
        sp = serviceProviders.top();
        heapMaint aux_maint = sp.getServiceProvider()->getSchedule();
        available = true;
        while (!aux_maint.empty()) {
            if (((aux_maint.top()->getMaintenanceSchedule() <= sched) &&
                 (sched <= aux_maint.top()->getMaintenanceSchedule() + aux_maint.top()->getMaintenanceDuration())) &&
                aux_maint.top()->getMaintenanceDate() == appoint) {
                available = false;
            } else if (((aux_maint.top()->getMaintenanceSchedule() > sched) &&
                        (aux_maint.top()->getMaintenanceSchedule() <=
                         sched + aux_maint.top()->getMaintenanceSchedule())) &&
                       aux_maint.top()->getMaintenanceDate() == appoint) {
                available = false;
            }
            aux_maint.pop();
        }
        if (available) {
            this->temporarilyRemoveEmployee(sp.getServiceProvider()->getNif());
            Maintenance *m = new Maintenance(l->getCode(), appoint, sched, sp.getServiceProvider()->getNif());
            MaintenanceRecord mr(m->getLiftCode(), m->getMaintenanceDate(), m->getMaintenanceSchedule(), m->getSpNIF());
            maintenances.insert(mr);
            sp.getServiceProvider()->scheduleMaintenance(m);
            this->addServiceProvider(sp);
            return true;
        }
        aux.pop();
    }
    return available;
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
    //if the evaluation is bigger, there is no doubts
    //but if the evaluation of both employees happens to be the same, the function returns the one with the highest number of evaluations since that means the evaluation, whatever it is, is more reliable

    if (serviceProviders.top().getServiceProvider()->getEvaluation() >
        contractedEmployees.top().getContractedEmployee()->getEvaluation())
        return serviceProviders.top().getServiceProvider();
    else if (serviceProviders.top().getServiceProvider()->getEvaluation() ==
             contractedEmployees.top().getContractedEmployee()->getEvaluation() &&
             serviceProviders.top().getServiceProvider()->getNumOfReviews() >
             contractedEmployees.top().getContractedEmployee()->getNumOfReviews())
        return serviceProviders.top().getServiceProvider();
    else
        return contractedEmployees.top().getContractedEmployee();
}

Employee *Pitch::getWorstEvaluatedEmployee() {
    //if the evaluation is lower and considering the fact that we are taking into account the last elements of both queues, there is no doubts
    //but if the evaluation of both employees happens to be the same, the function returns the one with the highest number of evaluations since that means the evaluation, whatever it is, is more reliable

    heapEmployeesSP auxSP = serviceProviders;
    heapEmployeesCE auxCE = contractedEmployees;

    Employee *sp; //will keep the last service provider on the queue
    while (!auxSP.empty()) {
        sp = auxSP.top().getServiceProvider();
        auxSP.pop();
    }

    Employee *ce; //will keep the last contracted employee on the queue
    while (!auxCE.empty()) {
        ce = auxCE.top().getContractedEmployee();
        auxCE.pop();
    }

    if (sp->getEvaluation() < ce->getEvaluation()) return sp;
    else if (sp->getEvaluation() == ce->getEvaluation() && sp->getNumOfReviews() > sp->getNumOfReviews()) return sp;
    else return ce;
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
    if (clients.empty()) return NULL;
    Date minDate = (*clients.begin())->getDateFirstLift();
    HashTableClient::iterator oldestClient = clients.begin();
    for (HashTableClient::iterator it = clients.begin(); it != clients.end(); it++) {
        if ((*it)->getDateFirstLift() < minDate) {
            minDate = (*it)->getDateFirstLift();
            oldestClient = it;
        }
    }
    return *oldestClient;
}

Client *Pitch::getMostRecentClient() {
    if (clients.empty()) return NULL;
    Date minDate = (*clients.begin())->getDateFirstLift();
    HashTableClient::iterator recentClient = clients.begin();
    for (HashTableClient::iterator it = clients.begin(); it != clients.end(); it++) {
        if (minDate < (*it)->getDateFirstLift()) {
            minDate = (*it)->getDateFirstLift();
            recentClient = it;
        }
    }
    return *recentClient;
}

Client *Pitch::getClientWithMostLiftsPurchased() {
    if (clients.empty()) return NULL;
    int maxNumberOfLifts = (*clients.begin())->getLifts().size();
    HashTableClient::iterator clientWithMostLifts = clients.begin();
    for (HashTableClient::iterator it = clients.begin(); it != clients.end(); it++) {
        if (maxNumberOfLifts < (*it)->getLifts().size()) {
            maxNumberOfLifts = (*it)->getLifts().size();
            clientWithMostLifts = it;
        }
    }
    return *clientWithMostLifts;
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
        //REMOVING A CONTRACTED EMPLOYEE
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

            //removing the contracted employee from the queue as well
            heapEmployeesCE temp;
            while (!contractedEmployees.empty()) {
                if (contractedEmployees.top().getContractedEmployee() == toRm) {
                    //not to preserve
                } else {
                    temp.push(contractedEmployees.top());
                }
                contractedEmployees.pop();
            }
            contractedEmployees = temp;
        } else {
            //REMOVING A SERVICE PROVIDER
            ServiceProvider *toRm = dynamic_cast<ServiceProvider *>(searchEmployee(nif));
            if (toRm != NULL) {
                vector<Maintenance *> maintsToReassign;
                if (toRm->getNumAppointments() !=
                    0) { //the service provider has scheduled maintenances that need to be reassigned
                    heapMaint temp = toRm->getSchedule();
                    while (!temp.empty()) {
                        maintsToReassign.push_back(temp.top());
                        temp.pop();
                    }
                }

                //removing the service provider from the queue to prevent him from interfere with the reassignment
                heapEmployeesSP temp;
                while (!serviceProviders.empty()) {
                    if (serviceProviders.top().getServiceProvider() == toRm) {
                        //not to preserve
                    } else {
                        temp.push(serviceProviders.top());
                    }
                    serviceProviders.pop();
                }
                serviceProviders = temp;

                //reassigning the maintenances that the service provider just removed had in hands
                vector<Maintenance *>::iterator it = maintsToReassign.begin();
                while (!maintsToReassign.empty()) {
                    Employee *emp = serviceProviders.top().getServiceProvider();
                    serviceProviders.pop();
                    ServiceProvider *sp;
                    sp = dynamic_cast<ServiceProvider *>(emp);
                    bool completed = sp->scheduleMaintenance(*it);
                    if (completed) {
                        maintsToReassign.erase(
                                it); //task was reassign, it is incremented automatically because of the erase operation
                    } else { //we ignore by now the fact that the maintenance failed to be reassigned and will come back to it later
                        it++;
                        if (it ==
                            maintsToReassign.end()) { //eventually we will have traversed all maintenances and need to restart
                            it = maintsToReassign.begin();
                        }
                    }
                    serviceProviders.push(
                            sp); //the service provider is pushed again into the queue whether the maintenance was assigned to him or not
                }
            }
        }

        //removing the employee, whatever is his type, from the vector
        if (binarySearch(employees, searchEmployee(nif)) != -1) {
            employees.erase(employees.begin() + SequentialSearch(employees, searchEmployee(nif)));
        } //there is no need to worry about an else condition because if employee does not exist, that exception will be catched
    }
    catch (EmployeeDoesNotExist &exc) {
        cout << exc.what();
    }
}

void Pitch::temporarilyRemoveEmployee(unsigned int nif) {
    ContractedEmployee *toRm = dynamic_cast<ContractedEmployee *>(searchEmployee(nif));
    if (toRm != NULL) {
        //removing the contracted employee from the queue as well
        heapEmployeesCE temp;
        while (!contractedEmployees.empty()) {
            if (contractedEmployees.top().getContractedEmployee() == toRm) {
                //not to preserve
            } else {
                temp.push(contractedEmployees.top());
            }
            contractedEmployees.pop();
        }
        contractedEmployees = temp;

    } else {
        ServiceProvider *toRm = dynamic_cast<ServiceProvider *>(searchEmployee(nif));
        if (toRm !=
            NULL) {//removing the service provider from the queue to prevent him from interfere with the reassignment
            heapEmployeesSP temp;
            while (!serviceProviders.empty()) {
                if (serviceProviders.top().getServiceProvider() == toRm) {
                    //not to preserve
                } else {
                    temp.push(serviceProviders.top());

                }
                serviceProviders.pop();
            }
            serviceProviders = temp;
        }
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

    for (HashTableClient::iterator it = clients.begin(); it != clients.end(); it++) {
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

void Pitch::showLift(Lift *toShow) {
    cout << toShow;
}

void Pitch::showAllClients() {
    for (HashTableClient::iterator it = clients.begin(); it != clients.end(); it++) {
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


void Pitch::updateMaintenances(Time actualTime, Date actualDate) {
    ServiceProvider *sp;
    BSTItrIn<MaintenanceRecord> itrIn(maintenances);
    while (!itrIn.isAtEnd()) {
        if (itrIn.retrieve().getMaintenance()->getMaintenanceDate() < actualDate &&
            !itrIn.retrieve().getMaintenance()->getFinished()) {
            itrIn.retrieve().getMaintenance()->setFinished(true);
            searchLift(itrIn.retrieve().getMaintenance()->getLiftCode())->addMaintenanceToMaintHistory(
                    itrIn.retrieve().getMaintenance());
            sp = dynamic_cast<ServiceProvider *> (searchEmployee(itrIn.retrieve().getMaintenance()->getSpNIF()));
            sp->executeMaintenance();
            this->temporarilyRemoveEmployee(sp->getNif());
            this->addServiceProvider(sp);
        } else if (itrIn.retrieve().getMaintenance()->getMaintenanceDate() == actualDate &&
                   !itrIn.retrieve().getMaintenance()->getFinished()) {
            if (itrIn.retrieve().getMaintenance()->getMaintenanceSchedule() < actualTime) {
                itrIn.retrieve().getMaintenance()->setFinished(true);
                searchLift(itrIn.retrieve().getMaintenance()->getLiftCode())->addMaintenanceToMaintHistory(
                        itrIn.retrieve().getMaintenance());
                sp = dynamic_cast<ServiceProvider *> (searchEmployee(itrIn.retrieve().getMaintenance()->getSpNIF()));
                sp->executeMaintenance();
                this->temporarilyRemoveEmployee(sp->getNif());
                this->addServiceProvider(sp);
            }
        }
        itrIn.advance();
    }
}



