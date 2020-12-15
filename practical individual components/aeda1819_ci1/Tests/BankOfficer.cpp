/*
 * BankOfficer.cpp
 */
#include "BankOfficer.h"

unsigned int BankOfficer::numBankOfficers = 0;

BankOfficer::BankOfficer(): id(0) {}
BankOfficer::BankOfficer(string name) {
    this->name = name;
    numBankOfficers++;
    id = numBankOfficers;
    myAccounts.clear();
}
void BankOfficer::setName(string nm){
	name = nm;
}
string BankOfficer::getName() const{
	return name;
}
unsigned int BankOfficer::getID() const{
    return id;
}
vector<Account *> BankOfficer::getAccounts() const {
	return myAccounts;
}
void BankOfficer::addAccount(Account *a) {
	myAccounts.push_back(a);
}
bool BankOfficer::operator>(const BankOfficer& b) {
    if (myAccounts.size() > b.getAccounts().size()) return true;
    else if (myAccounts.size() == b.getAccounts().size() && name > b.getName()) return true;
    else return false;
}



