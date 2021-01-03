#include "Maintenance.h"

/* -------------------- MAINTENANCE -------------------- */

Time Maintenance::duration = Time(MAINTDURATION_STR);

Maintenance::Maintenance() {}

Maintenance::Maintenance(unsigned int code, Date appointment, Time schedule, unsigned int spnif) {
    this->liftCode = code;
    this->appointment = appointment;
    this->schedule = schedule;
    this->spNIF = spnif;
    this->finished = false;
}

Maintenance::Maintenance(unsigned int code, Date appointment, Time schedule, unsigned int spnif, bool finished) {
    this->liftCode = code;
    this->appointment = appointment;
    this->schedule = schedule;
    this->spNIF = spnif;
    this->finished = finished;
}

void Maintenance::printToMaintsFile(ostream &op) const {
    op << liftCode << " " << spNIF << " " << appointment << " " << schedule;
}

void Maintenance::printToLiftsFile(ostream &op) const {
    op << appointment << " " << schedule << " " << spNIF;
}

void Maintenance::printToEmployeesFile(ostream &op) const {
    op << liftCode << " " << appointment << " " << schedule;
}

unsigned int Maintenance::getLiftCode() const {
    return liftCode;
}

unsigned int Maintenance::getSpNIF() const {
    return spNIF;
}

Date Maintenance::getMaintenanceDate() const {
    return appointment;
}

Time Maintenance::getMaintenanceSchedule() const {
    return schedule;
}

Time Maintenance::getMaintenanceDuration() const {
    return duration;
}

bool Maintenance::getFinished() const {
    return finished;
}

void Maintenance::setFinished(bool value) {
    finished = value;
}

bool Maintenance::operator<(const Maintenance &m) const {
    if (appointment == m.appointment) {
        if (schedule == m.schedule) {
            if (liftCode == m.liftCode) {
                return spNIF < m.spNIF;
            }
            return liftCode < m.liftCode;
        }
        return schedule < m.schedule;
    }
    return appointment < m.appointment;
}

ostream &Maintenance::operator<<(ostream &out) {
    out << "employee's NIF: " << spNIF << "     date: " << appointment << "     schedule: " << schedule << " -> ";
    if (finished) out << "already executed";
    else out << "still not executed";
    return out;
}

bool Maintenance::operator==(const Maintenance &m) const {
    return (liftCode == m.liftCode && spNIF == m.spNIF && schedule == m.schedule && appointment == m.appointment);
}


/* -------------------- MAINTENANCE RECORD -------------------- */

MaintenanceRecord::MaintenanceRecord() {}

MaintenanceRecord::MaintenanceRecord(Maintenance *maintenance) : maintenance(maintenance) {}

MaintenanceRecord::MaintenanceRecord(unsigned int code, Date appointment, Time schedule, unsigned int spnif) {
    Maintenance m(code, appointment, schedule, spnif);
    maintenance = &m;
}

MaintenanceRecord::MaintenanceRecord(unsigned int code, Date appointment, Time schedule, unsigned int spnif,
                                     bool finished) {
    Maintenance m(code, appointment, schedule, spnif, finished);
    maintenance = &m;
}

Maintenance *MaintenanceRecord::getMaintenance() const {
    return maintenance;
}

bool MaintenanceRecord::operator<(const MaintenanceRecord &m) const {
    if (maintenance->getMaintenanceDate() == m.maintenance->getMaintenanceDate()) {
        if (maintenance->getMaintenanceSchedule() == m.maintenance->getMaintenanceSchedule()) {
            if (maintenance->getLiftCode() == m.maintenance->getLiftCode()) {
                return maintenance->getSpNIF() < m.maintenance->getSpNIF();
            }
            return maintenance->getLiftCode() < m.maintenance->getLiftCode();
        }
        return maintenance->getMaintenanceSchedule() < m.maintenance->getMaintenanceSchedule();
    }
    return maintenance->getMaintenanceDate() < m.maintenance->getMaintenanceDate();
}

bool MaintenanceRecord::operator==(const MaintenanceRecord &m) const {
    return maintenance == m.maintenance;
}

