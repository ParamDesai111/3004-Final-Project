#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QDebug>
#include <QInputDialog>

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
    , ui(new Ui::MainWindow), currentScanPoint(0), totalScanPoints(24),isDeviceScanned(false)
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
    QPixmap loginImage("/home/student/Desktop/Final/3004-Final-Project/images/loginImage.png");
    ui->loginImage->setPixmap(loginImage.scaled(81,71,Qt::KeepAspectRatio));

    QPixmap Energy("/home/student/Desktop/Final/3004-Final-Project/images/EnergyLevel.png");
    ui->EnergyImage->setPixmap(Energy.scaled(71,41,Qt::KeepAspectRatio));

    QPixmap Immune("/home/student/Desktop/Final/3004-Final-Project/images/ImmuneSystem.png");
    ui->ImmuneSystemImage->setPixmap(Immune.scaled(81,71,Qt::KeepAspectRatio));

    QPixmap Metabolism("/home/student/Desktop/Final/3004-Final-Project/images/Metabolism.png");
    ui->MetabolismImage->setPixmap(Metabolism.scaled(71,41,Qt::KeepAspectRatio));

    QPixmap Psycho("/home/student/Desktop/Final/3004-Final-Project/images/Psycho.png");
    ui->PsychoImage->setPixmap(Psycho.scaled(81,71,Qt::KeepAspectRatio));

    QPixmap Muscle("/home/student/Desktop/Final/3004-Final-Project/images/Muscler.png");
    ui->MuscleImage->setPixmap(Muscle.scaled(71,41,Qt::KeepAspectRatio));

    //Save Button on Create Button
    connect(ui->SavePushButton, &QPushButton::clicked, this, &MainWindow::saveProfile);
//    currentUser = new User("example@example.com", "password123");
    connect(ui->AddNewProfileButton, &QPushButton::clicked, this, &MainWindow::goToCreateProfilePage);

    // Initialize preset users
    createPresetUsers();

    // Connect login button
    connect(ui->EnterLoginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);

    connect(ui->ViewDetailsButton, &QPushButton::clicked, this, &MainWindow::viewDetails);

    ui->ResultsTabWidget->setTabText(0, "Indicator");
    ui->ResultsTabWidget->setTabText(1, "Visulization");
    ui->ResultsTabWidget->setTabText(2, "Comments");
    ui->ResultsTabWidget->setTabText(3, "Recommendation");

    // Device Scan
    connect(ui->StartScanButton, &QPushButton::clicked, this, &MainWindow::startScan);
    connect(ui->NextButton, &QPushButton::clicked, this, &MainWindow::nextScanPoint);
    connect(ui->DeviceScanButton, &QPushButton::clicked, this, &MainWindow::performDeviceScan);
    connect(ui->GoToMeasureViewButton, &QPushButton::clicked, this, &MainWindow::showMeasureView);

    connect(ui->SaveButtonNotes, &QPushButton::clicked, this, &MainWindow::saveResults);
    showMeasureNowPage();


    // Power buttons
    connect(ui->OnButton, &QPushButton::clicked, this, &MainWindow::powerDevice);
    connect(ui->OffButton, &QPushButton::clicked, this, &MainWindow::shutDownDevice);

    batteryTimer = new QTimer(this);
    batteryTimer->setInterval(15000);
    connect(batteryTimer, &QTimer::timeout, this, &MainWindow::updateBatteryLevelLabel);

    chargedBatteryTimer = new QTimer(this);
    chargedBatteryTimer->setInterval(15000);
    connect(chargedBatteryTimer, &QTimer::timeout, this, &MainWindow::UpdateChargedBatteryLevelLabel);

    connect(ui->ChargeButton, &QPushButton::clicked, this, &MainWindow::ChargeBattery);

    ui->ChargeButton->setText("Charging OFF");
    powerDevice();

    connect(ui->PairButton, &QPushButton::clicked, this, &MainWindow::PairUp);

}

MainWindow::~MainWindow()
{
    delete ui;
}


