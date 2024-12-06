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
    currentUser = nullptr;
    // Connect Device View to App View button
    connect(ui->GoToAppViewButton, &QPushButton::clicked, this, &MainWindow::showAppView);
    // Connect App View to Device View button
    connect(ui->GoToDeviceViewButton, &QPushButton::clicked, this, &MainWindow::showDeviceView);
    // Connect App View buttons to respective pages
    connect(ui->HomePageButton, &QPushButton::clicked, this, &MainWindow::showHomePage);
    connect(ui->MeasureNowButton, &QPushButton::clicked, this, &MainWindow::showMeasureNowPage);
    connect(ui->MeasureNowHomePageButton, &QPushButton::clicked, this, &MainWindow::showMeasureNowPage);
    connect(ui->HistoryPageButton, &QPushButton::clicked, this, &MainWindow::showHistoricalPage);
    connect(ui->HistoryHomePageButton, &QPushButton::clicked, this, &MainWindow::showHistoricalPage);
    connect(ui->ProfilePageButton, &QPushButton::clicked, this, &MainWindow::showProfilePage);
    connect(ui->CreateProfileButton, &QPushButton::clicked, this, &MainWindow::showCreateProfilePage);
    connect(ui->EnterButton, &QPushButton::clicked, this, &MainWindow::showLoginPage);

    //Images
    QPixmap loginImage("/home/student/Desktop/Final/3004-Final-Project/images/loginImage.png");
    ui->loginImage->setPixmap(loginImage.scaled(81,71,Qt::KeepAspectRatio));
    ui->loginImageUpdate->setPixmap(loginImage.scaled(81,71,Qt::KeepAspectRatio));

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

    //User and Profile Buttons
    connect(ui->SavePushButton, &QPushButton::clicked, this, &MainWindow::saveProfile);
    connect(ui->SavePushButtonUpdate, &QPushButton::clicked, this, &MainWindow::editProfile);
    connect(ui->DeleteProfileButton, &QPushButton::clicked, this, &MainWindow::deleteProfile);
    connect(ui->AddNewProfileButton, &QPushButton::clicked, this, &MainWindow::goToCreateProfilePage);

    // Initialize preset users
    createPresetUsers();

    connect(ui->EnterLoginButton, &QPushButton::clicked, this, &MainWindow::handleLogin);
    connect(ui->ViewDetailsButton, &QPushButton::clicked, this, &MainWindow::viewDetails);
    connect(ui->ViewProfileDetailsButton, &QPushButton::clicked, this, &MainWindow::viewProfile);

    // History Tabs
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
    scanTimer = new QTimer(this);
    scanTimer->setInterval(5000);
    connect(scanTimer, &QTimer::timeout, this, &MainWindow::timeOutforScan);
    scanTimer->stop();

    // Battery, Device and Power Buttons Functionality
    connect(ui->OnButton, &QPushButton::clicked, this, &MainWindow::powerDevice);
    connect(ui->OffButton, &QPushButton::clicked, this, &MainWindow::shutDownDevice);
    ui->BatteryPowerProgressBar->setValue(100);

    batteryTimer = new QTimer(this);
    batteryTimer->setInterval(15000);
    connect(batteryTimer, &QTimer::timeout, this, &MainWindow::updateBatteryLevelLabel);

    chargedBatteryTimer = new QTimer(this);
    chargedBatteryTimer->setInterval(15000);
    connect(chargedBatteryTimer, &QTimer::timeout, this, &MainWindow::UpdateChargedBatteryLevelLabel);

    connect(ui->ChargeButton, &QPushButton::clicked, this, &MainWindow::ChargeBattery);
    ui->ChargeButton->setText("Charging ON");
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
    if (!currentUser)
    {
        ui->ViewsStackedWidget->setCurrentWidget(ui->AppStartPage);
        ui->HomePageButton->setDisabled(true);
        ui->MeasureNowButton->setDisabled(true);
        ui->HistoryPageButton->setDisabled(true);
        ui->ProfilePageButton->setDisabled(true);
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

// Show Create Profile Page in App View
void MainWindow::showCreateProfilePage()
{
    ui->ViewsStackedWidget->setCurrentWidget(ui->AppView);
    ui->AppStackedWidget->setCurrentWidget(ui->CreateProfilePage);

}

// Show Login Page in App View
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

// Go to the Create Profile Page from Profile List
void MainWindow::goToCreateProfilePage() {
    // Reset textboxes
    ui->ProfileNameTextEdit->clear();
    ui->GenderComboBox->setCurrentIndex(0);
    ui->WeightTextEdit->clear();
    ui->HeightTextEdit->clear();
    ui->DOBEdit->setDate(QDate::currentDate());
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
    ui->HomePageButton->setDisabled(false);
    ui->MeasureNowButton->setDisabled(false);
    ui->HistoryPageButton->setDisabled(false);
    ui->ProfilePageButton->setDisabled(false);
    ui->AppStackedWidget->setCurrentWidget(ui->ProfilePage);
}

// Update Profiles List on the Profile Page
void MainWindow::updateProfilesList() {
    if (!currentUser) {
        ui->ProfileListWidget->clear();
        QMessageBox::information(this, "No Data", "No profiles");
        return;
    }

    ui->ProfileListWidget->clear();
    QList<Profile*> profiles = currentUser->getProfiles();
    qDebug()<<profiles.size();
    for (Profile* profile : currentUser->getProfiles()) {
        if (!profile) continue;
        QString profilesText = QString(profile->getName());
        ui->ProfileListWidget->addItem(profilesText);
    }

}

// View Profile Details
void MainWindow::viewProfile(){
    if (!currentUser) {
        QMessageBox::critical(this, "Error", "No profiles selected");
        return;
    }

    QListWidgetItem* selectedItem = ui->ProfileListWidget->currentItem();

    if (!selectedItem) {
        QMessageBox::warning(this,"No Selection", "Please select a Profile");
    }

    // Find the profile that matches the selected item's text
    Profile* selectedProfile = nullptr;
    QString selectedProfileName = selectedItem->text();

    for (Profile* profile : currentUser->getProfiles()) {
        if (profile->getName() == selectedProfileName) {
            selectedProfile = profile;
            currProfile = profile;
            break;
        }
    }

    if (!selectedProfile) {
        QMessageBox::critical(this, "Error", "Selected profile not found.");
        return;
    }

    // Populate the profile details page and Navigate to the Profile Details Page
    ui->ProfileNameTextEditUpdate->setText(selectedProfile->getName());
    ui->GenderComboBoxUpdate->setCurrentText(selectedProfile->getGender());
    ui->WeightTextEditUpdate->setText(QString::number(selectedProfile->getWeight()));
    ui->HeightTextEditUpdate->setText(QString::number(selectedProfile->getHeight()));
    ui->DOBEditUpdate->setDate(selectedProfile->getDateOfBirth());
    ui->AppStackedWidget->setCurrentWidget(ui->ProfileDetailsPage);


}

// Edit Profile
void MainWindow::editProfile() {

    // Collect updated data
    QString updatedName = ui->ProfileNameTextEditUpdate->toPlainText();
    QString updatedGender = ui->GenderComboBoxUpdate->currentText();
    double updatedWeight = ui->WeightTextEditUpdate->toPlainText().toDouble();
    double updatedHeight = ui->HeightTextEditUpdate->toPlainText().toDouble();
    QDate updatedDOB = ui->DOBEditUpdate->date();

    // Apply the changes to the profile
    currProfile->setName(updatedName);
    currProfile->setGender(updatedGender);
    currProfile->setWeight(updatedWeight);
    currProfile->setHeight(updatedHeight);
    currProfile->setDateOfBirth(updatedDOB);

    updateProfilesList();

    QMessageBox::information(this, "Success", "Profile updated successfully!");

    // Navigate back to the profiles page
    ui->AppStackedWidget->setCurrentWidget(ui->ProfilePage);

}

// Delete Profile
void MainWindow::deleteProfile() {
    if (!currentUser) {
        QMessageBox::critical(this, "Error", "No user is logged in.");
        return;
    }

    // Get the selected profile from the list
    QListWidgetItem* selectedItem = ui->ProfileListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "No Selection", "Please select a profile to delete.");
        return;
    }

    QString selectedProfileName = selectedItem->text();

    // Confirm deletion
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Delete Profile",
        QString("Are you sure you want to delete the profile '%1'?").arg(selectedProfileName),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Find and delete the selected profile
    bool profileDeleted = false;
    QList<Profile*> profiles = currentUser->getProfiles();
    for (int i = 0; i < profiles.size(); ++i) {
        if (profiles[i]->getName() == selectedProfileName) {
            delete profiles[i]; // Free the memory for the profile
            profiles.removeAt(i); // Remove it from the list
            profileDeleted = true;
            break;
        }
    }

    if (!profileDeleted) {
        QMessageBox::critical(this, "Error", "Profile not found.");
        return;
    }

    currentUser->setProfiles(profiles);

    updateProfilesList();

    QMessageBox::information(this, "Success", "Profile deleted successfully.");
}

