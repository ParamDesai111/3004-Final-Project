


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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


private:
    Ui::MainWindow *ui;
    int currentScanPoint;
    int totalScanPoints;
    bool isDeviceScanned;

private slots:
    void startScan();
    void nextScanPoint();
    void performDeviceScan();

};
#endif // MAINWINDOW_H
