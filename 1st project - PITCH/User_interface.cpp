#include "User_interface.h"
#include <iomanip>
#include <iostream>
#include <windows.h>
#include <string>
#include <cctype>
#include <numeric>
#include <chrono>
#include <thread>
#include <algorithm>
#include <ctime>

#define stairsDelay 3
#define emergencyTime 5
#define maxEntrepreneurNif 600000000
#define minEntrepreneurNif 500000000
#define maxParticularNif  300000000
#define minParticularNif  200000000
#define maxEmployeeNif 300000000
#define minEmployeeNif 200000000

using namespace std;

User_interface::User_interface(Pitch &pitch) {
    if (pitch.getNumEmployees() == 0) {
        cout << "The Pitch company has no employees and therefore isn't able to work properly. Enter some employees in the text file!\n\n";
    } else {
        this->pitch = &pitch;
    }
}

void User_interface::delay(int time, int from, int to) {
    if (from < to) {
        cout << "Floor " << from << endl<<endl;
        for (int i = from + 1; i <= to; i++) {
            this_thread::sleep_for(chrono::seconds(time));
            cout << "Floor " << i << endl << endl;
        }
    } else {
        cout << "Floor " << from << endl<<endl;
        for (int i = from - 1; i >= to; i--) {
            this_thread::sleep_for(chrono::seconds(time));
            cout << "Floor " << i << endl<<endl;
        }
    }
}

template<class T>
bool User_interface::input(T &in, vector<T> possibilities) {
    bool failedInput = true;
    while (failedInput) {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cout << "Invalid Input!\n\nChoose again: ";
        cin >> in;
        if (find(possibilities.begin(), possibilities.end(), in) != possibilities.end())
            failedInput = false;
    }
    return failedInput;
}

bool User_interface::verifyNifClient(unsigned int nif) {
    bool clientNotExists = false;
    bool employeeNotExists = false;
    try {
        pitch->searchClient(nif);
    }
    catch (ClientDoesNotExist &e) {
        clientNotExists = true;
    }
    try {
        pitch->searchEmployee(nif);
    }
    catch (EmployeeDoesNotExist &e) {
        employeeNotExists = true;
    }
    if (employeeNotExists && clientNotExists) {
        if (nif == pitch->getNif())
            return false;
        if (nif < minParticularNif)
            return false;
        if (nif > maxEntrepreneurNif)
            return false;
        if (nif > maxParticularNif && nif < minEntrepreneurNif)
            return false;
        return true;
    }
    return false;
}

bool User_interface::verifyNifEmployee(unsigned int nif) {
    bool clientNotExists = false;
    bool employeeNotExists = false;
    try {
        pitch->searchClient(nif);
    }
    catch (ClientDoesNotExist &e) {
        clientNotExists = true;
    }
    try {
        pitch->searchEmployee(nif);
    }
    catch (EmployeeDoesNotExist &e) {
        employeeNotExists = true;
    }
    if (employeeNotExists && clientNotExists) {
        if (nif == pitch->getNif())
            return false;
        else if (nif < minEmployeeNif)
            return false;
        else if (nif > maxEmployeeNif)
            return false;
        return true;
    }
    return false;
}

int User_interface::startInteraction() {
    bool loop = true;
    while (loop) {
        bool failedInput = false;
        cout << "\nPitch Project\n\n";
        cout << "    1. Pitch Office\n    2. Nearby building\n    3. Exit\n";
        cout << "\nChoose: ";
        int op1, exit;
        cin >> op1;
        cin.ignore(1000, '\n');
        if ((op1 != 1 && op1 != 2 && op1 != 3) or cin.fail())
            failedInput = true;
        if (failedInput) {
            vector<int> possibilities = {1, 2, 3};
            failedInput = input(op1, possibilities);
        }
        //OFFICE
        if (op1 == 1) {
            office();
        }
            //ENTER BUILDING
        else if (op1 == 2) {
            enterBuilding();
            //EXIT
        } else {
            loop = false;
        }
    }
    pitch->saveData();
    return 0;
}

