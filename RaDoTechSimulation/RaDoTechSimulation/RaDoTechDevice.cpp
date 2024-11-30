#include "RaDoTechDevice.h"
#include <cstdlib> // For generating random numbers

RaDoTechDevice::RaDoTechDevice() : isPaired(false) {}

bool RaDoTechDevice::startScan()
{
    if (battery.getBatteryLevel() <= 0) {
        return false; // Cannot start scan due to low battery
    }

    return true;
}

std::vector<float> RaDoTechDevice::collectData()
{
    // Simulate raw data collection (random values for simplicity)
    std::vector<float> rawData(24); // Assuming 24 metrics per scan
    for (float &value : rawData) {
        value = static_cast<float>(rand() % 91); // Random values between 0 and 100
    }
    return rawData;
}

void RaDoTechDevice::depleteBattery()
{
    battery.deplete();
}

void RaDoTechDevice::chargeBattery()
{
    battery.charge();
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

void RaDoTechDevice::PairUp()
{
    this->isPaired = true;
}
