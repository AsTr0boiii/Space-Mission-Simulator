# Space Mission Simulator

## Overview

Space Mission Simulator is a C++ based educational simulation project that models a simplified Earth-to-Moon mission.

The project combines object-oriented programming, basic physics, spacecraft subsystem simulation, wireless communication modelling, sensor monitoring, PostgreSQL database integration, live telemetry logging, mission event tracking, and final mission analytics.

The main goal of the project was to build a larger C++ application where multiple independent systems interact with each other during a complete simulated mission.

---

## Main Features

- Object-oriented C++ architecture
- Multi-phase space mission simulation
- Propulsion and fuel monitoring
- Battery and power monitoring
- Wireless communication modelling
- Sensor warning and critical-state monitoring
- Simplified spacecraft movement physics
- Live terminal dashboard
- PostgreSQL database integration
- Periodic telemetry storage
- Communication data logging
- Mission event logging
- Mission phase tracking
- Database-generated final mission report
- Optional accelerated demonstration mode

---

## Mission Phases

The simulated mission progresses through multiple phases:

```text
PRE-LAUNCH
     ↓
LAUNCH
     ↓
ASCENT
     ↓
ORBIT
     ↓
SPACE TRAVEL
     ↓
APPROACH
     ↓
LANDING
     ↓
MISSION COMPLETE
```

The program uses a mission state system to determine how the spacecraft behaves during each stage.

---

## Project Architecture

```text
main.cpp
    |
    v
MissionControl
    |
    +----------------------+
    |                      |
    v                      v
SpaceShip             DatabaseManager
    |                      |
    |                      v
    |                  PostgreSQL
    |
    +-- PropulsionSystem
    |
    +-- PowerSystem
    |
    +-- CommunicationSystem
    |
    +-- SensorSystem
    |
    +-- Physics
```

### `MissionControl`

Acts as the main controller of the simulation.

It manages:

- simulation updates
- mission time
- mission execution
- dashboard output
- database snapshots
- mission events
- sensor events
- final mission completion

### `SpaceShip`

Represents the spacecraft and combines the major spacecraft subsystems.

It manages:

- altitude
- velocity
- spacecraft mass
- distance from Earth
- distance to destination
- mission phase
- mission status

It also coordinates the propulsion, power, communication, and sensor systems.

### `PropulsionSystem`

Responsible for:

- thrust level
- fuel level
- fuel consumption
- engine temperature
- acceleration calculations

### `PowerSystem`

Responsible for:

- battery level
- battery consumption
- power condition monitoring

### `CommunicationSystem`

Models a simplified Earth-to-spacecraft wireless communication link.

It calculates:

- free-space path loss
- received signal power
- signal-to-noise ratio
- modulation mode
- packet loss
- signal propagation delay
- communication link status

### `SensorSystem`

Monitors spacecraft conditions and classifies them as:

```text
NORMAL
WARNING
CRITICAL
```

The monitored values include:

- engine temperature
- fuel level
- battery level
- communication SNR

### `Physics`

Contains reusable physics calculations used by the simulation.

Examples include:

- acceleration
- velocity updates
- altitude updates
- free-space path loss
- received signal strength
- signal-to-noise ratio
- signal propagation delay

### `DatabaseManager`

Handles communication between the C++ application and PostgreSQL.

It is responsible for:

- creating mission records
- storing telemetry snapshots
- storing communication logs
- storing mission events
- completing mission records
- generating final mission statistics

---

## Database Design

The project uses PostgreSQL with four main tables.

### `missions`

Stores general information about each simulated mission.

Examples:

- mission ID
- mission name
- start time
- end time
- simulated mission duration
- final mission phase
- final mission status

### `telemetry_snapshots`

Stores periodic spacecraft telemetry.

Examples:

- simulation time
- mission phase
- altitude
- velocity
- spacecraft mass
- thrust
- fuel
- battery
- engine temperature
- sensor states

### `communication_logs`

Stores wireless communication measurements.

Examples:

- distance
- frequency
- path loss
- received power
- SNR
- modulation
- packet loss
- communication delay
- link status

### `mission_events`

Stores important events that occur during a mission.

Examples include:

- mission start
- mission phase changes
- sensor warnings
- critical sensor states

The tables are connected using `mission_id` as a primary-key / foreign-key relationship.

---

## Database Relationships

```text
missions
   |
   | mission_id
   |
   +------ telemetry_snapshots
   |
   +------ communication_logs
   |
   +------ mission_events
```

This allows telemetry, communication data, and events from the same mission to be queried together using SQL joins.

---

## Simulation Time

The simulator separates simulated mission time from real-world execution time.

Each normal update represents one simulated second.

```cpp
spaceship.update(1.0);
missionTime += 1.0;
```

Therefore:

```text
deltaTime
= time represented by one simulation update

missionTime
= total accumulated simulated mission time
```

---

## Normal Mode and FAST_DEMO

The project contains an optional accelerated demonstration mode.

By default:

```cpp
const bool FAST_DEMO = false;
```

This runs the project using its normal simulation settings.

Setting:

```cpp
const bool FAST_DEMO = true;
```

allows multiple simulation updates to be processed during one real-world second.

This mode was designed for testing and classroom demonstrations where waiting for the entire mission in real time would be impractical.

Some travel-speed parameters are also increased in demonstration mode to reduce presentation time.