// Show te Profiles Page
void MainWindow::showProfiles() {
    if (!currentUser) return;

    QString profileList = "Profiles:\n";
    for (Profile* profile : currentUser->getProfiles()) {
        profileList += profile->getName() + " (" + profile->getGender() + ")\n";
    }

    QMessageBox::information(this, "User Profiles", profileList);
}

// Creating Preset Users with there own Historical Data
void MainWindow::createPresetUsers() {
    User* user1 = new User("test1@example.com", "password1");
    User* user2 = new User("test2@example.com", "password2");

    Profile* profile1 = new Profile("Profile1ForTest", "Male", 70.0, 175.0, QDate(1990, 1, 1));
    user1->addProfile(profile1);
    Profile* profile2 = new Profile("Profile2ForTest", "Female", 89.0, 180.0, QDate(2004,1,1));
    user1->addProfile(profile2);
    Profile* profile3 = new Profile("Profile3ForTest", "Male", 80.0, 170.0, QDate(1995,1,1));
    user2->addProfile(profile3);
    Profile* profile4 = new Profile("Profile4ForTest", "Female", 70.0, 160.0, QDate(1990,1,1));
    user2->addProfile(profile4);

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
            {90.1, "9.1", 75, 8,90.1,"2","2","Not Feeling Great Today"}
    };
    HealthData* healthData = new HealthData(QDate(1990, 2, 2), results, comments);
    profile1->addHealthData(healthData);

    QList<Comments> comments1 = {
            {90.1, "9.1", 75, 8,90.1,"5","5","FEELING GREAT!"}
    };
    HealthData* healthData1 = new HealthData(QDate(1990, 4, 4),results1,comments1);
    profile2->addHealthData(healthData1);

    QList<Comments> comments2 = {
            {90.1, "9.1", 75, 8,90.1,"2","2","Not Feeling Great Today"}
    };
    HealthData* healthData2 = new HealthData(QDate(1990, 2, 2), results, comments2);
    profile3->addHealthData(healthData2);

    QList<Comments> comments3 = {
            {90.1, "9.1", 75, 8,90.1,"5","5","FEELING GREAT!"}
    };
    HealthData* healthData3 = new HealthData(QDate(1990, 4, 4),results1,comments3);
    profile4->addHealthData(healthData3);

    presetUsers.append(user1);
    presetUsers.append(user2);
}

