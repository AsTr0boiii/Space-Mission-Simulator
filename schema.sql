-- =========================================================
-- SPACE MISSION SIMULATOR DATABASE
-- PostgreSQL Schema
-- =========================================================


-- ---------------------------------------------------------
-- 1. MISSIONS
-- ---------------------------------------------------------

CREATE TABLE missions (

    mission_id BIGINT GENERATED ALWAYS AS IDENTITY
        PRIMARY KEY,

    mission_name VARCHAR(100) NOT NULL,

    started_at TIMESTAMPTZ NOT NULL
        DEFAULT CURRENT_TIMESTAMP,

    ended_at TIMESTAMPTZ,

    simulated_duration_seconds DOUBLE PRECISION
        DEFAULT 0,

    final_phase VARCHAR(30),

    final_status VARCHAR(120)
);



-- ---------------------------------------------------------
-- 2. TELEMETRY SNAPSHOTS
-- ---------------------------------------------------------

CREATE TABLE telemetry_snapshots (

    snapshot_id BIGINT GENERATED ALWAYS AS IDENTITY
        PRIMARY KEY,

    mission_id BIGINT NOT NULL,

    simulation_time_seconds DOUBLE PRECISION NOT NULL,

    mission_phase VARCHAR(30) NOT NULL,

    mission_status VARCHAR(120),

    altitude_m DOUBLE PRECISION NOT NULL,

    velocity_mps DOUBLE PRECISION NOT NULL,

    distance_from_earth_km DOUBLE PRECISION NOT NULL,

    distance_to_target_km DOUBLE PRECISION NOT NULL,

    spaceship_mass_kg DOUBLE PRECISION NOT NULL,

    thrust_percent DOUBLE PRECISION NOT NULL,

    fuel_percent DOUBLE PRECISION NOT NULL,

    battery_percent DOUBLE PRECISION NOT NULL,

    engine_temperature_c DOUBLE PRECISION NOT NULL,

    temperature_sensor_status VARCHAR(10),

    fuel_sensor_status VARCHAR(10),

    battery_sensor_status VARCHAR(10),

    communication_sensor_status VARCHAR(10),

    CONSTRAINT fk_telemetry_mission
        FOREIGN KEY (mission_id)
        REFERENCES missions(mission_id)
        ON DELETE CASCADE,

    CONSTRAINT chk_telemetry_thrust
        CHECK (thrust_percent >= 0 AND thrust_percent <= 100),

    CONSTRAINT chk_telemetry_fuel
        CHECK (fuel_percent >= 0 AND fuel_percent <= 100),

    CONSTRAINT chk_telemetry_battery
        CHECK (battery_percent >= 0 AND battery_percent <= 100)
);



-- ---------------------------------------------------------
-- 3. COMMUNICATION LOGS
-- ---------------------------------------------------------

CREATE TABLE communication_logs (

    communication_id BIGINT GENERATED ALWAYS AS IDENTITY
        PRIMARY KEY,

    mission_id BIGINT NOT NULL,

    simulation_time_seconds DOUBLE PRECISION NOT NULL,

    distance_km DOUBLE PRECISION NOT NULL,

    frequency_mhz DOUBLE PRECISION NOT NULL,

    path_loss_db DOUBLE PRECISION NOT NULL,

    received_power_dbm DOUBLE PRECISION NOT NULL,

    snr_db DOUBLE PRECISION NOT NULL,

    modulation VARCHAR(20),

    packet_loss_percent DOUBLE PRECISION NOT NULL,

    delay_seconds DOUBLE PRECISION NOT NULL,

    link_status VARCHAR(30),

    CONSTRAINT fk_communication_mission
        FOREIGN KEY (mission_id)
        REFERENCES missions(mission_id)
        ON DELETE CASCADE,

    CONSTRAINT chk_packet_loss
        CHECK (
            packet_loss_percent >= 0
            AND packet_loss_percent <= 100
        )
);



-- ---------------------------------------------------------
-- 4. MISSION EVENTS
-- ---------------------------------------------------------

CREATE TABLE mission_events (

    event_id BIGINT GENERATED ALWAYS AS IDENTITY
        PRIMARY KEY,

    mission_id BIGINT NOT NULL,

    simulation_time_seconds DOUBLE PRECISION NOT NULL,

    event_type VARCHAR(40) NOT NULL,

    severity VARCHAR(10) NOT NULL,

    event_name VARCHAR(100) NOT NULL,

    description TEXT,

    CONSTRAINT fk_event_mission
        FOREIGN KEY (mission_id)
        REFERENCES missions(mission_id)
        ON DELETE CASCADE,

    CONSTRAINT chk_event_severity
        CHECK (
            severity IN ('INFO', 'WARNING', 'CRITICAL')
        )
);



-- ---------------------------------------------------------
-- INDEXES
-- ---------------------------------------------------------

CREATE INDEX idx_telemetry_mission
ON telemetry_snapshots(mission_id);

CREATE INDEX idx_communication_mission
ON communication_logs(mission_id);

CREATE INDEX idx_events_mission
ON mission_events(mission_id);



CREATE INDEX idx_telemetry_mission_time
ON telemetry_snapshots(mission_id, simulation_time_seconds);

CREATE INDEX idx_communication_mission_time
ON communication_logs(mission_id, simulation_time_seconds);

CREATE INDEX idx_events_mission_time
ON mission_events(mission_id, simulation_time_seconds);