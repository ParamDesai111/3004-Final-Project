#include "Battery.h"

Battery::Battery()
{
    this->batteryLevel =  100;
}

void Battery::deplete()
{
    this->batteryLevel -= 5;
    if (batteryLevel < 0) {
        batteryLevel = 0;
    }
}

void Battery::charge()
{
    this->batteryLevel += 5;
    if (batteryLevel >= 100) {
        batteryLevel = 100;
    }
}

int Battery::getBatteryLevel() const
{
    return this->batteryLevel;
}

bool Battery::isLow() const
{
    return this->batteryLevel <= 20;
}
