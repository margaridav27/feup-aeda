#include "game.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;


unsigned int Game::numberOfWords(string phrase) {
    if ( phrase.length() == 0 ) return 0;

    unsigned n=1;
    size_t pos = phrase.find(' ');
    while (pos != string::npos) {
        phrase = phrase.substr(pos+1);
        pos = phrase.find(' ');
        n++;
    }
    return n;
}

Game::Game() { kids.clear(); }

Game::Game(list<Kid>& l2) { kids = l2; }

void Game::addKid(const Kid &k1) { kids.push_back(k1); }

list<Kid> Game::getKids() const { return kids; }

string Game::write() const {
    stringstream oss;
    for (list<Kid>::const_iterator it = kids.begin(); it != kids.end(); it++) {
        oss << it->write() << '\n';
    }
    return oss.str();
}

Kid& Game::loseGame(string phrase) {
    int nw = numberOfWords(phrase) - 1; //number of words
    list<Kid>::iterator it = kids.begin();
    list<Kid>::iterator end = kids.end();
    while (kids.size() > 1)
    {
        int p = nw % kids.size();
        for (int i = 1; i <= p ; i++) {
            it++;
            if (it == end) it = kids.begin(); //reached the last element but continues at the other end
        }
        it = kids.erase(it);
        if (it == end) it = kids.begin(); //in the next iteration it will start counting from the first element
    }
    return *it;
}

list<Kid>& Game::reverse() {
    kids.reverse();
    return kids;
}

list<Kid> Game::removeOlder(unsigned id)
{
    list<Kid> kidsCopy(kids.begin(), kids.end());
    list<Kid> removed;
    kids.clear();
    for (list<Kid>::const_iterator it = kidsCopy.begin(); it != kidsCopy.end(); it++) {
        if((*it).getAge() > id) {
            removed.push_back(*it);
        }
        else {
            kids.push_back(*it);
        }
    }
    return removed;
}

void Game::setKids(const list<Kid>& l1) { this->kids = l1; }

bool Game::operator==(Game& g2)
{
	if (kids.size() != g2.kids.size()) return false;
	list<Kid>::iterator itg2 = g2.kids.begin();
	for (list<Kid>::const_iterator itg1 = kids.begin(); itg1 != kids.end(); itg1++) {
	    if (!((*itg1)==(*itg2))) {
	        return false;
	    }
	    else {
	        itg2++;
	    }
	}
	return true;
}

list<Kid> Game::shuffle() const
{
    vector<Kid> kidsVector;
    for (list<Kid>::const_iterator it = kids.begin(); it != kids.end(); it++) {
        kidsVector.push_back(*it);
    }
    random_shuffle(kidsVector.begin(), kidsVector.end());
    list<Kid> shuffledKids;
    for (vector<Kid>::const_iterator it = kidsVector.begin(); it != kidsVector.end(); it++) {
        shuffledKids.push_back(*it);
    }
    return shuffledKids;
}
