#include "MissionControl.h"
#include "Constants.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>


MissionControl::MissionControl(
    const std::string& connectionString) 
    :database(connectionString) {
    missionRunning = false;
    paused = false;
    missionTime = 0.0;

    previousPhase = MissionPhase::PRE_LAUNCH;

    previousTemperatureStatus = SensorStatus::NORMAL;
    previousFuelStatus = SensorStatus::NORMAL;
    previousBatteryStatus = SensorStatus::NORMAL;
    previousCommunicationStatus = SensorStatus::NORMAL;
}


void MissionControl::startMission() {
    
    spaceship.startMission();

    missionRunning = true;
    paused = false;
    missionTime = 0.0;

    database.createMission("Lunar Mission");

    database.saveEvent(
        0.0,
        "MISSION",
        "INFO",
        "MISSION_STARTED",
        "Space mission simulation started."
    );

    previousPhase = spaceship.getMissionPhase();


}


void MissionControl::update() {

    if (paused || !missionRunning) {
        return;
    }

    spaceship.update(1.0);

    missionTime += 1.0;

    checkPhaseEvent();
    checkSensorEvents();

    if(static_cast<int>(missionTime) % DATABASE_LOG_INTERVAL == 0) {
        saveDatabaseSnapshot();
    }

    if (isMissionFinished()) {
        missionRunning = false;
    }
}


