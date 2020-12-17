/*
 * ReadingClub.cpp
 *
 *  Created on: 11/12/2016
 *      Author: RRossetti
 */

#include "ReadingClub.h"

ReadingClub::ReadingClub() : catalogItems(BookCatalogItem("", "", 0)) {
    //do nothing!
}

ReadingClub::ReadingClub(vector<Book *> books) : catalogItems(BookCatalogItem("", "", 0)) {
    this->books = books;
}

void ReadingClub::addBook(Book *book) {
    this->books.push_back(book);
}

void ReadingClub::addBooks(vector<Book *> books) {
    this->books = books;
}

vector<Book *> ReadingClub::getBooks() const {
    return this->books;
}

BookCatalogItem ReadingClub::getCatalogItem(string title, string author) {
    BookCatalogItem itemNotFound("", "", 0);
    BSTItrIn<BookCatalogItem> it(catalogItems);
    while (!it.isAtEnd()) {
        if (it.retrieve().getTitle() == title && it.retrieve().getAuthor() == author) {
            BookCatalogItem bci(it.retrieve().getTitle(), it.retrieve().getAuthor(), 0);
            bci.setItems(it.retrieve().getItems());
            return bci;
        }
        it.advance();
    }
    return itemNotFound;
}

void ReadingClub::addCatalogItem(Book *book) {
    BookCatalogItem itemNotFound("", "", 0);
    BookCatalogItem bci(book->getTitle(), book->getAuthor(), book->getYear());
    BookCatalogItem bciX = catalogItems.find(bci);
    if (bciX == itemNotFound) {
        bci.addItems(book);
        this->catalogItems.insert(bci);
    } else {
        this->catalogItems.remove(bciX);
        bciX.addItems(book);
        this->catalogItems.insert(bciX);
    }
    books.push_back(book);
}

BST<BookCatalogItem> ReadingClub::getCatalogItems() const {
    return this->catalogItems;
}

vector <UserRecord> ReadingClub::getUserRecords() const {
    vector <UserRecord> records;
    HashTabUserRecord::const_iterator it1 = this->userRecords.begin();
    HashTabUserRecord::const_iterator it2 = this->userRecords.end();
    for (; it1 != it2; it1++) {
        records.push_back(*it1);
    }
    return records;
}

void ReadingClub::setUserRecords(vector <UserRecord> &urs) {
    for (int i = 0; i < urs.size(); i++) userRecords.insert(urs[i]);
}

priority_queue <User> ReadingClub::getBestReaderCandidates() const {
    return readerCandidates;
}

void ReadingClub::setBestReaderCandidates(priority_queue <User> &candidates) {
    readerCandidates = candidates;
}

//
// TODO: Part I   - BST
//

void ReadingClub::generateCatalog() {
    for (auto b = books.begin(); b != books.end(); b++) {
        BSTItrIn<BookCatalogItem> itr(catalogItems);
        BookCatalogItem newCatalog((*b)->getTitle(), (*b)->getAuthor(), (*b)->getYear());

        while (!itr.isAtEnd()) {
            if (itr.retrieve().getTitle() == (*b)->getTitle() && itr.retrieve().getAuthor() == (*b)->getAuthor()) {
                newCatalog = itr.retrieve();
                catalogItems.remove(itr.retrieve());
                break;
            }
        }
        newCatalog.addItems((*b));
        catalogItems.insert(newCatalog);
    }

    /* //original resolution from https://github.com/andrefmrocha
	BookCatalogItem null("", "", 0);
	for(auto i: this->books)
	{
		BookCatalogItem newCat(i->getTitle(), i->getAuthor(), i->getYear());
		BookCatalogItem find = this->catalogItems.find(newCat);
		if(find == null)
		{
			newCat.addItems(i);
			this->catalogItems.insert(newCat);
		}
		else
		{
			find.addItems(i);
			this->catalogItems.remove(newCat);
			this->catalogItems.insert(find);
		}
	}
     */
}

