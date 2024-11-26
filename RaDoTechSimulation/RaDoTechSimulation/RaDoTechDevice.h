#ifndef RADOTECHDEVICE_H
#define RADOTECHDEVICE_H

#include <vector>
#include <string>

class RaDoTechDevice
{
private:
    bool isPaired;
    int batteryLevel;

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
