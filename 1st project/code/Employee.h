#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include "Client.h"

using namespace std;

/**
 * @class Employee
 */
class Employee {
protected:
    string name;
    unsigned int nif;
    unsigned int age;
    string function;
    unsigned int evaluation; /** can be 1, 2, 3, 4, or 5 */
    unsigned int numReviews; /** number of reviews given to the employee */
public:
    Employee(string name, unsigned int nif, unsigned int age);
    virtual ~Employee() = default;
    /**
     * @return string containing the relevant information about the employee
     */
    virtual string show();
    /**
     * Sends to the output file stream given as parameter information about the employee respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    virtual void printToFile(ofstream& op);
    /**
     * @return NIF of the employee
     */
    unsigned int getNif() const;
    /**
     * @return evaluation of the employee
     */
    unsigned int getEvaluation() const;
    /**
     * @return number of reviews given to the employee
     */
    unsigned int getNumOfReviews() const;
    /**
     * Sets the evaluation and the number of reviews of the employee to be equals to the ones given as parameters
     * @param eva evaluation
     * @param nR number of reviews
     */
    void setOldReviews(unsigned int eva, unsigned nR);
    /**
     * Reviews the employee by updating his evaluation and his number of reviews
     * @param punctuation
     */
    void review(unsigned int punctuation);
    virtual bool operator==(Employee* comp);
};

/**
 * @class ServiceProvider
 */
class ServiceProvider : public Employee {
private:
    double perHourSalary;
    double hoursWorked;
public:
    ServiceProvider(string name, unsigned int nif, unsigned int age, double perHourSalary);
    ~ServiceProvider() = default;
    /**
     * @return string containing the relevant information about the service provider
     */
    string show();
    /**
     * Sends to the output file stream given as parameter information about the service provider respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    void printToFile(ofstream& op);
    /**
     * @return total salary, given the per hour salary and the number of hours worked
     */
    double getSalary() const;
    /**
     * Sets the number of hours worked of the service provider to be equal to the one given as parameters
     * @param h number of hours
     */
    void setHoursWorked(double h);
    /**
     * Executes a maintenance request and updates the number of hours worked
     */
    void executeMaintenance();
    bool operator==(Employee* comp);
};

/**
 * @class ContractedEmployee
 */
class ContractedEmployee : public Employee {
private:
    double perMonthSalary;
    vector<Client*> clientsAssociated;
public:
    ContractedEmployee(string name, unsigned int nif, unsigned int age, double perMonthSalary);
    ~ContractedEmployee() = default;
    /**
     * @return string containing the relevant information about the contracted employee
     */
    string show();
    /**
     * Sends to the output file stream given as parameter information about the contracted employee respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    void printToFile(ofstream& op);
    /**
     * @return number of clients associated with the contracted employee
     */
    unsigned int getNumClientsAssociated() const;
    /**
     * @return vector containing all the clients associated to the contracted employee
     */
    vector<Client*> getClientsAssociated();
    /**
     * Associates the client given as parameter to the contracted employee
     * @param c pointer to the client to be associated to the contracted employee
     */
    void associateToClient(Client* c);
    /**
     * Removes the client given as parameter of the vector of clients associated
     * @param rm pointer to the client to be removed
     */
    void removeClient(Client* rm);
    bool operator==(Employee* comp);
};

/** output operator for objects of the class Employee */
ostream& operator<<(ostream& out, Employee* e);

/** functor to sort the employees by the number of reviews */
bool sortEmployeesByNumReviews(Employee* e1, Employee* e2);
/** functor to sort the employees by the evaluation */
bool sortEmployeesByEvaluation(Employee* e1, Employee* e2);
/** functor to sort the employees by the number of clients associated */
bool sortEmployeesByNumClientsAssociated(Employee* e1, Employee* e2);

#endif
