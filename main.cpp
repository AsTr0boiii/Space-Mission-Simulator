#include "MissionControl.h"
#include <iostream>

int main() {

    try {

        std::string connectionString =
            "host=localhost "
            "port=5432 "
            "dbname=space_mission_simulator "
            "user=postgres "
            "password=YOUR_PASSWORD";

        MissionControl missionControl(
            connectionString
        );

        missionControl.runSimulation();
    }

    catch (const std::exception& e) {

        std::cout
            << "Program error: "
            << e.what()
            << "\n";

        return 1;
    }

    return 0;
}