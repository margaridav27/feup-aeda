#include "player.h"

void Player::addBet(const Bet& b) {
    bets.insert(b);
}

unsigned Player::betsInNumber(unsigned num) const {
	unsigned int counter = 0;
	for (tabHBet::const_iterator it = bets.begin(); it != bets.end(); it++) {
	    if ((*it).contains(num)) {
	        counter++;
	    }
	}
	return counter;
}

tabHBet Player::drawnBets(const tabHInt& draw) const {
	tabHBet awarded;
    for (tabHBet::const_iterator it = bets.begin(); it != bets.end(); it++) {
        if ((*it).countRights(draw) > 3) {
            awarded.insert(*it);
        }
	}
	return awarded;
}
