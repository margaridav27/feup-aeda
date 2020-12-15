/*
 * Bank.cpp
 */

#include "Bank.h"
#include <algorithm>
#include <string>

Bank::Bank() {}
void Bank::addAccount(Account *a) {
	accounts.push_back(a);
}
void Bank::addBankOfficer(BankOfficer b){
	bankOfficers.push_back(b);
}
const BankOfficer & Bank::addAccountToBankOfficer(Account *ac, string name) {
    for (vector<BankOfficer>::iterator it = bankOfficers.begin(); it != bankOfficers.end(); it++) {
        if ((*it).getName() == name) {
            (*it).addAccount(ac);
            return (*it);
        }
    }
    throw NoBankOfficerException(name);
}
vector<BankOfficer> Bank::getBankOfficers() const {
	return bankOfficers;
}
vector<Account *> Bank::getAccounts() const {
	return accounts;
}
double Bank::getWithdraw(string cod1) const{
    double total = 0;
	for (vector<Account*>::const_iterator it = accounts.begin(); it != accounts.end(); it++) {
	    if ((*it)->getCodH() == cod1) {
	        total += (*it)->getWithdraw();
	    }
	}
	return total;
}
vector<Account *> Bank::removeBankOfficer(string name){
	vector<Account *> res;
    for (vector<BankOfficer>::const_iterator it = bankOfficers.begin(); it != bankOfficers.end(); it++) {
        if ((*it).getName() == name) {
            res = (*it).getAccounts();
            bankOfficers.erase(it);
            break;
        }
    }
    return res;
}
bool compAcc(Account* c1, Account* c2) {
    if (c1->getWithdraw() < c2->getWithdraw()) return true;
    else if (c1->getWithdraw() == c2->getWithdraw() && c1->getCodIBAN() < c2->getCodIBAN()) return true;
    else return false;
}
void Bank::sortAccounts() {
    sort(accounts.begin(), accounts.end(), compAcc);
}


NoBankOfficerException::NoBankOfficerException(string name) {
    this->name =  name;
}
string NoBankOfficerException::getName() {
    return name;
}



