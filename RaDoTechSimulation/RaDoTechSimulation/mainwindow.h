#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void showHistoricalPage();
    void showProfilePage();
    void showVisualizationPage();
    void showCreateProfilePage();
    void showLoginPage();
    void showBarGraph();
    void showRadarChart();
    void showMeasureView();


private:
    Ui::MainWindow *ui;
    int currentScanPoint;
    int totalScanPoints;
    bool isDeviceScanned;

    RaDoTechDevice device;
    DataProcessor processor;

    void updateBatteryLevelLabel();
    void updateProcessedDataUI(const std::map<std::string, float>& processedData);

private slots:
    void startScan();
    void nextScanPoint();
    void performDeviceScan();

};
#endif // MAINWINDOW_H
