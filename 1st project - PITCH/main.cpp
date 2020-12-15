#include <iostream>
#include "User_interface.h"
#include <string>

int main() {
    srand(time(NULL));
    string clientsFile = "clients",liftsFile = "lifts",employeesFile = "employees";
    Pitch pitch(clientsFile,employeesFile,liftsFile);
    User_interface user(pitch);
    user.getTodayDate();
    user.startInteraction();
    return 0;
}
