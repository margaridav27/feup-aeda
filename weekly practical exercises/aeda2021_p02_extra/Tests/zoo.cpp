#include "zoo.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
using namespace std;

unsigned Zoo::numAnimals() const { return animals.size(); }

unsigned Zoo::numVeterinarians() const { return veterinarians.size();}

void Zoo::addAnimal(Animal *a1) { animals.push_back(a1); }

string Zoo::getInfo() const {
    stringstream info;
    string data = "";
    for (unsigned int i = 0; i < numAnimals(); i++) {
        info << animals[i]->getInfo() << endl;
    }
    return info.str();
}

bool Zoo::isYoung(string nameA) {
    for (unsigned int i = 0; i < numAnimals(); i++) {
        if (animals[i]->getName() == nameA) {
            return animals[i]->isYoung();
        }
    }
    return false;
}

void Zoo::allocateVeterinarians(istream &isV) {
    string name, cod;
    while (!isV.eof()) {
        getline(isV, name);
        getline(isV, cod);
        long c = atoi(cod.c_str());
        Veterinary *vet = new Veterinary(name, c);
        veterinarians.push_back(vet);
    }
    for (int i = 0; i < numAnimals(); i++) {
        animals[i]->setVeterinary(veterinarians[i % numVeterinarians()]);
    }
}

bool Zoo::removeVeterinary(string nameV) {
    for (unsigned int i = 0; i < numVeterinarians(); i++) {
        if (veterinarians[i]->getName() == nameV) {
            for (unsigned int j = 0; j < numAnimals(); j++) {
                if (animals[j]->getVeterinary() == veterinarians[i]) {
                    animals[j]->setVeterinary(veterinarians[(i + 1) % veterinarians.size()]);
                }
            }
            veterinarians.erase(veterinarians.begin() + i);
            return true;
        }
    }
    return false;
}

bool Zoo::operator<(Zoo &zoo2) const {
    int agesZoo1 = 0, agesZoo2 = 0;
    for (int i = 0; i < numAnimals(); i++) {
        agesZoo1 += animals[i]->getAge();
    }
    for (int i = 0; i < zoo2.numAnimals(); i++) {
        agesZoo2 += zoo2.animals[i]->getAge();
    }
    return agesZoo1 < agesZoo2;
}