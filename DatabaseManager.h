#pragma once

#include <pqxx/pqxx>
#include <string>

class DatabaseManager {

    private:
    pqxx::connection connection;
    long long currentMissionId;

    public:
    DatabaseManager(const std::string& connectionString);

    bool isConnected() const;

    long long createMission(const std::string& missionName);

    void finishMission(
        double simulatedDuration,
        const std::string& finalPhase,
        const std::string& finalStatus
    );

    bool saveTelemetry(
        double simulationTime,
        const std::string& missionPhase,
        const std::string& missionStatus,
        double altitude,
        double velocity,
        double distanceFromEarth,
        double distanceToTarget,
        double spaceshipMass,
        double thrust,
        double fuel,
        double battery,
        double engineTemperature,
        const std::string& temperatureSensorStatus,
        const std::string& fuelSensorStatus,
        const std::string& batterySensorStatus,
        const std::string& communicationSensorStatus
    );

    bool saveCommunicationLog(
        double simulationTime,
        double distanceKm,
        double frequencyKm,
        double pathLossDb,
        double receivedPowerDbm,
        double snrDb,
        const std::string& modulation,
        double packetLossPercent,
        double delaySeconds,
        const std::string& linkStatus
    );

    bool saveEvent(
        double simulationTime,
        const std::string& eventType,
        const std::string& severity,
        const std::string& eventName,
        const std::string& description
    );

    long long getCurrentMissionId() const;

    void printFinalReport();
};