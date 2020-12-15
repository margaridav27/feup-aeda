#include "fleet.h"
#include <string>

using namespace std;

void Fleet::addVehicle(Vehicle *v1) { vehicles.push_back(v1); }

int Fleet::numVehicles() const { return vehicles.size(); }

int Fleet::lowestYear() const {
    if (numVehicles() == 0) return 0; // there is no vehicle
    int lowest = 3000;
    for (int i = 0; i < numVehicles(); i++) {
        if (vehicles[i]->getYear() < lowest) {
            lowest = vehicles[i]->getYear();
        }
    }
    return lowest;
}

float Fleet::totalTax() const {
    float total = 0;
    for (int i = 0; i < numVehicles(); i++) {
        total += vehicles[i]->calculateTax();
    }
    return total;
}

unsigned Fleet::removeOldVehicles(int y1) {
    vector<Vehicle *> vehiclesUpdated;
    unsigned removed;
    for (int i = 0; i < numVehicles(); i++) {
        if (vehicles[i]->getYear() > y1) vehiclesUpdated.push_back(vehicles[i]); }
    removed = numVehicles() - vehiclesUpdated.size();
    vehicles.clear();
    for (int i = 0; i < vehiclesUpdated.size(); i++)
        vehicles.push_back(vehiclesUpdated[i]);
    return removed;
}

vector<Vehicle *> Fleet::getVehicles() const { return vehicles; }

vector<Vehicle *> Fleet::operator()(int y1) const {
    vector<Vehicle *> v;
    for (size_t i = 0; i < numVehicles(); i++) {
        if (vehicles[i]->getYear() == y1) v.push_back(vehicles[i]);
    }
    return v;
}

ostream & operator<<(ostream & o, const Fleet & f) {
    for (int i = 0; i < f.numVehicles(); i++) {
        f.vehicles[i]->info(o);
    }
    return o;
}