// Switch to App View
void MainWindow::showAppView()
{
    // TODO: When we implement login and create profile, here we do if statement of which view to first go to after device view
    if (!currentUser)
    {
        ui->ViewsStackedWidget->setCurrentWidget(ui->AppStartPage);
    } else {
        ui->ViewsStackedWidget->setCurrentWidget(ui->AppView);
        ui->AppStackedWidget->setCurrentWidget(ui->HomePage);
    }
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
//    showBarGraph();
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
        {"H1 (Lung)", "Left", 44, "Normal"},
        {"H1 (Lung)", "Right", 40, "Normal"},
        {"H2 (Pericardium)", "Left", 40, "Low (Deficient)"},
        {"H2 (Pericardium)", "Right", 39, "Normal"},
        {"H3 (Heart)", "Left", 43, "High (Excess)"},
        {"H3 (Heart)", "Right", 45, "Normal"},
        {"H4 (Small Intestine)", "Left", 55, "Low (Deficient)"},
        {"H4 (Small Intestine)", "Right", 55, "Low (Deficient)"},
        {"H5 (Lymph)", "Left", 9, "Normal"},
        {"H5 (Lymph)", "Right", 12, "Normal"},
        {"H6 (Large Intestine)", "Left", 50, "Low (Deficient)"},
        {"H6 (Large Intestine)", "Right", 22, "Normal"},
        {"F1 (Spleen)", "Left", 58, "High (Excess)"},
        {"F1 (Spleen)", "Right", 71, "High (Excess)"},
        {"F2 (Liver)", "Left", 54, "Normal"},
        {"F2 (Liver)", "Right", 58, "Normal"},
        {"F3 (Kidney)", "Left", 31, "Normal"},
        {"F3 (Kidney)", "Right", 34, "Low (Deficient)"},
        {"F4 (Bladder)", "Left", 75, "Normal"},
        {"F4 (Bladder)", "Right", 32, "Normal"},
        {"F5 (Gallbladder)", "Left", 26, "Normal"},
        {"F5 (Gallbladder)", "Right", 45, "Low (Deficient)"},
        {"F6 (Stomach)", "Left", 55, "Low (Deficient)"},
        {"F6 (Stomach)", "Right", 48, "High (Excess)"}
    };

    QList<Comments> comments = {
            {90.1, "9.1", 75, 8,90.1,"2","5","blahblah"}
    };
    HealthData* healthData = new HealthData(QDate(1990, 2, 2), results, comments);
    profile1->addHealthData(healthData);

    QDate currentDate = QDate::currentDate();
    QDate tomorrow = currentDate.addDays(1);
    QList<Comments> comments1 = {
            {90.1, "9.1", 75, 8,90.1,"2","5","blahblah"}
    };
    HealthData* healthData1 = new HealthData(QDate(1990, 4, 4),results1,comments1);
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
//    QString detailsText;
//    for (const MeridianResult& result : selectedData->getData()) {
//        detailsText += QString("%1 (%2): %3 µA, %4\n")
//                           .arg(result.meridian)
//                           .arg(result.side)
//                           .arg(result.conductance)
//                           .arg(result.status);
//    }


    // Display the results on the Detailed Results Page
//    ui->DetailedResultsLabel->setText(detailsText);
    populateIndicators(selectedData);
    showBarGraph(selectedData);
    showRadarChart(selectedData);
    ui->AppStackedWidget->setCurrentWidget(ui->DetailedResultsPage);


    // COMMENTS
    QString detailsComments;
    for (const Comments& comment: selectedData->getComments()){
        detailsComments += QString("Blood Pressure: %1, \n Body Temperture: %2, \n Heart Rate: %3, \n Sleeping Time: %4, \n Current Weight %5, \n Emotional State %6, \n Over Feeling %7, \n Notes: %8")
                .arg(comment.bloodPressure)
                .arg(comment.bodyTemperature)
                .arg(comment.heartRate)
                .arg(comment.sleepingTime)
                .arg(comment.currentWeight)
                .arg(comment.emotionalState)
                .arg(comment.overallFeeling)
                .arg(comment.notes);
    }
    ui->CommentsLabel->setText(detailsComments);


}

