#include "vehicle.h"
#include <iostream>

using namespace std;

Vehicle::Vehicle(string b, int m, int y) { brand = b; month = m; year = y; }

MotorVehicle::MotorVehicle(string b, int m, int y, string f, int cyl) : Vehicle(b, m, y) { fuel = f; cylinder = cyl; }

Car::Car(string b, int m, int y, string f, int cyl) : MotorVehicle(b, m, y, f, cyl) { }

Truck::Truck(string b, int m, int y, string f, int cyl, int ml) : MotorVehicle(b, m, y, f, cyl) { maximumLoad = ml; }

Bicycle::Bicycle(string b, int m, int y, string t) :Vehicle(b, m, y) { type = t; }

string MotorVehicle::getFuel() const { return fuel; }

int Vehicle::getYear() const { return year; }

string Vehicle::getBrand() const { return brand; }

int Vehicle::info() const { return 3; }

int MotorVehicle::info() const { return 5; }
int MotorVehicle::info(ostream &o) const {
    o << year << "/" << month << " brand: " << brand << " fuel: " << fuel << " cylinder: " << cylinder << endl;
    return 5;
}

int Car::info() const { return 5; }
int Car::info(ostream &o) const {
    o << year << "/" << month << " brand: " << brand << " fuel: " << fuel << " cylinder: " << cylinder << endl;
    return 5;
}

int Truck::info() const { return 6; }
int Truck::info(ostream &o) const {
    o << year << "/" << month << ", brand: " << brand << ", fuel: " << fuel << ", cylinder: " << cylinder << ", maximum load: " << maximumLoad << endl;
    return 6;
}

int Bicycle::info() const { return 4; }
int Bicycle::info(ostream &o) const {
    o << year << "/" << month << " brand: " << brand << " type: " << type << endl;
    return 6;
}

bool Vehicle::operator < (const Vehicle & v) const {
    if (year < v.year) return true;
    else if (year == v.year & month < v.month) return true;
    return false;
}

float MotorVehicle::calculateTax() const {
    if (fuel == "gas") {
        if (cylinder <= 1000) {
            if (year > 1995) return 14.56;
            else return 8.10;
        } else if (cylinder > 1000 & cylinder <= 1300) {
            if (year > 1995) return 29.06;
            else return 14.56;
        } else if (cylinder > 1300 & cylinder <= 1750) {
            if (year > 1995) return 45.15;
            else return 22.65;
        } else if (cylinder > 1750 & cylinder <= 2600) {
            if (year > 1995) return 113.98;
            else return 54.89;
        } else if (cylinder > 2600 & cylinder <= 3500) {
            if (year > 1995) return 181.17;
            else return 87.13;
        } else if (cylinder > 3500) {
            if (year > 1995) return 320.89;
            else return 148.37;
        }
    } else {
        if (cylinder <= 1500) {
            if (year > 1995) return 14.56;
            else return 8.10;
        } else if (cylinder > 1500 & cylinder <= 2000) {
            if (year > 1995) return 29.06;
            else return 14.56;
        } else if (cylinder > 2000 & cylinder <= 3000) {
            if (year > 1995) return 45.15;
            else return 22.65;
        } else if (cylinder > 3000) {
            if (year > 1995) return 113.98;
            else return 54.89;
        }
    }
}

float Bicycle::calculateTax() const { return 0; }






