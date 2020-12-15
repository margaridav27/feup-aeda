#include "game.h"
#include <sstream>


//TODO
ostream &operator << (ostream &os, Circle &c1) {
	return os;
}

Game::Game(int h, vector<int> &points, vector<bool> &states) {
    game = build(h, 0, points, states);
}

BinaryTree<Circle> Game::build(int h, int n, vector<int> &points, vector<bool> &states) {
    Circle c(points[n], states[n]);
    if(h == 0) {
        return BinaryTree<Circle>(c);
    }
    BinaryTree<Circle> left = build(h - 1, 2 * n + 1, points, states);
    BinaryTree<Circle> right = build(h - 1, 2 * n + 2, points, states);
    return BinaryTree<Circle>(c, left, right);
}

string Game::writeGame() {
    stringstream stream;
	BTItrLevel<Circle> it(game);
	while (!it.isAtEnd()) {
        stream << it.retrieve().getPoints() << '-';
	    if (it.retrieve().getState()) {
            stream << "true-";
	    }
	    else {
            stream << "false-";
	    }
        stream << it.retrieve().getNVisits() << '\n';
	    it.advance();
	}
	return stream.str();
}

int Game::move() {
    BTItrLevel<Circle> it(game);
    int current = 1, next = 1, points = 0;
    while(!it.isAtEnd()) {
        if (current == next) {
            if (it.retrieve().getState()) {
                next = 2 * it.retrieve().getPoints() + 1; //true -> go to the right
            }
            else {
                next = 2 * it.retrieve().getPoints(); //false -> go to the left
            }
            it.retrieve().changeState();
            it.retrieve().addVisit();
            points = it.retrieve().getPoints();
        }
        current++;
        it.advance();
    }
    return points;
}

int Game::mostVisited() {
    int max = 0;
	BTItrLevel<Circle> it(game);
	it.advance(); //the root does not count
	while(!it.isAtEnd()) {
	    if (it.retrieve().getNVisits() > max) {
	        max = it.retrieve().getNVisits();
	    }
	    it.advance();
	}
	return max;
}
