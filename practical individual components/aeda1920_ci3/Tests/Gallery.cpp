#include "Gallery.h"
#include "Paint.h"
#include <ostream>
#include <algorithm>
using namespace std;

Gallery::Gallery(vector<Paint*> c): catalog(PaintCatalogItem("", "", 0, 0.00)), collection(c) {

}

vector<Paint*> Gallery::getCollection() const {
    return collection;
}

void Gallery::setCollection(vector<Paint*> c) {
    collection = c;
}

priority_queue<ExhibitionItem> Gallery::getPaintsToShow() const {
    return paintsToShow;
}

HashTableAuthorRecord Gallery::getAuthorRecords () const {
    return  authorRecords;
}

void Gallery::addAuthorRecord(AuthorRecord ar) {
    authorRecords.insert(ar);
}

void Gallery::generateCatalog() {
    catalog.makeEmpty();
    for(int i = 0; i < collection.size(); i++) {
        catalog.insert(collection[i]);
    }
}

BST<PaintCatalogItem> Gallery::getCatalog() const {
    return catalog;
}

void Gallery::prepareExhibition() {
    while( !paintsToShow.empty())
        paintsToShow.pop();
    for(int i = 0; i < collection.size(); i++) {
        ExhibitionItem ei(collection[i]);
        paintsToShow.push(ei);
    }
}

//-------------------------------------------------------------------------------

//TODO
vector<Paint*> Gallery::getPaintsBy(string a) const {
    vector<Paint*> res;
    BSTItrIn<PaintCatalogItem> itr(catalog);
    while (!itr.isAtEnd()) {
        if (itr.retrieve().getAuthor() == a) {
            res.push_back(itr.retrieve().getPaint());
        }
        itr.advance();
    }
    return res;
}

//TODO
vector<Paint*> Gallery::getPaintsBetween(int y1, int y2) const {
    vector<Paint*> res;
    BSTItrIn<PaintCatalogItem> itr(catalog);
    while (!itr.isAtEnd()) {
        if (itr.retrieve().getYear() >= y1 && itr.retrieve().getYear() <= y2) {
            res.push_back(itr.retrieve().getPaint());
        }
        itr.advance();
    }
    return res;
}

//TODO
bool Gallery::updateTitle(Paint* p, string tnew) {
    BSTItrIn<PaintCatalogItem> itr(catalog);
    while (!itr.isAtEnd()) {
        if (itr.retrieve() == p) {
            if (!catalog.remove(itr.retrieve())) {
                return false;
            }
            PaintCatalogItem updated(p->getAuthor(), tnew, p->getYear(), p->getPrice());
            catalog.insert(updated);
            return true;
        }
        itr.advance();
    }
    return false;

    /*
    if(!catalog.remove(PaintCatalogItem(p))) {
        return false;
    }
    PaintCatalogItem updated(p->getAuthor(), tnew, p->getYear(), p->getPrice());
    catalog.insert(updated);
    return true;
     */

}

//TODO
int Gallery::recordAvailablePainters() {
    for (auto p = collection.begin(); p != collection.end(); p++) {
        bool found = false;
        for (auto ar = authorRecords.begin(); ar != authorRecords.end(); ar++) {
            if (ar->getAuthor() == (*p)->getAuthor()) {
                AuthorRecord newRecord(ar->getAuthor(), ar->getAvailablePaints() + 1, ar->getTotalSells());
                authorRecords.erase(ar);
                authorRecords.insert(newRecord);
                found = true;
                break;
            }
        }
        if (!found) {
            AuthorRecord record((*p)->getAuthor(), 1, 0);
            authorRecords.insert(record);
        }
    }
    return authorRecords.size();
}

//TODO
double Gallery::sellPaint(string a, string t) {
    double transaction = 0;
    for (auto ar = authorRecords.begin(); ar != authorRecords.end(); ar++) {
        if (ar->getAuthor() == a) {
            for (auto p = collection.begin(); p != collection.end(); p++) {
                if ((*p)->getAuthor() == a && (*p)->getTitle() == t) {
                    transaction = (*p)->getPrice(); //saves the transaction value in order to update the record
                    collection.erase(p); //updates collection by removing the sold paint
                    break;
                }
            }
            AuthorRecord author(ar->getAuthor(), ar->getAvailablePaints() - 1, ar->getTotalSells() + transaction);
            authorRecords.erase(ar); //removes old record from authorRecords
            authorRecords.insert(author); //inserts updated record in the authorRecords
            break;
        }
    }
    return transaction;
}

//TODO
double Gallery::totalSells() const {
    double totalAmount = 0;
    for (auto ar = authorRecords.begin(); ar != authorRecords.end(); ar++) {
        totalAmount += ar->getTotalSells();
    }
    return totalAmount;
}

//TODO
int Gallery::itemExhibitionOrder(string a, string t) {
    priority_queue<ExhibitionItem> temp = paintsToShow;
    int order = 1;
    bool found = false;
    while (!temp.empty()) {
        if (temp.top().getAuthor() == a && temp.top().getTitle() == t) {
            found = true;
            break;
        }
        temp.pop();
        order++;
    }
    if (found) return order;
    else return 0;
}

//TODO
vector<Paint*> Gallery::nBestExhibition(int n, int maxPerYear) {
    vector<Paint*> exhibition;
    priority_queue<ExhibitionItem> aux; //keeps track of the paints that are not added to the exhibition
    int countYear = 0, currYear = paintsToShow.top().getYear();
    while (true) {
        if (paintsToShow.empty()) { //there are no more paints that could potentially be added to the exhibition
            break;
        }
        else if (exhibition.size() == n && !paintsToShow.empty()) { //the paints left in the queue need to be transferred to the aux queue otherwise will get lost
            while (!paintsToShow.empty()) {
                aux.push(paintsToShow.top());
                paintsToShow.pop();
            }
            break;
        }
        if (paintsToShow.top().getYear() == currYear) {
            if (countYear == maxPerYear) {
                aux.push(paintsToShow.top());
            }
            else {
                exhibition.push_back(paintsToShow.top().getPaint());
                countYear++;
            }
            paintsToShow.pop();
        }
        else {
            countYear = 0;
            currYear = paintsToShow.top().getYear();
        }
    }
    paintsToShow = aux;
    return exhibition;
}