// Handle the Login of User
void MainWindow::handleLogin() {
    // Get input from login fields
    QString email = ui->LoginEmailTextEdit->toPlainText();
    QString password = ui->PasswordTextEdit->toPlainText();

    // Validate credentials
    User* user = User::validateCredentials(email, password, presetUsers);

    if (user) {
        currentUser = user;
        QMessageBox::information(this, "Login Successful", "Welcome to the app!");
        showProfiles();
        ui->HomePageButton->setDisabled(false);
        ui->MeasureNowButton->setDisabled(false);
        ui->HistoryPageButton->setDisabled(false);
        ui->ProfilePageButton->setDisabled(false);
        ui->AppStackedWidget->setCurrentWidget(ui->HomePage);
    } else {
        QMessageBox::critical(this, "Login Failed", "Incorrect email or password. Please try again.");
    }
}

// Populate the History List
void MainWindow::populateHistoryList() {
    if (!currentUser || currentUser->getProfiles().isEmpty()) {
        ui->HistoryListWidget->clear();
        QMessageBox::information(this, "No Data", "No profiles or health data available.");
        return;
    }

    ui->HistoryListWidget->clear();

    // Iterate through all profiles of the current user
    for (Profile* profile : currentUser->getProfiles()) {
        if (!profile) continue;

        // Iterate through each HealthData entry in the profile's history
        for (HealthData* data : profile->getHistory()) {
            if (!data) continue;

            QString itemText = QString("%1 - %2")
                                   .arg(profile->getName())
                                   .arg(data->getDate().toString("yyyy-MM-dd"));

            // Add the item to the QListWidget
            ui->HistoryListWidget->addItem(itemText);
        }
    }
}

