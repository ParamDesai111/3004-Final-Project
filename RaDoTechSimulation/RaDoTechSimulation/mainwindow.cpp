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

    // Connect App View to Device View button
    connect(ui->GoToDeviceViewButton, &QPushButton::clicked, this, &MainWindow::showDeviceView);

    // Connect App View buttons to respective pages
    connect(ui->HomePageButton, &QPushButton::clicked, this, &MainWindow::showHomePage);
    connect(ui->MeasureNowButton, &QPushButton::clicked, this, &MainWindow::showMeasureNowPage);
    connect(ui->HistoryPageButton, &QPushButton::clicked, this, &MainWindow::showHistoricalPage);
    connect(ui->ProfilePageButton, &QPushButton::clicked, this, &MainWindow::showProfilePage);
    connect(ui->VisulizationPageButton, &QPushButton::clicked, this, &MainWindow::showVisualizationPage);
    connect(ui->CreateProfileButton, &QPushButton::clicked, this, &MainWindow::showCreateProfilePage);
    connect(ui->EnterButton, &QPushButton::clicked, this, &MainWindow::showLoginPage);

    //Images
    QPixmap loginImage("/home/student/Desktop/FinalProject/3004-Final-Project/images/loginImage.png");
    ui->loginImage->setPixmap(loginImage.scaled(81,71,Qt::KeepAspectRatio));
    QPixmap Energy("/home/student/Desktop/FinalProject/3004-Final-Project/images/EnergyLevel.png");
    ui->EnergyImage->setPixmap(Energy.scaled(71,41,Qt::KeepAspectRatio));
    QPixmap Immune("/home/student/Desktop/FinalProject/3004-Final-Project/images/ImmuneSystem.png");
    ui->ImmuneSystemImage->setPixmap(Immune.scaled(81,71,Qt::KeepAspectRatio));
    QPixmap Metabolism("/home/student/Desktop/FinalProject/3004-Final-Project/images/Metabolism.png");
    ui->MetabolismImage->setPixmap(Metabolism.scaled(71,41,Qt::KeepAspectRatio));


    //Save Button on Create Button
    connect(ui->SavePushButton, &QPushButton::clicked, this, &MainWindow::saveProfile);
//    currentUser = new User("example@example.com", "password123");
    connect(ui->AddNewProfileButton, &QPushButton::clicked, this, &MainWindow::goToCreateProfilePage);

    // Initialize preset users
    createPresetUsers();

    // Connect login button
    connect(ui->EnterLoginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);

    connect(ui->ViewDetailsButton, &QPushButton::clicked, this, &MainWindow::viewDetails);

    ui->ResultsTabWidget->setTabText(0, "Visulization");
    ui->ResultsTabWidget->setTabText(1, "Indicator");
    ui->ResultsTabWidget->setTabText(2, "Comments");
    ui->ResultsTabWidget->setTabText(3, "Recommendation");

    // Device Scan
    connect(ui->StartScanButton, &QPushButton::clicked, this, &MainWindow::startScan);
    connect(ui->NextButton, &QPushButton::clicked, this, &MainWindow::nextScanPoint);
    connect(ui->DeviceScanButton, &QPushButton::clicked, this, &MainWindow::performDeviceScan);
    connect(ui->GoToMeasureViewButton, &QPushButton::clicked, this, &MainWindow::showMeasureView);
    connect(ui->onButton, &QPushButton::clicked, this, &MainWindow::powerDevice);
    connect(ui->offButton, &QPushButton::clicked, this, &MainWindow::shutDownDevice);

    batteryTimer = new QTimer(this);
    batteryTimer->setInterval(15000);
    connect(batteryTimer, &QTimer::timeout, this, &MainWindow::updateBatteryLevelLabel);


    powerDevice();

}

MainWindow::~MainWindow()
{
    delete ui;
}

