#include "Battery.h"

Battery::Battery()
{
    this->batteryLevel =  100;
}

void Battery::deplete()
{
    this->batteryLevel -= 10;
    if (batteryLevel < 0) {
        batteryLevel = 0;
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
