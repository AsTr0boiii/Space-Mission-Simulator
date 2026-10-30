#include "CommunicationSystem.h"
#include "Physics.h"
#include "Constants.h"

CommunicationSystem::CommunicationSystem() {
    frequencyMHz = FREQUENCY_MHZ;
    transmitPowerDbm = TRANSMIT_POWER_DBM;
    antennaGainDb = ANTENNA_GAIN_DB;
    receiverGainDb = RECEIVER_GAIN_DB;
    noisePowerDbm = NOISE_POWER_DBM;

    distanceKm = 0.001;
    pathLossDb = 0.0;
    receivedPowerDbm = 0.0;
    snrDb = 0.0;
    packetLossPercent = 0.0;
    delaySeconds = 0.0;

    modulation = "16-QAM";
    linkStatus = "Stable";

}

void CommunicationSystem::updateLink(double newDistanceKm, double BatteryLevel) {
    if(newDistanceKm <= 0) {
        distanceKm = 0.001;
    }
    else {
        distanceKm = newDistanceKm;
    }

    calculatePathLoss();
    calculateReceivedPower(BatteryLevel);
    calculateSNR();
    selectModulation();
    calculatePacketLoss();
    calculateDelay();

}

void CommunicationSystem::calculatePathLoss() {
    pathLossDb = Physics::calculateFreeSpacePathLoss(distanceKm, frequencyMHz);
}

void CommunicationSystem::calculateReceivedPower(double BatterLevel) {
    double effectiveTransmitPower = transmitPowerDbm;

    if(BatterLevel <= 5.0) {
        effectiveTransmitPower -= 25.0;
    }

    else if(BatterLevel <= 20.0) {
        effectiveTransmitPower -= 10.0;
    }

    receivedPowerDbm = Physics::calculateReceivedPower(
        effectiveTransmitPower,
        antennaGainDb,
        receiverGainDb,
        pathLossDb

    );

}

void CommunicationSystem::calculateSNR() {
    snrDb = Physics::calculateSNR(receivedPowerDbm, noisePowerDbm);
}

void CommunicationSystem::selectModulation() {
    if(snrDb >= 18.0) {
        modulation = "16-QAM";
        linkStatus = "Stable";
    }
    else if(snrDb >= 10.0) {
        modulation = "QPSK";
        linkStatus = "Moderate";
    }
    else if(snrDb >= 5.0) {
        modulation = "BPSK";
        linkStatus = "Weak";
    }
    else {
        modulation = "None";
        linkStatus = "Communication Lost";
    }
}

void CommunicationSystem::calculatePacketLoss() {
    if(snrDb >= 18.0) {
        packetLossPercent = 1.0;
    } 
    else if(snrDb >= 10.0) {
        packetLossPercent = 8.0;
    }
    else if(snrDb >= 5.0) {
        packetLossPercent = 35.0;
    }
    else {
        packetLossPercent = 100.0;
    }
}

void CommunicationSystem::calculateDelay() {
    delaySeconds = Physics::calculateCommunicationDelay(distanceKm);
}

double CommunicationSystem::getDistanceKm() const {
    return distanceKm;
}

double CommunicationSystem::getPathLoss() const {
    return pathLossDb;
}

double CommunicationSystem::getReceivedPower() const {
    return receivedPowerDbm;
}

double CommunicationSystem::getSNR() const {
    return snrDb;
}

double CommunicationSystem::getPacketLoss() const {
    return packetLossPercent;
}

double CommunicationSystem::getDelay() const {
    return delaySeconds;
}

double CommunicationSystem::getFrequencyMHz() const {
    return frequencyMHz;
}

std::string CommunicationSystem::getModulation() const {
    return modulation;
}

std::string CommunicationSystem::getLinkStatus() const {
    return linkStatus;
}