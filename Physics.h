#pragma once 

namespace Physics {
   
    double calculateAcceleration(double thrustForce, double mass);

    double calculateVelocity(double currentVelocity, double acceleration, double deltaTime);

    double calculateAltitude(double currentAltitude, double velocity, double deltaTime);

    double calculateFuelConsumption(double currentFuel, double burnFuel, double deltaTime);

    double calculateFreeSpacePathLoss(double distanceKm, double frequencyMHz);

    double calculateReceivedPower(
        double transmitPowerDbm,
        double antennaGainDb,
        double receiverGainDb,
        double pathLossDb

    );

    double calculateSNR(double receivedPowerDbm, double noisePowerDbm);

    double calculateCommunicationDelay(double distanceKm);

    double clamp(double value, double minValue, double maxValue);

}