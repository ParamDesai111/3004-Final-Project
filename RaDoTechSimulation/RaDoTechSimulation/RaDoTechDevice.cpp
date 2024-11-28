#include "RaDoTechDevice.h"
#include <cstdlib> // For generating random numbers

RaDoTechDevice::RaDoTechDevice() : isPaired(true) {}

bool RaDoTechDevice::startScan()
{
    if (battery.getBatteryLevel() <= 0) {
        return false; // Cannot start scan due to low battery
    }
    depleteBattery();
    return true;
}

std::vector<float> RaDoTechDevice::collectData()
{
    // Simulate raw data collection (random values for simplicity)
    std::vector<float> rawData(5); // Assuming 5 metrics per scan
    for (float &value : rawData) {
        value = static_cast<float>(rand() % 101); // Random values between 0 and 100
    }
    return rawData;
}

void RaDoTechDevice::depleteBattery()
{
    battery.deplete();
}

bool RaDoTechDevice::getIsPaired() const
{
    return isPaired;
}

int RaDoTechDevice::getBatteryLevel() const
{
    return battery.getBatteryLevel();
}

bool RaDoTechDevice::isBatteryLow() const
{
    return battery.isLow(); // Consider battery low if less than 20%
}
