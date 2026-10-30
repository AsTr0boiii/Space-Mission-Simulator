#pragma once

class PropulsionSystem {

    double thrustLevel;
    double fuelLevel;
    double fuelBurnRate;
    double engineTemperature;
    bool engineActive;

    public:

    PropulsionSystem();

    void startEngine();
    void stopEngine();

    void increaseThrust(double amount);
    void decreaseThrust(double amount);

    double calculateThrustForce() const;
    double calculateAcceleration(double spaceshipMass) const;

    void updateFuel(double deltaTime);
    void updateEngineTemperature(double deltaTime);

    double getThrustLevel() const;
    double getFuelLevel() const;
    double getEngineTemperature() const;
    bool isEngineActive() const;

};