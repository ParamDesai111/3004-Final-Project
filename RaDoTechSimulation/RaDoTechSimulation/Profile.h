#ifndef PROFILE_H
#define PROFILE_H

#include <QString>
#include <QDate>
#include "HealthData.h"

class Profile {
private:
    QString name;
    QString gender;
    double weight;
    double height;
    QDate dateOfBirth;
    QList<HealthData*> history;  // Historical health data
    Profile* selectedProfile;

public:
    Profile(const QString& name, const QString& gender, double weight, double height, const QDate& dob)
        : name(name), gender(gender), weight(weight), height(height), dateOfBirth(dob) {}

    QString getName() const { return name; }
    QString getGender() const { return gender; }
    double getWeight() const { return weight; }
    double getHeight() const { return height; }
    QDate getDateOfBirth() const { return dateOfBirth; }

    QList<HealthData*> getHistory() const { return history; }

    void addHealthData(HealthData* data) { history.append(data); }
    void setSelectedProfile(Profile* profile) {selectedProfile = profile; }
    Profile* getSelectedProfile() {return selectedProfile;}

};

#endif // PROFILE_H