// depeletes battery label on ui
void MainWindow::updateBatteryLevelLabel()
{
    this->device.depleteBattery();
    ui->BatteryPowerProgressBar->setValue(device.getBatteryLevel());

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

void MainWindow::powerDevice()
{
    this->batteryTimer->start();
    ui->DeviceScanButton->setDisabled(false);
    ui->onButton->setDisabled(true);
    ui->offButton->setDisabled(false);
}

void MainWindow::shutDownDevice()
{
    this->batteryTimer->stop();
    ui->DeviceScanButton->setDisabled(true);
    ui->offButton->setDisabled(true);
    ui->onButton->setDisabled(false);
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
    populateHistoryList();
}

// Show Profile Page in App View
void MainWindow::showProfilePage()
{
    ui->AppStackedWidget->setCurrentWidget(ui->ProfilePage);
    updateProfilesList();
}

// Show Visualization Page in App View
void MainWindow::showVisualizationPage()
{
    ui->AppStackedWidget->setCurrentWidget(ui->VisulizationPage);
    showBarGraph();
//    showRadarChart();updateBatteryLevelLabel();
}

void MainWindow::goToCreateProfilePage() {
    // Clear all input fields
    ui->ProfileNameTextEdit->clear();
    ui->GenderComboBox->setCurrentIndex(0); // Reset dropdown to the first option
    ui->WeightTextEdit->clear();
    ui->HeightTextEdit->clear();
    ui->DOBEdit->setDate(QDate::currentDate()); // Reset to current date
    ui->LoginTextEdit->setText(currentUser ? currentUser->getEmail() : "");
    ui->PasswordTextEditCreate->setText(currentUser ? currentUser->getPassword() : "");

    // Navigate to the Create Profile Page
    ui->AppStackedWidget->setCurrentWidget(ui->CreateProfilePage);
}


void MainWindow::saveProfile() {
    // Collect data for User

    QString email = ui->LoginTextEdit->toPlainText();
    QString password = ui->PasswordTextEditCreate->toPlainText();

    // Validate User data

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Email and password cannot be empty.");
        return;
    }

    // Create or update the User

    if (!currentUser) {
        currentUser = new User(email, password);
    }

    // Collect data for Profile

    QString name = ui->ProfileNameTextEdit->toPlainText();
    QString gender = ui->GenderComboBox->currentText();
    double weight = ui->WeightTextEdit->toPlainText().toDouble();
    double height = ui->HeightTextEdit->toPlainText().toDouble();
    QDate dob = ui->DOBEdit->date();

    // Validate Profile data

    if (name.isEmpty() || gender.isEmpty() || weight <= 0 || height <= 0) {
        QMessageBox::warning(this, "Input Error", "All profile fields must be filled correctly.");
        return;
    }

    // Create and associate the Profile with the User

    Profile* newProfile = new Profile(name, gender, weight, height, dob);
    currentUser->addProfile(newProfile);

    // Show success message

    QMessageBox::information(this, "Profile Created", "Profile and user created successfully!");

    // Navigate to the Home Page
    qDebug()<<"Name: "<<name<<"Gender: "<<gender<<" Weight: "<<weight<<" Height: "<<height<<" Dob: "<<dob;

    // Populate profiles on the Profiles Page
    updateProfilesList();

    // Navigate to the Profiles Page
    ui->AppStackedWidget->setCurrentWidget(ui->ProfilePage);
}

void MainWindow::updateProfilesList() {
    if (!currentUser) {
        ui->ProfileListLabel->setText("No profiles found.");
        return;
    }

    // Retrieve profiles
    QList<Profile*> profiles = currentUser->getProfiles();

    qDebug()<<profiles.size();

    // Build profile list as a string
    QString profilesText;
    for (int i = 0; i < profiles.size(); ++i) {
        Profile* profile = profiles[i];
        profilesText += QString("Current User: %1\n").arg(currentUser->getEmail());

        profilesText += QString("Profile %1:\n").arg(i + 1);
        profilesText += QString("Name: %1\n").arg(profile->getName());
        profilesText += QString("Gender: %1\n").arg(profile->getGender());
        profilesText += QString("Weight: %1 kg\n").arg(profile->getWeight());
        profilesText += QString("Height: %1 cm\n").arg(profile->getHeight());
        profilesText += QString("Date of Birth: %1\n\n").arg(profile->getDateOfBirth().toString("yyyy-MM-dd"));
    }

    // Update the QLabel
    ui->ProfileListLabel->setText(profilesText);
}


void MainWindow::showProfiles() {
    if (!currentUser) return;

    QString profileList = "Profiles:\n";
    for (Profile* profile : currentUser->getProfiles()) {
        profileList += profile->getName() + " (" + profile->getGender() + ")\n";
    }

    QMessageBox::information(this, "User Profiles", profileList);
}

//TODO: THIS PRESET MAKE BETTER WITH DATA I HAVE, JUST FOR DEMO RIGHT NOW
void MainWindow::createPresetUsers() {
    // Create users with a test profile
    User* user1 = new User("test1@example.com", "password1");
//    User* user2 = new User("test2@example.com", "password2");

    Profile* profile1 = new Profile("Profile1ForTest", "Male", 70.0, 175.0, QDate(1990, 1, 1));
    user1->addProfile(profile1);
    Profile* profile2 = new Profile("Profile2ForTest", "Female", 89.0, 180.0, QDate(2004,1,1));
    user1->addProfile(profile2);

    QList<MeridianResult> results = {
        {"H1 (Lung)", "Left", 115, "Normal"},
        {"H1 (Lung)", "Right", 125, "Normal"},
        {"H2 (Pericardium)", "Left", 80, "Low (Deficient)"},
        {"H2 (Pericardium)", "Right", 90, "Normal"},
        {"H3 (Heart)", "Left", 130, "High (Excess)"},
        {"H3 (Heart)", "Right", 120, "Normal"},
        {"H4 (Small Intestine)", "Left", 75, "Low (Deficient)"},
        {"H4 (Small Intestine)", "Right", 78, "Low (Deficient)"},
        {"H5 (Lymph)", "Left", 95, "Normal"},
        {"H5 (Lymph)", "Right", 100, "Normal"},
        {"H6 (Large Intestine)", "Left", 85, "Low (Deficient)"},
        {"H6 (Large Intestine)", "Right", 115, "Normal"},
        {"F1 (Spleen)", "Left", 125, "High (Excess)"},
        {"F1 (Spleen)", "Right", 140, "High (Excess)"},
        {"F2 (Liver)", "Left", 105, "Normal"},
        {"F2 (Liver)", "Right", 110, "Normal"},
        {"F3 (Kidney)", "Left", 95, "Normal"},
        {"F3 (Kidney)", "Right", 85, "Low (Deficient)"},
        {"F4 (Bladder)", "Left", 110, "Normal"},
        {"F4 (Bladder)", "Right", 100, "Normal"},
        {"F5 (Gallbladder)", "Left", 90, "Normal"},
        {"F5 (Gallbladder)", "Right", 85, "Low (Deficient)"},
        {"F6 (Stomach)", "Left", 78, "Low (Deficient)"},
        {"F6 (Stomach)", "Right", 135, "High (Excess)"}
    };


    QList<MeridianResult> results1 = {
            {"H1 (Lung)", "Left", 1414, "Normal"},
            {"H1 (Lung)", "Right", 1414, "Normal"}
    };

    HealthData* healthData = new HealthData(QDate::currentDate(), results);
    profile1->addHealthData(healthData);

    QDate currentDate = QDate::currentDate();
    QDate tomorrow = currentDate.addDays(1);

    HealthData* healthData1 = new HealthData(tomorrow,results1);
    profile2->addHealthData(healthData1);
//    user2->addProfile(profile1); // Both users share the same test profile

    presetUsers.append(user1);
//    presetUsers.append(user2);
}

