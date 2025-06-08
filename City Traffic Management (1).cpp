#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <iomanip>

#include <sstream>


using namespace std;

// Function Prototypes
void login();
string get_city_file(string, int);
string city(string[], int&);
string change_city(string[], int&);
void show_menu(string);
void booth_system(string, string[], int[3][2], string);
void challan_system(string, string[], int[3], string);
void helpline(string, int[4]);
void view_challan(string,string );
void view_booth_system(string, int[3][2], string);
void view_helpline(string, int[4],string);
void save_helpline_data(string, int[4]);
void save_booth_data(string, int[3][2]);
void load_helpline_data(string, int[4]);
void load_booth_data(string, int[3][2]);
void resetCityFile(string fileName);
int main() {
    // Initialize arrays and variables
     SetConsoleOutputCP(65001);
    int boothArray[3][2] = {0};            
    int issueCounts[4] = {0};              
    string cities[3] = {"Lahore", "Karachi", "Multan"}; 
    string vehicleTypes[3] = {"Two-Wheeler", "Four-Wheeler", "Heavy Vehicle"}; // Vehicle types
    int cityChoice; 
    string cityName = city(cities, cityChoice); //city selection
    int user_choice;                 // User's menu choice

    // Require login before showing the menu
    login();

    // Main program loop
    while (true) {
        show_menu(cityName);
        cout << "Enter your choice: ";
        cin >> user_choice;

        // Validate user input
        if (cin.fail() || user_choice < 1 || user_choice > 9) {
            cout << "Invalid choice! Please try again.\n";
            //Clearing buffer overflow if input wrong
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        // File names for booth, challan, and helpline systems
        string cityBoothFileName, cityChallanFileName, cityHelplineFileName;

        // Handle user menu choices
        if (user_choice == 1) {
            cityBoothFileName = get_city_file(cityName, user_choice);
            load_booth_data(cityBoothFileName, boothArray);
            booth_system(cityBoothFileName, vehicleTypes, boothArray, cityName);
        } 
        // Challan System
        else if (user_choice == 2) {
            cityChallanFileName = get_city_file(cityName, user_choice);
            challan_system(cityChallanFileName, vehicleTypes, issueCounts, cityName);
        } 
        // Helpline System
        else if (user_choice == 3) {
            cityHelplineFileName = get_city_file(cityName, user_choice);
            load_helpline_data(cityHelplineFileName, issueCounts);
            helpline(cityHelplineFileName, issueCounts);
        } 
        // View Challan Logs
        else if (user_choice == 4) {
            cityChallanFileName = get_city_file(cityName, user_choice);
            view_challan(cityChallanFileName, cityName);
        } 
        // View Booth Traffic Data
        else if (user_choice == 5) {
            cityBoothFileName = get_city_file(cityName, user_choice);
            load_booth_data(cityBoothFileName, boothArray);
            view_booth_system(cityBoothFileName, boothArray, cityName);
        } 
        // View Helpline data
        else if (user_choice == 6) {
            cityHelplineFileName = get_city_file(cityName, user_choice);
            load_helpline_data(cityHelplineFileName, issueCounts);
            view_helpline(cityHelplineFileName, issueCounts,cityName);
        } 
        // Change cities 
        else if (user_choice == 7) {
            cityName = change_city(cities, cityChoice);
        } 
        // Exit
        else if (user_choice == 8) {
            cout<<"Goodbye! Thank you for using the City Traffic Management System.\n";
            cout << "Exiting the program...\n";
            exit(0);
        } 
        // Reset files
        else if (user_choice == 9) {
        resetCityFile(cityName);  
    }
    }

    return 0;
}

// Login system function
void login() {
    string username, password;
    const string correctUsername = "admin";
    const string correctPassword = "password123";

    cout << "===== LOGIN =====\n";
    while (true) {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (username == correctUsername && password == correctPassword) {
            cout << "Login successful!\n";
            break;
        } else {
            cout << "Invalid username or password. Try again.\n";
        }
    }
    
}

// Lets the user pick a city from the list
string city(string cities[], int &cityChoice) {
    cout << "===== CITY TRAFFIC MANAGEMENT SYSTEM =====\n"
         << "    Select City:\n"
         << "    1. Lahore\n"
         << "    2. Karachi\n"
         << "    3. Multan\n";
    cout << "Enter your choice: ";
    cin >> cityChoice;

    if (cityChoice >= 1 && cityChoice <= 3) {
        string selectedCity = cities[cityChoice - 1];
        cout << "City selected successfully: " << selectedCity << endl;
        return selectedCity;
    } 
    else {
        cout << "Invalid choice! Defaulting to Lahore.\n";
        cityChoice = 1;
        return cities[0];
    }
}

// Changes the current city
string change_city(string cities[], int &cityChoice) 
{
    string cityName = city(cities, cityChoice);
    cout << "City changed successfully: " << cityName << endl;
    return cityName;
}

// Returns the appropriate file name based on the city and user choice
string get_city_file(string selectedCity, int user_choice) {
    string fileName;
    if (user_choice == 1 || user_choice == 5) {
        fileName = selectedCity + "Booth_System.txt";
    } 
    else if (user_choice == 2 || user_choice == 4) 
    {
        fileName = selectedCity + "Challan_Logs.txt";
    } 
    else if (user_choice == 3 || user_choice == 6) 
    {
        fileName = selectedCity + "Helpline_System.txt";
    }
    return fileName;
}

// Displays the main menu for the selected city
void show_menu(string cityName) {
    cout << "\n--- MAIN MENU for " << cityName << " ---\n"
         << "    1. Booth System\n"
         << "    2. Challan System\n"
         << "    3. Helpline\n"
         << "    4. View Challan Logs\n"
         << "    5. View Booth Traffic Data\n"
         << "    6. View Helpline Usage\n"
         << "    7. Change City\n"
         << "    8. Exit\n"
         << "    9. Reset Files (Admin Only)\n";
    cout << "-----------------------------------------\n";
}

// Booth System
void booth_system(string fileName, string vehicleTypesArray[], int array[3][2], string city) {
    string vehicleType;
    int choice, actionChoice, vehicleCount;

    while (true) {
        cout << "--- BOOTH SYSTEM ---\n"
             << "Select Vehicle Type:\n"
             << "    1. Two-Wheeler\n"
             << "    2. Four-Wheeler\n"
             << "    3. Heavy Vehicle\n"
             << "Enter your choice (1-3): ";
        cin >> choice;

        if (choice >= 1 && choice <= 3) 
        {
            vehicleType = vehicleTypesArray[choice - 1];
            break;
        } 
        else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    while (true) {
        cout << "\nSelect Action:\n"
             << "    1. Entry\n"
             << "    2. Exit\n"
             << "Enter your choice (1-2): ";
        cin >> actionChoice;

        if (actionChoice >= 1 && actionChoice <= 2) 
        {
            break;
        } 
        else {
            cout << "Invalid choice! Try again.\n";
        }
    }

    cout << "Enter number of vehicles: ";
    cin >> vehicleCount;
    while (vehicleCount < 0) {
        cout << "Invalid number! Enter a non-negative number: ";
        cin >> vehicleCount;
    }

    // Update the count for the selected vehicle type and action
    array[choice - 1][actionChoice - 1] += vehicleCount;
   
    //Saving to the booth file.
    ofstream outFile(fileName);
    if (!outFile) {
        cerr << "Error opening file: " << fileName << "\n";
        return;
    }
    for (int i = 0; i < 3; ++i) {
        outFile << array[i][0] << " " << array[i][1] << "\n";
    }
    outFile.close();
    
    }

// Challan System
void challan_system(string fileName, string vehicleTypesArray[], int challanCounts[3], string city) 
{
    string vehicleNumber, selectedVehicleType;
    int vehicleTypeChoice, challanReasonChoice, fine;
    string challanReasonsArray[] = {"Over-speeding", "Signal Violation", "No Helmet", "Other"};

    cout << "--- CHALLAN SYSTEM ---\n";
    cout << "Enter Vehicle Number: ";
    cin >> vehicleNumber;

    cout << "Select Vehicle Type:\n"
         << "    1. Two-Wheeler\n"
         << "    2. Four-Wheeler\n"
         << "    3. Heavy Vehicle\n"
         << "Enter your choice (1-3): ";

    cin >> vehicleTypeChoice;

    while (vehicleTypeChoice < 1 || vehicleTypeChoice > 3) 
    {
        cout << "Invalid choice! Enter a number between 1 and 3: ";
        cin >> vehicleTypeChoice;
    }

    selectedVehicleType = vehicleTypesArray[vehicleTypeChoice - 1];

    cout << "\nSelect Challan Reason:\n"
         << "    1. Over-speeding\n"
         << "    2. Signal Violation\n"
         << "    3. No Helmet\n"
         << "    4. Other\n"
         << "Enter your choice (1-4): ";

    cin >> challanReasonChoice;

    while (challanReasonChoice < 1 || challanReasonChoice > 4) 
    {
        cout << "Invalid choice! Enter a number between 1 and 4: ";
        cin >> challanReasonChoice;
    }

    cout << "Enter Fine Amount: ";
    cin >> fine;

    while (fine <= 0) 
    {
        cout << "Invalid fine! Enter a positive number: ";
        cin >> fine;
    }

    // Save log history the the challan file
    ofstream logFile(fileName, ios::app);
    if (!logFile) 
    {
        cerr << "Error opening log file!\n";
        return;
    }
    logFile << vehicleNumber << "|" << selectedVehicleType << "|"
            << challanReasonsArray[challanReasonChoice - 1] << "|" << fine << "|" << city << "\n";
    logFile.close();

}

//Helpline System
void helpline(string fileName, int issueCounts[4]) {
    int issueChoice;
    string issues[] = {"Accident", "Vehicle Breakdown", "Security Concern", "Medical Emergency"};
    string phoneNumbers[] = {"130", "1135", "15", "1122"};

    cout << "--- HELPLINE SYSTEM ---\n"
         << "Select Issue:\n"
         << "    1. Accident\n"
         << "    2. Vehicle Breakdown\n"
         << "    3. Security Concern\n"
         << "    4. Medical Emergency\n"
         << "Enter your choice (1-4): ";
    cin >> issueChoice;

    while (issueChoice < 1 || issueChoice > 4) {
        cout << "Invalid choice! Enter a number between 1 and 4: ";
        cin >> issueChoice;
    }

    // Increment issue count on array index.
    issueCounts[issueChoice - 1]++;
    
    // Save data to helpline file
    ofstream outFile(fileName);
    if (!outFile) {
        cerr << "Error opening file: " << fileName << "\n";
        return;
    }
    for (int i = 0; i < 4; ++i) {
        outFile << issueCounts[i] << "\n";
    }
    outFile.close();
    
    cout << " 📞 Calling " << issues[issueChoice - 1] << ": " << phoneNumbers[issueChoice - 1] << "\n";

    
}
// View functions 

// Display challan logs from the log file
void view_challan(string fileName, string city) {
    ifstream inFile(fileName);
    if (!inFile) {
        cout << "No challan records found for " << city << ".\n";
        return;
    }

  cout << "\n------ Challan Logs for " << city << " -----------\n";
string line;
while (getline(inFile, line)) {
    string vehicleNo, type, reason, fine, city;
    stringstream ss(line);

    // Split the line by '|' from the file
    getline(ss, vehicleNo, '|');
    getline(ss, type, '|');
    getline(ss, reason, '|');
    getline(ss, fine, '|');
    getline(ss, city, '|');
    // Display the formatted output
    cout << "Vehicle Number : " << vehicleNo << "\n";
    cout << "Type           : " << type << "\n";
    cout << "Reason         : " << reason << "\n";
    cout << "Fine           : " << fine << "\n";
    cout << "City           : " << city << "\n\n";
}

    inFile.close();
    cout << "-------------------------------------------\n\n";
    cout << "✓ Returning to main menu...\n";
}

// Shows booth traffic data (entries and exits)
void view_booth_system(string fileName, int array[3][2], string city) {


      if (fileName.empty()) {
        cout << "No Booth records found for " << city << ".\n";
        return;
    }

    // Custom formatting copied for output

    cout << "\n--- Booth Traffic Data for " << city << " ---\n";
    cout << left << setw(20) << "Vehicle Type" << setw(10) << "Entry" << setw(10) << "Exit" << "\n";
    cout << "----------------------------------------\n";
    cout << left << setw(20) << "Two-Wheeler" << setw(10) << array[0][0] << setw(10) << array[0][1] << "\n";
    cout << left << setw(20) << "Four-Wheeler" << setw(10) << array[1][0] << setw(10) << array[1][1] << "\n";
    cout << left << setw(20) << "Heavy Vehicle" << setw(10) << array[2][0] << setw(10) << array[2][1] << "\n";
    cout << "----------------------------------------\n";
       cout << "✓ Returning to main menu...\n";
}

// Displays helpline usage statistics
   void view_helpline(string fileName, int issueCounts[4],string city)
    {
        
      if (fileName.empty()) {
        cout << "No Helpline records found for " << city << ".\n";
        return;
    }
    cout << "\n--- Helpline Usage Logs for " << city << " ---------\n";
    cout << "Accident Calls: " << issueCounts[0] << "\n";
    cout << "Vehicle Issue Calls: " << issueCounts[1] << "\n";
    cout << "Security Concern Calls: " << issueCounts[2] << "\n";
    cout << "Medical Emergency Calls: " << issueCounts[3] << "\n";
    cout << "------------------------------------------\n\n";
    cout << "✓ Returning to main menu...\n";
}

// Load functions (reading data from files):

// Loads booth entry/exit counts from a file
void load_booth_data(string fileName, int array[3][2]) {
    ifstream inFile(fileName);
    if (!inFile) {
        // Initialize to zero if file doesn't exist
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 2; ++j) {
                array[i][j] = 0;
            }
        }
        return;
    }
    for (int i = 0; i < 3; ++i) {
        inFile >> array[i][0] >> array[i][1];
    }
    inFile.close();
}

