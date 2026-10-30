#include "SensorSystem.h"

SensorSystem::SensorSystem() {
    engineTemperature = 0.0;
    fuelLevel = 0.0;
    batteryLevel = 0.0;
    snrDB = 0.0;
    altitude = 0.0;
    velocity = 0.0;

    temperatureStatus = SensorStatus::NORMAL;
    fuelStatus = SensorStatus::NORMAL;
    batteryStatus = SensorStatus::NORMAL;
    communicationStatus = SensorStatus::NORMAL;

}

void SensorSystem::updateSensors(
        double engineTemp,
        double fuel,
        double battery,
        double snr,
        double currentAltitude,
        double currentVelocity
)  {
    engineTemperature = engineTemp;
    fuelLevel = fuel;
    batteryLevel = battery;
    snrDB = snr;
    altitude = currentAltitude;
    velocity = currentVelocity;

    checkTemperature();
    checkFuel();
    checkBattery();
    checkCommunication();

}

void SensorSystem::checkTemperature() {
    if(engineTemperature >= 900.0) {
        temperatureStatus = SensorStatus::CRITICAL;
    }
    else if(engineTemperature >= 700) {
        temperatureStatus = SensorStatus::WARNING;
    }
    else {
        temperatureStatus = SensorStatus::NORMAL;
    }

}

void SensorSystem::checkFuel() {
    if(fuelLevel <= 10.0) {
        fuelStatus = SensorStatus::CRITICAL;
    }
    else if(fuelLevel <= 30.0) {
        fuelStatus = SensorStatus::WARNING;
    }
    else {
        fuelStatus = SensorStatus::NORMAL;
    }

}

void SensorSystem::checkBattery() {
    if(batteryLevel <= 5.0) {
        batteryStatus = SensorStatus::CRITICAL;
    }
    else if(batteryLevel <= 20.0) {
        batteryStatus = SensorStatus::WARNING;
    }
    else {
        batteryStatus = SensorStatus::NORMAL;
    }

}

void SensorSystem::checkCommunication() {
    if(snrDB < 5.0) {
        communicationStatus = SensorStatus::CRITICAL;
    }
    else if(snrDB <= 18) {
        communicationStatus = SensorStatus::WARNING;
    }
    else {
        communicationStatus = SensorStatus::NORMAL;
    }
    
}

double SensorSystem::getEngineTemperature() const {
    return engineTemperature;
}

double SensorSystem::getFuelLevel() const {
    return fuelLevel;
}

double SensorSystem::getBatteryLevel() const {
    return batteryLevel;
}

double SensorSystem::getSNR() const {
    return snrDB;
}

double SensorSystem::getAltitude() const {
    return altitude;
}

double SensorSystem::getVelocity() const {
    return velocity;
}

SensorStatus SensorSystem::getTemperatureStatus() const {
    return temperatureStatus;
}

 SensorStatus SensorSystem::getFuelStatus() const {
    return fuelStatus;
}

 SensorStatus SensorSystem::getBatteryStatus() const {
    return batteryStatus;
}

 SensorStatus SensorSystem::getCommunicationStatus() const {
    return communicationStatus;
}

std::string SensorSystem::getStatusText(SensorStatus status) const {
    switch(status) {

    case SensorStatus::NORMAL:
    return "NORMAL";

    case SensorStatus::WARNING:
    return "WARNING";

    case SensorStatus::CRITICAL:
    return "CRITICAL";

    default:
    return "UNKNOWN";

    }

}

bool SensorSystem::hasCriticalWarning() const {
    if(temperatureStatus == SensorStatus::CRITICAL ||
        fuelStatus == SensorStatus::CRITICAL ||
        batteryStatus == SensorStatus::CRITICAL ||
        communicationStatus == SensorStatus::CRITICAL
    ) {
        return true;
    }
    
    return false;

}















