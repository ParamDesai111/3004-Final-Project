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
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect Device View to App View button
    connect(ui->GoToAppViewButton, &QPushButton::clicked, this, &MainWindow::showAppView);

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
    currentScanPoint = 0;
    totalScanPoints = 5; // For now, assume there are 5 points
    isDeviceScanned = false;

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

    if (currentScanPoint < totalScanPoints) {
        currentScanPoint++;
        isDeviceScanned = false;
        ui->MeasureNowLabel->setText(QString("Scan Point %1: Navigate to Device View and press Scan.").arg(currentScanPoint));
        ui->DeviceStatusLabel->setText(QString("Ready for Scan %1.").arg(currentScanPoint));
    } else {
        ui->MeasureNowLabel->setText("All scan points completed!");
    }
}


void MainWindow::performDeviceScan()
{
    isDeviceScanned = true;
    ui->DeviceStatusLabel->setText(QString("Scan %1 complete. Return to App View and press Next.").arg(currentScanPoint));
}

