#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "User.h"
#include "HealthData.h"
#include "RaDoTechDevice.h"
#include "DataProcessor.h"

#include <qtimer.h>

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

private slots:
    void handleLogin(); // Slot for login button
    void populateHistoryList();  // Populate history on Historical Page
    void viewDetails();

    void startScan();
    void nextScanPoint();
    void performDeviceScan();

private:
    Ui::MainWindow *ui;
    QList<User*> presetUsers; // List of preset users
    User* currentUser;
    void createPresetUsers(); // Initialize preset users


    // Device and Measure
    int currentScanPoint;
    int totalScanPoints;
    bool isDeviceScanned;

    RaDoTechDevice device;
    DataProcessor processor;

    void updateBatteryLevelLabel();
    void updateProcessedDataUI(const std::map<std::string, float>& processedData);

    QTimer* timer;

};
#endif // MAINWINDOW_H
