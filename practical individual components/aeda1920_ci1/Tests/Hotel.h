#ifndef SRC_HOTEL_H_
#define SRC_HOTEL_H_

#include <vector>
#include <list>

template <class Chamber>
class Hotel {
	std::vector<Chamber *> chambers;
	const unsigned int maxCapacity;
	const std::string city;
    std::list<Chamber> reservationsDone;
public:
	Hotel(int size, std::string municipality);
	~Hotel();
	std::vector<Chamber *> getChambers() const;
	void setChambers(std::vector<Chamber *> chambers);
	int getCapacity() const;
    std::list<Chamber> getReservationsDone() const;
	bool addChamber(Chamber *chamber);
    void addReservationDone(Chamber chamber);
    Chamber* removeChamber(std::string code, int floor);
	float avgArea(int floor) const;
	void sortChambers();
	bool doReservation(std::string code, int floor);
    std::list<Chamber> roomsNeverReserved() const;
};

class NoSuchChamberException {
public:
	NoSuchChamberException() { }
};

class NoSuchFloorException {
private:
    int floor;
public:
    NoSuchFloorException(int floor);
    int getFloor();
};
NoSuchFloorException::NoSuchFloorException(int floor) {
    this->floor = floor;
}
int NoSuchFloorException::getFloor() {
    return floor;
}

template <class Chamber>
Hotel<Chamber>::Hotel(int size, std::string municipality) : maxCapacity(size), city(municipality) {
}

template <class Chamber>
Hotel<Chamber>::~Hotel(){
	typename std::vector<Chamber *>::const_iterator it;
	for (it=chambers.begin(); it!=chambers.end(); it++) {
		delete *it;
	}
}

template<class Chamber>
std::vector<Chamber *> Hotel<Chamber>::getChambers() const {
	return chambers;
}

template<class Chamber>
void Hotel<Chamber>::setChambers(std::vector<Chamber*> chambers) {
	this->chambers = chambers;
}

template<class Chamber>
int Hotel<Chamber>::getCapacity() const {
	return maxCapacity;
}

template<class Chamber>
void Hotel<Chamber>::addReservationDone(Chamber chamber) {
    reservationsDone.push_back(chamber);
}

template<class Chamber>
std::list<Chamber> Hotel<Chamber>::getReservationsDone() const {
    return reservationsDone;
}

template<class Chamber>
bool Hotel<Chamber>::addChamber(Chamber *chamber) {
    if (chambers.size() == maxCapacity) { //reached maximum capacity
        return false;
    }
    for (typename std::vector<Chamber*>::const_iterator it = chambers.begin(); it != chambers.end(); it++) {
        if (*(*it) == *chamber) { //chamber already exists
            return false;
        }
    }
    chambers.push_back(chamber);
    return true;
}

bool sortRooms(Room *r1, Room *r2) {
    if (r1->getCode() < r2->getCode()) return true;
    else if (r1->getCode() == r2->getCode() && r2->getFloor() < r1->getFloor()) return true;
    else return false;
}

template<class Chamber>
void Hotel<Chamber>::sortChambers() {
    sort(chambers.begin(), chambers.end(), sortRooms);
}

template<class Chamber>
Chamber* Hotel<Chamber>::removeChamber(std::string code, int floor) {
    for (typename std::vector<Chamber*>::const_iterator it = chambers.begin(); it != chambers.end(); it++) {
        if ((*it)->getCode() == code && (*it)->getFloor() == floor) {
            Chamber* temp = *it;
            chambers.erase(it);
            return temp;
        }
    }
    throw NoSuchChamberException();
}

template<class Chamber>
float Hotel<Chamber>::avgArea(int floor) const {
    float sum = 0;
    int n = 0;
    for (typename std::vector<Chamber*>::const_iterator it = chambers.begin(); it != chambers.end(); it++) {
        if ((*it)->getFloor() == floor) {
            sum += (*it)->getArea();
            n++;
        }
    }
    if (sum == 0 && n == 0) {
        throw NoSuchFloorException(floor);
    }
    return sum/n;
}

template<class Chamber>
bool Hotel<Chamber>::doReservation(std::string code, int floor) {
    for (typename std::vector<Chamber*>::const_iterator it = chambers.begin(); it != chambers.end(); it++) {
        if ((*it)->getCode() == code && (*it)->getFloor() == floor) {
            if ((*it)->getReservation() == false) {
                (*it)->setReservation(true);
                reservationsDone.push_back(*(*it));
                return true;
            }
        }
    }
    return false;
}

template<class Chamber>
std::list<Chamber> Hotel<Chamber>::roomsNeverReserved() const {
    std::list<Chamber> res;
    bool hasBeen;
    typename std::vector<Chamber*>::const_iterator i;
    typename std::list<Chamber>::const_iterator j;
    for (i = chambers.begin(); i != chambers.end(); i++) {
        hasBeen = false;
        for (j = reservationsDone.begin(); j != reservationsDone.end(); j++) {
            if ((*j).getCode() == (*i)->getCode() && (*j).getFloor() == (*i)->getFloor()) {
                hasBeen = true;
            }
        }
        if (!hasBeen) {
            res.push_back(*(*i));
            hasBeen = false;
        }
    }
    return res;
}


#endif /* SRC_HOTEL_H_ */
