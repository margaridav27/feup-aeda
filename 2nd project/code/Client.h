#ifndef CLIENT_H
#define CLIENT_H

#include "Lift.h"
#include <sstream>

using namespace std;

/**
 * @class Client
 */
class Client {
protected:
    string name;
    unsigned int nif;
    Date firstLift;
    vector<Lift *> lifts;
public:
    Client() = default;

    Client(string name, unsigned int nif, string date);

    Client(string name, unsigned int nif, Date date);

    virtual ~Client() = default;

    /**
    * @return string containing the relevant information about the client
    */
    virtual string show();

    /**
     * Sends to the output file stream, given as parameter, information about the client respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    virtual void printToFile(ofstream &op);

    /**
     * @return name of the client
     */
    string getName() const;

    /**
     * @return NIF of the client
     */
    unsigned int getNif() const;

    /**
     * @return date of the day that this became a client
     */
    Date getDateFirstLift();

    /**
     * @return vector of the lifts of the client
     */
    vector<Lift *> getLifts();

    /**
     * @return true if the client has lifts operating, false otherwise
     */
    bool hasLifts() const;

    /**
     * Adds a lift to the vector of lifts bought by the client
     * @param lift pointer to the lift to be bought
     */
    void purchaseNewLift(Lift *lift);

    bool operator==(Client *comp);

    bool operator<(Client *comp);
};

/**
 * @class Entrepreneur
 * Has a NIF number that starts with a 5
 */
class Entrepreneur : public Client {
private:
    unsigned int employeeNif; /** NIF of the employee associated to check all the needs*/
public:
    Entrepreneur(string name, unsigned int nif, Date date);

    Entrepreneur(string name, unsigned int nif, string date, unsigned int employeeNif);

    ~Entrepreneur() = default;

    /**
    * @return string containing the relevant information about the Entrepreneur
    */
    string show();

    /**
     * Sends to the output file stream, given as parameter, information about the Entrepreneur respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    void printToFile(ofstream &op);

    /**
     * @return NIF of the employee associated to the entrepreneur
     */
    unsigned int getEmployeeNif() const;

    /**
     * Sets the NIF of the associated employee to be equal to the one given as parameter
     * @param NIF of the employee to be associated to the entrepreneur
     */
    void associateEmployee(unsigned int employeeNif);
};

/**
 * @class ParticularClient
 * Has a NIF number that starts with a 2
 */
class ParticularClient : public Client {
public:
    ParticularClient(string name, unsigned int nif, string date);

    ParticularClient(string name, unsigned int nif, Date date);

    ~ParticularClient() = default;

    /**
    * @return string containing the relevant information about the Particular Client
    */
    string show();

    /**
     * Sends to the output file stream, given as parameter, information about the particular client respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    void printToFile(ofstream &op);
};

/** output operator for objects of the class Client */
ostream &operator<<(ostream &out, Client *c);

/** functor to sort the employees by the number of lifts purchased and, when the same, by the date he became client */
bool sortClientsByNumLiftsPurchasedOrByAge(Client *c1, Client *c2);

#endif
