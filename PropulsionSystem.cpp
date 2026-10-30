#include "PropulsionSystem.h"
#include "Physics.h"
#include "Constants.h"

PropulsionSystem::PropulsionSystem() {
    thrustLevel = 0.0;
    fuelLevel = 100.0;
    fuelBurnRate = 0.02;
    engineTemperature = 25.0;
    engineActive = false;

}

void PropulsionSystem::startEngine() {
    if(fuelLevel > 0) {
        engineActive = true;

        if(thrustLevel == 0) {
            thrustLevel = 30.0;
        }
    }
}

void PropulsionSystem::stopEngine() {
    engineActive = false;
    thrustLevel = 0.0;
}

void PropulsionSystem::increaseThrust(double amount) {
    if(!engineActive || fuelLevel <= 0) {
        return;
    }

    thrustLevel = Physics::clamp(thrustLevel + amount, 0.0, 100.0); 

}

void PropulsionSystem::decreaseThrust(double amount) {
    thrustLevel = Physics::clamp(thrustLevel - amount, 0.0, 100.0); 

    if(thrustLevel == 0) {
        engineActive = false;
    }

}

double PropulsionSystem::calculateThrustForce() const {
    if(!engineActive || fuelLevel <= 0) {
        return 0.00;
    }
    return MAX_THRUST_FORCE * (thrustLevel / 100.0);

}

double PropulsionSystem::calculateAcceleration(double spaceshipMass) const {
    double thrustForce = calculateThrustForce();
    return Physics::calculateAcceleration(thrustForce, spaceshipMass);

}

void PropulsionSystem::updateFuel(double deltaTime) {
    if(!engineActive || thrustLevel <= 0) {
        return;
    }

    double actualBurnRate = fuelBurnRate * (thrustLevel /100.0);
    fuelLevel = Physics::calculateFuelConsumption(fuelLevel, actualBurnRate, deltaTime);

    if(fuelLevel <= 0) {
        fuelLevel = 0.0;
        stopEngine();
    }
}

void PropulsionSystem::updateEngineTemperature(double deltaTime) {
    if(engineActive && thrustLevel > 0) {
        engineTemperature += 0.002 * thrustLevel * deltaTime;
    }
    else {
        engineTemperature -= 0.05 * deltaTime; 
    }
    engineTemperature = Physics::clamp(engineTemperature, 20.0, 1000.0);
}

double PropulsionSystem::getThrustLevel() const {
    return thrustLevel;
}

double PropulsionSystem::getFuelLevel() const {
    return fuelLevel;
}

double PropulsionSystem::getEngineTemperature() const {
    return engineTemperature;
}

bool PropulsionSystem::isEngineActive() const {
    return engineActive;
}