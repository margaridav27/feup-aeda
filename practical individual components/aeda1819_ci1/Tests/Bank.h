/*
 * Bank.h
 */

#ifndef SRC_BANK_H_
#define SRC_BANK_H_

#include "Account.h"
#include "BankOfficer.h"
#include <vector>

class Bank {
	vector<Account *> accounts;
	vector<BankOfficer> bankOfficers;
public:
	Bank();
	void addAccount(Account *a);
	void addBankOfficer(BankOfficer b);
	vector<BankOfficer> getBankOfficers() const;
	vector<Account *> getAccounts() const;
	double getWithdraw(string cod1) const;
	vector<Account *> removeBankOfficer(string name);
	const BankOfficer& addAccountToBankOfficer(Account *ac, string name);
	void sortAccounts();
};

template <class T>
unsigned int numberDifferent (const vector<T> & v1) {
    unsigned int counter = 0;
    bool repeated;
    for (typename vector<T>::const_iterator i = v1.begin(); i != v1.end(); i++) {
        repeated = false;
        for (typename vector<T>::const_iterator j = v1.begin(); j != i; j++) {
            if ((*i) == (*j)) {
                repeated = true;
            }
        }
        if (!repeated) {
            counter++;
        }
    }
    return counter;
}

class NoBankOfficerException {
        private:
            string name;
        public:
            NoBankOfficerException(string name);
            string getName();
        };

#endif /* SRC_BANK_H_ */