// View the Details of a HealthData record
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
    // Populate the Indicators
    populateIndicators(selectedData);

    // Populate the Visualization
    visualizer->showBarGraph(selectedData, ui);
    visualizer->showCircleGraph(selectedData, ui);
    visualizer->showBodyGraph(selectedData, ui);

    // Populate the Recommendations
    recommendations->generateRecommendations(selectedData, ui);
    ui->AppStackedWidget->setCurrentWidget(ui->DetailedResultsPage);

    // Populate the Comments
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

// Show the Measure Now Page
void MainWindow::showMeasureView(){
    if (currentUser){
        ui->ViewsStackedWidget->setCurrentWidget(ui->AppView);
        showMeasureNowPage();
    }
    else{
        QMessageBox::warning(this, "Not Logged In", "User must be logged in to go to this view");
    }
}

// Start the Scan
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
        profileNames.append(profile->getName());
    }

    bool ok;
    QString selectedProfileName = QInputDialog::getItem(
        this,
        "Select Profile",
        "Choose a profile for the scan:",
        profileNames,
        0,  
        false,
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
        QString organ = QString::fromStdString(organs[currentScanPoint - 1]);
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
            ui->MeasureNowImage->clear(); 
            ui->MeasureNowLabel->setText(QString("Scan Point %1: %2\nImage not found.").arg(currentScanPoint).arg(organ));
        }

    } else {
        ui->MeasureNowLabel->setText("All scan points completed!");
        showPersonalInfoPage();
        currProfile = nullptr;
    }
}

// Show the Personal Metrics Page
void MainWindow::showPersonalInfoPage(){
ui->AppStackedWidget->setCurrentWidget(ui->PersonalMetricsPage);
}

// Convert the processed data to MeridianResults
QList<MeridianResult> MainWindow::convertProcessedDataToMeridianResults(const std::map<std::string, float>& processedData) {
    QList<MeridianResult> meridianResults;

    QString side;
    QString status;
    int conductance;

    for (const auto& entry : processedData) {
        MeridianResult result;

        result.meridian = QString::fromStdString(entry.first);
        conductance = static_cast<int>(entry.second);

        if (result.meridian.contains("Left")) {
            side = "Left";
        } else if (result.meridian.contains("Right")) {
            side = "Right";
        } else {
            side = "Unknown";
        }

        result.side = side;

        // Set the status based on the conductance value
        if (conductance < 80) {
            status = "Low (Deficient)";
        } else if (conductance <= 120) {
            status = "Normal";
        } else {
            status = "High (Excess)";
        }

        result.status = status;
        result.conductance = conductance; 

        // Add to the results list
        meridianResults.append(result);
    }

    return meridianResults;
}

