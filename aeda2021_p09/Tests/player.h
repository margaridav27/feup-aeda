#ifndef PLAYER_H_
#define PLAYER_H_

#include "bet.h"
#include <string>
using namespace std;

struct betHash {
    //the bets are hashed by the sum of their numbers
    //i could have defined any other hash function
	int operator() (const Bet& b) const {
		return b.getHash();
	}
	
	bool operator() (const Bet& b1, const Bet& b2) const {
		if (b1.getHash() != b2.getHash()) {
		    return false; //they are certainly not equal
		}
		//if the bets have the same hash number, they might be equal or not
		if (b1.getNumbers() == b2.getNumbers()) {
		    return true;
		}
	}
};


typedef unordered_set<Bet, betHash, betHash> tabHBet;

class Player
{
	tabHBet bets;
	string name;
public:
	Player(string nm="anonymous") { name=nm; }
	void addBet(const Bet & ap);
	unsigned betsInNumber(unsigned num) const;
	tabHBet drawnBets(const tabHInt& draw) const;
	unsigned getNumBets() const { return bets.size(); }
};

#endif 
