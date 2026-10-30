#include "PowerSystem.h"
#include "Physics.h"

PowerSystem::PowerSystem() {
    batteryLevel = 100.0;
    powerConsumptionRate = 0.005;
    solarChargeRate = 0.003;
    solarCharging = false;
    lowPowerMode = false;
}

void PowerSystem::enableSolarCharging() {
    solarCharging = true;
}

void PowerSystem::disableSolarCharging() {
    solarCharging = false;
}

void PowerSystem::updateBattery(double deltaTime) {
    if(solarCharging) {
        batteryLevel += solarChargeRate * deltaTime;
    }

    batteryLevel -= powerConsumptionRate * deltaTime;

    batteryLevel = Physics::clamp(batteryLevel, 0.0, 100.0);

    if(batteryLevel <= 20.0) {
        lowPowerMode = true;
    }

    else {
        lowPowerMode = false;
    } 
}

bool PowerSystem::isBatteryLow() const {
    return batteryLevel <= 20.0;
}

bool PowerSystem::isBatteryCritical() const {
    return batteryLevel <= 5.0;
}

double PowerSystem::getBatteryLevel() const {
    return batteryLevel;
}

bool PowerSystem::isSolarCharging() const {
    return solarCharging;
}

bool PowerSystem::isLowPowerMode() const {
    return lowPowerMode;
}

