#include "carPark.h"
#include "insertionSort.h"
#include "sequentialSearch.h"
#include <algorithm>
#include <vector>
using namespace std;

CarPark::CarPark(unsigned cap, unsigned nMaxCli): capacity(cap), numMaxClients(nMaxCli) { freePlaces=cap; }

vector<InfoCard> CarPark::getClients() const { return clients; }

unsigned CarPark::getNumPlaces() const { return capacity; }

unsigned CarPark::getNumOccupiedPlaces() const { return capacity-freePlaces; }

unsigned CarPark::getNumMaxClients() const { return numMaxClients; }

unsigned CarPark::getNumClients() const { return clients.size(); }

int CarPark::clientPosition(const string & name) const
{
    InfoCard temp;
    temp.name = name;
    return sequentialSearch(clients, temp);
}

unsigned CarPark::getFrequency(const string &name) const
{
    int pos = clientPosition(name);
    if (pos == -1) throw ClientDoesNotExist(name);
    return clients[pos].frequency;
}

bool CarPark::addClient(const string & name)
{
    if (clients.size() == numMaxClients) return false;
    if (clientPosition(name) != -1) return false;
    InfoCard info;
    info.name=name;
    info.present=false;
    info.frequency=0;
    clients.push_back(info);
    return true;
}

// TODO: to modify
bool CarPark::removeClient(const string & name)
{
    for (vector<InfoCard>::iterator it = clients.begin(); it != clients.end(); it++)
        if ( (*it).name == name ) {
            if ( (*it).present == false ) {
                clients.erase(it);
                return true;
            }
            else return false;
        }
    return false;
}

bool CarPark::enter(const string & name)
{
    if (freePlaces == 0) return false;
    int pos = clientPosition(name);
    if (pos == -1) return false;
    if (clients[pos].present == true) return false;
    clients[pos].present = true;
    clients[pos].frequency++;
    freePlaces--;
    return true;
}

bool CarPark::leave(const string & name)
{
    int pos = clientPosition(name);
    if (pos == -1) return false;
    if (clients[pos].present == false) return false;
    clients[pos].present = false;
    freePlaces++;
    return true;
}

InfoCard CarPark::getClientAtPos(unsigned p) const
{
    if (p > getNumClients() || p < 0) throw PositionDoesNotExist(p);
    InfoCard info;
    info.name = clients[p].name;
    info.frequency = clients[p].frequency;
    info.present = clients[p].present;
    return info;
}

void CarPark::sortClientsByFrequency()
{
    insertionSort(clients);
}

bool byName(InfoCard &ic1, InfoCard &ic2) {
    return (ic1.name < ic2.name);
}

void CarPark::sortClientsByName()
{
    sort(clients.begin(), clients.end(), byName);
}

vector<string> CarPark::clientsBetween(unsigned f1, unsigned f2)
{
    vector<string> names;
    sortClientsByFrequency();
    for(int c = 0; c < getNumClients(); c++) {
        if (clients[c].frequency >= f1 & clients[c].frequency <= f2) {
            names.push_back(clients[c].name);
        }
    }
    return names;
}

ostream & operator<<(ostream & os, const CarPark & cp)
{
    for (int c = 0; c < cp.getNumClients(); c++) {
        os << "name: " << cp.getClientAtPos(c).name << ", presence: " << cp.getClientAtPos(c).present << ", frequency: " << cp.getClientAtPos(c).frequency << endl;
    }
    return os;
}

bool InfoCard::operator==(InfoCard &ic) const {
    return (name == ic.name);
}

bool InfoCard::operator<(InfoCard &ic) const {
    if (frequency == ic.frequency)
        return (name < ic.name);
    else
        return (frequency > ic.frequency);
}
