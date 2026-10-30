#pragma once

#include "DatabaseManager.h"
#include "SpaceShip.h"

class MissionControl {
    private:
    SpaceShip spaceship;

    DatabaseManager database;

    bool missionRunning;
    bool paused;

    double missionTime;

    MissionPhase previousPhase;

    void saveDatabaseSnapshot();
    void checkPhaseEvent();

    public:

    MissionControl(const std::string& connectionString);

    void startMission();
    void runSimulation();
    void update();

    void displayDashboard() const;
    void clearScreen() const;

    bool isMissionFinished() const;
    
SensorStatus previousTemperatureStatus;
SensorStatus previousFuelStatus;
SensorStatus previousBatteryStatus;
SensorStatus previousCommunicationStatus;

void checkSensorEvents();

};