void MissionControl::runSimulation() {


    std::cout << "\033[2J\033[H";

    startMission();

    while (missionRunning) {

        MissionPhase phase = spaceship.getMissionPhase();

        if ((phase == MissionPhase::LAUNCH ||
             phase == MissionPhase::ASCENT) &&
            spaceship.getPropulsionSystem().getThrustLevel() < 100.0) {
            spaceship.increaseThrust();
        }

        for(int i = 0; i < SIMULATION_SPEED && missionRunning; i++) {
            update();
        }

        clearScreen();
        displayDashboard();

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    
    clearScreen();
    displayDashboard();

    database.finishMission(
        missionTime,
        spaceship.getMissionPhaseText(),
        spaceship.getMissionStatus()
    );

    database.printFinalReport();

    std::cout << "\nMission simulation ended.\n";
}


void MissionControl::clearScreen() const {
    std::cout << "\033[H\033[J";
}

bool MissionControl::isMissionFinished() const {

    MissionPhase phase = spaceship.getMissionPhase();

    return phase == MissionPhase::MISSION_COMPLETE ||
           phase == MissionPhase::MISSION_FAILED;
}


void MissionControl::displayDashboard() const {

    auto makeBar = [](double percentage) {

    const int width = 20;

    int filled =
        static_cast<int>(
            (percentage / 100.0) * width
        );

    if (filled < 0)
        filled = 0;

    if (filled > width)
        filled = width;

    std::string bar = "[";

    for (int i = 0; i < width; i++) {

        if (i < filled)
            bar += "#";
        else
            bar += "-";
    }

    bar += "]";

    return bar;
};

    const PropulsionSystem& propulsion =
        spaceship.getPropulsionSystem();

    const PowerSystem& power =
        spaceship.getPowerSystem();

    const CommunicationSystem& communication =
        spaceship.getCommunicationSystem();

    const SensorSystem& sensors =
        spaceship.getSensorSystem();


    int totalSeconds = static_cast<int>(missionTime);

    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;


    std::cout << std::fixed << std::setprecision(2);


    std::cout
        << "============================================================\n"
        << "                 SPACE MISSION SIMULATOR\n"
        << "============================================================\n\n";


    std::cout << "MISSION TIME:     "
              << std::setfill('0')
              << std::setw(2) << hours << ":"
              << std::setw(2) << minutes << ":"
              << std::setw(2) << seconds
              << std::setfill(' ')
              << "\n";

    std::cout << "MISSION PHASE:    "
              << spaceship.getMissionPhaseText()
              << "\n";

    std::cout << "MISSION STATUS:   "
              << spaceship.getMissionStatus()
              << "\n";


    std::cout
        << "\n"
        << "-------------- SPACESHIP TELEMETRY ----------------\n";


    std::cout << "Altitude:            "
              << spaceship.getAltitude() / 1000.0
              << " km\n";

    std::cout << "Velocity:            "
              << spaceship.getVelocity()
              << " m/s\n";

    std::cout << "Distance From Earth: "
              << spaceship.getDistanceFromEarth()
              << " km\n";

    std::cout << "Distance To Target:  "
              << spaceship.getDistanceToDestination()
              << " km\n";

    std::cout << "Spaceship Mass:      "
              << spaceship.getSpaceshipMass()
              << " kg\n";

    std::cout << "Thrust:              "
              << propulsion.getThrustLevel()
              << " %\n";

   std::cout << "Fuel:                "
          << makeBar(propulsion.getFuelLevel())
          << " "
          << propulsion.getFuelLevel()
          << " %  ["
          << sensors.getStatusText(
                 sensors.getFuelStatus()
             )
          << "]\n";

    std::cout << "Battery:             "
          << makeBar(power.getBatteryLevel())
          << " "
          << power.getBatteryLevel()
          << " %  ["
          << sensors.getStatusText(
                 sensors.getBatteryStatus()
             )
          << "]\n";

    std::cout << "Engine Temperature:  "
          << propulsion.getEngineTemperature()
          << " C  ["
          << sensors.getStatusText(
                 sensors.getTemperatureStatus()
             )
          << "]\n";


    std::cout
        << "\n"
        << "------------- WIRELESS COMMUNICATION ----------------\n";


    std::cout << "Distance:             "
              << communication.getDistanceKm()
              << " km\n";

    std::cout << "Path Loss:            "
              << communication.getPathLoss()
              << " dB\n";

    std::cout << "Received Power:       "
              << communication.getReceivedPower()
              << " dBm\n";

    std::cout << "SNR:                  "
          << communication.getSNR()
          << " dB  ["
          << sensors.getStatusText(
                 sensors.getCommunicationStatus()
             )
          << "]\n";

    std::cout << "Modulation:           "
              << communication.getModulation()
              << "\n";

    std::cout << "Packet Loss:          "
              << communication.getPacketLoss()
              << " %\n";

    std::cout << "Communication Delay:  "
              << communication.getDelay() * 1000.0
              << " ms\n";

    std::cout << "Link Status:          "
              << communication.getLinkStatus()
              << "\n";


   std::cout << "\n"
          << "SYSTEM CONNECTIONS\n"
          << "Database:           "
          << (database.isConnected()
              ? "CONNECTED"
              : "DISCONNECTED")
          << "\n"
          << "Sensor Monitoring:  ACTIVE\n";

        
    std::cout
        << "============================================================\n";

    std::cout << std::flush;

}

void MissionControl::saveDatabaseSnapshot() {

    const PropulsionSystem& propulsion =
    spaceship.getPropulsionSystem();

    const PowerSystem& power =
    spaceship.getPowerSystem();

    const CommunicationSystem& communication =
    spaceship.getCommunicationSystem();
    
    const SensorSystem& sensors =
    spaceship.getSensorSystem();

   database.saveTelemetry(
    missionTime,
    spaceship.getMissionPhaseText(),
    spaceship.getMissionStatus(),
    spaceship.getAltitude(),
    spaceship.getVelocity(),
    spaceship.getDistanceFromEarth(),
    spaceship.getDistanceToDestination(),
    spaceship.getSpaceshipMass(),
    propulsion.getThrustLevel(),
    propulsion.getFuelLevel(),
    power.getBatteryLevel(),
    propulsion.getEngineTemperature(),

    sensors.getStatusText(
        sensors.getTemperatureStatus()
    ),

    sensors.getStatusText(
        sensors.getFuelStatus()
    ),

    sensors.getStatusText(
        sensors.getBatteryStatus()
    ),

    sensors.getStatusText(
        sensors.getCommunicationStatus()
    )

 );


    database.saveCommunicationLog(
        missionTime,

        communication.getDistanceKm(),
        communication.getFrequencyMHz(),

        communication.getPathLoss(),
        communication.getReceivedPower(),
        communication.getSNR(),

        communication.getModulation(),

        communication.getPacketLoss(),
        communication.getDelay(),

        communication.getLinkStatus()
    );

}

 void MissionControl::checkPhaseEvent() {
        
        MissionPhase currentPhase =
        spaceship.getMissionPhase();
        
        if(currentPhase != previousPhase) {

            std::string phaseText =
            spaceship.getMissionPhaseText();

            database.saveEvent(
                missionTime,
                "PHASE_CHANGE",
                "INFO",
                phaseText,
                "Mission entered " + phaseText + " phase."
            );

            previousPhase = currentPhase;
        }
    }

    void MissionControl::checkSensorEvents() {

    const SensorSystem& sensors =
        spaceship.getSensorSystem();



    if (sensors.getTemperatureStatus() != previousTemperatureStatus) {

        SensorStatus status =
            sensors.getTemperatureStatus();

        if (status == SensorStatus::WARNING) {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "WARNING",
                "ENGINE_TEMPERATURE_WARNING",
                "Engine temperature entered warning range."
            );
        }
        else if (status == SensorStatus::CRITICAL) {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "CRITICAL",
                "ENGINE_TEMPERATURE_CRITICAL",
                "Engine temperature entered critical range."
            );
        }
        else {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "INFO",
                "ENGINE_TEMPERATURE_NORMAL",
                "Engine temperature returned to normal range."
            );
        }

        previousTemperatureStatus = status;
    }



    if (sensors.getFuelStatus() != previousFuelStatus) {

        SensorStatus status =
            sensors.getFuelStatus();

        if (status == SensorStatus::WARNING) {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "WARNING",
                "LOW_FUEL_WARNING",
                "Fuel level entered warning range."
            );
        }
        else if (status == SensorStatus::CRITICAL) {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "CRITICAL",
                "LOW_FUEL_CRITICAL",
                "Fuel level entered critical range."
            );
        }
        else {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "INFO",
                "FUEL_NORMAL",
                "Fuel sensor returned to normal range."
            );
        }

        previousFuelStatus = status;
    }


    
    if (sensors.getBatteryStatus() != previousBatteryStatus) {

        SensorStatus status =
            sensors.getBatteryStatus();

        if (status == SensorStatus::WARNING) {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "WARNING",
                "LOW_BATTERY_WARNING",
                "Battery level entered warning range."
            );
        }
        else if (status == SensorStatus::CRITICAL) {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "CRITICAL",
                "LOW_BATTERY_CRITICAL",
                "Battery level entered critical range."
            );
        }
        else {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "INFO",
                "BATTERY_NORMAL",
                "Battery sensor returned to normal range."
            );
        }

        previousBatteryStatus = status;
    }


    
    if (sensors.getCommunicationStatus() !=
        previousCommunicationStatus) {

        SensorStatus status =
            sensors.getCommunicationStatus();

        if (status == SensorStatus::WARNING) {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "WARNING",
                "COMMUNICATION_WARNING",
                "Communication signal entered warning range."
            );
        }
        else if (status == SensorStatus::CRITICAL) {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "CRITICAL",
                "COMMUNICATION_CRITICAL",
                "Communication signal entered critical range."
            );
        }
        else {
            database.saveEvent(
                missionTime,
                "SENSOR",
                "INFO",
                "COMMUNICATION_NORMAL",
                "Communication signal returned to normal range."
            );
        }

        previousCommunicationStatus = status;
    }
}