// Function to populate health indicators in the DetailedResultsPage
void MainWindow::populateIndicators(HealthData* selectedData) {
    if (!selectedData) {
        QMessageBox::warning(this, "No Data", "No health data available.");
        return;
    }

    // Functional Health Indicators
    double energyLevel = selectedData->calculateEnergyLevel();
    double immuneSystem = selectedData->calculateImmuneSystem();
    double metabolism = selectedData->calculateMetabolism();
    double psychoEmotionalState = selectedData->calculatePsychoEmotionalState();
    double musculoskeletalSystem = selectedData->calculateMusculoskeletalSystem();

    // Update Functional Health Indicators Labels
    ui->EnergyNumberLabel->setText(QString::number(energyLevel, 'f', 2));
    ui->ImmuneSystemNumberLabel->setText(QString::number(immuneSystem, 'f', 2));
    ui->MetabolismNumberLabel->setText(QString::number(metabolism, 'f', 2));
    ui->PsychoNumberLabel->setText(QString::number(psychoEmotionalState, 'f', 2));
    ui->MuscleNumberLabel->setText(QString::number(musculoskeletalSystem, 'f', 2));

    // Add classification ranges
    ui->EnergyRangeLabel->setText(getClassification(energyLevel, 25, 55));
    ui->ImmuneSystemRangeLabel->setText(getClassification(immuneSystem, 47, 57));
    ui->MetabolismRangeLabel->setText(getClassification(metabolism, 1.1, 1.2));
    ui->PsychoRangeLabel->setText(getClassification(psychoEmotionalState, 0.8, 1.2));
    ui->MuscleRangeLabel->setText(getClassification(musculoskeletalSystem, 0.9, 1.2));

    // Professional Practitioner Data
    double averageValue = selectedData->calculateAverageValue();
    double leftTotal = selectedData->calculateLeftTotal();
    double rightTotal = selectedData->calculateRightTotal();
    double leftRightRatio = selectedData->calculateLeftRightRatio();
    double upperTotal = selectedData->calculateUpperTotal();
    double lowerTotal = selectedData->calculateLowerTotal();
    double upperLowerRatio = selectedData->calculateUpperLowerRatio();

    // Update Practitioner Data Labels
    ui->AverageValueNumber->setText(QString::number(averageValue, 'f', 2));
    ui->LeftTotalNumber->setText(QString::number(leftTotal, 'f', 2));
    ui->RightTotalNumber->setText(QString::number(rightTotal, 'f', 2));
    ui->LeftRightTotalNumber->setText(QString::number(leftRightRatio, 'f', 2));
    ui->UpperTotalNumber->setText(QString::number(upperTotal, 'f', 2));
    ui->LowerTotalNumber->setText(QString::number(lowerTotal, 'f', 2));
    ui->UpperLowerTotalNumber->setText(QString::number(upperLowerRatio, 'f', 2));
}

// Helper function to determine the classification range
QString MainWindow::getClassification(double value, double min, double max) {
    if (value < min) {
        return "Below Normal";
    } else if (value > max) {
        return "Above Normal";
    }
    return "Normal";
}




