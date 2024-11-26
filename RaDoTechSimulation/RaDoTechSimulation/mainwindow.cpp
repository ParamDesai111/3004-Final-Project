#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>

#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QtCharts/QPolarChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QLineSeries>
#include <QChartView>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), currentScanPoint(0), totalScanPoints(5),isDeviceScanned(false)
{
    ui->setupUi(this);

    // Connect Device View to App View button
    connect(ui->GoToAppViewButton, &QPushButton::clicked, this, &MainWindow::showAppView);
    connect(ui->GoToMeasureViewButton, &QPushButton::clicked, this, &MainWindow::showMeasureView);

    // Connect App View to Device View button
    connect(ui->GoToDeviceViewButton, &QPushButton::clicked, this, &MainWindow::showDeviceView);

    // Connect App View buttons to respective pages
    connect(ui->HomePageButton, &QPushButton::clicked, this, &MainWindow::showHomePage);
    connect(ui->MeasureNowButton, &QPushButton::clicked, this, &MainWindow::showMeasureNowPage);
    connect(ui->HistoricalPageButton, &QPushButton::clicked, this, &MainWindow::showHistoricalPage);
    connect(ui->ProfilePageButton, &QPushButton::clicked, this, &MainWindow::showProfilePage);
    connect(ui->VisulizationPageButton, &QPushButton::clicked, this, &MainWindow::showVisualizationPage);
    connect(ui->CreateProfileButton, &QPushButton::clicked, this, &MainWindow::showCreateProfilePage);
    connect(ui->EnterButton, &QPushButton::clicked, this, &MainWindow::showLoginPage);
    connect(ui->StartScanButton, &QPushButton::clicked, this, &MainWindow::startScan);
    connect(ui->NextButton, &QPushButton::clicked, this, &MainWindow::nextScanPoint);
    connect(ui->DeviceScanButton, &QPushButton::clicked, this, &MainWindow::performDeviceScan);


    //Images
    QPixmap pix("/home/student/Desktop/FinalProject/3004-Final-Project/images/loginImage.png");
    ui->loginImage->setPixmap(pix.scaled(81,71,Qt::KeepAspectRatio));

    updateBatteryLevelLabel();

}

MainWindow::~MainWindow()
{
    delete ui;
}


// Switch to App View
void MainWindow::showAppView()
{
    // TODO: When we implement login and create profile, here we do if statement of which view to first go to after device view
    ui->ViewsStackedWidget->setCurrentWidget(ui->AppStartPage);
}

// Switch to Device View
void MainWindow::showDeviceView()
{
    ui->ViewsStackedWidget->setCurrentWidget(ui->DeviceView);
}

// Show Home Page in App View
void MainWindow::showHomePage()
{
    ui->AppStackedWidget->setCurrentWidget(ui->HomePage);
}

void MainWindow::showCreateProfilePage()
{
    ui->ViewsStackedWidget->setCurrentWidget(ui->AppView);
    ui->AppStackedWidget->setCurrentWidget(ui->CreateProfilePage);

}

void MainWindow::showLoginPage()
{
    ui->ViewsStackedWidget->setCurrentWidget(ui->AppView);
    ui->AppStackedWidget->setCurrentWidget(ui->LoginPage);
}

// Show Measure Now Page in App View
void MainWindow::showMeasureNowPage()
{
    ui->AppStackedWidget->setCurrentWidget(ui->MeasureNowPage);
}

// Show Historical Data Page in App View
void MainWindow::showHistoricalPage()
{
    ui->AppStackedWidget->setCurrentWidget(ui->HistoricalPage);
}

// Show Profile Page in App View
void MainWindow::showProfilePage()
{
    ui->AppStackedWidget->setCurrentWidget(ui->ProfilePage);
}

// Show Visualization Page in App View
void MainWindow::showVisualizationPage()
{
    ui->AppStackedWidget->setCurrentWidget(ui->VisulizationPage);
//    showBarGraph();
    showRadarChart();
}


// Just to show Bar Graph
void MainWindow::showBarGraph()
{
    // Create a new bar set and populate it with data
    QBarSet *set0 = new QBarSet("Organ 1");
    QBarSet *set1 = new QBarSet("Organ 2");
    QBarSet *set2 = new QBarSet("Organ 3");

    // Sample data values
    *set0 << 70 << 85 << 60;
    *set1 << 55 << 90 << 40;
    *set2 << 65 << 75 << 50;

    // Create a bar series and add the sets
    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);

    // Create a chart and add the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Organ Health Metrics");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Define categories (x-axis labels)
    QStringList categories;
    categories << "Scan 1" << "Scan 2" << "Scan 3";

    // Create category axis (x-axis)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Create value axis (y-axis)
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100); // Set the range for y-axis
    axisY->setTitleText("Health Metric (%)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Remove legend for simplicity
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create a QChartView and set it as the central widget
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Add the chart to the placeholder widget (chartContainer)
    QVBoxLayout *layout = new QVBoxLayout(ui->chartContainer);
    layout->addWidget(chartView);
    ui->chartContainer->setLayout(layout);
}

