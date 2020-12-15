#ifndef CARPARK_H_
#define CARPARK_H_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class InfoCard {
public:
    string name;
    bool present;
    int frequency;
    bool operator< (InfoCard &ic) const;
    bool operator== (InfoCard &ic) const;
};


class CarPark {
    unsigned freePlaces;
    const unsigned  capacity;
    vector<InfoCard> clients;
    const unsigned numMaxClients;
public:
    CarPark(unsigned cap, unsigned nMaxCli);
    unsigned getNumPlaces() const;
    unsigned getNumMaxClients() const;
    unsigned getNumOccupiedPlaces() const;
    vector<InfoCard> getClients() const;
    unsigned getNumClients() const;
    bool addClient(const string & name);
    bool removeClient(const string & name);
    bool enter(const string & name);
    bool leave(const string & name);
    int clientPosition(const string & name) const;
    friend ostream & operator<<(ostream & os, const CarPark & cp);
    unsigned getFrequency(const string &name) const;
    InfoCard getClientAtPos(unsigned p) const;
    void sortClientsByFrequency();
    void sortClientsByName();
    vector<string> clientsBetween(unsigned f1, unsigned f2);
};

//exception ClientDoesNotExist
class ClientDoesNotExist {
public:
    string name;
    ClientDoesNotExist(string name) { this->name = name; }
    string getName() { return name; }
};

//exception PositionDoesNotExist
class PositionDoesNotExist {
public:
    unsigned position;
    PositionDoesNotExist(unsigned p) { position = p; }
    unsigned getPosition() { return position; }
};

#endif /*CARPARK_H_*/
