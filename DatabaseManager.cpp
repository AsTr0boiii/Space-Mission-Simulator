#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager(
    const std::string& connectionString
) 
  : connection(connectionString),
  currentMissionId(0)

  {}

  bool DatabaseManager::isConnected() const {
    return connection.is_open();

  }

  long long DatabaseManager::createMission(const std::string& missionName)
  {
    try {

        pqxx::work transaction(connection);

        pqxx::result result = transaction.exec(
            R"(
                INSERT INTO missions (mission_name)
                VALUES ($1)
                RETURNING mission_id;
            )",
            pqxx::params{missionName}
        );

        currentMissionId = result.one_row()[0].as<long long>();

        transaction.commit();

        return currentMissionId;
    }

    catch (const std::exception& e) {

        std::cerr
            << "Failed to create mission: "
            <<e.what()
            << "\n";

            return 0;
    }
  }

  void DatabaseManager::finishMission(
    double simulatedDuration,
    const std::string& finalPhase,
    const std::string& finalStatus
  ) {

    if(currentMissionId == 0) {
        return;
    }

    try {

        pqxx::work transaction(connection);

        transaction.exec(
            R"(
                UPDATE missions
                SET
                   ended_at = CURRENT_TIMESTAMP,
                   simulated_duration_seconds = $1,
                   final_phase = $2,
                   final_status = $3
                   WHERE mission_id = $4;
            )",

            pqxx::params{
                simulatedDuration,
                finalPhase,
                finalStatus,
                currentMissionId
            }
        );

        transaction.commit();
    }

    catch (const std::exception& e) {

        std::cerr
           << "Failed to finish mission: "
           << e.what()
           << "/n";
    }
  }

  long long DatabaseManager::getCurrentMissionId() const {
    return currentMissionId;
    
  }

bool DatabaseManager::saveTelemetry(
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
) {
    if(currentMissionId == 0) {
        return false;
    }

    try {

        pqxx::work transaction(connection);

        transaction.exec(
            R"(
               INSERT INTO telemetry_snapshots (
                     mission_id,
                    simulation_time_seconds,
                    mission_phase,
                    mission_status,
                    altitude_m,
                    velocity_mps,
                    distance_from_earth_km,
                    distance_to_target_km,
                    spaceship_mass_kg,
                    thrust_percent,
                    fuel_percent,
                    battery_percent,
                    engine_temperature_c,
                    temperature_sensor_status,
                    fuel_sensor_status,
                    battery_sensor_status,
                    communication_sensor_status
               )
                    VALUES (
                        $1, $2, $3, $4, $5, $6, $7, $8, $9,
                    $10, $11, $12, $13, $14, $15, $16, $17
                    
                    );
            )",

        pqxx::params {
                currentMissionId,
                simulationTime,
                missionPhase,
                missionStatus,
                altitude,
                velocity,
                distanceFromEarth,
                distanceToTarget,
                spaceshipMass,
                thrust,
                fuel,
                battery,
                engineTemperature,
                temperatureSensorStatus,
                fuelSensorStatus,
                batterySensorStatus,
                communicationSensorStatus
            }
            
        );

        transaction.commit();

        return true;
    }

    catch(const std::exception& e) {
        
        std::cerr
           << "Failed to save telemetry: "
           << e.what()
           << "\n";

           return false;
    }
}

bool DatabaseManager::saveCommunicationLog(
    double simulationTime,
    double distanceKm,
    double frequencyMHz,
    double pathLossDb,
    double receivedPowerDbm,
    double snrDb,
    const std::string& modulation,
    double packetLossPercent,
    double delaySeconds,
    const std::string& linkStatus
) {

    if(currentMissionId == 0) {
        return false;
    }

    try {

        pqxx::work transaction(connection);

        transaction.exec(
            R"(
                INSERT INTO communication_logs (
                     mission_id,
                    simulation_time_seconds,
                    distance_km,
                    frequency_mhz,
                    path_loss_db,
                    received_power_dbm,
                    snr_db,
                    modulation,
                    packet_loss_percent,
                    delay_seconds,
                    link_status
                )
                    VALUES (
                        $1, $2, $3, $4, $5, $6,
                        $7, $8, $9, $10, $11
                    );
            )",

            pqxx::params{
                currentMissionId,
                simulationTime,
                distanceKm,
                frequencyMHz,
                pathLossDb,
                receivedPowerDbm,
                snrDb,
                modulation,
                packetLossPercent,
                delaySeconds,
                linkStatus
            }
        );

        transaction.commit();

        return true;
    }

    catch(const std::exception& e) {
        
        std::cerr
           << "Failed to save communication log: "
           << e.what()
           << "\n";

           return false;
    }
}