void MainWindow::showRadarChart()
{
    // Create a polar chart
    QtCharts::QPolarChart *polarChart = new QtCharts::QPolarChart();
    polarChart->setTitle("Organ Health Radar Chart");

    // Sample Data for the Radar Chart
    QVector<double> leftSideData = {85, 70, 90, 95, 80};
    QVector<double> rightSideData = {75, 85, 70, 90, 88};
    QVector<double> averageValues = {80, 78, 80, 92, 84};

    // Labels for the axes
    QStringList categories = {"Heart", "Liver", "Lungs", "Kidneys", "Stomach"};

    // Create a Value Axis (for the circular range)
    QtCharts::QValueAxis *valueAxis = new QtCharts::QValueAxis();
    valueAxis->setRange(0, 100); // Scale from 0 to 100
    valueAxis->setTickCount(6);  // Add tick marks
    valueAxis->setLabelFormat("%.0f");
    valueAxis->setTitleText("Health Metric (%)");
    polarChart->addAxis(valueAxis, QtCharts::QPolarChart::PolarOrientationRadial);

    // Create a Category Axis (for the angular labels)
    QtCharts::QCategoryAxis *angularAxis = new QtCharts::QCategoryAxis();
    angularAxis->setLabelsPosition(QtCharts::QCategoryAxis::AxisLabelsPositionOnValue);
    for (int i = 0; i < categories.size(); ++i)
    {
        angularAxis->append(categories[i], i);
    }
    angularAxis->setRange(0, categories.size());
    polarChart->addAxis(angularAxis, QtCharts::QPolarChart::PolarOrientationAngular);

    // Create Line Series for each data set
    QtCharts::QLineSeries *leftSeries = new QtCharts::QLineSeries();
    leftSeries->setName("Left Side");
    for (int i = 0; i < leftSideData.size(); ++i)
    {
        leftSeries->append(i, leftSideData[i]);
    }
    polarChart->addSeries(leftSeries);
    leftSeries->attachAxis(valueAxis);
    leftSeries->attachAxis(angularAxis);

    QtCharts::QLineSeries *rightSeries = new QtCharts::QLineSeries();
    rightSeries->setName("Right Side");
    for (int i = 0; i < rightSideData.size(); ++i)
    {
        rightSeries->append(i, rightSideData[i]);
    }
    polarChart->addSeries(rightSeries);
    rightSeries->attachAxis(valueAxis);
    rightSeries->attachAxis(angularAxis);

    QtCharts::QLineSeries *averageSeries = new QtCharts::QLineSeries();
    averageSeries->setName("Average Values");
    for (int i = 0; i < averageValues.size(); ++i)
    {
        averageSeries->append(i, averageValues[i]);
    }
    polarChart->addSeries(averageSeries);
    averageSeries->attachAxis(valueAxis);
    averageSeries->attachAxis(angularAxis);

    // Create a Chart View for display
    QtCharts::QChartView *chartView = new QtCharts::QChartView(polarChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Add the chart to the placeholder widget (chartContainer)
    QVBoxLayout *layout = new QVBoxLayout(ui->chartContainer);
    layout->addWidget(chartView);
    ui->chartContainer->setLayout(layout);
}

void MainWindow::showMeasureView(){
    ui->ViewsStackedWidget->setCurrentWidget(ui->AppView);
    showMeasureNowPage();
}

void MainWindow::startScan()
{
    currentScanPoint = 1;
    isDeviceScanned = false;
    ui->MeasureNowLabel->setText("Scan Point 1: Navigate to Device View and press Scan.");
    ui->DeviceStatusLabel->setText("Ready for Scan 1.");
}


void MainWindow::nextScanPoint()
{
    if (!isDeviceScanned) {
        ui->MeasureNowLabel->setText("Please perform the scan on the device first.");
        return;
    }

    // Process data after a valid scan
    std::map<std::string, float> processedData = processor.processData();


    if (currentScanPoint < totalScanPoints) {
        currentScanPoint++;
        isDeviceScanned = false;
        ui->MeasureNowLabel->setText(QString("Scan Point %1: Navigate to Device View and press Scan.").arg(currentScanPoint));
        ui->DeviceStatusLabel->setText(QString("Ready for Scan %1.").arg(currentScanPoint));
    } else {
        ui->MeasureNowLabel->setText("All scan points completed!");
        updateProcessedDataUI(processedData);
    }
}



void MainWindow::performDeviceScan()
{
    if (!device.startScan()) {
        ui->DeviceStatusLabel->setText("Low battery. Cannot perform scan.");
        return;
    }

    // Collect data from the device
    std::vector<float> rawData = device.collectData();
    processor.setRawData(rawData);

    if (processor.validateData()) {
        isDeviceScanned = true;
        ui->DeviceStatusLabel->setText(QString("Scan %1 complete. Return to App View and press Next.").arg(currentScanPoint));
    } else {
        ui->DeviceStatusLabel->setText("Scan failed. Please try again.");
    }

    updateBatteryLevelLabel();
}


void MainWindow::updateBatteryLevelLabel()
{
    int batteryLevel = device.getBatteryLevel();

    // Update the progress bar value
    ui->BatteryPowerProgressBar->setValue(batteryLevel);

    // Change the progress bar color based on the battery level
    if (device.isBatteryLow()) {
        ui->BatteryPowerProgressBar->setStyleSheet(
            "QProgressBar::chunk { background-color: red; }"
            "QProgressBar { border: 1px solid gray; border-radius: 3px; text-align: center; }"
        );
    } else {
        ui->BatteryPowerProgressBar->setStyleSheet(
            "QProgressBar::chunk { background-color: green; }"
            "QProgressBar { border: 1px solid gray; border-radius: 3px; text-align: center; }"
        );
    }
}

void MainWindow::updateProcessedDataUI(const std::map<std::string, float>& processedData)
{
    QString dataText;
    for (const auto& [organ, value] : processedData) {
        dataText += QString("%1: %2%\n").arg(QString::fromStdString(organ)).arg(value);
    }
    ui->ProcessedDataLabel->setText(dataText);
}