void User_interface::office() {
    int op;
    bool failedInput = false;
    cout << "\n\nPitch Office\n\n";
    cout
            << "    1. Already a client\n    2. Add Client\n    3. Add Employee\n    4. Remove Client\n    5. Remove Employee\n    6. Pitch data info\n    7. Main menu\n";
    cout << "\nChoose: ";
    cin >> op;
    cin.ignore(1000, '\n');
    if ((op < 1 || op > 7) or cin.fail())
        failedInput = true;
    //ERROR READING
    if (failedInput) {
        vector<int> possibilities = {1, 2, 3, 4, 5, 6, 7};
        failedInput = input(op, possibilities);
    }

    unsigned int nif;

    switch (op) {
        case 1: { //ALREADY A CLIENT
            cout << "\nEnter your NIF: ";
            cin >> nif;
            while (cin.fail()) {
                cout << "Invalid Input!\n\nEnter your NIF again: ";
                cin.clear();
                cin.ignore(1000, '\n');
                cin >> nif;
            }
            try {
                ParticularClient *cl = dynamic_cast<ParticularClient *>(pitch->searchClient(nif));
                if (cl != nullptr)
                    client(cl);
                else {
                    Entrepreneur *cl = dynamic_cast<Entrepreneur *>(pitch->searchClient(nif));
                    client(cl);
                }
            }
            catch (ClientDoesNotExist &e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 2: { //BECOME A CLIENT
            bool noError = true;
            bool clientCreated = false;
            while (noError && !clientCreated) {
                string name, date;

                cout << "\nEnter your name: ";
                getline(cin, name);
                //ERROR READING
                while (cin.fail()) {
                    cout << "Invalid Input!\n\nEnter your name again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    getline(cin, name);
                }

                cout << "\nEnter your NIF: ";
                cin >> nif;

                //INVALID NIF OR ERROR READING
                while (cin.fail() or !verifyNifClient(nif)) {
                    cout << "Invalid Input!\n\nEnter your NIF again: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cin >> nif;
                }

                if (nif < maxParticularNif) {
                    Client *newUserClient = new ParticularClient(name, nif, today);
                    pitch->addClient(newUserClient);
                    clientCreated = true;

                } else {
                    Client *newUserClient = new Entrepreneur(name, nif, today);
                    pitch->addClient(newUserClient);
                    clientCreated = true;
                    try {
                        pitch->associateEmployeeToClient(newUserClient);
                    }
                    catch (NotAnEntrepreneur &e) {
                        cout << e.what() << endl;
                    }
                }

                if (clientCreated)
                    cout << "You are now a client!\n";
            }
            break;
        }
        case 3: {
            string name, date;

            cout << "\nEnter your name: ";
            getline(cin, name);
            //ERROR READING
            while (cin.fail()) {
                cout << "Invalid Input!\n\nEnter your name again: ";
                cin.clear();
                cin.ignore(1000, '\n');
                getline(cin, name);
            }

            cout << "\nEnter your NIF: ";
            cin >> nif;

            //INVALID NIF OR ERROR READING
            while (cin.fail() || !verifyNifEmployee(nif)) {
                if (cin.fail()) {
                    cout << "Invalid Input!\n\nEnter your NIF again: ";
                    cin.clear();
                } else if (pitch->employeeExists(nif)) {
                    cout << "Employee already exists!\n\nEnter a new NIF: ";
                }
                cin.ignore(1000, '\n');
                cin >> nif;
                cout << endl;
            }

            int age;
            cout << "\nEnter your age: ";
            cin >> age;
            while (cin.fail()) {
                cout << "Invalid Input!\n\nEnter your age again: ";
                cin.clear();
                cin.ignore(1000, '\n');
                cin >> age;
            }

            int salary;
            cout << "\nEnter your salary: ";
            cin >> salary;
            while (cin.fail()) {
                cout << "Invalid Input!\n\nEnter your salary again: ";
                cin.clear();
                cin.ignore(1000, '\n');
                cin >> age;
            }

            pitch->addEmployee(name, nif, age, salary);

            cout << "\nEmployee was successfully added.\n";
            break;
        }
        case 4: {
            cout << "\nEnter client's to remove NIF: ";
            cin >> nif;

            while (cin.fail()) {
                cout << "Invalid Input!\n\nEnter your NIF again: ";
                cin.clear();
                cin.ignore(1000, '\n');
                cin >> nif;
            }
            //INVALID NIF OR ERROR READING
            try {
                pitch->searchClient(nif);
                pitch->removeClient(nif);
            }
            catch (ClientDoesNotExist &e) {
                cout << e.what() << endl;
            }
            break;
        }
        case 5: {
            cout << "\nEnter Employee's to remove NIF: ";
            cin >> nif;

            //INVALID NIF OR ERROR READING
            while (cin.fail()) {
                cout << "Invalid Input!\n\nEnter your NIF again: ";
                cin.clear();
                cin.ignore(1000, '\n');
                cin >> nif;
            }
            try{
                pitch->searchEmployee(nif);
                pitch->removeEmployee(nif);
            }
            catch(EmployeeDoesNotExist &e)
            {
                cout<< e.what() <<endl;
            }
            break;
        }
        case 6: {
            data();
            break;
        }
        default: {
            break;
        }
    }
}

void User_interface::enterBuilding() {
    size_t nrLifts = this->pitch->getLifts().size();
    if (nrLifts == 0) {
        cout << "We apologize for the inconvenient but there are no lifts available.\n\n";
    }
    else {
        int op;
        bool failedInput = false;
        cout << "\nMASKS ARE MANDATORY.\nDON'T FORGET TO DISINFECT YOUR HANDS BEFORE ENTERING AND LEAVING THE LIFT.\n";
        cout << "Choose a building: \n\n";
        for (int i = 0; i < nrLifts; i++)
            cout << i + 1 << ". " << pitch->getLifts().at(i)->getAddress() << "\n";
        cout << nrLifts + 1 << ". Main menu\n";
        cout << "\n\n";
        cout << "Choose:\n";
        cin >> op;
        cin.ignore(1000, '\n');

        if ((op < 0 || op > nrLifts + 1) or cin.fail()) {
            failedInput = true;
        }
        if (failedInput) {
            vector<int> possibilities(pitch->getNumLift() + 1);
            iota(possibilities.begin(), possibilities.end(), 1);
            failedInput = input(op, possibilities);
        }
        if (op != nrLifts + 1) {
            building(*this->pitch->getLifts().at(op - 1));
        }
    }
}

void User_interface::client(ParticularClient *client) {
    bool loop = true;
    bool failedInput = false;
    while (loop) {
        cout << "\nHey, " << client->getName() << ".\n";
        cout
                << "    1. Purchase new lift\n    2. See my lifts information\n    3. Request maintenance for a lift\n    4. Main menu\n";
        cout << "\nChoose: ";
        int op;
        cin >> op;

        if ((op < 1 || op > 4) or cin.fail())
            failedInput = true;
        if (failedInput) {
            vector<int> possibilities = {1, 2, 3, 4};
            failedInput = input(op, possibilities);
        }

        switch (op) {
            case 1: {
                //PURCHASE NEW LIFT
                bool proceedele = false;
                while (!proceedele) {
                    bool proceed = true;
                    cin.ignore(1000, '\n');
                    //BUILDING NAME
                    cout << "\nEnter the building name of the lift: ";
                    string buildingName;
                    getline(cin, buildingName);
                    while (cin.fail()) {
                        cout << "Invalid Input!\n\nEnter the building name of the lift again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        getline(cin, buildingName);
                    }

                    //ADDRESS
                    cout << "\nEnter the address of the lift: ";

                    string ad;
                    getline(cin, ad);
                    while (cin.fail()) {
                        cout << "Invalid Input!\n\nEnter the address of the lift again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        getline(cin, ad);
                    }

                    //CAPACITY
                    cout << "\nEnter how many people you want the lift to carry: ";
                    unsigned int numberPeopleCap;
                    cin >> numberPeopleCap;
                    while (cin.fail()) {
                        cout << "Invalid Input!\n\nEnter how many people you want the lift to carry again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cin >> numberPeopleCap;
                    }
                    unsigned int capacity = numberPeopleCap * 70;

                    //NUMBER OF FLOORS
                    cout << "\nEnter the number of floors the lift travels (without garage floors): ";
                    int noFloors;
                    cin >> noFloors;
                    while (cin.fail()) {
                        cout << "Invalid Input!\n\nEnter the number of floors the lift travels again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cin >> noFloors;
                    }
                    int garageFloor;
                    cout << "\nEnter the number of garage floors (0-2): ";
                    cin >> garageFloor;
                    while (cin.fail() or (garageFloor > 2 || garageFloor < 0)) {
                        cout << "Invalid Input!\n\nEnter the number of floors the lift travels again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cin >> noFloors;
                    }

                    //BUILD THE LIFT
                    Lift *lftptr = new Lift(buildingName, ad, capacity, client->getNif(), noFloors, today);
                    lftptr->setHigherFloor(noFloors);
                    lftptr->setLowerFloor(garageFloor * -1);
                    lftptr->setRegularTime();

                    //PRICE
                    lftptr->setPrice();
                    cout << "\nThis lift costs " << lftptr->getPrice() << ".\n";
                    cout << "Let's proceed with the deal?\n";
                    cout << "    1. Yes.\n    2. No, thank you.\n";
                    cout << "\nChoose: ";
                    int opt;
                    cin >> opt;
                    if ((opt != 1 && opt != 2) or cin.fail())
                        proceed = false;
                    if (failedInput) {
                        vector<int> possibilities = {1, 2};
                        failedInput = input(opt, possibilities);
                    }

                    switch (opt) {
                        case 1: {
                            proceed = true;
                            pitch->addLift(lftptr);
                            client->purchaseNewLift(lftptr);

                            cout << endl << client->getName() << ", it was a nice deal.\n";
                            proceedele = true;
                            break;
                        }
                        case 2: {
                            proceedele = true; //HE DOES NOT WANT ANYTHING
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                }
                break;
            }
            case 2: {
                cout << "These are your current lifts:\n";
                for (unsigned int i = 0; i < client->getLifts().size(); i++) {
                    cout << client->getLifts()[i];
                }
                break;
            }
            case 3: { //VERIFY IF THE CLIENT HAS HAD ANY EMPLOYEE SERVICE
                if (client->hasLifts()) {
                    int op2;
                    size_t nrLifts = client->getLifts().size();

                    cout << "\nRequest maintenance for which of your buildings? \n";
                    for (int i = 0; i < nrLifts; i++)
                        cout << i + 1 << ". " << client->getLifts().at(i)->getAddress() << "\t";
                    cout << "\n\n";
                    cout << "\nChoose: ";
                    cin >> op2;
                    cin.ignore(1000, '\n');

                    if ((op2 < 0 || op2 > nrLifts) or cin.fail())
                        failedInput = true;
                    if (failedInput) {
                        vector<int> possibilities(nrLifts);
                        iota(possibilities.begin(), possibilities.end(), 1);
                        failedInput = input(op2, possibilities);
                    }

                    ServiceProvider *sp = pitch->getServiceProvider();
                    pitch->searchLift(client->getLifts()[op2 - 1]->getAddress())->Maintenance(sp->getNif(), today);
                    sp->executeMaintenance();
                    cout << "Want to review the service: \n";
                    cout << "1.yes  2.no\n";
                    cout << "\nChoose: ";
                    int answer;
                    cin >> answer;
                    cin.ignore(1000, '\n');
                    if ((answer != 1 && answer != 2) or cin.fail())
                        failedInput = true;
                    if (failedInput) {
                        vector<int> possibilities = {1, 2};
                        failedInput = input(answer, possibilities);
                    }
                    if (answer == 1) {
                        cout << "\nEnter an evaluation (0-5): ";
                        int eval;
                        cin >> eval;
                        if ((eval < 0 || eval > 5) or cin.fail())
                            failedInput = true;
                        if (failedInput) {
                            vector<int> possibilities = {1, 2, 3, 4, 5};
                            failedInput = input(eval, possibilities);
                        }
                        sp->review(eval);
                    }
                } else {
                    cout << "You have no lifts.\n\n";
                }
                break;
            }
            default: {
                loop = false;
            }
        }
    }
}

void User_interface::client(Entrepreneur *client) {
    bool failedInput = false;
    bool loop = true;
    while (loop) {
        cout << "\nHey, " << client->getName() << ".\n";
        cout
                << "    1. Purchase new lift\n    2. See my lifts information\n    3. Evaluate my Employee\n    4. Request maintenance for a lift\n    5. Main menu\n";
        cout << "Choose: ";
        int op;
        cin >> op;

        if ((op < 1 || op > 5) or cin.fail())
            failedInput = true;
        if (failedInput) {
            vector<int> possibilities = {1, 2, 3, 4, 5};
            failedInput = input(op, possibilities);
        }

        switch (op) {
            case 1: { //PURCHASE NEW LIFT
                bool proceedele = false;
                while (!proceedele) {

                    cin.ignore(1000, '\n');
                    //BUILDING NAME
                    cout << "\nEnter the building name of the lift: ";
                    string buildingName;
                    getline(cin, buildingName);
                    while (cin.fail()) {
                        cout << "\nInvalid Input!\n";
                        cout << "\nEnter the building name of the lift again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        getline(cin, buildingName);
                    }

                    //ADRESS
                    cout << "\nEnter the address of the lift: ";

                    string ad;
                    getline(cin, ad);
                    while (cin.fail()) {
                        cout << "Invalid Input!\n\nEnter the address of the lift again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        getline(cin, ad);
                    }

                    //CAPACITY
                    cout << "\nEnter how many people you want the lift to carry: ";
                    unsigned int numberPeopleCap;
                    cin >> numberPeopleCap;
                    while (cin.fail()) {
                        cout << "Invalid Input!\n\nEnter how many people you want the lift to carry again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cin >> numberPeopleCap;
                    }
                    double capacity = numberPeopleCap * 70.0;

                    //NUMBER OF FLOORS
                    cout << "\nEnter the number of floors the lift travels(without garage floors): ";
                    unsigned int noFloors;
                    cin >> noFloors;
                    while (cin.fail()) {
                        cout << "Invalid Input!\n\nEnter the number of floors the lift travels again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cin >> noFloors;
                    }
                    int garageFloor;
                    cout << "\nEnter the number of garage floors (0-2): \n";
                    cin >> garageFloor;
                    while (cin.fail() or (garageFloor > 2 || garageFloor < 0)) {
                        cout << "Invalid Input!\n\nEnter the number of floors the lift travels again: ";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cin >> noFloors;
                    }


                    //BUILD THE LIFT
                    Lift *lftptr = new Lift(buildingName, ad, capacity, client->getNif(), noFloors, today);
                    lftptr->setHigherFloor(noFloors);
                    lftptr->setLowerFloor(garageFloor * -1);
                    lftptr->setRegularTime();

                    //PRICE
                    lftptr->setPrice();
                    cout << "This lift costs " << lftptr->getPrice() << ".\n";
                    cout << "Let's proceed with the deal?\n";
                    cout << "    1. Yes.\n    2. No, thank you.\n";
                    cout << "Choose:\n";
                    int opt;
                    cin >> opt;
                    if ((opt != 1 && opt != 2) or cin.fail())
                        failedInput = true;
                    if (failedInput) {
                        vector<int> possibilities = {1, 2};
                        failedInput = input(opt, possibilities);
                    }

                    switch (opt) {
                        case 1: {
                            pitch->addLift(lftptr);
                            client->purchaseNewLift(lftptr);

                            cout << client->getName() << ", it was a nice deal.\n";
                            proceedele = true;
                            break;
                        }
                        case 2: {
                            proceedele = true; //HE DOES NOT WANT ANYTHING
                            break;
                        }
                    }
                }
                break;
            }
            case 2: {
                if (client->hasLifts()) {
                    cout << "\nThese are your current lifts:\n";
                    for (unsigned int i = 0; i < client->getLifts().size(); i++) {
                        cout << client->getLifts()[i];
                    }
                } else
                    cout << "You have no lifts.\n";
                break;
            }
            case 3: {
                cout << "\nPlease enter your evaluation (1-5): ";
                unsigned int eval;
                cin >> eval;
                if ((eval < 1 || eval > 5) or cin.fail())
                    failedInput = true;
                if (failedInput) {
                    vector<unsigned int> possibilities = {1, 2, 3, 4, 5};
                    failedInput = input(eval, possibilities);
                }

                pitch->reviewContractedEmployee(pitch->getEmployeeAssociatedToEntrepreneur(client), eval);
                cout << "\nThank you for giving us your opinion.\n";

                break;
            }
            case 4: {
                if (client->hasLifts()) {
                    int op2;
                    size_t nrLifts = client->getLifts().size();

                    cout << "\nRequest maintenance for which of your buildings? \n";
                    for (int i = 0; i < nrLifts; i++)
                        cout << i + 1 << ". " << client->getLifts().at(i)->getAddress() << "\t";
                    cout << "\n";
                    cout << "\nChoose: ";
                    cin >> op2;
                    cin.ignore(1000, '\n');

                    if ((op2 < 0 || op2 > nrLifts) or cin.fail())
                        failedInput = true;
                    if (failedInput) {
                        vector<int> possibilities(nrLifts);
                        iota(possibilities.begin(), possibilities.end(), 1);
                        failedInput = input(op2, possibilities);
                    }

                    ServiceProvider *sp = pitch->getServiceProvider();
                    pitch->searchLift(client->getLifts()[op2 - 1]->getAddress())->Maintenance(sp->getNif(), today);
                    sp->executeMaintenance();
                    cout << "Maintenance executed.\nAll set!\n";

                    cout << "\nWant to review the service: \n";
                    cout << "1.yes  2.no\n";
                    cout << "\nChoose: ";
                    int answer;
                    cin >> answer;
                    cin.ignore(1000, '\n');
                    if ((answer != 1 && answer != 2) or cin.fail())
                        failedInput = true;
                    if (failedInput) {
                        vector<int> possibilities = {1, 2};
                        failedInput = input(answer, possibilities);
                    }
                    if (answer == 1) {
                        cout << "\nEnter an evaluation (0-5): ";
                        int eval;
                        cin >> eval;
                        if ((eval < 0 || eval > 5) or cin.fail())
                            failedInput = true;
                        if (failedInput) {
                            vector<int> possibilities = {1, 2, 3, 4, 5};
                            failedInput = input(eval, possibilities);
                        }
                        pitch->reviewMaintenance(sp, eval);
                    }

                } else {
                    cout << "You have no lifts.\n\n";
                }
                break;
            }
            default: {
                loop = false;
            }
        }
    }
}

void User_interface::building(Lift &lift) {
    int timePerFloor = lift.getRegularTime();
    const std::time_t time = std::time(nullptr);
    const std::tm currentTime = *std::localtime(std::addressof(time));
    Time now(currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);

    bool failedInput = false, onTheBuilding = true;
    while (onTheBuilding) {
        cout << lift.getName() << "\n\n";
        cout << "CurrentFloor: " << floor << "\n";
        cout
                << "    1. Call lift\n    2. Go to the stairs\n    3. Call emergency\n    4. Leave the building/Main menu\n";
        cout << "\nChoose:";
        int op1, op2, op;
        cin >> op1;
        cin.ignore(1000, '\n');
        if ((op1 < 1 && op1 > 4) or cin.fail())
            failedInput = true;

        if (failedInput) {
            vector<int> possibilities = {1, 2, 3, 4};
            failedInput = input(op1, possibilities);
        }

        switch (op1) {
            case 1: { //CALL LIFT TO CURRENT FLOOR
                if (lift.getCurrentFloor() != floor) {
                    cout << "Lift was called. It's on its way.\n";
                    delay(timePerFloor, lift.getCurrentFloor(), floor);
                    this->pitch->registerMove(lift, floor, today, now);
                }

                cout << lift.getName() << "\n\n";
                cout << "\nFloor options from " << lift.getLowerFloor() << " to " << lift.getHigherFloor() << endl;

                cout << "Choose which floor you want to go to: ";
                cin >> op2;
                cin.ignore(1000, '\n');
                if ((op2 < lift.getLowerFloor() || op2 > lift.getHigherFloor()) or cin.fail())
                    failedInput = true;
                if (failedInput) {
                    vector<int> possibilities(lift.nLifts);
                    iota(possibilities.begin(), possibilities.end(), lift.getLowerFloor());
                    failedInput = input(op2, possibilities);
                }
                cout << "\nYou are on the move.\n";
                delay(timePerFloor, floor, op2);
                this->pitch->registerMove(lift, op2, today, now);
                floor = op2;
                break;
            }
            case 2: {

                cout << "CurrentFloor: " << floor << "\n";
                cout << "1.up \t 2.down\n";
                cout << "Choose: ";
                int op;
                cin >> op;
                cin.ignore(1000, '\n');
                if ((op != 1 && op != 2) or cin.fail())
                    failedInput = true;
                if (failedInput) {
                    vector<int> possibilities = {1, 2};
                    failedInput = input(op, possibilities);
                }

                if (op == 1) { //UP
                    if (floor == lift.getHigherFloor()) {
                        cout << "You are on the highest floor. Can't go up anymore.\n\n";
                    }
                    else {
                        delay(stairsDelay, floor, floor + 1);
                        floor++;
                    }

                } else { //DOWN
                    if (floor == lift.getLowerFloor())
                        cout << "You are on the lowest floor. Can't go down anymore.\n";
                    else {
                        delay(stairsDelay, floor, floor - 1);
                        floor--;
                    }
                }  //DOWN
                break;
            }
            case 3: {
                this->pitch->maintenance(*pitch->getServiceProvider(), lift, today);
                cout << "AN ALERT HAS BEEN SET, EMERGENCY IS COMING!\n";
                this_thread::sleep_for(chrono::seconds(emergencyTime));
                cout << "We are very sorry for the inconvenient, the last maintenance was due "
                     << lift.getLastMaintenance().date;
                cout << " and the number of door opens is " << lift.getPreviousDoorOpens()
                     << ".\nWe are clueless to what caused this problem but it is now fixed and you are free to go.\n"
                     << "Given the times we are living, the lift has also been disinfected!\n\n";
                break;
            } //MAINTENANCE
             default: {
                 if (floor != 0) { //NEEDS TO GO TO FRONT DOOR
                     if (lift.getCurrentFloor() != floor) {
                         cout << "Lift was called. It's on its way to take you to floor 0.\n";
                         delay(timePerFloor, lift.getCurrentFloor(), floor);
                         if (lift.checkMaintenanceNeeded(today) == 1)
                             lift.Maintenance(pitch->getServiceProvider()->getNif(),
                                              today); //BEFORE ANY MOVE WE CHECK IF MAINTENANCE IS NEEDED
                         this->pitch->registerMove(lift, floor, today, now);
                     }
                     cout << "You are on the move to floor 0.\n";
                     delay(timePerFloor, floor, 0);
                     if (lift.checkMaintenanceNeeded(today) == 1) {
                         lift.Maintenance(pitch->getServiceProvider()->getNif(),
                                          today); //BEFORE ANY MOVE WE CHECK IF MAINTENANCE IS NEEDED
                     }
                     this->pitch->registerMove(lift, 0, today, now); //GO TO FLOOR 0 AND LEAVE THE BUILDING
                 }
                 onTheBuilding = false;
            }
        }
    }
}

void User_interface::getTodayDate() {
    const std::time_t now = std::time(nullptr);
    const std::tm calendar_time = *std::localtime(std::addressof(now));
    today.day = calendar_time.tm_mday;
    today.month = calendar_time.tm_mon + 1;
    today.year = 1900 + calendar_time.tm_year;
}

void User_interface::data() {
    bool loop = true;
    while (loop) {
        int op;
        bool failedInput = false;
        cout << "\nPitch Office\n\n";
        cout << "    1. Lifts info \n    2. Employees info\n    3. Clients info\n    4. Main menu\n";
        cout << "\nChoose: ";
        cin >> op;
        cin.ignore(1000, '\n');
        if ((op < 1 || op > 4) or cin.fail())
            failedInput = true;

        if (failedInput) {
            vector<int> possibilities = {1, 2, 3, 4};
            failedInput = input(op, possibilities);
        }

        switch (op) {
            case 1: {
                liftsData();
                break;
            }
            case 2: {
                employeesData();
                break;
            }
            case 3: {
                clientsData();
                break;
            }
            default: {
                loop = false;
            }
        }
    }
}

void User_interface::employeesData() {
    int op;
    bool failedInput = false;
    cout << "\nPitch Office\n\n";
    cout
            << "    1. All Employees data \n    2. Most reviewed Employee\n    3. Least reviewed Employee\n    4. Best evaluated Employee\n    5. Worst evaluated Employee\n    6. Back\n";
    cout << "\nChoose: ";
    cin >> op;
    cout << '\n';
    cin.ignore(1000, '\n');
    if ((op < 1 || op > 6) or cin.fail())
        failedInput = true;

    if (failedInput) {
        vector<int> possibilities = {1, 2, 3, 4, 5, 6};
        failedInput = input(op, possibilities);
    }

    switch (op) {
        case 1: {
            this->pitch->showAllEmployees();
            break;
        }
        case 2: {
            this->pitch->showMostReviewedEmployee();
            break;
        }
        case 3: {
            this->pitch->showLeastReviewedEmployee();
            break;
        }
        case 4: {
            this->pitch->showBestEvaluatedEmployee();
            break;
        }
        case 5: {
            this->pitch->showWorstEvaluatedEmployee();
            break;
        }
        default: {
            break;
        }
    }
}

void User_interface::clientsData() {
    int op;
    bool failedInput = false;
    cout << "\nPitch Office\n\n";
    cout
            << "    1. All clients data \n    2. Oldest client\n    3. Client with most lifts purchased\n    4. Most recent client\n    5. Back\n";
    cout << "\nChoose: ";
    cout << '\n';
    cin >> op;
    cin.ignore(1000, '\n');
    if ((op < 1 || op > 5) or cin.fail())
        failedInput = true;

    if (failedInput) {
        vector<int> possibilities = {1, 2, 3, 4, 5};
        failedInput = input(op, possibilities);
    }

    switch (op) {
        case 1: {
            this->pitch->showAllClients();
            break;
        }
        case 2: {
            this->pitch->showOldestClient();
            break;
        }
        case 3: {
            this->pitch->showClientWithMostLiftsPurchased();
            break;
        }
        case 4: {
            this->pitch->showMostRecentClient();
            break;
        }
        default: {
            break;
        }
    }
}

void User_interface::liftsData() {
    int op;
    bool failedInput = false;
    cout << "\nPitch Office\n\n";
    cout << "    1. All lifts data \n    2. Specific lift\n    3. Back\n";
    cout << "\nChoose: ";
    cout << '\n';
    cin >> op;
    cin.ignore(1000, '\n');
    if ((op != 1 && op != 2 && op != 3) or cin.fail())
        failedInput = true;

    if (failedInput) {
        vector<int> possibilities = {1, 2, 3};
        failedInput = input(op, possibilities);
    }

    switch (op) {
        case 1: {
            this->pitch->showAllLifts();
            break;
        }
        case 2: {
            int op2;
            unsigned int numLifts = this->pitch->getNumLift();
            cout << "\nChoose a Lift: ";
            for (int i = 0; i < numLifts; i++)
                cout << i + 1 << ". " << pitch->getLifts().at(i)->getAddress() << "\n";
            cout << "\n";
            cout << "\nChoose: ";
            cin >> op2;
            cin.ignore(1000, '\n');

            if ((op2 < 0 || op2 > numLifts) or cin.fail())
                failedInput = true;
            if (failedInput) {
                vector<int> possibilities(numLifts);
                iota(possibilities.begin(), possibilities.end(), 1);
                failedInput = input(op2, possibilities);
            }

            Lift *l = this->pitch->getLifts()[op2 - 1];

            int op3;
            bool failedInput = false;
            cout << "Pitch Office\n\n";
            cout
                    << "    1.Show lift data\n    2.Show client associated to lift\n    3.Show lift install date\n    4.Show lift last move\n    5.Show maintenance history\n    6.Show last maintenance\n    7. Back\n";
            cout << "\nChoose: ";
            cin >> op3;
            cin.ignore(1000, '\n');
            if ((op3 < 1 || op3 > 7) or cin.fail())
                failedInput = true;

            if (failedInput) {
                vector<int> possibilities = {1, 2, 3, 4, 5, 6, 7};
                failedInput = input(op3, possibilities);
            }
            cout << "\n Lift " << l->getName() << endl;
            switch (op3) {
                case 1: {
                    this->pitch->showLift(l);
                    break;
                }
                case 2: {
                    this->pitch->showClientAssociatedToLift(l);
                    break;
                }
                case 3: {
                    this->pitch->showLiftInstallDate(l);
                    break;
                }
                case 4: {
                    if (!l->getMoves().empty())
                        this->pitch->showLiftLastMove(l);
                    else
                        cout << "\nNo moves history.\n";
                    break;
                }
                case 5: {
                    if (l->hasHadMaintenance())
                        this->pitch->showLiftMaintenanceHist(l);
                    else
                        cout << "\nNo maintenance history.\n";
                    break;
                }
                case 6: {
                    if (l->hasHadMaintenance())
                        this->pitch->showLiftLastMaintenance(l);
                    else
                        cout << "\nNo maintenance history.\n";
                    break;
                }
                default: {
                    break;
                }
            }
        }
        default: {
            break;
        }
    }
}
