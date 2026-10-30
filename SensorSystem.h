#pragma once
#include <string>

enum class SensorStatus {
    NORMAL,
    WARNING,
    CRITICAL
};

class SensorSystem {
    private:

    double engineTemperature;
    double fuelLevel;
    double batteryLevel;
    double snrDB;
    double altitude;
    double velocity;

    SensorStatus temperatureStatus;
    SensorStatus fuelStatus;
    SensorStatus batteryStatus;
    SensorStatus communicationStatus;

    public:

    SensorSystem();

    void updateSensors(
        double engineTemp,
        double fuel,
        double battery,
        double snr,
        double currentAltitude,
        double currentVelocity
    );

    void checkTemperature();
    void checkFuel();
    void checkBattery();
    void checkCommunication();

    double getEngineTemperature() const;
    double getFuelLevel() const;
    double getBatteryLevel() const;
    double getSNR() const;
    double getAltitude() const;
    double getVelocity() const;

    SensorStatus getTemperatureStatus() const;
    SensorStatus getFuelStatus() const;
    SensorStatus getBatteryStatus() const;
    SensorStatus getCommunicationStatus() const;

    std::string getStatusText(SensorStatus status) const;

    bool hasCriticalWarning() const;

};