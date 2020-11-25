#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "dictionary.h"
#include "bst.h"

using namespace std;

BST<WordMeaning> Dictionary::getWords() const {
    return words;
}

bool WordMeaning::operator < (const WordMeaning &wm1) const {
    return word < wm1.word;
}

void Dictionary::readDictionary(ifstream &f) {
    string w, m;
     while (!f.eof()) {
         getline(f, w);
         getline(f, m);
         WordMeaning newWM(w,m);
         words.insert(newWM);
     }
}

string Dictionary::searchFor(string word) const {
    BSTItrIn<WordMeaning> it(words);
    string wb = "", mb = "", wa = "", ma = "";
    while(!it.isAtEnd()) {
        if (it.retrieve().getWord() == word) { //word was found
            return it.retrieve().getMeaning();
        }
        else if (it.retrieve().getWord() < word) { //word still might be found
            wb = it.retrieve().getWord();
            mb = it.retrieve().getMeaning();
        }
        else if (it.retrieve().getWord() > word) { //we know for sure that word does not exist
            wa = it.retrieve().getWord();
            ma = it.retrieve().getMeaning();
            break;
        }
        it.advance();
    }
    throw WordInexistent(wb, mb, wa, ma);
}

bool Dictionary::correct(string word, string newMeaning) {
    BSTItrIn<WordMeaning> it(words);
    while (!it.isAtEnd()) {
        if (it.retrieve().getWord() == word) {
            words.remove(it.retrieve()); //removes the incorrect one
            WordMeaning corrected(word, newMeaning);
            words.insert(corrected); //inserts the corrected one
            return true;
        }
        it.advance();
    }
    WordMeaning newWM(word, newMeaning); //otherwise just inserts a new one
    words.insert(newWM);
    return false;
}

void Dictionary::print() const {
    BSTItrIn<WordMeaning> it(words);
    while (!it.isAtEnd()) {
        cout << it.retrieve().getWord() << endl << it.retrieve().getMeaning() << endl;
        it.advance();
    }
}