bool DatabaseManager::saveEvent(
     double simulationTime,
    const std::string& eventType,
    const std::string& severity,
    const std::string& eventName,
    const std::string& description
) {

    if(currentMissionId == 0) {
        return false;

    }

    try {

        pqxx::work transaction(connection);

        transaction.exec(
            R"(
               INSERT INTO mission_events (
                    mission_id,
                    simulation_time_seconds,
                    event_type,
                    severity,
                    event_name,
                    description
                )
                VALUES ($1, $2, $3, $4, $5, $6);
            )",
            pqxx::params{
                currentMissionId,
                simulationTime,
                eventType,
                severity,
                eventName,
                description
            }
        );

        transaction.commit();

        return true;
    }

    catch(const std::exception& e) {

        std::cerr
           << "Failed to save mission event: "
           << e.what()
           << "\n";

           return false;
    }
}

void DatabaseManager::printFinalReport() {

    if (currentMissionId == 0) {
        std::cout << "No mission available for report.\n";
        return;
    }

    try {

        pqxx::read_transaction transaction(connection);

        // -----------------------------------------
        // MISSION INFORMATION
        // -----------------------------------------

        pqxx::result missionResult = transaction.exec(
            R"(
                SELECT
                    mission_name,
                    simulated_duration_seconds,
                    final_phase,
                    final_status
                FROM missions
                WHERE mission_id = $1;
            )",
            pqxx::params{currentMissionId}
        );


        // -----------------------------------------
        // TELEMETRY STATISTICS
        // -----------------------------------------

        pqxx::result telemetryResult = transaction.exec(
            R"(
                SELECT
                    MAX(altitude_m),
                    MAX(velocity_mps),
                    MIN(fuel_percent),
                    MIN(battery_percent),
                    MAX(engine_temperature_c)
                FROM telemetry_snapshots
                WHERE mission_id = $1;
            )",
            pqxx::params{currentMissionId}
        );


        // -----------------------------------------
        // COMMUNICATION STATISTICS
        // -----------------------------------------

        pqxx::result communicationResult = transaction.exec(
            R"(
                SELECT
                    MIN(snr_db),
                    MAX(path_loss_db),
                    MAX(packet_loss_percent),
                    MAX(delay_seconds)
                FROM communication_logs
                WHERE mission_id = $1;
            )",
            pqxx::params{currentMissionId}
        );


        // -----------------------------------------
        // EVENT STATISTICS
        // -----------------------------------------

        pqxx::result eventResult = transaction.exec(
            R"(
                SELECT
                    COUNT(*) FILTER (
                        WHERE severity = 'WARNING'
                    ),

                    COUNT(*) FILTER (
                        WHERE severity = 'CRITICAL'
                    )

                FROM mission_events
                WHERE mission_id = $1;
            )",
            pqxx::params{currentMissionId}
        );


        if (missionResult.empty()) {
            std::cout << "Mission report could not be generated.\n";
            return;
        }


        auto missionRow = missionResult[0];
        auto telemetryRow = telemetryResult[0];
        auto communicationRow = communicationResult[0];
        auto eventRow = eventResult[0];


        std::cout << "\n\n";
        std::cout << "============================================================\n";
        std::cout << "                 FINAL MISSION REPORT\n";
        std::cout << "============================================================\n\n";


        std::cout << "Mission ID:              "
                  << currentMissionId << "\n";

        std::cout << "Mission Name:            "
                  << missionRow[0].c_str() << "\n";

        std::cout << "Mission Duration:        "
                  << missionRow[1].as<double>()
                  << " sec\n";

        std::cout << "Final Phase:             "
                  << missionRow[2].c_str() << "\n";

        std::cout << "Final Status:            "
                  << missionRow[3].c_str() << "\n";


        std::cout << "\n---------------- FLIGHT STATISTICS ----------------\n";

        std::cout << "Maximum Altitude:        "
                  << telemetryRow[0].as<double>() / 1000.0
                  << " km\n";

        std::cout << "Maximum Velocity:        "
                  << telemetryRow[1].as<double>()
                  << " m/s\n";

        std::cout << "Lowest Fuel Level:       "
                  << telemetryRow[2].as<double>()
                  << " %\n";

        std::cout << "Lowest Battery Level:    "
                  << telemetryRow[3].as<double>()
                  << " %\n";

        std::cout << "Maximum Engine Temp:     "
                  << telemetryRow[4].as<double>()
                  << " C\n";


        std::cout << "\n------------- COMMUNICATION STATISTICS -------------\n";

        std::cout << "Lowest SNR:              "
                  << communicationRow[0].as<double>()
                  << " dB\n";

        std::cout << "Highest Path Loss:       "
                  << communicationRow[1].as<double>()
                  << " dB\n";

        std::cout << "Maximum Packet Loss:     "
                  << communicationRow[2].as<double>()
                  << " %\n";

        std::cout << "Maximum Signal Delay:    "
                  << communicationRow[3].as<double>() * 1000.0
                  << " ms\n";


        std::cout << "\n---------------- SENSOR EVENTS ---------------------\n";

        std::cout << "Warning Events:          "
                  << eventRow[0].as<long long>()
                  << "\n";

        std::cout << "Critical Events:         "
                  << eventRow[1].as<long long>()
                  << "\n";


        std::cout << "\n============================================================\n";
    }

    catch (const std::exception& e) {

        std::cerr
            << "Failed to generate final report: "
            << e.what()
            << "\n";
    }
}