#ifndef HEALTHDATA_H
#define HEALTHDATA_H

#include <QString>
#include <QDate>
#include <QList>

struct MeridianResult {
    QString meridian;
    QString side;
    int conductance;  // µA
    QString status;
};

struct Comments
{
    double bodyTemperature;
    QString bloodPressure;
    int heartRate;
    int sleepingTime;
    double currentWeight;
    QString emotionalState;
    QString overallFeeling;
    QString notes;
};


class HealthData {
private:
    QDate date;                  // Date of the measurement
    QList<MeridianResult> data;  // List of results for the measurement
    QList<Comments> comments;

public:
    HealthData(const QDate& date, const QList<MeridianResult>& data, const QList<Comments>& comments)
        : date(date), data(data), comments(comments) {}

    QDate getDate() const { return date; }
    QList<MeridianResult> getData() const { return data; }
    QList<Comments> getComments() const {return comments;}

    // Algorithms Functions
    // Functional Health Indicators
    double calculateEnergyLevel() const;
    double calculateImmuneSystem() const;
    double calculateMetabolism() const;
    double calculatePsychoEmotionalState() const;
    double calculateMusculoskeletalSystem() const;

    // Professional Practitioner Data
    double calculateAverageValue() const;
    double calculateLeftTotal() const;
    double calculateRightTotal() const;
    double calculateLeftRightRatio() const;
    double calculateUpperTotal() const;
    double calculateLowerTotal() const;
    double calculateUpperLowerRatio() const;
};

#endif // HEALTHDATA_H
