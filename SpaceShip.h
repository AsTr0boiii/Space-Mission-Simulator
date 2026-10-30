#pragma once

#include <string>
#include "MissionTypes.h"
#include "PropulsionSystem.h"
#include "PowerSystem.h"
#include "CommunicationSystem.h"
#include "SensorSystem.h"

class SpaceShip {
    private:
    double altitude;
    double velocity;
    double distanceFromEarth;
    double distanceToDestination;
    double spaceshipMass;

    MissionPhase missionPhase;
    std::string missionStatus;

    PropulsionSystem propulsion;
    PowerSystem power;
    CommunicationSystem communication;
    SensorSystem sensors;

    public:
    SpaceShip();

    void startMission();
    void update(double deltaTime);

    void updateMass();
    void updateMovement(double deltaTime);
    void updateMissionPhase();
    void checkMissionStatus();

    void increaseThrust();
    void decreaseThrust();

    MissionPhase getMissionPhase() const;
    std::string getMissionPhaseText() const;
    std::string getMissionStatus() const;

    double getAltitude() const;
    double getVelocity() const;
    double getDistanceFromEarth() const;
    double getDistanceToDestination() const;
    double getSpaceshipMass() const;

    PropulsionSystem& getPropulsionSystem();
    PowerSystem& getPowerSystem();
    CommunicationSystem& getCommunicationSystem();
    SensorSystem& getSensorSystem();

    const PropulsionSystem& getPropulsionSystem() const;
    const PowerSystem& getPowerSystem() const;
    const CommunicationSystem& getCommunicationSystem() const;
    const SensorSystem& getSensorSystem() const;
};