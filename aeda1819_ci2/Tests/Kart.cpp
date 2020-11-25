#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <queue>
#include <sstream>
#include <vector>
#include <time.h>
#include <string>
#include "Kart.h"

using namespace std;

//To help build better array
string itos(int i) {
    stringstream s;
    s << i;
    return s.str();
}

void CGrupo::criaGrupo() {
    float cilindradas[4]  = {134,250,450,600};
    bool avariados[3]  = {true,false,false};

    srand (1);   //Criar Pistas e Karts de Teste
    for(int i=1;i<=50;i++){
        vector<CKart> frota;
        for(int j=1;j<=50;j++){
            frota.push_back(CKart(avariados[rand()% 3],
                                  ("KART - "+itos(i))+itos(j),(i-1)*50+j,cilindradas[(rand() % 4)]));
        }
        adicionaPista(CPista("Pista - "+itos(i),frota));
    }
}

vector<CKart> CPista::getKartsAvariados() {
    vector<CKart> aux;
    for (vector<CKart>::iterator it = frotaKartsPista.begin() ; it != frotaKartsPista.end(); ++it) {
        if ((it)->getAvariado()) aux.push_back((*it));
    }
    return aux;
}

bool compKarts(CKart k1, CKart k2) {
    return (k1.getNumero() < k2.getNumero());
}

//Exercicio 1 a)     
vector<CKart> CGrupo::ordenaKarts() {
    vector<CKart> fleet;
	for (vector<CPista>::iterator it = pistasG.begin(); it != pistasG.end(); it++) {
	    sort((*it).getFrotaActual().begin(), (*it).getFrotaActual().end(), compKarts);
	    for (vector<CKart>::iterator k = (*it).getFrotaActual().begin(); k != (*it).getFrotaActual().end(); k++) {
	        fleet.push_back((*k));
	    }
	}
	return fleet;
}

//Exercicio 1 b)  
int CGrupo::numAvariados(int cilind) {
    vector<CKart> fleet = ordenaKarts();
    int n = 0;
    for (vector<CKart>::iterator it = fleet.begin(); it != fleet.end(); it++) {
        if ((*it).getAvariado() && (*it).getCilindrada() == cilind) {
            n++;
        }
    }
    return n;
}

//Exercicio 1 c)   
bool CPista::prepararCorrida(int numeroKarts, int cilind) {
    int added = 0;
    for (vector<CKart>::iterator it = frotaKartsPista.begin(); it != frotaKartsPista.end(); it++) {
        if (added < numeroKarts) {
            if (!(*it).getAvariado() && (*it).getCilindrada() == cilind) {
                kartsLinhaPartida.push((*it));
                added++;
            }
        }
    }
    if (added < numeroKarts) {
        return false;
    }
    else {
        return true;
    }
}

//Exercicio 1 d) 
int CPista::inicioCorrida() {
    while (!kartsLinhaPartida.empty()) {
        kartsEmProva.push_back(kartsLinhaPartida.front());
        kartsLinhaPartida.pop();
    }
    return kartsEmProva.size();
}