void MainWindow::showBarGraph(HealthData* healthdata)
{
    // Get results and validate data
    QList<MeridianResult> results = healthdata->getData();
    if (results.size() < 24) {
        qDebug() << "Insufficient data for the bar graph. Expected 24, got:" << results.size();
        return;
    }

    // Create bar sets for "Left" and "Right"
    QBarSet* leftSet = new QBarSet("Left");
    QBarSet* rightSet = new QBarSet("Right");

    // Map data into bar sets
    for (int i = 0; i < results.size(); i += 2) {
        leftSet->append(results[i].conductance);     // Even indices are "Left"
        rightSet->append(results[i + 1].conductance); // Odd indices are "Right"
    }

    // Create a bar series and add the sets
    QBarSeries* series = new QBarSeries();
    series->append(leftSet);
    series->append(rightSet);

    // Create categories (meridian names)
    QStringList categories;
    categories << "Lungs" << "Pericardium" << "Heart" << "Small Intestine"
               << "Lymph" << "Large Intestine" << "Spleen" << "Liver"
               << "Kidney" << "Bladder" << "Gallbladder" << "Stomach";

    // Create category axis (x-axis)
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsAngle(90); // Rotate labels for readability

    // Create value axis (y-axis)
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 200); // Adjust based on your data range
    axisY->setTitleText("Conductance (µA)");

    // Create a chart
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chart->setTitle("Organ Health Metrics");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create a QChartView and display the chart
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Clear and set the layout for the chart container
    QLayout* existingLayout = ui->ChartWidget->layout();
    if (existingLayout != nullptr) {
        QLayoutItem* item;
        while ((item = existingLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete existingLayout;
    }

    QVBoxLayout* layout = new QVBoxLayout(ui->ChartWidget);
    layout->addWidget(chartView);
    ui->ChartWidget->setLayout(layout);
}


void MainWindow::showRadarChart(HealthData* healthdata)
{
    // Create a polar chart
    QtCharts::QPolarChart *polarChart = new QtCharts::QPolarChart();
    polarChart->setTitle("Organ Health Radar Chart");

    // Get the data
    QList<MeridianResult> results = healthdata->getData();

    if (results.size() < 24) {
        qDebug() << "Insufficient data for radar chart. Expected at least 24 results, got:" << results.size();
        return;
    }

    // Vectors to hold computed values
    QVector<double> leftSideData;
    QVector<double> rightSideData;
    QVector<double> averageValues;

    // Process the data for left and right sides
    for (int i = 0; i < results.size(); i += 2) { // Step by 2: 0=Left, 1=Right
        double leftValue = results[i].conductance;     // Left value
        double rightValue = results[i + 1].conductance; // Right value
        double averageValue = (leftValue + rightValue) / 2.0; // Average

        leftSideData.append(leftValue);
        rightSideData.append(rightValue);
        averageValues.append(averageValue);
    }

    // Labels for the axes (corresponding to organ names)
    QStringList categories;
    categories << "Lungs" << "Pericardium" << "Heart" << "Small Intestine"
               << "Lymph" << "Large Intestine" << "Spleen" << "Liver"
               << "Kidney" << "Bladder" << "Gallbladder" << "Stomach";

    // Create a Value Axis (for the circular range)
    QtCharts::QValueAxis *valueAxis = new QtCharts::QValueAxis();
    valueAxis->setRange(0, 200); // Adjust range based on conductance values
    valueAxis->setTickCount(6);  // Add tick marks
    valueAxis->setLabelFormat("%.0f");
    valueAxis->setTitleText("Conductance");
    polarChart->addAxis(valueAxis, QtCharts::QPolarChart::PolarOrientationRadial);

    // Create a Category Axis (for the angular labels)
    QtCharts::QCategoryAxis *angularAxis = new QtCharts::QCategoryAxis();
    angularAxis->setLabelsPosition(QtCharts::QCategoryAxis::AxisLabelsPositionOnValue);
    for (int i = 0; i < categories.size(); ++i)
    {
        angularAxis->append(categories[i], i);
    }
    angularAxis->setRange(0, categories.size() - 1); // Adjust for zero-based indexing
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

    // Clear any existing layout in CircleChartWidget
    QLayout *existingLayout = ui->CircleChartWidget->layout();
    if (existingLayout != nullptr)
    {
        QLayoutItem *item;
        while ((item = existingLayout->takeAt(0)) != nullptr)
        {
            delete item->widget(); // Remove widgets
            delete item;           // Remove layout items
        }
        delete existingLayout; // Delete the existing layout
    }

    // Add the chart to the placeholder widget (CircleChartWidget)
    QVBoxLayout *layout = new QVBoxLayout(ui->CircleChartWidget);
    layout->addWidget(chartView);
    ui->CircleChartWidget->setLayout(layout);
}

void MainWindow::updateBodyChart(HealthData* healthdata)
{
    QList<MeridianResult> results = healthdata->getData();

    for (const MeridianResult& result : results)
    {
        QString organ = result.meridian.toLower();
        QString status;

        if (result.status == "Above") {
            status = "above";
        } else if (result.status == "Normal") {
            status = "normal";
        } else {
            status = "below";
        }

        // Construct the image path dynamically
        QString imagePath = QString(":/images/%1_%2.png").arg(organ).arg(status);

        // Set the appropriate image for the QLabel
        QLabel* organLabel = findChild<QLabel*>(organ + "Label");
        if (organLabel) {
            organLabel->setPixmap(QPixmap(imagePath).scaled(organLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}





// Device Scan

void MainWindow::showMeasureView(){
    ui->ViewsStackedWidget->setCurrentWidget(ui->AppView);
    showMeasureNowPage();
}

void MainWindow::startScan()
{
    // Ensure the current user is set
    if (!currentUser) {
        ui->MeasureNowLabel->setText("No user is logged in.");
        return;
    }
    // Get the list of profiles
    QList<Profile*> profiles = currentUser->getProfiles();
    if (profiles.isEmpty()) {
        ui->MeasureNowLabel->setText("No profiles found for the current user.");
        return;
    }
    // Create a list of profile names for display
    QStringList profileNames;
    for (Profile* profile : profiles) {
        // Assuming Profile has a method getName() to get the profile name
        profileNames.append(profile->getName());
    }
    // Show a dialog to select a profile
    bool ok;
    QString selectedProfileName = QInputDialog::getItem(
        this,
        "Select Profile",
        "Choose a profile for the scan:",
        profileNames,
        0,  // Default index
        false, // Editable: false (user can't edit the list)
        &ok
    );
    if (!ok) {
        ui->MeasureNowLabel->setText("Scan canceled.");
        return;
    }
    // Find the selected profile
    Profile* selectedProfile = nullptr;
    for (Profile* profile : profiles) {
        if (profile->getName() == selectedProfileName) {
            selectedProfile = profile;
            currProfile = profile;
            break;
        }
    }
    if (!selectedProfile) {
        ui->MeasureNowLabel->setText("Selected profile not found.");
        return;
    }
    // Proceed with scan for the selected profile
    currentScanPoint = 1;
    isDeviceScanned = false;
    ui->MeasureNowLabel->setText("Scan Point 1: Navigate to Device View and press Scan.");
    ui->MeasureNowLabel->setText(QString("Starting scan for profile: %1").arg(selectedProfileName));
    QString imagePath = QString("/home/student/Desktop/Final/3004-Final-Project/images/ScanImages/Point1.png");
    QPixmap pointImage(imagePath);
    ui->MeasureNowImage->setPixmap(pointImage.scaled(501, 311, Qt::KeepAspectRatio));
    ui->DeviceStatusLabel->setText("Ready for Scan 1.");
}

void MainWindow::nextScanPoint()
{
    if (!isDeviceScanned) {
        ui->MeasureNowLabel->setText("Please perform the scan on the device first.");
        return;
    }

    // Process data after a valid scan
    processedData = processor.processData();

    const std::vector<std::string> organs = {
        "H1 (Lung), Left",
        "H2 (Pericardium), Left",
        "H3 (Heart), Left",
        "H4 (Small Intestine), Left",
        "H5 (Lymph), Left",
        "H6 (Large Intestine), Left",
        "F1 (Spleen), Left",
        "F2 (Liver), Left",
        "F3 (Kidney), Left",
        "F4 (Bladder), Left",
        "F5 (Gallbladder), Left",
        "F6 (Stomach), Left",
        "H1 (Lung), Right",
        "H2 (Pericardium), Right",
        "H3 (Heart), Right",
        "H4 (Small Intestine), Right",
        "H5 (Lymph), Right",
        "H6 (Large Intestine), Right",
        "F1 (Spleen), Right",
        "F2 (Liver), Right",
        "F3 (Kidney), Right",
        "F4 (Bladder), Right",
        "F5 (Gallbladder), Right",
        "F6 (Stomach), Right"
    };

    if (currentScanPoint < totalScanPoints) {
        currentScanPoint++;
        isDeviceScanned = false;

        // Get the organ being measured for the current scan point
        QString organ = QString::fromStdString(organs[currentScanPoint - 1]); // -1 to match zero-based indexing

        ui->MeasureNowLabel->setText(
            QString("Scan Point %1: %2\nNavigate to Device View and press Scan.")
                .arg(currentScanPoint)
                .arg(organ)
        );
        ui->DeviceStatusLabel->setText(QString("Ready for Scan %1: %2").arg(currentScanPoint).arg(organ));

        // Load and set the corresponding image
        QString imagePath = QString("/home/student/Desktop/Final/3004-Final-Project/images/ScanImages/Point%1.png").arg(currentScanPoint);
        QPixmap pointImage(imagePath);

        if (!pointImage.isNull()) {
            ui->MeasureNowImage->setPixmap(pointImage.scaled(501, 311, Qt::KeepAspectRatio));
        } else {
            ui->MeasureNowImage->clear(); // Clear the image if loading fails
            ui->MeasureNowLabel->setText(QString("Scan Point %1: %2\nImage not found.").arg(currentScanPoint).arg(organ));
        }

    } else {
        ui->MeasureNowLabel->setText("All scan points completed!");
        showPersonalInfoPage();
    }
}



void MainWindow::showPersonalInfoPage(){
ui->AppStackedWidget->setCurrentWidget(ui->PersonalMetricsPage);
}

QList<MeridianResult> MainWindow::convertProcessedDataToMeridianResults(const std::map<std::string, float>& processedData) {
    QList<MeridianResult> meridianResults;

    // For demonstration purposes, assume we have a logic for assigning sides and statuses.
    // You'll want to adapt this based on your logic for determining the "side" (Left/Right) and the "status".
    QString side;
    QString status;
    int conductance;

    for (const auto& entry : processedData) {
        MeridianResult result;

        // Set meridian name
        result.meridian = QString::fromStdString(entry.first);  // Convert from std::string to QString
        conductance = static_cast<int>(entry.second);  // Convert the float to an integer (µA)

        // For now, we alternate between Left and Right. You can modify this as needed.
        if (result.meridian.contains("Left")) {
            side = "Left";
        } else if (result.meridian.contains("Right")) {
            side = "Right";
        } else {
            side = "Unknown";  // If no specific side, assign as "Unknown"
        }

        result.side = side;

        // Set the status based on the conductance (use ranges or other criteria to define status)
        if (conductance < 80) {
            status = "Low (Deficient)";
        } else if (conductance <= 120) {
            status = "Normal";
        } else {
            status = "High (Excess)";
        }

        result.status = status;
        result.conductance = conductance;  // Set the conductance value in µA

        // Add to the results list
        meridianResults.append(result);
    }

    return meridianResults;
}




void MainWindow::saveResults(){
    double bodyTemprature = ui->BodyTempBox->value();
    QString bloodPressure = ui->BloodPressureBox->text();
    int heartRate = ui->HeartRateBox->value();
    int sleepingTime = ui->SleepTimeBox->value();
    double currentWeight = ui->BodyWeightBox->value();
    QString emotionalState = ui->EmoStateBox->text();
    QString overallFeeling = ui->OverallFeelingBox->text();
    QString notes = ui->NotesBox->toPlainText();

    QList<Comments> commentsList = {
        {bodyTemprature, bloodPressure, heartRate, sleepingTime, currentWeight, emotionalState, overallFeeling, notes}
    };
    QList<MeridianResult> data = convertProcessedDataToMeridianResults(processedData);
    updateProcessedDataUI(processedData);
    HealthData* healthData = new HealthData(QDate::currentDate(), data, commentsList);
    currProfile->addHealthData(healthData);
    ui->AppStackedWidget->setCurrentWidget(ui->HomePage);
}


void MainWindow::performDeviceScan()
{
    if(!device.getIsPaired()) {
        QMessageBox NoPairMsg;
        NoPairMsg.setText("Warning: The device is not paired up with the app");
        NoPairMsg.setBaseSize(200, 200);
        NoPairMsg.setIcon(QMessageBox::Warning);
        NoPairMsg.exec();
        return;
    }
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

//    updateBatteryLevelLabel();
}


void MainWindow::updateProcessedDataUI(const std::map<std::string, float>& processedData)
{
    QString dataText;
    for (const auto& [organ, value] : processedData) {
        dataText += QString("%1: %2 ").arg(QString::fromStdString(organ)).arg(value);
    }
    qDebug()<<"Data: "<<dataText;

//    ui->ProcessedDataLabel->setText(dataText);
}

void MainWindow::PairUp() {
    device.PairUp();

    ui->PairButton->setDisabled(true);
}

void MainWindow::ChargeBattery() {
    if(chargedBatteryTimer->isActive()) { //if was Charging
        chargedBatteryTimer->stop();
        batteryTimer->start();
        ui->ChargeButton->setText("Charging OFF");
    } else { //if was not Charging
        chargedBatteryTimer->start();
        batteryTimer->stop();
        ui->ChargeButton->setText("Charging ON");
    }
}

void MainWindow::UpdateChargedBatteryLevelLabel() {

    if(device.getBatteryLevel() == 0){
        ui->OnButton->setDisabled(false);
        ui->DeviceScanButton->setDisabled(false);
    }
    this->device.chargeBattery();
    ui->BatteryPowerProgressBar->setValue(device.getBatteryLevel());

    if(device.isBatteryLow()) {
        ui->BatteryPowerProgressBar->setStyleSheet(
            "QProgressBar::chunk { background-color: yellow; }"
            "QProgressBar { border: 1px solid gray; border-radius: 3px; text-align: center; }"
        );
    } else {
        ui->BatteryPowerProgressBar->setStyleSheet(
            "QProgressBar::chunk { background-color: green; }"
            "QProgressBar { border: 1px solid gray; border-radius: 3px; text-align: center; }"
        );
    }


}

// depeletes battery label on ui
void MainWindow::updateBatteryLevelLabel()
{
    this->device.depleteBattery();
    ui->BatteryPowerProgressBar->setValue(device.getBatteryLevel());

    qDebug() << "curr battery level: " << device.getBatteryLevel() << "\n";
    if (device.isBatteryLow() && device.getBatteryLevel() == 20) {
        ui->BatteryPowerProgressBar->setStyleSheet(
            "QProgressBar::chunk { background-color: red; }"
            "QProgressBar { border: 1px solid gray; border-radius: 3px; text-align: center; }"
        );
        QMessageBox LowBatteryMsg;
        LowBatteryMsg.setText("Warning: Low Battery");
        LowBatteryMsg.setIcon(QMessageBox::Warning);
        LowBatteryMsg.setBaseSize(200, 200);
        LowBatteryMsg.exec();

    } else if(device.isBatteryLow()) {
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

    if(device.getBatteryLevel() == 0) {
        ui->OnButton->setDisabled(true);
        ui->DeviceScanButton->setDisabled(true);
        ui->OffButton->setDisabled(true);
    }
}

void MainWindow::powerDevice()
{
    this->batteryTimer->start();
    ui->DeviceScanButton->setDisabled(false);
    ui->OnButton->setDisabled(true);
    ui->OffButton->setDisabled(false);
}

void MainWindow::shutDownDevice()
{
    this->batteryTimer->stop();
    //ui->DeviceScanButton->setDisabled(true);
    ui->OffButton->setDisabled(true);
    ui->OnButton->setDisabled(false);
}
