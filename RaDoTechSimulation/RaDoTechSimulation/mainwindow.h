#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "User.h"
#include "HealthData.h"
#include "RaDoTechDevice.h"
#include "DataProcessor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Switching View UI Functions
    void showAppView();
    void showDeviceView();

    // Switching App Pages UI Functions
    void showHomePage();

    void showMeasureNowPage();
    void showMeasureView();

    void showHistoricalPage();
    void showProfilePage();
    void showVisualizationPage();
    void showCreateProfilePage();
    void showLoginPage();
    void showBarGraph();
    void showRadarChart();

    void saveProfile(); // Save a new profile for the user
    void showProfiles(); // Display all profiles for the user
    void updateProfilesList();
    void goToCreateProfilePage();


    void showPersonalInfoPage();
    void saveResults();

private slots:
    void handleLogin(); // Slot for login button
    void populateHistoryList();  // Populate history on Historical Page
    void viewDetails();
    void populateIndicators(HealthData* selectedData);
    void startScan();
    void nextScanPoint();
    void performDeviceScan();

private:
    Ui::MainWindow *ui;
    QList<User*> presetUsers; // List of preset users
    User* currentUser;
    void createPresetUsers(); // Initialize preset users

//    void setProcessedData(std::map<std::string, float>* processedDataResults);
//    std::map<std::string, float> getProcessedData() { return processedData;}

    // Device and Measure
    int currentScanPoint;
    int totalScanPoints;
    bool isDeviceScanned;
    Profile* currProfile;

    RaDoTechDevice device;
    DataProcessor processor;
    std::map<std::string, float> processedData;
    QString getClassification(double value, double min, double max);

    void updateProcessedDataUI(const std::map<std::string, float>& processedData);
    QList<MeridianResult> convertProcessedDataToMeridianResults(const std::map<std::string, float>& processedData);

    QTimer* batteryTimer;
    QTimer* chargedBatteryTimer;

    void updateBatteryLevelLabel();
    void powerDevice();
    void shutDownDevice();
    void UpdateChargedBatteryLevelLabel();
    void ChargeBattery();
    void PairUp();


};
#endif // MAINWINDOW_H
