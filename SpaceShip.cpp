#include "SpaceShip.h"
#include "Physics.h"
#include "Constants.h"

SpaceShip::SpaceShip() {
    altitude = 0.0;
    velocity = 0.0;
    distanceFromEarth = 0.001;
    distanceToDestination = MOON_DISTANCE_KM;
    spaceshipMass = SPACESHIP_DRY_MASS + MAX_FUEL_MASS;

    missionPhase = MissionPhase::PRE_LAUNCH;
    missionStatus = "Ready for Launch";

}

void SpaceShip::startMission() {
    if(missionPhase == MissionPhase::PRE_LAUNCH) {
        missionPhase = MissionPhase::LAUNCH;
        missionStatus = "Mission Started";
        propulsion.startEngine();
    }
}

void SpaceShip::update(double deltaTime) {
    if(missionPhase == MissionPhase::PRE_LAUNCH ||
       missionPhase == MissionPhase::MISSION_COMPLETE ||
       missionPhase == MissionPhase ::MISSION_FAILED) {
        communication.updateLink(distanceFromEarth, power.getBatteryLevel());
        return;
       }

       propulsion.updateFuel(deltaTime);
       propulsion.updateEngineTemperature(deltaTime);

       power.updateBattery(deltaTime);

       updateMass();
       updateMovement(deltaTime);

       communication.updateLink(distanceFromEarth, power.getBatteryLevel());

       sensors.updateSensors(
        propulsion.getEngineTemperature(),
        propulsion.getFuelLevel(),
        power.getBatteryLevel(),
        communication.getSNR(),
        altitude,
        velocity
       );

       updateMissionPhase();
       checkMissionStatus();

}

void SpaceShip::updateMass() {
    double fuelMass = MAX_FUEL_MASS * (propulsion.getFuelLevel() / 100.0);
    spaceshipMass = SPACESHIP_DRY_MASS + fuelMass;

}

void SpaceShip::updateMovement(double deltaTime) {
    double acceleration = propulsion.calculateAcceleration(spaceshipMass);

    if(missionPhase == MissionPhase::LAUNCH || missionPhase == MissionPhase::ASCENT) {
        acceleration -= GRAVITY;

        velocity = Physics::calculateVelocity(velocity, acceleration, deltaTime);
        altitude = Physics::calculateAltitude(altitude, velocity, deltaTime);

        if(altitude < 0) {
            altitude = 0;
            velocity = 0;
        }

        distanceFromEarth = altitude / 1000.0;
    }
    else if(missionPhase == MissionPhase::ORBIT) {
        altitude = ORBIT_ALTITUDE_M;
        velocity = ORBIT_VELOCITY_MPS;
        distanceFromEarth = altitude / 1000.0;

        propulsion.decreaseThrust(2.0);
    } 
    else if(missionPhase == MissionPhase::SPACE_TRAVEL) {
        altitude = ORBIT_ALTITUDE_M;

        double travelSpeedKmPerSec = TRAVEL_SPEED_KM_PER_SEC;
        distanceFromEarth += travelSpeedKmPerSec * deltaTime;

        if(distanceFromEarth > MOON_DISTANCE_KM) {
            distanceFromEarth = MOON_DISTANCE_KM;
        }

        distanceToDestination = MOON_DISTANCE_KM - distanceFromEarth;

        velocity = travelSpeedKmPerSec * 1000.0;
    }
    else if(missionPhase == MissionPhase::APPROCH) {
        double approachSpeedKmPerSec = APPROACH_SPEED_KM_PER_SEC;

        distanceFromEarth += approachSpeedKmPerSec * deltaTime;

        if(distanceFromEarth > MOON_DISTANCE_KM) {
            distanceFromEarth = MOON_DISTANCE_KM;
        }

        distanceToDestination = MOON_DISTANCE_KM - distanceFromEarth;
        velocity = approachSpeedKmPerSec * 1000.0;
        altitude = LANDING_START_ALTITUDE_M;

        }
        else if(missionPhase == MissionPhase::LANDING) {
            propulsion.startEngine();

            double descentSpeed = LANDING_DESCENT_SPEED;

            if(propulsion.getThrustLevel() > 50.0) {
                descentSpeed = LANDING_CONTROLLED_DESCENT_SPEED;
            }

            altitude -= descentSpeed * deltaTime;
            velocity = descentSpeed;

            if(altitude <= 0) {
                altitude = 0;
                velocity = 0;
            }

            distanceFromEarth = MOON_DISTANCE_KM;
            distanceToDestination = 0.0;
        }
}