// Save the results to the current profile
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

// Perform the scan from the device
void MainWindow::performDeviceScan()
{
    // Check if the device is paired
    if(!device.getIsPaired()) {
        QMessageBox NoPairMsg;
        NoPairMsg.setText("Warning: The device is not paired up with the app");
        NoPairMsg.setBaseSize(200, 200);
        NoPairMsg.setIcon(QMessageBox::Warning);
        NoPairMsg.exec();
        return;
    }
    // Check if the device is powered on
    if (!device.startScan()) {
        ui->DeviceStatusLabel->setText("Low battery. Cannot perform scan.");
        return;
    }
    // Check if a profile is selected
    if (!currProfile){
        QMessageBox noProfile;
        noProfile.setText("Warning: Please set up a profile for the device to scan in App view");
        noProfile.setBaseSize(200, 200);
        noProfile.setIcon(QMessageBox::Warning);
        noProfile.exec();
        return;
    }
    // Check if the device is already scanning
    if (scantimercomplete && !scanTimer->isActive()){
        // Collect data from the device
        std::vector<float> rawData = device.collectData();
        processor.setRawData(rawData);

        if (processor.validateData()) {
            isDeviceScanned = true;
            ui->DeviceStatusLabel->setText(QString("Scan %1 complete. Return to App View and press Next.").arg(currentScanPoint));
        } else {
            ui->DeviceStatusLabel->setText("Scan failed. Please try again.");
        }
        scantimercomplete = false;
        ui->DeviceScanButton->setText("Scan On Skin");

        qDebug()<<"SCAN DONE";
    }
    // Start the scan
    else if (!scantimercomplete && !scanTimer->isActive()){
        ui->DeviceScanButton->setText("Take Off Skin");
        scanTimer->start();
        qDebug()<<"Scan Started";
    }
    // If the scan is not complete and the timer is active for skin contact
    else if(!scantimercomplete && scanTimer->isActive()){
        QMessageBox scanoffskin;
        scanoffskin.setText(QString("Please redo scan since Scan %1 did not complete").arg(currentScanPoint));
        scanoffskin.setBaseSize(200, 200);
        scanoffskin.setIcon(QMessageBox::Warning);
        scanoffskin.exec();
        ui->DeviceScanButton->setText("Scan On Skin");
        scanTimer->stop();
        qDebug()<<"Scan skin";
    }

}

// Timer for the scan
void MainWindow::timeOutforScan(){
    scantimercomplete = true;
    scanTimer->stop();
    qDebug()<<"Timer Done";

}

// Update the processed data on the UI
void MainWindow::updateProcessedDataUI(const std::map<std::string, float>& processedData)
{
    QString dataText;
    for (const auto& [organ, value] : processedData) {
        dataText += QString("%1: %2 ").arg(QString::fromStdString(organ)).arg(value);
    }
    qDebug()<<"Data: "<<dataText;

}

// Pair the Device with the App
void MainWindow::PairUp() {
    device.PairUp();

    ui->PairButton->setDisabled(true);
}

// Charge the Battery
void MainWindow::ChargeBattery() {
    if(chargedBatteryTimer->isActive()) {
        chargedBatteryTimer->stop();
        batteryTimer->start();
        ui->ChargeButton->setText("Charging ON");
    } else {
        chargedBatteryTimer->start();
        batteryTimer->stop();
        ui->ChargeButton->setText("Charging OFF");
    }
}

// Update the Battery Level
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

// Depeletes battery label on ui
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

// Power the Device
void MainWindow::powerDevice()
{
    this->batteryTimer->start();
    ui->DeviceScanButton->setDisabled(false);
    ui->OnButton->setDisabled(true);
    ui->OffButton->setDisabled(false);
}

// Shut Down the Device
void MainWindow::shutDownDevice()
{
    this->batteryTimer->stop();
    ui->OffButton->setDisabled(true);
    ui->OnButton->setDisabled(false);
}
