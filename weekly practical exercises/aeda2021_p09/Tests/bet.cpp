#include "bet.h"
#include <iostream>
#include <sstream>
using namespace std;


bool Bet::contains(unsigned num) const {
	return (numbers.find(num) != numbers.end());
}

void Bet::generateBet(const vector<unsigned>& values, unsigned n) {
    for (vector<unsigned>::const_iterator it = values.begin(); it != values.end(); it++) {
        if (numbers.size() < n) {
            if (numbers.find(*it) == numbers.end()) { //value does not yet exist
                numbers.insert(*it);
            }
        }
        else break;
    }
}

unsigned Bet::countRights(const tabHInt& draw) const {
    unsigned int counter = 0;
	for (tabHInt::const_iterator it = draw.begin(); it != draw.end(); it++) {
	    if (numbers.find(*it) != numbers.end()) {
	        counter++;
	    }
	}
	return counter;
}

int Bet::getHash() const {
    int hash = 0;
    for (tabHInt::const_iterator it = numbers.begin(); it != numbers.end(); it++) {
        hash += *it;
    }
    return hash;
}