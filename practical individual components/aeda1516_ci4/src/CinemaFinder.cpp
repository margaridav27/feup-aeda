/*
 * CinemaFinder.cpp
 */

#include "CinemaFinder.h"
#include "Cinema.h"
#include "Film.h"
#include <algorithm>

CinemaFinder::CinemaFinder() {}

CinemaFinder::~CinemaFinder() {}

void CinemaFinder::addFilm(Film *f1) {
    FilmPtr fp1;
    fp1.film = f1;
    films.insert(fp1);
}

void CinemaFinder::addCinema(const Cinema &c1) { cinemas.push(c1); }

tabHFilm CinemaFinder::getFilms() const { return films; }

priority_queue <Cinema> CinemaFinder::getCinemas() const { return cinemas; }


// TODO

//b1
list <string> CinemaFinder::filmsWithActor(string actorName) const {
    list <string> res;
    for (auto film : films) {
        for (auto actor : film->getActors()) {
            if (actor == actorName) {
                res.push_back(film->getTitle());
                break;
            }
        }
    }
    return res;

    /* //original resolution from https://github.com/andrefmrocha
	list<string> res;
	for(auto i: this->films) {
		for(auto j: i.film->getActors()) {
			if(j == actorName) {
				res.push_back(i.film->getTitle());
				break;
			}
		}
	}
	return res;
     */
}

//b2
void CinemaFinder::addActor(string filmTitle, string actorName) {
    FilmPtr newFilm;
    newFilm.film = new Film(filmTitle);
    for (auto film : films) {
        if (film->getTitle() == filmTitle) {
            newFilm = film;
            films.erase(film);
            break;
        }
    }
    newFilm.film->addActor(actorName);
    filmes.insert(newFilm);

    /* //original resolution from https://github.com/andrefmrocha
	for(auto i: this->films) {
		if(i.film->getTitle() == filmTitle)	{
			FilmPtr film = i;
			this->films.erase(i);
			film.film->addActor(actorName);
			this->films.insert(film);
			return;
		}
	}
	FilmPtr film;
	film.film = new Film(filmTitle);
	film.film->addActor(actorName);
	this->films.insert(film);
     */
}

//c1
string CinemaFinder::nearestCinema(string service1) const {
    priority_queue <Cinema> aux = cinemas;
    while (!aux.empty()) {
        for (auto s = aux.top().getServices().begin(); s != aux.top().getServices().end(); s++) {
            if ((*s) == service1) {
                return aux.top().getName();
            }
        }
        aux.pop();
    }
    return "";

    /* //original resolution from https://github.com/andrefmrocha
	priority_queue<Cinema> copy = this->cinemas;
	while(!copy.empty()) {
		for(auto i: copy.top().getServices()) {
			if(i == service1)
				return copy.top().getName();
		}
		copy.pop();
	}
	return "";
     */
}

//c2
void CinemaFinder::addServiceToNearestCinema(string service1, unsigned maxDistance) {
    if (cinemas.empty()) {
        throw CinemaNotFound();
    }
    Cinema nearest = cinemas.top();
    if (nearest.getDistance() > maxDistance) {
        throw CinemaNotFound();
    }
    nearest.addService(service1);
    cinemas.pop();
    cinemas.push(nearest);

    /* //original resolution from https://github.com/andrefmrocha
	if(this->cinemas.empty())
		throw(CinemaNotFound());
	Cinema nearest = this->cinemas.top();
	if(nearest.getDistance() > maxDistance)
		throw(CinemaNotFound());
	nearest.addService(service1);
	this->cinemas.pop();
	this->cinemas.push(nearest);
     */
}
