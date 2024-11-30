#include "HealthData.h"
#include <QDebug>
// Helper function to filter meridian results by name prefix
static QList<MeridianResult> filterByPrefix(const QList<MeridianResult>& data, const QString& prefix) {
    QList<MeridianResult> filtered;
    for (const MeridianResult& result : data) {
        if (result.meridian.startsWith(prefix)) {
            filtered.append(result);
        }
    }
    return filtered;
}

// Helper function to calculate sum for a specific side
static double calculateSideSum(const QList<MeridianResult>& data, const QString& side) {
    double sum = 0.0;
    for (const MeridianResult& result : data) {
        if (result.side == side) {
            sum += result.conductance;
        }
    }
    return sum;
}

// Scaling factors for normalization
static const double ENERGY_LEVEL_SCALING_FACTOR = 2.78;
static const double IMMUNE_SYSTEM_SCALING_FACTOR = 52.1;
static const double PSYCHO_EMOTIONAL_SCALING_FACTOR = 80.71;
static const double MUSCULOSKELETAL_SCALING_FACTOR = 86.0;

// Functional Health Indicators
double HealthData::calculateEnergyLevel() const {
    double leftTotal = calculateSideSum(data, "Left");
    double rightTotal = calculateSideSum(data, "Right");
    qDebug()<<"Left Total: "<< leftTotal<<" Right Total: "<<rightTotal;
    double total = leftTotal + rightTotal;
    qDebug()<<"Total: " <<total / (data.size() * ENERGY_LEVEL_SCALING_FACTOR);
    return total / (data.size() * ENERGY_LEVEL_SCALING_FACTOR); // Normalized Energy Level
}

double HealthData::calculateImmuneSystem() const {
    QList<MeridianResult> upperBody = filterByPrefix(data, "H");
    double leftTotal = calculateSideSum(upperBody, "Left");
    double rightTotal = calculateSideSum(upperBody, "Right");
    double upperTotal = leftTotal + rightTotal;
    return upperTotal / (6 * IMMUNE_SYSTEM_SCALING_FACTOR); // Normalize by upper meridians and scaling factor
}

double HealthData::calculateMetabolism() const {
    double leftTotal = calculateSideSum(data, "Left");
    double rightTotal = calculateSideSum(data, "Right");
    return leftTotal / rightTotal; // Ratio of left to right energy
}

double HealthData::calculatePsychoEmotionalState() const {
    QList<MeridianResult> emotionalMeridians = filterByPrefix(data, "H1");
    emotionalMeridians.append(filterByPrefix(data, "H2"));
    double sum = 0.0;
    for (const MeridianResult& result : emotionalMeridians) {
        sum += result.conductance;
    }
    return sum / (emotionalMeridians.size() * PSYCHO_EMOTIONAL_SCALING_FACTOR); // Normalize by scaling factor
}

double HealthData::calculateMusculoskeletalSystem() const {
    QList<MeridianResult> lowerBody = filterByPrefix(data, "F");
    double leftTotal = calculateSideSum(lowerBody, "Left");
    double rightTotal = calculateSideSum(lowerBody, "Right");
    double lowerTotal = leftTotal + rightTotal;
    return lowerTotal / (6 * MUSCULOSKELETAL_SCALING_FACTOR); // Normalize by lower meridians and scaling factor
}

// Professional Practitioner Data
double HealthData::calculateAverageValue() const {
    double leftTotal = calculateSideSum(data, "Left");
    double rightTotal = calculateSideSum(data, "Right");
    return (leftTotal + rightTotal) / (data.size() * ENERGY_LEVEL_SCALING_FACTOR); // Average of all conductance values
}

double HealthData::calculateLeftTotal() const {
    return calculateSideSum(data, "Left"); // Sum of left-side meridians
}

double HealthData::calculateRightTotal() const {
    return calculateSideSum(data, "Right"); // Sum of right-side meridians
}

double HealthData::calculateLeftRightRatio() const {
    double leftTotal = calculateLeftTotal();
    double rightTotal = calculateRightTotal();
    return leftTotal / rightTotal; // Ratio of left to right totals
}

double HealthData::calculateUpperTotal() const {
    QList<MeridianResult> upperBody = filterByPrefix(data, "H");
    double leftTotal = calculateSideSum(upperBody, "Left");
    double rightTotal = calculateSideSum(upperBody, "Right");
    return leftTotal + rightTotal; // Total energy for upper body meridians
}

double HealthData::calculateLowerTotal() const {
    QList<MeridianResult> lowerBody = filterByPrefix(data, "F");
    double leftTotal = calculateSideSum(lowerBody, "Left");
    double rightTotal = calculateSideSum(lowerBody, "Right");
    return leftTotal + rightTotal; // Total energy for lower body meridians
}

double HealthData::calculateUpperLowerRatio() const {
    double upperTotal = calculateUpperTotal();
    double lowerTotal = calculateLowerTotal();
    return upperTotal / lowerTotal; // Ratio of upper to lower body totals
}
