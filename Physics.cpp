#include "Physics.h"
#include "Constants.h"
#include <cmath>

namespace Physics {

    double calculateAcceleration(double thrustForce, double mass) {
        if(mass <= 0) {
            return 0;
        }

        return thrustForce/mass;
    }

    double calculateVelocity(double currentVelocity, double acceleration, double deltaTime) {
        return currentVelocity + (acceleration * deltaTime);
    }

    double calculateAltitude(double currentAltitude, double velocity, double deltaTime) {
        return currentAltitude + (velocity * deltaTime);
    }

    double calculateFuelConsumption(double currentFuel, double burnRate, double deltaTime) {
        double newFuel = currentFuel - (burnRate * deltaTime);
        return clamp(newFuel, 0.0, 100.0);
    }

    double calculateFreeSpacePathLoss(double distanceKm, double frequencyMhz) {
        if(distanceKm <= 0) {
            distanceKm = 0.001;
        }
        return 20 * log10(distanceKm) + 20 * log10(frequencyMhz) + 32.44;
    }

    double calculateReceivedPower(
    double transmitPowerDbm,
    double antennaGainDb,
    double receiverGainDb,
    double pathLossDb
) {
    return transmitPowerDbm + antennaGainDb + receiverGainDb - pathLossDb;
}

    double calculateSNR(double receivedPowerDbm, double noisePowerDbm) {
        return receivedPowerDbm - noisePowerDbm;
    }

    double calculateCommunicationDelay(double distanceKm) {
        return distanceKm / SPEED_OF_LIGHT_KM_PER_SEC;
    }

    double clamp(double value, double minValue, double maxValue) {
        if(value < minValue) {
            return minValue;
        }
        if(value > maxValue) {
            return maxValue;
        }

        return value;
    }
}