// Loads helpline issue counts from a file
void load_helpline_data(string fileName, int issueCounts[4]) {
    ifstream inFile(fileName);
    if (!inFile) {
        // Initialize to zero if file doesn't exist
        for (int i = 0; i < 4; ++i) 
        {
            issueCounts[i] = 0;
        }
        return;
    }
    // Reads issue count from file
    for (int i = 0; i < 4; ++i) {
        inFile >> issueCounts[i];
    }
    inFile.close();
}



// Reset files for a specific city
void resetCityFile(string cityName) {
    string boothFileName = cityName + "Booth_System.txt";
    string challanFileName = cityName + "Challan_Logs.txt";
    string helplineFileName = cityName + "Helpline_System.txt";

    // Reset booth file with default entries
    ofstream boothFile(boothFileName);
    if (boothFile.is_open()) {
        boothFile << "0 0\n0 0\n0 0\n"; // default entries for each vehicle type
        boothFile.close();
        cout << "✓ " << boothFileName << " reset.\n";
    } else {
        cerr << "✗ Could not open " << boothFileName << "\n";
    }

    // Reset helpline file with default issue counts
    ofstream helplineFile(helplineFileName);
    if (helplineFile.is_open()) {
        helplineFile << "0\n0\n0\n0\n"; // default for each helpline issue
        helplineFile.close();
        cout << "✓ " << helplineFileName << " reset.\n";
    } else {
        cerr << "✗ Could not open " << helplineFileName << "\n";
    }

    // Truncate challan file 
    ofstream challanFile(challanFileName, ios::trunc);
    if (challanFile.is_open()) {
        challanFile.close();
        cout << "✓ " << challanFileName << " cleared.\n";
    } else {
        cerr << "✗ Could not open " << challanFileName << "\n";
    }

    cout << "\n🎉 All data reset for city: " << cityName << "!\n";
}


   