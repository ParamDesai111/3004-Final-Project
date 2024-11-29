#ifndef BATTERY_H
#define BATTERY_H


class Battery
{
    public:
        Battery();
        void deplete();
        void charge();
        int getBatteryLevel() const;
        bool isLow() const;
    private:
        int batteryLevel;
};

#endif // BATTERY_H