vector<Book *> ReadingClub::getAvailableItems(Book *book) const {
    BSTItrIn<BookCatalogItem> itr(catalogItems);
    vector < Book * > available;
    while (!itr.isAtEnd()) {
        if (itr.retrieve().getTitle() == book->getTitle() && itr.retrieve().getAuthor() == book->getAuthor()) {
            for (auto b = itr.retrieve().getItems().begin(); b != itr.retrieve().getItems().end(); b++) {
                if ((*b)->getReader() == NULL) {
                    available.push_back((*b));
                }
            }
            break;
        }
    }
    return available;

    /* //original resolution from https://github.com/andrefmrocha
	vector<Book*> temp;
	BSTItrIn<BookCatalogItem> it(this->catalogItems);
	while(!it.isAtEnd()) {
		if(it.retrieve().getAuthor() == book->getAuthor() && it.retrieve().getTitle() == book->getTitle()) {
			for(auto i: it.retrieve().getItems()) {
				if(i->getReader() == NULL) {
					temp.push_back(i);
				}
			}
		}
		it.advance();
	}
	return temp;
     */
}

bool ReadingClub::borrowBookFromCatalog(Book *book, User *reader) {
    BSTItrIn<BookCatalogItem> itr(catalogItems);
    vector < Book * > available;
    while (!itr.isAtEnd()) {
        if (itr.retrieve().getTitle() == book->getTitle() && itr.retrieve().getAuthor() == book->getAuthor()) {
            for (auto b = itr.retrieve().getItems().begin(); b != itr.retrieve().getItems().end(); b++) {
                if ((*b)->getReader() == NULL) {
                    (*b)->setReader(reader);
                    reader->addReading(book->getTitle(), book->getAuthor());
                    return true;
                }
            }
        }
    }
    return false;

    /* //original resolution from https://github.com/andrefmrocha
	vector<Book *> books = this->getAvailableItems(book);
	if(books.size() == 0)
		return false;
	reader->addReading(book->getTitle(), book->getAuthor());
	books[0]->setReader(reader);
	return true;
     */
}

//
// TODO: Part II  - Hash Table
//

void ReadingClub::addUserRecord(User *user) {
    this->userRecords.insert(user);
}

void ReadingClub::changeUserEMail(User *user, string newEMail) {
    userRecords.erase(user);
    user->setEMail(newEMail);
    userRecords.insert(user);

    /* //original resolution from https://github.com/andrefmrocha
	for(auto i: this->userRecords) {
		if(i.getEMail() == user->getEMail()) {
			this->userRecords.erase(i);
			user->setEMail(newEMail);
			this->addUserRecord(user);
			return;
		}
	}
     */
}

//
// TODO: Part III - Priority Queue
//

void ReadingClub::addBestReaderCandidates(const vector <User> &candidates, int min) {
    for (auto it = candidates.begin(); it != candidates.end(); it++) {
        if (it->getReadings().size() > min) {
            readerCandidates.push(it);
        }
    }

    /* //original resolution from https://github.com/andrefmrocha
	for(auto i: candidates)	{
		if(i.getReadings().size() + 1>= min)
			this->readerCandidates.push(i);
	}
     */
}

int ReadingClub::awardReaderChampion(User &champion) {
    if (readerCandidates.empty()) {
        return 0;
    }
    priority_queue <User> aux = readerCandidates;
    User winner = aux.top();
    aux.pop();
    if (aux.top().getReadings().size() == winner.getReadings().size()) {
        return 0;
    }
    champion = winner;
    return aux.size();

    /* //original resolution from https://github.com/andrefmrocha
	if(this->readerCandidates.size() == 0)
		return 0;
	priority_queue<User> copy = this->readerCandidates;
	User champ = copy.top();
	copy.pop();
	if(champ.getReadings().size() == copy.top().getReadings().size())
		return 0;
	champion = champ;
	return copy.size() + 1;
     */
}
























