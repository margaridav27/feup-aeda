#include "player.h"

void Player::addBet(const Bet& b) {
    bets.insert(b);
}

unsigned Player::betsInNumber(unsigned num) const {
	unsigned int counter = 0;
	for (auto it = bets.begin(); it != bets.end(); it++) { //why did not 'tabHInt::const_iterator' work here??
	    if ((*it).contains(num)) {
	        counter++;
	    }
	}
	return counter;
}

tabHBet Player::drawnBets(const tabHInt& draw) const {
	tabHBet awarded;
	for (auto it = bets.begin(); it != bets.end(); it++) {
        if ((*it).countRights(draw) > 3) {
            awarded.insert(*it);
        }
	}
	return awarded;
}
