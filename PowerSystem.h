#pragma once

class PowerSystem {
    private:
    double batteryLevel;
    double powerConsumptionRate;
    double solarChargeRate;
    bool solarCharging;
    bool lowPowerMode;

    public:
    PowerSystem();

    void enableSolarCharging();
    void disableSolarCharging();

    void updateBattery(double deltaTime);

    bool isBatteryLow() const;
    bool isBatteryCritical() const;

    double getBatteryLevel() const;
    bool isSolarCharging() const;
    bool isLowPowerMode() const;
};