#ifndef LIFT_H
#define LIFT_H
#include "Utils.h"
#include <vector>

using namespace std;

/**
 * @struct Moves
 */
struct Moves {
    Date date;
    int from;
    int to;
    int floorCount;
    Time tInitial;
    Time tFinal;
};

void printMove(ofstream& op, const Moves &move); /** function that prints with the correct format a move to the file */
ostream& operator<<(ostream& out, Moves move);

/**
 * @struct Maintenance
 */
struct Maintenance{
    Date date;
    unsigned int nifEmployee;
};

void printMaint(ofstream& op, Maintenance &maint); /** function that prints with the correct format a maintenance to the file */
ostream& operator<<(ostream& out, Maintenance maint);

/**
 * @class Lift
 */
class Lift {
private:
    unsigned int code;
    string buildingName;
    string address;
    Date install;
    unsigned int capacity;
    unsigned int clientNif;
    unsigned int nFloors;
    unsigned int price;
    int regularTime;
    int currentFloor = 0;
    int higherFloor;
    int lowerFloor;
    int previousDoorOpens = 0;
    vector<Maintenance> maintHistory;
    vector<Moves> movesHistory;
public:
    static unsigned int nLifts; /** static variable since the lift code equals the number of lifts plus 100 */
    int doorOpens; /** number of door openings saved for maintenance control purposes, set again to 0 once a maintenance occurs */

    Lift();
    /**
     * Constructor for class Lift
     * @param buildingName name of the building
     * @param address address of the building
     * @param capacity capacity, in Kg, of the lift
     * @param clientNif NIF of the client that owns the lift
     * @param nFloors number of the building floors
     * @param d installation date of the lift
     * @throw InvalidNumFloors
     */
    Lift(string buildingName, string address, unsigned int capacity, unsigned int clientNif, unsigned int nFloors, Date &d);
    ~Lift() = default;
    /**
     * @return string containing the relevant information about the lift
     */
    string show();
    /**
     * Sends to the output file stream given as parameter information about the lift respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    void printToFile(ofstream& op);
    /**
     * Determines whether or not the lift has had maintenance
     * @return true is it has had, false otherwise
     */
    bool hasHadMaintenance() const;
    /**
     * @return lift's code
     */
    unsigned int getCode() const;
    /**
     * @return NIF of the client that owns the lift
     */
    unsigned int getClientNif() const;
    /**
     * @return duration that the lift takes to go from a floor to the next
     */
    int getRegularTime() const;
    /**
     * @return name of the building
     */
    string getName() const;
    /**
     * @return address of the building
     */
    string getAddress() const;
    /**
     * @return installation date of the lift
     */
    Date getInstallDate() const;
    /**
     * @return price of the lift
     */
    unsigned int getPrice() const;
    /**
     * @return floor where the lift is at
     */
    int getCurrentFloor() const;
    /**
     * @return highest floor number of the building
     */
    int getHigherFloor() const;
    /**
     * @return lowest floor number of the building
     */
    int getLowerFloor() const;
    /**
     * @return number of door openings immediately before it receives a maintenance
     */
    int getPreviousDoorOpens() const;
    /**
     * @return last maintenance that the lift has had
     */
    Maintenance getLastMaintenance() const;
    /**
     * @return vector of the moves executed by the lift
     */
    vector<Moves> getMoves();
    /**
     * @return last move that the lift has executed
     */
    Moves getLastMove() const;
    /**
     * @param nif NIF of the client that owns the lift
     */
    void setOwner(unsigned int nif);
    /**
     * Sets the lift price based on his capacity and the building number of floors
     */
    void setPrice();
    /**
     * Randomly sets the duration that the lift takes to go from a floor to the next
     */
    void setRegularTime();
    /**
     * Sets the duration that the lift takes to go from a floor to the next to be equal to the one given as parameter
     * @param secs
     */
    void setRegularTime(int secs);
    /**
     * Sets the floor where the lift is at to equal to the one given as parameter
     * @param floor
     */
    void setCurrentFloor(int floor);
    /**
     * Sets highest floor number of the building to be equal to the one given as parameter
     * @param floor
     */
    void setHigherFloor(int floor);
    /**
     * Sets lowest floor number of the building to be equal to the one given as parameter
     * @param floor
     */
    void setLowerFloor(int floor);
    /**
     * Registers move
     * @param from
     * @param to
     * @param d date of when the move occurs
     * @param t time at when the move occurs
     */
    void readMove(int from, int to, Date d, Time t);
    /**
 * Executes maintenance
 * @param employeeNif NIF of the service provider who will execute the maintenance
 * @param today current date
 */
    void Maintenance(unsigned int employeeNif, Date today);
    /**
     * Registers maintenance
     * @param employeeNif
     * @param today
     */
    void registerMaintenance(int employeeNif, Date today);
    /**
     * Checks if the lift is in need of maintenance based on the last maintenance date and the number of door openings
     * @param today current date
     * @return 0, if it needs, 1 otherwise
     */
    int checkMaintenanceNeeded(Date today);
    /**
     * Shows in the screen the maintenance history information
     */
    void printMaintenanceHistory();
    /**
     * Shows on the screen the last executed maintenance information
     */
    void printLastMaintenance();
    bool operator==(Lift* comp) const;
};

/** output operator for objects of the class Lift */
ostream& operator<<(ostream& out, Lift* l);

#endif
