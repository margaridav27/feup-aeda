#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include "Pitch.h"

/**
 * @class User_interface
 */
class User_interface {
private:
    Pitch *pitch;
    Date today;
    int floor = 0;
public:
    /**
     * Sets private parameter today to today's date
     */
    void getTodayDate();
    /**
     * @param nif NIF that is going to be verified
     * @return true if client does not exist and if it fulfills all NIF parameters, false otherwise
     */
    bool verifyNifClient(unsigned int nif);
    /**
     * @param nif NIF that is going to be verified
     * @return true if employee does not exist and if it fulfills all NIF parameters, false otherwise
     */
    bool verifyNifEmployee(unsigned int nif);
    /**
     * Function created to deal with input errors, either input fail or wrong input, loops until right input
     * @tparam T template class
     * @param in input variable that is going to be changed to valid value
     * @param possibilities vector with every possibilities that are a valid input
     * @return true if valid input, else otherwise (but the last case never happens)
     */
    template<class T>
    bool input(T &in, vector<T> possibilities);
    /**
     *Initializes the parameter *pitch with the parameter provided
     * @param pitch is an object of class Pitch
     */
    User_interface(Pitch &pitch);
    /**
     * Function created to represent waiting lift time from one floor to another (generates delays in between outputs)
     * @param time travel time of one floor
     * @param from floor number from which the lift goes
     * @param to floor number which the lift goes to
     */
    static void delay(int time,int from, int to);
    /**
     * Starts the interaction with the user
     * @return 0 once finished
     */
    int startInteraction();
    /**
     * Interaction with the user, focused in the lift company "office functions" e.g.: see data
     */
    void office();
    /**
     * Function where the user chooses which building he wants to "enter"
     */
    void enterBuilding();
    /**
     * Function that will deal with a specific Particular Client's data
     * @param client represents the client that the user chose to "be"
     */
    void client(ParticularClient *client);
    /**
     * Function that will deal with a specific Entrepreneur's data
     * @param client represents the client that the user chose to "be"
     */
    void client(Entrepreneur *client);
    /**
     * Function that recreates the act of being in a lift
     * @param lift is the lift the user chose to enter
     */
    void building(Lift &lift);
    /**
     * Function that will receive which type of data the user wants to access (lifts/clients/employee)
     */
    void data();
    /**
     * Function that shows employee's data
     */
    void employeesData();
    /**
     * Function that shows client's data
     */
    void clientsData();
    /**
     * Function that shows lift's data
     */
    void liftsData();
};

#endif