void SpaceShip::updateMissionPhase() {

    if (missionPhase == MissionPhase::ASCENT &&
    altitude >= 100000.0 &&
    altitude < ORBIT_ALTITUDE_M) {

    missionStatus = "Ascending toward orbital insertion";
}

    if (missionPhase == MissionPhase::LAUNCH &&
        altitude > 1000.0) {

        missionPhase = MissionPhase::ASCENT;
        missionStatus = "Ascending through Atmosphere";
    }

    else if (missionPhase == MissionPhase::ASCENT &&
             altitude >= ORBIT_ALTITUDE_M &&
             velocity >= ORBIT_VELOCITY_REQUIREMENT) {

        missionPhase = MissionPhase::ORBIT;
        missionStatus = "Orbit Achieved";
    }

    else if (missionPhase == MissionPhase::ORBIT &&
             propulsion.getFuelLevel() > 50.0) {

        missionPhase = MissionPhase::SPACE_TRAVEL;
        missionStatus = "Travelling toward destination";
        propulsion.stopEngine();
    }

    else if (missionPhase == MissionPhase::SPACE_TRAVEL &&
             distanceToDestination <= APPROACH_DISTANCE_KM) {

        missionPhase = MissionPhase::APPROCH;
        missionStatus = "Approaching destination";
    }

    else if (missionPhase == MissionPhase::APPROCH &&
             distanceToDestination <= 1000.0) {

        missionPhase = MissionPhase::LANDING;
        missionStatus = "Landing sequence active";
        altitude = LANDING_START_ALTITUDE_M;
    }

    else if (missionPhase == MissionPhase::LANDING &&
             altitude <= 0) {

        missionPhase = MissionPhase::MISSION_COMPLETE;
        missionStatus = "Landing successful";
        propulsion.stopEngine();
    }
}

void SpaceShip::checkMissionStatus() {
    if(propulsion.getFuelLevel() <= 0 &&
       missionPhase != MissionPhase::MISSION_COMPLETE) {
        missionPhase = MissionPhase::MISSION_FAILED;
        missionStatus = "Mission failed: fuel depleted";
       }

       if(power.isBatteryCritical()) {
        missionPhase = MissionPhase::MISSION_FAILED;
        missionStatus = "Mission failed: bettery critical";
       }

       if(propulsion.getEngineTemperature() >= 900.0) {
        missionPhase = MissionPhase::MISSION_FAILED;
        missionStatus = "Mission failed: engine overheated";
       }
}

void SpaceShip::increaseThrust() {
    propulsion.increaseThrust(10.0);
}

void SpaceShip::decreaseThrust() {
    propulsion.decreaseThrust(10.0);
}

MissionPhase SpaceShip::getMissionPhase() const {
    return missionPhase;
}

std::string SpaceShip::getMissionPhaseText() const {
    switch(missionPhase) {
        case MissionPhase::PRE_LAUNCH:
        return "PRE_LAUNCH";
        case MissionPhase::LAUNCH:
        return "LAUNCH";
        case MissionPhase::ASCENT:
        return "ASCENT";
        case MissionPhase::ORBIT:
        return "ORBIT";
        case MissionPhase::SPACE_TRAVEL:
        return "SPACE TRAVEL";
        case MissionPhase::APPROCH:
        return "APPROACH";
        case MissionPhase::LANDING:
        return "LANDING";
        case MissionPhase::MISSION_COMPLETE:
        return "MISSION COMPLETE";
        case MissionPhase::MISSION_FAILED:
        return "MISSION FAILED";
        default:
        return "UNKNOWN";
    }
}

std::string SpaceShip::getMissionStatus() const {
    return missionStatus;
}

double SpaceShip::getAltitude() const {
    return altitude;
}

double SpaceShip::getVelocity() const {
    return velocity;
}

double SpaceShip::getDistanceFromEarth() const {
    return distanceFromEarth;
}

double SpaceShip::getDistanceToDestination() const {
    return distanceToDestination;
}

double SpaceShip::getSpaceshipMass() const {
    return spaceshipMass;
}

PropulsionSystem& SpaceShip::getPropulsionSystem() {
    return propulsion;
}

PowerSystem& SpaceShip::getPowerSystem() {
    return power;
}

CommunicationSystem& SpaceShip::getCommunicationSystem() {
    return communication;
}

SensorSystem& SpaceShip::getSensorSystem() {
    return sensors;
}

const PropulsionSystem& SpaceShip::getPropulsionSystem() const {
    return propulsion;
}

const PowerSystem& SpaceShip::getPowerSystem() const {
    return power;
}

const CommunicationSystem& SpaceShip::getCommunicationSystem() const {
    return communication;
}

const SensorSystem& SpaceShip::getSensorSystem() const {
    return sensors;
}