void MainWindow::handleLogin() {
    // Get input from login fields
    QString email = ui->LoginEmailTextEdit->toPlainText();
    QString password = ui->PasswordTextEdit->toPlainText();

    // Validate credentials
    User* user = User::validateCredentials(email, password, presetUsers);

    if (user) {
        // Successful login
        currentUser = user;
        QMessageBox::information(this, "Login Successful", "Welcome to the app!");

        // Navigate to Home Page
        showProfiles();
        ui->AppStackedWidget->setCurrentWidget(ui->HomePage);
    } else {
        // Failed login
        QMessageBox::critical(this, "Login Failed", "Incorrect email or password. Please try again.");
    }
}

void MainWindow::populateHistoryList() {
    if (!currentUser || currentUser->getProfiles().isEmpty()) {
        ui->HistoryListWidget->clear();
        QMessageBox::information(this, "No Data", "No profiles or health data available.");
        return;
    }

    // Clear the QListWidget before populating
    ui->HistoryListWidget->clear();

    // Iterate through all profiles of the current user
    for (Profile* profile : currentUser->getProfiles()) {
        if (!profile) continue;

        // Iterate through each HealthData entry in the profile's history
        for (HealthData* data : profile->getHistory()) {
            if (!data) continue;

            // Combine profile name and date for display
            QString itemText = QString("%1 - %2")
                                   .arg(profile->getName())
                                   .arg(data->getDate().toString("yyyy-MM-dd"));

            // Add the item to the QListWidget
            ui->HistoryListWidget->addItem(itemText);
        }
    }
}

void MainWindow::viewDetails() {
    if (!currentUser || currentUser->getProfiles().isEmpty()) {
        QMessageBox::critical(this, "Error", "No profiles or health data available.");
        return;
    }

    // Get the selected item
    QListWidgetItem* selectedItem = ui->HistoryListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "No Selection", "Please select a record.");
        return;
    }

    // Extract the profile name and date from the selected item's text
    QStringList parts = selectedItem->text().split(" - ");
    if (parts.size() != 2) {
        QMessageBox::critical(this, "Error", "Invalid selection format.");
        return;
    }

    QString selectedProfileName = parts[0];
    QString selectedDate = parts[1];

    // Search for the corresponding HealthData in all profiles
    HealthData* selectedData = nullptr;
    for (Profile* profile : currentUser->getProfiles()) {
        if (!profile || profile->getName() != selectedProfileName) {
            continue;
        }

        for (HealthData* data : profile->getHistory()) {
            if (data->getDate().toString("yyyy-MM-dd") == selectedDate) {
                selectedData = data;
                break;
            }
        }

        if (selectedData) {
            break;
        }
    }

    if (!selectedData) {
        QMessageBox::critical(this, "Error", "Data not found.");
        return;
    }

    // Build the detailed results text
    QString detailsText;
    for (const MeridianResult& result : selectedData->getData()) {
        detailsText += QString("%1 (%2): %3 µA, %4\n")
                           .arg(result.meridian)
                           .arg(result.side)
                           .arg(result.conductance)
                           .arg(result.status);
    }

    // Display the results on the Detailed Results Page
    ui->DetailedResultsLabel->setText(detailsText);
    ui->AppStackedWidget->setCurrentWidget(ui->DetailedResultsPage);
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





// Device Scan

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

void MainWindow::updateProcessedDataUI(const std::map<std::string, float>& processedData)
{
    QString dataText;
    for (const auto& [organ, value] : processedData) {
        dataText += QString("%1: %2%\n").arg(QString::fromStdString(organ)).arg(value);
    }
    ui->ProcessedDataLabel->setText(dataText);
}


