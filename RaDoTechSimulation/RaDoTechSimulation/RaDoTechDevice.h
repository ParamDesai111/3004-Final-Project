#ifndef RADOTECHDEVICE_H
#define RADOTECHDEVICE_H

#include <vector>
#include <string>

#include "Battery.h"

class RaDoTechDevice
{
private:
    bool isPaired;
    Battery battery;

public:
    RaDoTechDevice();

    // Functions
    bool startScan();
    std::vector<float> collectData();
    void depleteBattery();
    void chargeBattery();

    // Getters
    bool getIsPaired() const;
    int getBatteryLevel() const;

    // Utility
    bool isBatteryLow() const;
    void PairUp();
};

#endif // RADOTECHDEVICE_H
