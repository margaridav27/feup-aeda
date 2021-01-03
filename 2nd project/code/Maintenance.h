#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include "Utils.h"

#define MAINTDURATION_STR "1:30:0"
#define MAINTDURATION 1.5

/**
 * Class Maintenance
 */
class Maintenance {
private:
    unsigned int liftCode;
    unsigned int spNIF;
    Date appointment;
    Time schedule;
    static Time duration;
    bool finished;
public:
    /**
     * Constructor for class Maintenance
     */
    Maintenance();

    /**
     * Constructor for class Maintenance
     * @param code of the correspondent lift
     * @param appointment date of the maintenance
     * @param schedule hours of the maintenance to be scheduled
     * @param spnif nif of the service provider allocated to the maintenance
     */
    Maintenance(unsigned int code, Date appointment, Time schedule, unsigned int spnif);

    /**
     * Constructor for class Maintenance
     * @param code of the correspondent lift
     * @param appointment date of the maintenance
     * @param schedule hours of the maintenance to be scheduled
     * @param spnif nif of the service provider allocated to the maintenance
     * @param finished true if it was already done and false if not
     */
    Maintenance(unsigned int code, Date appointment, Time schedule, unsigned int spnif, bool finished);

    /**
     * Sends to the output file stream given as parameter information about the maintenance respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    void printToMaintsFile(ostream &op) const;

    /**
     * Sends to the output file stream, with information about the lifts, given as parameter information about that lift maintenance respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    void printToLiftsFile(ostream &op) const;

    /**
     * Sends to the output file stream, with information about the employees, given as parameter information about that employee maintenance respecting a certain structure
     * @param op output file stream to which the information will be sent
     */
    void printToEmployeesFile(ostream &op) const;

    /**
     * @return lift code
     */
    unsigned int getLiftCode() const;

    /**
     * @return NIF of the employee
     */
    unsigned int getSpNIF() const;

    /**
     * @return maintenance's schedule date
     */
    Date getMaintenanceDate() const;

    /**
     *
     * @return maintenance's scheduted time
     */
    Time getMaintenanceSchedule() const;

    /**
     *
     * @return maintenance duration
     */
    Time getMaintenanceDuration() const;

    /**
     *
     * @return if the maintenance is finished or not
     */
    bool getFinished() const;

    /**
     *Sets maintenance as finished or not, according to value
     * @param value
     */
    void setFinished(bool value);

    ostream &operator<<(ostream &out);

    bool operator<(const Maintenance &m) const;

    bool operator==(const Maintenance &m) const;
};

/**
 * Class MaintenanceRecord
 */
class MaintenanceRecord {
    Maintenance *maintenance;
public:
    /**
     * Constructor for class MaintenanceRecord
     */
    MaintenanceRecord();

    /**
     * Constructor for class MaintenanceRecord
     * @param maintenance
     */
    MaintenanceRecord(Maintenance *maintenance);

    /**
     * Constructor for class MaintenanceRecord
     * @param code
     * @param appointment
     * @param schedule
     * @param spnif
     */
    MaintenanceRecord(unsigned int code, Date appointment, Time schedule, unsigned int spnif);

    /**
     * Constructor for class MaintenanceRecord
     * @param code
     * @param appointment
     * @param schedule
     * @param spnif
     * @param finished
     */
    MaintenanceRecord(unsigned int code, Date appointment, Time schedule, unsigned int spnif, bool finished);

    /**
     * @return pointer to its maintenance
     */
    Maintenance *getMaintenance() const;

    bool operator<(const MaintenanceRecord &m) const;

    bool operator==(const MaintenanceRecord &m) const;
};


#endif