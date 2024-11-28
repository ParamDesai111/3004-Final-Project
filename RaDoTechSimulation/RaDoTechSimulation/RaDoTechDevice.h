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

    // Getters
    bool getIsPaired() const;
    int getBatteryLevel() const;

    // Utility
    bool isBatteryLow() const;
};

#endif // RADOTECHDEVICE_H
