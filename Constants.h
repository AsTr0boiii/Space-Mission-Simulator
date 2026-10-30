#pragma once 

// Physics Constants:-

const double GRAVITY = 9.81;                                              // m/s^2
const double SPACESHIP_DRY_MASS = 12000.0;                               //  kg
const double MAX_THRUST_FORCE = 450000.0;                               //   Newtons

// Mission Distance 

const double MOON_DISTANCE_KM = 384400.0;                            //      km

// Wireless Communication Constants:-

const double FREQUENCY_MHZ = 2200.0;                             //          MHz
const double TRANSMIT_POWER_DBM = 45.0;                         //           dBm
const double ANTENNA_GAIN_DB = 20.0;                           //            dB
const double RECEIVER_GAIN_DB = 25.0;                         //             dB
const double NOISE_POWER_DBM = -100.0;                       //              dBm

// Speed of Light to calculate Communication Delay

const double SPEED_OF_LIGHT_KM_PER_SEC = 300000.0;       //                  km/s
const double MAX_FUEL_MASS = 8000.0;                // kg, simplified fuel mass
const double ORBIT_ALTITUDE_M = 200000.0;           // 200 km
const double ORBIT_VELOCITY_MPS = 7600.0;           // simplified orbital velocity
const double APPROACH_DISTANCE_KM = 50000.0;        // when close to destination
const double LANDING_START_ALTITUDE_M = 10000.0;    // 10 km above destination

// =====================================================
// SIMULATION / DEMONSTRATION SETTINGS
// =====================================================

// Change only these values when switching demo modes.

const bool FAST_DEMO = false;

const int SIMULATION_SPEED =
    FAST_DEMO ? 40 : 1;

const int DATABASE_LOG_INTERVAL =
    FAST_DEMO ? 40 : 10;

const double TRAVEL_SPEED_KM_PER_SEC =
    FAST_DEMO ? 45.0 : 1.2;

const double APPROACH_SPEED_KM_PER_SEC =
    FAST_DEMO ? 25.0 : 0.4;
    
const double ORBIT_VELOCITY_REQUIREMENT = 1000.0;

const double LANDING_DESCENT_SPEED = 200.0;
const double LANDING_CONTROLLED_DESCENT_SPEED = 100.0;