`FAST_DEMO` is therefore a presentation/testing feature and should not be interpreted as realistic spacecraft performance.

---

## Wireless Communication Model

The communication subsystem uses a simplified wireless link-budget model.

Free-space path loss is calculated using:

```text
FSPL =
20 log10(distance)
+
20 log10(frequency)
+
32.44
```

Received power is estimated from:

```text
Received Power =
Transmit Power
+
Antenna Gain
+
Receiver Gain
-
Path Loss
```

Signal-to-noise ratio is then calculated from the received signal and noise level.

As the spacecraft travels farther from Earth:

```text
Distance increases
        ↓
Path loss increases
        ↓
Received signal decreases
        ↓
SNR decreases
        ↓
Packet loss increases
        ↓
Communication quality decreases
```

This allows the communication subsystem to transition between different link conditions during the mission.

---

## Live Dashboard

The application displays continuously updated mission information in the terminal.

The dashboard includes:

- mission time
- mission phase
- mission status
- altitude
- velocity
- distance from Earth
- distance to destination
- spacecraft mass
- thrust
- fuel
- battery
- engine temperature
- communication path loss
- received power
- SNR
- modulation
- packet loss
- communication delay
- link status
- database connection status
- sensor monitoring status

ANSI terminal escape sequences are used to redraw the dashboard during the simulation.

---

## Event Monitoring

Sensor events are stored only when the sensor state changes.

For example:

```text
NORMAL
   ↓
WARNING
   ↓
CRITICAL
```

This prevents the database from storing the same warning every simulation second.

Mission phase changes are handled in a similar way.

---

## Final Mission Report

After the mission ends, PostgreSQL aggregate queries are used to calculate final statistics such as:

- maximum altitude
- maximum velocity
- lowest fuel level
- lowest battery level
- maximum engine temperature
- lowest SNR
- highest path loss
- maximum packet loss
- maximum communication delay
- number of warning events
- number of critical events

This means the database is used not only for storage but also for mission analysis.

---

## Technologies Used

- C++
- C++20
- Object-Oriented Programming
- PostgreSQL
- SQL
- libpqxx
- MSYS2 UCRT64
- GCC / G++
- pgAdmin
- Terminal-based user interface

---

## Project Files

```text
Space-Mission-Simulator/
│
├── main.cpp
├── Constants.h
├── MissionTypes.h
│
├── Physics.cpp
├── Physics.h
│
├── PropulsionSystem.cpp
├── PropulsionSystem.h
│
├── PowerSystem.cpp
├── PowerSystem.h
│
├── CommunicationSystem.cpp
├── CommunicationSystem.h
│
├── SensorSystem.cpp
├── SensorSystem.h
│
├── SpaceShip.cpp
├── SpaceShip.h
│
├── MissionControl.cpp
├── MissionControl.h
│
├── DatabaseManager.cpp
├── DatabaseManager.h
│
├── schema.sql
├── README.md
└── .gitignore
```

---

## PostgreSQL Setup

Create a PostgreSQL database named:

```text
space_mission_simulator
```

Run the provided:

```text
schema.sql
```

file to create the required tables.

The PostgreSQL connection string in `main.cpp` contains a placeholder:

```text
password=YOUR_PASSWORD
```

Replace `YOUR_PASSWORD` with the password for your own local PostgreSQL installation before compiling the application.

Do not commit real database passwords or other credentials to a public repository.

---

## Compilation

The project was developed using MSYS2 UCRT64 with libpqxx.

From the project directory:

```bash
g++ -std=c++20 main.cpp Physics.cpp PropulsionSystem.cpp PowerSystem.cpp CommunicationSystem.cpp SensorSystem.cpp SpaceShip.cpp MissionControl.cpp DatabaseManager.cpp -o SpaceMissionSimulator.exe $(pkg-config --cflags --libs libpqxx)
```

---

## Running the Program

After compilation:

```bash
./SpaceMissionSimulator.exe
```

PostgreSQL must be running locally for database functionality to work.

An internet connection is not required to run the simulator when PostgreSQL and the required libraries are already installed locally.

---

## Project Scope and Limitations

This project is an educational simulation rather than a high-fidelity aerospace simulator.

Several parts of the mission have intentionally simplified behaviour, including:

- spacecraft dynamics
- orbital mechanics
- propulsion modelling
- engine temperature modelling
- battery behaviour
- Moon approach and landing
- communication link-budget assumptions

The purpose is to demonstrate software architecture, simulation logic, subsystem interaction, database integration, and basic engineering calculations rather than reproduce the exact behaviour of a real lunar mission.

---

## Future Improvements

Possible future versions could include:

- more realistic orbital mechanics
- gravitational variation
- trajectory calculations
- improved fuel and propulsion modelling
- realistic lunar orbital insertion
- improved landing guidance
- more advanced communication link budgets
- dynamic antenna parameters
- graphical mission visualization
- real-time graphs
- configurable missions
- multiple celestial destinations
- improved database analytics
- more detailed failure simulation

---

## Disclaimer

This project is intended for educational and programming purposes.

The physics, spacecraft behaviour, communication calculations, and mission procedures are simplified and should not be used for real aerospace engineering, spacecraft navigation, or mission planning.

---

## Author

Mohd Nabeel Qureshi

B.Sc. Computer Science with Research
