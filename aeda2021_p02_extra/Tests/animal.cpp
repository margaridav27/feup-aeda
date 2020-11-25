#include "animal.h"
#include <sstream>
using namespace std;

int Animal::youngest = 10000;

int Animal::getYoungest() { return youngest; }

Animal::Animal(string name, int age) {
    this->name = name;
    this->age = age;
    this->vet = 0;
    if (age < youngest) youngest = age;
}

Dog::Dog(string name, int age, string breed) : Animal(name, age) { this->breed = breed; }

Flying::Flying(int maxv, int maxa) { maxAltitude = maxa; maxVelocity = maxv; }

Bat::Bat(string name, int age, int maxv, int maxa) : Animal(name, age), Flying(maxv, maxa) { }

string Animal::getName() const { return name; }

int Animal::getAge() const { return age; }

void Animal::setVeterinary(Veterinary *vet) { this->vet = vet; }

Veterinary *Animal::getVeterinary() const { return vet; }

bool Dog::isYoung() const { return age < 5; }

bool Bat::isYoung() const { return age < 4; }

string Animal::getInfo() const {
    stringstream info;
    info << name << ", " << age;
    if (vet != NULL) info << ", " << vet->getInfo();
    return info.str();
}

string Dog::getInfo() const {
    stringstream info;
    info << Animal::getInfo() << ", " << breed;
    return info.str();
}

string Bat::getInfo() const {
    stringstream info;
    info << Animal::getInfo() << ", " << maxVelocity << maxAltitude;
    return info.str();
}