#pragma once 

#include <string>

class CommunicationSystem {
    private:
    double frequencyMHz;
    double transmitPowerDbm;
    double antennaGainDb;
    double receiverGainDb;
    double noisePowerDbm;

    double distanceKm;
    double pathLossDb;
    double receivedPowerDbm;
    double snrDb;
    double packetLossPercent;
    double delaySeconds;

    std::string modulation;
    std::string linkStatus;

    public:
    CommunicationSystem();

    void updateLink(double newDistanceKm, double BatteryLevel);

    void calculatePathLoss();
    void calculateReceivedPower(double BatteryLevel);
    void calculateSNR();
    void selectModulation();
    void calculatePacketLoss();
    void calculateDelay();

    double getDistanceKm() const;
    double getPathLoss() const;
    double getReceivedPower() const;
    double getSNR() const;
    double getPacketLoss() const;
    double getDelay() const;
    double getFrequencyMHz() const;

    std::string getModulation() const;
    std::string getLinkStatus() const;

};