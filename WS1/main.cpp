#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <string>
#include <iomanip>
#include <limits> // For std::numeric_limits
#include <chrono>
#include <ctime>
#include <algorithm>
#include<thread>
#include <algorithm> // Include this for std::all_of and std::any_of
#include <cctype>    // Include this for ::isdigit
#include "DatabaseConnection.h"

using namespace std;
// Function prototypes for login and verify

string currentUserRole; 
string currentUserID; 
string currentUserBatch;
string currentUserSquad;
string currentBatch;
string currentSquad;

// Global variable to store the role of the current user
float CalculateBMI(float weight, float height) {
    return weight / (height * height);  // BMI formula: weight (kg) / height (m)^2
}
struct Admin {
    string adminID;
    string name;
    string password;
    string phone;
    int age;
    string gender;
};
Admin adminUsers[3]; // Array to store up to 3 Admin users
int adminCount = 0;  // Track the number of Admin users
void LoadAdminsFromDatabase();

void Login();
void getPasswordWithConfirmation(string& password);
void Register();
bool VerifyAdmin();
bool VerifyCoach();
bool VerifyCadet();
bool VerifyCadetID(const string& cadetID);
void AssignCoachToBatchAndSquad();
bool IsCadetInSquad(const string& cadetID, const string& currentSquad);


// function prototypes for add/remove /delete /view user
void EditAdmin();
void ViewAdminDetails();
void AddAdmin();
void RemoveAdmin();


void EditCoach();
void ViewCoachDetails();
void AddCoach();
void RemoveCoach();
void SearchCoach();

void EditCadet();
void ViewCadetDetails();
void AddCadet();
void RemoveCadet();
void SearchCadet();

//function for menu page
void AdminMenu();
void CoachMenu();
void CadetMenu();
void FitnessMenu();
void ReportMenu();

//fucntion for manage page
void ManageCoach();
void ManageAdmin();
void ManageCadet();
void ManageOfficialFitness();
void ManageUnofficialFitness();
void DisplayOverallFitness(string cadetID);

//function for view fitness
void ViewOfficialFitness();
void ViewUnofficialFitness();

//function for view fitness
void AddOfficialFitness();
void AddUnofficialFitness();


//function for view fitness
void RemoveOfficialFitness();
void RemoveUnofficialFitness();

//function for view fitness
void EditOfficialFitness();
void EditUnofficialFitness();

//function update 
void UpdateOfficialFitness();
void UpdateUnofficialFitness();

//function for view fitness
void SearchOfficialFitness();
void SearchUnofficialFitness();

void DisplayOverallOfficialFitness();
void DisplayOverallUnofficialFitness();
void CalculateOverallUnofficialFitness(string cadetID);
double CalculateOverallFitness(string cadetID);
double CalculateOverallFitness(int pushup, int pullup, int situp, int lunges, int squat);
void OfficialFitnessReport();
void UnofficialFitnessReport();
void GenerateFitnessReport(const string& query, const string& reportType);
void DisplayFeedbackAndRecommendations(int pushup, int situp, int pullup, int lunges, int squat, float bmi, float beepTest, float distance, double overallPercentage);


//main function
int main();

// verify and login function //
void Login() {
    string cadetSquad;

    system("cls");
    cout << "===== Login Menu =====" << endl;
    int choice;
    cout << "1. Login as Admin" << endl;
    cout << "2. Login as Cadet" << endl;
    cout << "3. Login as Coach" << endl;
    cout << "4. Back to Main Menu" << endl;
    cout << "Choose: ";
    cin >> choice;

    switch (choice) {
    case 1:
        if (VerifyAdmin()) {
            currentUserRole = "Admin";
            AdminMenu();
        }
        else {
            cout << "Admin verification failed. Press Enter to retry...";
            _getch();
            Login();
        }
        break;
    case 2:
        if (VerifyCadet()) {
            currentUserRole = "Cadet";
            string cadetID = currentUserID; // Assuming currentUserID is set with cadet's ID

            // Query to fetch the squad for this cadet
            string query = "SELECT squad FROM Cadet WHERE cadetID = '" + cadetID + "'";
            const char* q = query.c_str();
            int qstate = mysql_query(conn, q);

            if (!qstate) {
                MYSQL_RES* res = mysql_store_result(conn);
                MYSQL_ROW row = mysql_fetch_row(res);

                if (row) {
                    // Assign the squad value to currentSquad
                    currentSquad = row[0];
                    cout << "DEBUG: Cadet logged in. Assigned Squad = " << currentSquad << endl; // Debugging output
                    CadetMenu();
                }
                else {
                    cout << "Error: No squad assigned to this cadet. Press Enter to retry...";
                    _getch();
                    Login(); // Retry the login process
                }
                mysql_free_result(res);
            }
            else {
                cout << "Query Execution Problem: " << mysql_error(conn) << endl;
                _getch();
                Login(); // Retry the login process
            }
        }
        else {
            cout << "Cadet verification failed. Press Enter to retry...";
            _getch();
            Login(); // Retry the login process
        }
        break;

    case 3:
        if (VerifyCoach()) {
            currentUserRole = "Coach";
            string coachID = currentUserID; // Assuming currentUserID is set with coach's ID

            // Query to fetch the squad for this coach
            string query = "SELECT squad FROM Coach WHERE coachID = '" + coachID + "'";
            const char* q = query.c_str();
            int qstate = mysql_query(conn, q);

            if (!qstate) {
                MYSQL_RES* res = mysql_store_result(conn);
                MYSQL_ROW row = mysql_fetch_row(res);

                if (row) {
                    // Assign the squad value to currentSquad
                    currentSquad = row[0];
                    cout << "DEBUG: Coach logged in. Assigned Squad = " << currentSquad << endl; // Debugging output
                    CoachMenu();
                }
                else {
                    cout << "Error: No squad assigned to this coach. Press Enter to retry...";
                    _getch();
                    Login(); // Retry the login process
                }
                mysql_free_result(res);
            }
            else {
                cout << "Query Execution Problem: " << mysql_error(conn) << endl;
                _getch();
                Login(); // Retry the login process
            }
        }
        else {
            cout << "Coach verification failed. Press Enter to retry...";
            _getch();
            Login(); // Retry the login process
        }
        break;

    }
}


void getPasswordWithConfirmation(string& password) {
    password = "";  // Clear previous password
    char ch;
    while (true) {
        ch = _getch();  // Get character input
        if (ch == 13) {  // Enter key
            break;
        }
        else if (ch == 8) {  // Backspace key
            if (!password.empty()) {
                password.pop_back();  // Remove the last character
                cout << "\b \b";  // Erase '*' on the screen
            }
        }
        else {
            password += ch;  // Append the character to password
            cout << "*";  // Display '*' instead of the character
        }
    }
    cout << endl;  // Add a new line after input
}
void Register() {
    system("cls");
    cout << "--- Registration Menu ---" << endl;
    int roleChoice;
    cout << "1. Register as Admin" << endl;
    cout << "2. Register as Cadet" << endl;
    cout << "3. Register as Coach" << endl;
    cout << "4. Back to Main Menu" << endl;
    cout << "Choose: ";
    cin >> roleChoice;

    if (roleChoice == 4) {
        return; // Exit to main menu
    }

    if (conn == nullptr) {
        cout << "ERROR: Database connection failed. Returning to main menu." << endl;
        _getch();
        return;
    }

    // Define variables
    string name, phone, age, username, password, passwordConfirm, gender;
    string batch = "Not Assigned", squad = "Not Assigned"; // Default
    int ageValue;

    // Input name (validate length)
    cin.ignore();
    while (true) {
        cout << "Enter Name (max 20 characters): ";
        getline(cin, name);
        if (name.length() <= 20) break;
        cout << "Name too long. Try again.\n";
    }

    // Input phone number (validate format)
    while (true) {
        cout << "Enter Phone (11 digits, e.g., 01812345678): ";
        getline(cin, phone);
        if (phone.length() == 11 && phone.find('-') == string::npos &&
            std::all_of(phone.begin(), phone.end(), ::isdigit)) {
            break;
        }
        cout << "Invalid phone number. Try again.\n";
    }

    // Input age (validate based on role)
    while (true) {
        cout << "Enter Age: ";
        getline(cin, age);
        try {
            ageValue = stoi(age); // Convert to integer
            if ((roleChoice == 2 && ageValue > 19 && ageValue <= 25) ||
                (roleChoice == 3 && ageValue > 25) ||
                roleChoice == 1) {
                break;
            }
            cout << "Invalid age. Cadets must be 20-25 years old, and Coaches must be older than 25.\n";
        }
        catch (...) {
            cout << "Invalid age input. Try again.\n";
        }
    }

    // Input username
    cout << "Enter Username: ";
    getline(cin, username);

    // Check if user already exists
    string checkQuery;
    if (roleChoice == 1) {
        checkQuery = "SELECT COUNT(*) FROM Admin WHERE adminID = '" + username + "'";
    }
    else if (roleChoice == 2) {
        checkQuery = "SELECT COUNT(*) FROM Cadet WHERE cadetID = '" + username + "'";
    }
    else if (roleChoice == 3) {
        checkQuery = "SELECT COUNT(*) FROM Coach WHERE coachID = '" + username + "'";
    }

    if (mysql_query(conn, checkQuery.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        if (row && stoi(row[0]) > 0) {
            cout << "Error: Username '" << username << "' already exists. Please try a different username." << endl;
            mysql_free_result(res);
            _getch();
            return;
        }
        mysql_free_result(res);
    }
    else {
        cout << "Error checking user existence: " << mysql_error(conn) << endl;
        _getch();
        return;
    }

    // Input and validate password
    while (true) {
        cout << "Enter Password (min 6 characters with at least one number or symbol): ";
        getPasswordWithConfirmation(password);
        if (password.length() >= 6 &&
            std::any_of(password.begin(), password.end(), ::isdigit)) {
            break;
        }
        cout << "Password does not meet requirements. Try again.\n";
    }

    // Confirm password
    cout << "Re-enter Password: ";
    getPasswordWithConfirmation(passwordConfirm);
    if (password != passwordConfirm) {
        cout << "Passwords do not match. Press Enter to retry..." << endl;
        _getch();
        Register(); // Restart registration
        return;
    }

    // Input gender
    while (true) {
        cout << "Enter Gender (M for Male, F for Female): ";
        cin >> gender;
        if (gender == "M" || gender == "F") break;
        cout << "Invalid gender input. Please enter 'M' or 'F'.\n";
    }

    // Handle Admin limitation
    if (roleChoice == 1) { // Admin
        // Validation for Admin limit
        string validateQuery = "SELECT COUNT(*) FROM Admin";
        if (mysql_query(conn, validateQuery.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);
            int count = atoi(row[0]);
            mysql_free_result(res);
            if (count >= 3) { // Limit to 3 Admins
                cout << "Error: Maximum number of Admin users reached. Returning to main menu.\n";
                _getch();
                return; // Exit registration for Admin
            }
        }
        else {
            cout << "Database validation error: " << mysql_error(conn) << endl;
            _getch();
            return; // Exit on error
        }
    }

    // Handle batch and squad for Cadet
    if (roleChoice == 2) { // Cadet
        while (true) {
            cout << "Select Batch:\n1. 2023 (Alpha)\n2. 2024 (Bravo)\n3. 2025 (Charlie)\nChoose: ";
            int batchChoice;
            cin >> batchChoice;

            if (batchChoice == 1) {
                batch = "2023";
                squad = "Alpha";
            }
            else if (batchChoice == 2) {
                batch = "2024";
                squad = "Bravo";
            }
            else if (batchChoice == 3) {
                batch = "2025";
                squad = "Charlie";
            }
            else {
                cout << "Invalid choice. Try again.\n";
                continue;
            }

            // Validation for batch limit (Cadets per batch)
            string validateQuery = "SELECT COUNT(*) FROM Cadet WHERE batch = '" + batch + "'";
            if (mysql_query(conn, validateQuery.c_str()) == 0) {
                MYSQL_RES* res = mysql_store_result(conn);
                MYSQL_ROW row = mysql_fetch_row(res);
                int count = atoi(row[0]);
                mysql_free_result(res);
                if (count >= 30) { // Limit to 30 cadets per batch
                    cout << "Error: The selected batch already has the maximum number of cadets (30). Choose a different batch.\n";
                    _getch();
                    continue; // Loop again to select another batch
                }
            }
            else {
                cout << "Database validation error: " << mysql_error(conn) << endl;
                _getch();
                return; // Exit on error
            }
            break; // Exit loop if validation passes
        }
    }

    // Handle batch and squad for Coach
    if (roleChoice == 3) { // Coach
        while (true) {
            cout << "Select Batch:\n1. 2023 (Alpha)\n2. 2024 (Bravo)\n3. 2025 (Charlie)\nChoose: ";
            int batchChoice;
            cin >> batchChoice;

            if (batchChoice == 1) {
                batch = "2023";
                squad = "Alpha";
            }
            else if (batchChoice == 2) {
                batch = "2024";
                squad = "Bravo";
            }
            else if (batchChoice == 3) {
                batch = "2025";
                squad = "Charlie";
            }
            else {
                cout << "Invalid choice. Try again.\n";
                continue;
            }

            // Validation for batch limit
            string validateQuery = "SELECT COUNT(*) FROM Coach WHERE batch = '" + batch + "'";
            if (mysql_query(conn, validateQuery.c_str()) == 0) {
                MYSQL_RES* res = mysql_store_result(conn);
                MYSQL_ROW row = mysql_fetch_row(res);
                int count = atoi(row[0]);
                mysql_free_result(res);
                if (count >= 3) { // Limit to 3 coaches per batch
                    cout << "Error: The selected batch already has the maximum number of coaches (3). Choose a different batch.\n";
                    _getch();
                    continue; // Loop again to select another batch
                }
            }
            else {
                cout << "Database validation error: " << mysql_error(conn) << endl;
                _getch();
                return; // Exit on error
            }
            break; // Exit loop if validation passes
        }
    }

    // Construct and execute INSERT query
    string insertQuery;
    if (roleChoice == 1) {
        insertQuery = "INSERT INTO Admin (name, numPhone, age, adminID, password, gender) VALUES ('" +
            name + "', '" + phone + "', '" + age + "', '" + username + "', '" + password + "', '" + gender + "')";
    }
    else if (roleChoice == 2) {
        insertQuery = "INSERT INTO Cadet (name, numPhone, age, cadetID, password, gender, batch, squad) VALUES ('" +
            name + "', '" + phone + "', '" + age + "', '" + username + "', '" + password + "', '" + gender + "', '" + batch + "', '" + squad + "')";
    }
    else if (roleChoice == 3) {
        insertQuery = "INSERT INTO Coach (name, numPhone, age, coachID, password, gender, batch, squad) VALUES ('" +
            name + "', '" + phone + "', '" + age + "', '" + username + "', '" + password + "', '" + gender + "', '" + batch + "', '" + squad + "')";
    }

    if (mysql_query(conn, insertQuery.c_str()) == 0) {
        cout << "Registration successful! Press Enter to return to the main menu...";
        _getch();
    }
    else {
        cout << "Error adding user: " << mysql_error(conn) << endl;
        _getch();
    }
}

// Verify Admin function
bool VerifyAdmin() {
    string adminID, password;
    cout << "Enter Admin ID : ";
    cin >> adminID;
    cout << "Enter password : ";
    getPasswordWithConfirmation(password);  // Using getPasswordWithConfirmation to hide input with *

    string query = "SELECT * FROM Admin WHERE adminID = '" + adminID + "' AND password = '" + password + "'";

    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) == 1) {
            currentUserID = adminID; // Assign the logged-in admin's ID to currentUserID
            cout << "Admin verified successfully. Press Enter to continue...";
            _getch();
            return true;
        }
        else {
            cout << "Invalid Admin ID or Password. Press Enter to retry...";
            _getch();
            return false;
        }
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
        return false;
    }
}

// Verify Coach function
bool VerifyCoach() {
    string coachID, password;
    cout << "Enter Coach ID: ";
    cin >> coachID;
    cout << "Enter Password: ";
    getPasswordWithConfirmation(password);

    string query = "SELECT squad FROM Coach WHERE coachID = '" + coachID + "' AND password = '" + password + "'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row) {
            currentUserID = coachID; // Set the current user ID
            currentSquad = row[0];  // Set the current squad of the coach
            cout << "Coach verified successfully. Press Enter to continue...";
            _getch();
            return true;
        }
        else {
            cout << "Invalid Coach ID or Password. Press Enter to retry...";
            _getch();
            return false;
        }
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
        return false;
    }
}
// Verify Cadet function
bool VerifyCadet() {
    string cadetID, password;
    cout << "Enter Cadet ID: ";
    cin >> cadetID;
    cout << "Enter Password: ";
    getPasswordWithConfirmation(password);

    string query = "SELECT * FROM Cadet WHERE cadetID = '" + cadetID + "' AND password = '" + password + "'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) == 1) {
            currentUserID = cadetID; // Set global current user ID
            cout << "Cadet verified successfully. Press Enter to continue...";
            _getch();
            return true;
        }
        else {
            cout << "Invalid Cadet ID or Password. Press Enter to retry...";
            _getch();
            return false;
        }
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
        return false;
    }
}


bool VerifyCadetID(const string& cadetID) {
    string query = "SELECT * FROM Cadet WHERE cadetID = '" + cadetID + "'"; // Check if CadetID exists in Cadet table
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res && mysql_num_rows(res) == 1) {
            return true; // Cadet ID exists
        }
        else {
            cout << "CadetID not found in Cadet table." << endl; // Debugging output
            return false; // Cadet ID does not exist in Cadet table
        }
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
        return false;
    }
}
bool IsCadetInSquad(const string& cadetID, const string& currentSquad) {
    string query = "SELECT cadetID FROM Cadet WHERE cadetID = '" + cadetID + "' AND squad = '" + currentSquad + "'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row) {
            mysql_free_result(res);
            return true; // Cadet exists in the current squad
        }
        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    return false; // Cadet does not exist in the current squad
}


void CurrentLogin() {
    if (currentUserRole == "Admin") {
        AdminMenu();
    }
    else if (currentUserRole == "Coach") {
        string coachID;
        cout << "Enter your Coach ID: ";
        cin >> coachID;

        // Fetch the assigned batch and squad for the logged-in coach
        string query = "SELECT batch, squad FROM Coach WHERE coachID = '" + coachID + "'";
        const char* q = query.c_str();
        int qstate = mysql_query(conn, q);

        if (!qstate) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);

            if (row) {
                string batch = row[0];
                string squad = row[1];
                cout << "Logged in as Coach for Batch: " << batch << ", Squad: " << squad << endl;
                _getch();

                // Display menu for managing only their assigned squad and batch
                int menu;
                while (true) {
                    system("cls");
                    cout << "----- Coach Menu -----" << endl;
                    cout << "1. View Cadet Details in Your Squad" << endl;
                    cout << "2. Manage Unofficial Fitness Records" << endl;
                    cout << "3. View Fitness Reports of Your Squad" << endl;
                    cout << "4. Logout" << endl;
                    cout << "Choose: ";
                    cin >> menu;

                    switch (menu) {
                    case 1:
                        ViewCadetDetails();
                        break;
                    case 2:
                        ManageUnofficialFitness();
                        break;
                    case 3:
                        ReportMenu();
                        break;
                    case 4:
                        return;  // Logout
                    default:
                        cout << "Invalid choice! Press Enter to retry...";
                        _getch();
                        break;
                    }
                }
            }
            else {
                cout << "Coach ID not found. Returning to login menu..." << endl;
                _getch();
                return;
            }
        }
        else {
            cout << "Query Execution Problem: " << mysql_error(conn) << endl;
            _getch();
            return;
        }
    }
    else if (currentUserRole == "Cadet") {
        string cadetID;
        cout << "Enter your Cadet ID: ";
        cin >> cadetID;

        // Verify the cadet and fetch basic details
        string query = "SELECT * FROM Cadet WHERE cadetID = '" + cadetID + "'";
        const char* q = query.c_str();
        int qstate = mysql_query(conn, q);

        if (!qstate) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);

            if (row) {
                cout << "Welcome Cadet: " << row[1] << endl;
                _getch();

                // Display menu for cadets
                int menu;
                while (true) {
                    system("cls");
                    cout << "----- Cadet Menu -----" << endl;
                    cout << "1. View Your Fitness Records" << endl;
                    cout << "2. Manage Your Unofficial Fitness Records" << endl;
                    cout << "3. Generate Your Fitness Report" << endl;
                    cout << "4. Logout" << endl;
                    cout << "Choose: ";
                    cin >> menu;

                    switch (menu) {
                    case 1:
                        ViewOfficialFitness();
                        break;
                    case 2:
                        ManageUnofficialFitness();
                        break;
                    case 3:
                        ReportMenu();
                        break;
                    case 4:
                        return;  // Logout
                    default:
                        cout << "Invalid choice! Press Enter to retry...";
                        _getch();
                        break;
                    }
                }
            }
            else {
                cout << "Cadet ID not found. Returning to login menu..." << endl;
                _getch();
                return;
            }
        }
        else {
            cout << "Query Execution Problem: " << mysql_error(conn) << endl;
            _getch();
            return;
        }
    }
    else {
        cout << "Invalid user role. Returning to login menu..." << endl;
        _getch();
    }
}



// Edit Admin details
void EditAdmin() {
    system("cls");
    string id, field, newValue;

    // Restrict normal Admins from editing other admins
    if (currentUserRole == "Admin") {
        id = currentUserID; // Ensure they edit only their own details
    }
    else {
        // Super Admin (if applicable) can edit other admins
        cout << "Enter Admin ID to edit: ";
        cin >> id;
    }

    cout << "Enter the field to edit (e.g., name, numPhone, age, password, gender): ";
    cin >> field;
    cout << "Enter the new value: ";
    cin.ignore();
    getline(cin, newValue);

    string query = "UPDATE Admin SET " + field + " = '" + newValue + "' WHERE adminID = '" + id + "'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate) {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    else {
        cout << "Admin details updated successfully. Press Enter to continue...";
    }
    _getch();
}

// Edit Coach details
void EditCoach() {
    system("cls");
    string id, field, newValue;

    // Check if the current user is a Coach and restrict edits to their own details
    if (currentUserRole == "Coach") {
        id = currentUserID; // Restrict to current logged-in coach
    }
    else if (currentUserRole == "Admin") {
        cout << "Enter Coach ID to edit: ";
        cin >> id; // Admins can specify any coach ID
    }
    else {
        cout << "Access Denied: You do not have permission to edit coach details." << endl;
        cout << "Press Enter to return to the menu...";
        _getch();
        return;
    }

    cout << "Enter the field to edit (e.g., name, numPhone, age, password, gender): ";
    cin >> field;

    // Validate the field input
    if (field != "name" && field != "numPhone" && field != "age" &&
        field != "password" && field != "gender") {
        cout << "Invalid field entered. Returning to the menu...\n";
        _getch();
        return;
    }

    if (field == "name") {
        // Name must be max 20 characters
        cout << "Enter the new name (max 20 characters): ";
        cin.ignore();
        getline(cin, newValue);
        if (newValue.length() > 20) {
            cout << "Error: Name cannot exceed 20 characters. Returning to the menu...\n";
            _getch();
            return;
        }
    }
    else if (field == "password") {
        // Password must be hidden and validated
        string confirmPassword;
        cout << "Enter the new password (min 6 characters with at least one symbol or number): ";
        getPasswordWithConfirmation(newValue);
        if (newValue.length() < 6 ||
            !std::any_of(newValue.begin(), newValue.end(), ::isdigit) &&
            !std::any_of(newValue.begin(), newValue.end(), ::ispunct)) {
            cout << "Error: Password must be at least 6 characters long with at least one number or symbol. Returning to the menu...\n";
            _getch();
            return;
        }

        // Confirm password
        cout << "Confirm the new password: ";
        getPasswordWithConfirmation(confirmPassword);
        if (newValue != confirmPassword) {
            cout << "Error: Passwords do not match. Returning to the menu...\n";
            _getch();
            return;
        }
    }
    else if (field == "age") {
        // Age must be 26 or above
        cout << "Enter the new age (26 or above): ";
        cin >> newValue;
        try {
            int age = stoi(newValue);
            if (age < 26) {
                cout << "Error: Age must be 26 or above. Returning to the menu...\n";
                _getch();
                return;
            }
        }
        catch (...) {
            cout << "Error: Invalid age input. Returning to the menu...\n";
            _getch();
            return;
        }
    }
    else if (field == "numPhone") {
        // Phone number must be 11 digits
        cout << "Enter the new phone number (11 digits): ";
        cin >> newValue;
        if (newValue.length() != 11 || !std::all_of(newValue.begin(), newValue.end(), ::isdigit)) {
            cout << "Error: Phone number must be exactly 11 digits. Returning to the menu...\n";
            _getch();
            return;
        }
    }
    else if (field == "gender") {
        // Gender must be M or F
        cout << "Enter the new gender (M for Male, F for Female): ";
        cin >> newValue;
        if (newValue != "M" && newValue != "F") {
            cout << "Error: Gender must be 'M' or 'F'. Returning to the menu...\n";
            _getch();
            return;
        }
    }

    // Update the Coach's information in the database
    string query = "UPDATE Coach SET " + field + " = '" + newValue + "' WHERE coachID = '" + id + "'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate) {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    else {
        cout << "Coach details updated successfully. Press Enter to continue...";
    }
    _getch();
}

// Edit Cadet details
void EditCadet() {
    system("cls");
    string id, field, newValue;

    cout << "Enter Cadet ID to edit: ";
    cin >> id;

    // Check if the Cadet exists
    string checkQuery = "SELECT COUNT(*) FROM Cadet WHERE cadetID = '" + id + "'";
    const char* checkQ = checkQuery.c_str();
    int checkState = mysql_query(conn, checkQ);

    if (!checkState) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row && stoi(row[0]) > 0) { // Cadet exists
            mysql_free_result(res);

            // Proceed with editing
            cout << "Enter the field to edit (e.g., name, numPhone, age, password, gender, batch, squad): ";
            cin >> field;

            // Validate field input (to prevent SQL injection issues)
            if (field != "name" && field != "numPhone" && field != "age" &&
                field != "password" && field != "gender" && field != "batch" && field != "squad") {
                cout << "Invalid field entered. Returning to the menu...\n";
                _getch();
                return;
            }

            cout << "Enter the new value: ";
            cin.ignore();
            getline(cin, newValue);

            // Update the Cadet's information
            string query = "UPDATE Cadet SET " + field + " = '" + newValue + "' WHERE cadetID = '" + id + "'";
            const char* q = query.c_str();
            int qstate = mysql_query(conn, q);

            if (qstate) {
                cout << "Query Execution Problem: " << mysql_error(conn) << endl;
            }
            else {
                cout << "Cadet details updated successfully. Press Enter to continue...";
            }
        }
        else {
            cout << "Cadet with ID '" << id << "' does not exist. Returning to the menu...\n";
            mysql_free_result(res);
        }
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    _getch();
}



// View Admin details
void ViewAdminDetails() {
    system("cls");

    // Ensure the current admin ID is available
    string currentAdminID = currentUserID; // Assuming 'currentUserID' stores the logged-in admin's ID

    if (currentAdminID.empty()) {
        cout << "Error: Current admin ID is not available. Please log in again." << endl;
        _getch();
        return;
    }

    // Display menu options for viewing details
    cout << "==========================================" << endl;
    cout << "           ADMIN DETAILS VIEWER           " << endl;
    cout << "==========================================" << endl;
    cout << "1. View My Details" << endl;
    cout << "2. View All Admins' Details" << endl;
    cout << "==========================================" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        // Constructing query to get the logged-in admin's details
        string query = "SELECT adminID, name, numPhone, age, gender FROM Admin WHERE adminID = '" + currentAdminID + "'";
        const char* q = query.c_str();
        int qstate = mysql_query(conn, q);

        if (!qstate) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row;

            cout << "\n==========================================" << endl;
            cout << "           YOUR ADMIN DETAILS             " << endl;
            cout << "==========================================" << endl;

            // Top border
            cout << "+------------+----------------------+-------------+------+--------+" << endl;

            // Table header
            cout << "| ID         | Name                 | Phone       | Age  | Gender |" << endl;

            // Separator
            cout << "+------------+----------------------+-------------+------+--------+" << endl;

            // Fetch and display data
            if ((row = mysql_fetch_row(res))) {
                cout << "| " << left << setw(11) << row[0]  
                    << "| " << setw(21) << row[1]       
                    << "| " << setw(12) << row[2]       
                    << "| " << setw(5) << row[3]        
                    << "| " << setw(7) << row[4]        
                    << "|" << endl;
            }

            // Bottom border
            cout << "+------------+----------------------+-------------+------+--------+" << endl;

            mysql_free_result(res);
        }
        else {
            cout << "Query Execution Problem: " << mysql_error(conn) << endl;
        }
    }
    else if (choice == 2) {
        // Constructing query to get all admins' details
        string query = "SELECT adminID, name, numPhone, age, gender FROM Admin";
        const char* q = query.c_str();
        int qstate = mysql_query(conn, q);

        if (!qstate) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row;

            cout << "\n\n==========================================" << endl;
            cout << "          ALL ADMINS' DETAILS             " << endl;
            cout << "==========================================" << endl;

            // Adjust `setw()` values and borders
            cout << "\n+------------+----------------------+---------------+-------+--------+" << endl; // 12, 20, 15, 6, 8
            cout << "| ID         | Name                 | Phone         | Age   | Gender |" << endl;
            cout << "+------------+----------------------+---------------+-------+--------+" << endl;


            // Fetch and display data
            while ((row = mysql_fetch_row(res))) {
                cout <<"| " << left << setw(11) << row[0]  // Adjust for ID
                    << "| " << setw(21) << row[1]        // Adjust for Name
                    << "| " << setw(14) << row[2]        // Adjust for Phone
                    << "| " << setw(6) << row[3]         // Adjust for Age
                    << "| " << setw(7) << row[4]         // Adjust for Gender
                    << "|" << endl;

            }

            // Bottom border
            cout << "+------------+----------------------+---------------+-------+--------+" << endl;

            mysql_free_result(res);
        }
        else {
            cout << "Query Execution Problem: " << mysql_error(conn) << endl;
        }
    }
    else {
        cout << "Invalid choice. Returning to the main menu." << endl;
    }

    cout << "\nPress Enter to continue...";
    _getch();
}

// Function to display Coach details
void ViewCoachDetails() {
    system("cls");
    string query;

    if (currentUserRole == "Admin") {
        // Admin can view all coach details
        query = "SELECT coachID, name, numPhone, age, gender, squad FROM Coach";
    }
    else if (currentUserRole == "Coach") {
        // Coach can view their own details or other coaches in their squad
        int choice;
        cout << "1. View My Details\n2. View Coaches in My Squad\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            query = "SELECT coachID, name, numPhone, age, gender, squad FROM Coach WHERE coachID = '" + currentUserID + "'";
        }
        else if (choice == 2) {
            query = "SELECT coachID, name, numPhone, age, gender, squad FROM Coach WHERE squad = '" + currentSquad + "'";
        }
        else {
            cout << "Invalid option. Returning to the menu..." << endl;
            _getch();
            return;
        }
    }
    else if (currentUserRole == "Cadet") {
        // Cadet can view their assigned coach details
        query = "SELECT coachID, name, numPhone, age, gender, squad FROM Coach WHERE squad = '" + currentSquad + "'";
    }
    else {
        cout << "Access Denied. You don't have permission to view this information." << endl;
        _getch();
        return;
    }

    // Execute query and display results
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "------------------------------------------------------------------------------------" << endl;
        cout << "| " << left << setw(10) << "ID"
            << "| " << setw(20) << "Name"
            << "| " << setw(15) << "Phone"
            << "| " << setw(5) << "Age"
            << "| " << setw(10) << "Gender"
            << "| " << setw(10) << "Squad" << " |" << endl;
        cout << "------------------------------------------------------------------------------------" << endl;

        while ((row = mysql_fetch_row(res))) {
            cout << "| " << left << setw(10) << row[0]
                << "| " << setw(20) << row[1]
                << "| " << setw(15) << row[2]
                << "| " << setw(5) << (row[3] ? row[3] : "N/A")
                << "| " << setw(10) << row[4]
                << "| " << setw(10) << row[5] << " |" << endl;
        }

        cout << "------------------------------------------------------------------------------------" << endl;
        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    cout << "Press Enter to continue...";
    _getch();
}



// Function to display Cadet details
void ViewCadetDetails() {
    system("cls");
    string query;

    if (currentUserRole == "Admin") {
        int choice;
        cout << "1. View All Cadets\n2. View Cadets by Squad\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            query = "SELECT cadetID, name, numPhone, batch, gender, squad, age FROM Cadet";
        }
        else if (choice == 2) {
            string squadFilter;
            cout << "Enter Squad to filter: ";
            cin.ignore();
            getline(cin, squadFilter);

            query = "SELECT cadetID, name, numPhone, batch, gender, squad, age FROM Cadet WHERE squad = '" + squadFilter + "'";
        }
        else {
            cout << "Invalid option. Returning to the menu..." << endl;
            _getch();
            return;
        }
    }
    else if (currentUserRole == "Coach") {
        if (!currentSquad.empty()) {
            query = "SELECT cadetID, name, numPhone, batch, gender, squad, age FROM Cadet WHERE squad = '" + currentSquad + "'";
        }
        else {
            cout << "Error: No squad assigned to the coach. Contact the admin. Press Enter to return...";
            _getch();
            return;
        }
    }
    else if (currentUserRole == "Cadet") {
        int choice;
        cout << "1. View My Details\n2. View Cadets in My Squad\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            query = "SELECT cadetID, name, numPhone, batch, gender, squad, age FROM Cadet WHERE cadetID = '" + currentUserID + "'";
        }
        else if (choice == 2) {
            query = "SELECT cadetID, name, numPhone, batch, gender, squad, age FROM Cadet WHERE squad = '" + currentSquad + "' AND cadetID != '" + currentUserID + "'";
        }
        else {
            cout << "Invalid option. Returning to the menu..." << endl;
            _getch();
            return;
        }
    }
    else {
        cout << "Access Denied. You don't have permission to view this information." << endl;
        _getch();
        return;
    }

   

    // Execute query
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "------------------------------------------------------------------------------------------------" << endl;
        cout << "| " << left << setw(10) << "ID"
            << "| " << setw(20) << "Name"
            << "| " << setw(15) << "Phone"
            << "| " << setw(10) << "Batch"
            << "| " << setw(10) << "Gender"
            << "| " << setw(10) << "Squad"
            << "| " << setw(5) << "Age" << " |" << endl;
        cout << "------------------------------------------------------------------------------------------------" << endl;

        while ((row = mysql_fetch_row(res))) {
            cout << "| " << left << setw(10) << row[0]
                << "| " << setw(20) << row[1]
                << "| " << setw(15) << row[2]
                << "| " << setw(10) << row[3]
                << "| " << setw(10) << row[4]
                << "| " << setw(10) << row[5]
                << "| " << setw(5) << (row[6] ? row[6] : "N/A") << " |" << endl;
        }

        cout << "------------------------------------------------------------------------------------------------" << endl;
        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    cout << "Press Enter to continue...";
    _getch();
}







// Remove Admin
void RemoveAdmin() {
    system("cls");
    string id;
    char confirmation;

    // Ask for the Admin ID to delete
    cout << "Enter Admin ID to remove: ";
    cin >> id;

    // Check if the admin exists in the database
    string checkQuery = "SELECT COUNT(*) FROM Admin WHERE adminID = '" + id + "'";
    const char* checkQ = checkQuery.c_str();
    int checkState = mysql_query(conn, checkQ);

    if (!checkState) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row && stoi(row[0]) > 0) { // If admin exists
            // Confirm deletion
            cout << "Are you sure you want to delete Admin ID '" << id << "'? (Y/N): ";
            cin >> confirmation;

            // If user confirms, proceed with deletion
            if (toupper(confirmation) == 'Y') {
                string query = "DELETE FROM Admin WHERE adminID = '" + id + "'";
                const char* q = query.c_str();
                int qstate = mysql_query(conn, q);

                if (!qstate) {
                    cout << "Admin removed successfully. Press Enter to continue...";
                    // Reload Admin data into memory if needed
                    // LoadAdminsFromDatabase();
                }
                else {
                    cout << "Query Execution Problem: " << mysql_error(conn) << endl;
                }
            }
            else {
                cout << "Operation cancelled. Press Enter to return to the menu...";
            }
        }
        else {
            // If admin does not exist
            cout << "Error: Admin ID '" << id << "' does not exist. Returning to the menu..." << endl;
            _getch();
            return;
        }

        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    cin.ignore();
    cin.get();
}

// Remove Coach
void RemoveCoach() {
    system("cls");
    string id;
    char confirmation;

    // Ask for the Coach ID to delete
    cout << "Enter Coach ID to remove: ";
    cin >> id;

    // Check if the coach exists in the database
    string checkQuery = "SELECT COUNT(*) FROM Coach WHERE coachID = '" + id + "'";
    const char* checkQ = checkQuery.c_str();
    int checkState = mysql_query(conn, checkQ);

    if (!checkState) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row && stoi(row[0]) > 0) { // If coach exists
            // Confirm deletion
            cout << "Are you sure you want to delete Coach ID '" << id << "'? (Y/N): ";
            cin >> confirmation;

            // If user confirms, proceed with deletion
            if (toupper(confirmation) == 'Y') {
                string query = "DELETE FROM Coach WHERE coachID = '" + id + "'";
                const char* q = query.c_str();
                int qstate = mysql_query(conn, q);

                if (!qstate) {
                    cout << "Coach removed successfully. Press Enter to continue...";
                }
                else {
                    cout << "Query Execution Problem: " << mysql_error(conn) << endl;
                }
            }
            else {
                cout << "Operation cancelled. Press Enter to return to the menu...";
            }
        }
        else {
            // If coach does not exist
            cout << "Error: Coach ID '" << id << "' does not exist. Returning to the menu..." << endl;
            _getch();
            return;
        }

        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    cin.ignore();
    _getch();
}


// Remove Cadet
void RemoveCadet() {
    system("cls");
    string id;
    char confirmation;

    // Ask for the Cadet ID to delete
    cout << "Enter Cadet ID to remove: ";
    cin >> id;

    // Check if the cadet exists in the database
    string checkQuery = "SELECT COUNT(*) FROM Cadet WHERE cadetID = '" + id + "'";
    const char* checkQ = checkQuery.c_str();
    int checkState = mysql_query(conn, checkQ);

    if (!checkState) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row && stoi(row[0]) > 0) { // If cadet exists
            // Confirm deletion
            cout << "Are you sure you want to delete Cadet ID '" << id << "'? (Y/N): ";
            cin >> confirmation;

            // If user confirms, proceed with deletion
            if (toupper(confirmation) == 'Y') {
                string query = "DELETE FROM Cadet WHERE cadetID = '" + id + "'";
                const char* q = query.c_str();
                int qstate = mysql_query(conn, q);

                if (!qstate) {
                    cout << "Cadet removed successfully. Press Enter to continue...";
                }
                else {
                    cout << "Query Execution Problem: " << mysql_error(conn) << endl;
                }
            }
            else {
                // If user cancels, display a cancellation message
                cout << "Operation cancelled. Press Enter to return to the menu...";
            }
        }
        else {
            // If cadet does not exist
            cout << "Error: Cadet ID '" << id << "' does not exist. Returning to the main menu..." << endl;
        }

        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    cin.ignore();
    _getch();
}





// Add Admin
void AddAdmin() {
    system("cls");

    // Check if the admin count exceeds the limit
    string countQuery = "SELECT COUNT(*) FROM Admin";
    const char* cq = countQuery.c_str();
    int qstate = mysql_query(conn, cq);

    if (!qstate) {
        MYSQL_RES* resCount = mysql_store_result(conn);
        MYSQL_ROW rowCount = mysql_fetch_row(resCount);
        int adminCount = atoi(rowCount[0]); // Convert count to integer
        mysql_free_result(resCount);

        if (adminCount >= 3) {
            cout << "Cannot add more Admins. Maximum limit of 3 has been reached.\n";
            _getch();
            return; // Exit registration
        }
    }
    else {
        cout << "Error checking Admin count: " << mysql_error(conn) << endl;
        return; // Exit registration
    }

    // Proceed with registration if count is less than 3
    string name, phone, age, username, password, gender;

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Phone: ";
    getline(cin, phone);
    cout << "Enter Age: ";
    getline(cin, age);
    cout << "Enter Username: ";
    getline(cin, username);
    cout << "Enter Password: ";
    getline(cin, password);
    cout << "Enter Gender: ";
    getline(cin, gender);

    string query = "INSERT INTO Admin (name, numPhone, age, adminID, password, gender) VALUES ('" +
        name + "', '" + phone + "', '" + age + "', '" + username + "', '" + password + "', '" + gender + "')";
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    if (!qstate) {
        cout << "New Admin added successfully. Press Enter to continue...";
        _getch();
    }
    else {
        cout << "Error adding Admin: " << mysql_error(conn) << endl;
    }
}


void LoadAdminsFromDatabase() {
    string query = "SELECT * FROM Admin";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        adminCount = 0; // Reset the admin count
        while ((row = mysql_fetch_row(res)) && adminCount < 3) {
            adminUsers[adminCount++] = { row[0], row[1], row[2], row[3], atoi(row[4]), row[5] };
        }
    }
    else {
        cout << "Error loading Admin data: " << mysql_error(conn) << endl;
    }
}

// Add Coach
void AddCoach() {
    system("cls");

    string name, phone, age, username, password, gender, batch;
    int batchOption;

    // Input validation for name
    cout << "Enter Name (max 20 characters): ";
    cin.ignore();
    getline(cin, name);
    while (name.length() > 20) {
        cout << "Name too long! Please enter a name with a maximum of 20 characters: ";
        getline(cin, name);
    }

    // Input validation for phone number
    cout << "Enter Phone (max 11 digits): ";
    getline(cin, phone);
    while (phone.length() > 11 || !all_of(phone.begin(), phone.end(), ::isdigit)) {
        cout << "Invalid phone number! Enter a valid phone number with a maximum of 11 digits: ";
        getline(cin, phone);
    }

    // Input validation for age
    cout << "Enter Age: ";
    getline(cin, age);
    while (!all_of(age.begin(), age.end(), ::isdigit) || stoi(age) <= 0 || stoi(age) > 120) {
        cout << "Invalid age! Enter a valid age between 1 and 120: ";
        getline(cin, age);
    }

    // Input validation for username
    cout << "Enter Username: ";
    getline(cin, username);

    // Input validation for password
    cout << "Enter Password (min 6 characters, must include letters and symbols): ";
    getline(cin, password);
    while (password.length() < 6 || password.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") == string::npos) {
        cout << "Invalid password! Password must be at least 6 characters and include at least one symbol: ";
        getline(cin, password);
    }

    // Input validation for gender
    cout << "Enter Gender (M/F): ";
    getline(cin, gender);
    while (gender != "M" && gender != "F") {
        cout << "Invalid gender! Enter 'M' for Male or 'F' for Female: ";
        getline(cin, gender);
    }

    // Input batch and map to squad
    cout << "Enter Batch (1 for 2023, 2 for 2024, 3 for 2025): ";
    cin >> batchOption;
    while (batchOption < 1 || batchOption > 3) {
        cout << "Invalid option! Enter 1 for 2023, 2 for 2024, 3 for 2025: ";
        cin >> batchOption;
    }
    if (batchOption == 1) batch = "2023";
    else if (batchOption == 2) batch = "2024";
    else batch = "2025";

    string squad = (batch == "2023") ? "Alpha" : (batch == "2024") ? "Bravo" : "Charlie";

    // Insert into database
    string query = "INSERT INTO Coach (name, numPhone, age, coachID, password, gender, batch, squad) VALUES('" +
        name + "', '" + phone + "', '" + age + "', '" + username + "', '" + password + "', '" + gender + "', '" + batch + "', '" + squad + "')";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        cout << "New Coach added successfully. Press Enter to continue...";
        _getch();
    }
    else {
        cout << "Error adding Coach: " << mysql_error(conn) << endl;
    }
}


// Add Cadet
void AddCadet() {
    system("cls");

    string name, phone, age, username, password, gender, batch, squad;
    int ageInt;

    // Input and validate Name
    cin.ignore();
    while (true) {
        cout << "Enter Name (max 20 characters): ";
        getline(cin, name);
        if (name.length() <= 20) break;
        cout << "Name too long. Try again.\n";
    }

    // Input and validate Phone Number
    while (true) {
        cout << "Enter Phone (11 characters, numbers only): ";
        getline(cin, phone);
        if (phone.length() == 11 && all_of(phone.begin(), phone.end(), ::isdigit)) break;
        cout << "Invalid phone number. It must be 11 digits. Try again.\n";
    }

    // Input and validate Age
    while (true) {
        cout << "Enter Age (20-25): ";
        getline(cin, age);
        try {
            ageInt = stoi(age);  // Convert string to integer
            if (ageInt >= 20 && ageInt <= 25) break;
            cout << "Age must be between 20 and 25. Try again.\n";
        }
        catch (...) {
            cout << "Invalid age. Please enter a number. Try again.\n";
        }
    }

    // Input and validate Username
    while (true) {
        cout << "Enter Username: ";
        getline(cin, username);

        // Check if username already exists in the database
        string checkQuery = "SELECT COUNT(*) FROM Cadet WHERE cadetID = '" + username + "'";
        if (mysql_query(conn, checkQuery.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row && stoi(row[0]) == 0) {  // Username does not exist
                mysql_free_result(res);
                break;
            }
            cout << "Username already exists. Returning to the main menu...\n";
            mysql_free_result(res);
            return;  // Return to main menu
        }
        else {
            cout << "Database error: " << mysql_error(conn) << "\n";
            return;  // Exit the function
        }
    }

    // Input and validate Password
    while (true) {
        cout << "Enter Password (min 6 characters with at least one symbol or number): ";
        getPasswordWithConfirmation(password); // Use your existing function here
        if (password.length() >= 6 &&
            (any_of(password.begin(), password.end(), ::ispunct) ||
                any_of(password.begin(), password.end(), ::isdigit))) {
            break;
        }
        cout << "Password must be at least 6 characters long and include at least one symbol or number. Try again.\n";
    }

    // Input and validate Gender
    while (true) {
        cout << "Enter Gender (M for Male, F for Female): ";
        getline(cin, gender);
        if (gender == "M" || gender == "F") break;
        cout << "Invalid gender input. Please enter 'M' or 'F'.\n";
    }

    // Input and validate Batch
    while (true) {
        cout << "Select Batch:\n1. 2023 (Alpha)\n2. 2024 (Bravo)\n3. 2025 (Charlie)\nChoose: ";
        int batchChoice;
        cin >> batchChoice;

        if (batchChoice == 1) {
            batch = "2023";
            squad = "Alpha";
        }
        else if (batchChoice == 2) {
            batch = "2024";
            squad = "Bravo";
        }
        else if (batchChoice == 3) {
            batch = "2025";
            squad = "Charlie";
        }
        else {
            cout << "Invalid choice. Try again.\n";
            continue;
        }
        break;
    }

    // Insert data into the database
    string query = "INSERT INTO Cadet (name, numPhone, age, cadetID, password, gender, batch, squad) VALUES('" +
        name + "', '" + phone + "', '" + age + "', '" + username + "', '" + password + "', '" +
        gender + "', '" + batch + "', '" + squad + "')";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate) {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    else {
        cout << "New Cadet added successfully. Press Enter to continue...";
    }
    _getch();
}




// Search Coach
void SearchCoach() {
    system("cls");
    cout << "----- Search Coach -----" << endl;
    cout << "1. Search by Coach ID" << endl;
    cout << "2. Search by Squad" << endl;
    cout << "3. Return to Previous Menu" << endl;
    cout << "Choose an option: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1: {
        // Search by Coach ID
        system("cls");
        string coachID;
        cout << "Enter Coach ID to search: ";
        cin >> coachID;

        string query = "SELECT coachID, name, numPhone, age, gender, squad FROM Coach WHERE coachID = '" + coachID + "'";
        const char* q = query.c_str();
        int qstate = mysql_query(conn, q);

        if (!qstate) {
            MYSQL_RES* res = mysql_store_result(conn);
            if (mysql_num_rows(res) > 0) {
                MYSQL_ROW row;
                cout << "----- Coach Details -----" << endl;
                while ((row = mysql_fetch_row(res))) {
                    cout << "ID: " << row[0]
                        << " | Name: " << row[1]
                        << " | Phone: " << row[2]
                        << " | Age: " << row[3]
                        << " | Gender: " << row[4]
                        << " | Squad: " << row[5]
                        << endl;
                }
            }
            else {
                cout << "No coach found with ID: " << coachID << endl;
            }
            mysql_free_result(res);
        }
        else {
            cout << "Query Execution Problem: " << mysql_error(conn) << endl;
        }
        _getch();
        break;
    }
    case 2: {
        // Search by Squad
        system("cls");
        string squadName;
        cout << "Enter Squad to search (e.g., Alpha, Bravo, Charlie): ";
        cin >> squadName;

        string query = "SELECT coachID, name, squad FROM Coach WHERE squad = '" + squadName + "'";
        const char* q = query.c_str();
        int qstate = mysql_query(conn, q);

        if (!qstate) {
            MYSQL_RES* res = mysql_store_result(conn);
            if (mysql_num_rows(res) > 0) {
                MYSQL_ROW row;
                cout << "----- Coaches in Squad: " << squadName << " -----" << endl;
                cout << left << setw(10) << "ID"
                    << setw(20) << "Name"
                    << setw(10) << "Squad" << endl;
                cout << "--------------------------------------" << endl;
                while ((row = mysql_fetch_row(res))) {
                    cout << left << setw(10) << row[0]
                        << setw(20) << row[1]
                        << setw(10) << row[2] << endl;
                }
            }
            else {
                cout << "No coaches found in Squad: " << squadName << endl;
            }
            mysql_free_result(res);
        }
        else {
            cout << "Query Execution Problem: " << mysql_error(conn) << endl;
        }
        _getch();
        break;
    }
    case 3:
        // Return to previous menu
        return;
    default:
        cout << "Invalid choice. Press Enter to retry..." << endl;
        _getch();
        SearchCoach();
        break;
    }
}

// Search Cadet
void SearchCadet() {
    system("cls");
    int searchOption;
    cout << "----- Search Cadet -----" << endl;
    cout << "1. Search by Cadet ID" << endl;
    cout << "2. Search by Squad" << endl;
    cout << "Choose an option: ";
    cin >> searchOption;

    string query;

    if (searchOption == 1) {
        // Search by Cadet ID
        string cadetID;
        cout << "Enter Cadet ID: ";
        cin >> cadetID;

        // All roles (Admin, Coach, Cadet) can search by Cadet ID
        query = "SELECT cadetID, name, numPhone, age, squad FROM Cadet WHERE cadetID = '" + cadetID + "'";
    }
    else if (searchOption == 2) {
        // Search by Squad
        string squad;
        cout << "Enter Squad Name: ";
        cin >> squad;

        // All roles (Admin, Coach, Cadet) can search by Squad
        query = "SELECT cadetID, name, numPhone, age, squad FROM Cadet WHERE squad = '" + squad + "'";
    }
    else {
        cout << "Invalid option. Press Enter to retry..." << endl;
        _getch();
        return;
    }

    // Execute query and display results
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "----- Search Results -----" << endl;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(15) << "Phone"
            << setw(5) << "Age" << setw(10) << "Squad" << endl;
        cout << "-------------------------------------------------------------" << endl;

        bool found = false;
        while ((row = mysql_fetch_row(res))) {
            found = true;
            cout << left << setw(10) << row[0] << setw(20) << row[1] << setw(15) << row[2]
                << setw(5) << row[3] << setw(10) << row[4] << endl;
        }

        if (!found) {
            cout << "No cadet found with the given criteria." << endl;
        }

        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    cout << "Press Enter to continue...";
    _getch();
}

void AssignCoachToBatchAndSquad() {
    system("cls");
    cout << "--- Assign Coach to Batch and Squad ---" << endl;

    string coachID, batch, squad;

    cout << "Enter Coach ID: ";
    cin >> coachID;

    // Check if Coach Exists
    string checkQuery = "SELECT COUNT(*) FROM Coach WHERE coachID = '" + coachID + "'";
    const char* checkQ = checkQuery.c_str();
    int checkState = mysql_query(conn, checkQ);

    if (!checkState) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (!row || stoi(row[0]) == 0) {  // If no coach found
            cout << "Error: Coach ID '" << coachID << "' does not exist.\nPress Enter to return to the menu...";
            mysql_free_result(res);
            _getch();
            return;
        }

        mysql_free_result(res);
    }
    else {
        cout << "SQL Error (Check Query): " << mysql_error(conn) << endl;
        return;
    }

    // Select batch
    cout << "\nSelect Batch:\n1. 2023\n2. 2024\n3. 2025\nChoose: ";
    int batchChoice;
    cin >> batchChoice;

    if (batchChoice == 1) {
        batch = "2023";
        squad = "Alpha";
    }
    else if (batchChoice == 2) {
        batch = "2024";
        squad = "Bravo";
    }
    else if (batchChoice == 3) {
        batch = "2025";
        squad = "Charlie";
    }
    else {
        cout << "Invalid choice. Returning to main menu.\n";
        return;
    }

    // Check squad limitation
    string countQuery = "SELECT COUNT(*) FROM Coach WHERE batch = '" + batch + "' AND squad = '" + squad + "'";
    const char* cq = countQuery.c_str();
    int countState = mysql_query(conn, cq);
    if (!countState) {
        MYSQL_RES* resCount = mysql_store_result(conn);
        MYSQL_ROW rowCount = mysql_fetch_row(resCount);
        int count = atoi(rowCount[0]);
        mysql_free_result(resCount);

        if (count >= 2) {
            cout << "This squad already has 2 coaches. Please select a different batch/squad.\n";
            _getch();
            return;
        }
    }
    else {
        cout << "Error checking squad count: " << mysql_error(conn) << endl;
        return;
    }

    // Update coach assignment
    string updateQuery = "UPDATE Coach SET batch = '" + batch + "', squad = '" + squad + "' WHERE coachID = '" + coachID + "'";
    if (mysql_query(conn, updateQuery.c_str()) == 0) {
        cout << "Coach successfully assigned to Batch: " << batch << ", Squad: " << squad << ".\nPress Enter to continue...";
        _getch();
    }
    else {
        cout << "SQL Error (Update Query): " << mysql_error(conn) << endl;
    }
}



// Admin Menu
void AdminMenu() {
    while (true) {
        system("cls");
        // Display the welcome message using the currentUserID
        cout << "Welcome, Admin " << currentUserID << "!" << endl;

        cout << "----- Admin Menu -----" << endl;
        cout << "1. Manage Cadet" << endl;
        cout << "2. Manage Coach" << endl;
        cout << "3. Manage Admin" << endl;
        cout << "4. Manage Official Fitness Records" << endl;
        cout << "5. Manage Unofficial Fitness Records" << endl;
        cout << "6. Fitness Reports" << endl;
        cout << "7. Logout" << endl;
        cout << "Choose menu: ";
        int menu;
        cin >> menu;

        switch (menu) {
        case 1:
            ManageCadet();
            break;
        case 2:
            ManageCoach();
            break;
        case 3:
            ManageAdmin();
            break;
        case 4:
            ManageOfficialFitness();
            break;
        case 5:
            ManageUnofficialFitness();
            break;
        case 6:
            ReportMenu();
            break;
        case 7:
            return; // Logout
        default:
            cout << "Invalid option. Press Enter to retry...";
            cin.get();
            cin.ignore();
            break;
        }
    }
}

// Coach Menu
void CoachMenu() {
    while (true) {
        system("cls");
        cout << "Welcome, Coach " << currentUserID << "!" << endl;

        cout << "----- Coach Menu -----" << endl;
        cout << "1. View My Details" << endl;
        cout << "2. Edit My Details" << endl;
        cout << "3. Search Details (by Coach ID or Squad)" << endl;
        cout << "4. Manage Cadet" << endl;
        cout << "5. Manage Official Fitness Records" << endl;
        cout << "6. View Unofficial Fitness Records (Squad-Based)" << endl;
        cout << "7. Fitness Reports (Squad-Based)" << endl;
        cout << "8. Logout" << endl;
        cout << "Choose menu: ";
        int menu;
        cin >> menu;

        switch (menu) {
        case 1:
            ViewCoachDetails(); // View current coach's details
            break;
        case 2:
            EditCoach(); // Edit current coach's details
            break;
        case 3:
            SearchCoach(); // Search based on Coach ID or Squad
            break;
        case 4:
            ManageCadet(); // Manage cadets in the coach's squad
            break;
        case 5:
            ManageOfficialFitness(); // Manage official fitness for squad
            break;
        case 6:
            ViewUnofficialFitness(); // View unofficial fitness records for squad
            break;
        case 7:
            ReportMenu(); // View fitness reports for squad
            break;
        case 8:
            return; // Logout
        default:
            cout << "Invalid option. Press Enter to retry...";
            cin.get();
            cin.ignore();
            break;
        }
    }
}


void CadetMenu() {
    while (true) {
        system("cls");
        cout << "Welcome, Cadet " << currentUserID << "!" << endl;

        cout << "----- Cadet Menu -----" << endl;
        cout << "1. View My Details" << endl;
        cout << "2. View Coach Details" << endl;
        cout << "3. Search Cadet" << endl;
        cout << "4. Search Coach" << endl;
        cout << "5. View My Official Fitness Records" << endl;
        cout << "6. Search Official Fitness Records" << endl;
        cout << "7. Manage My Unofficial Fitness Records" << endl;
        cout << "8. View My Fitness Report" << endl;
        cout << "9. Logout" << endl;
        cout << "Choose menu: ";
        int menu;
        cin >> menu;

        switch (menu) {
        case 1:
            ViewCadetDetails(); // View the cadet's own details
            break;
        case 2:
            ViewCoachDetails(); // View coach details based on squad
            break;
        case 3:
            SearchCadet(); // Search for cadets by ID or squad (restricted to own squad)
            break;
        case 4:
            SearchCoach(); // Search for coaches by ID or squad
            break;
        case 5:
            ViewOfficialFitness(); // View cadet's own official fitness records
            break;
        case 6:
            SearchOfficialFitness(); // Search official fitness records (restricted)
            break;
        case 7:
            ManageUnofficialFitness(); // Manage cadet's own unofficial fitness records
            break;
        case 8:
            ReportMenu(); // View fitness report for the cadet
            break;
        case 9:
            return; // Logout
        default:
            cout << "Invalid option. Press Enter to retry...";
            cin.get();
            cin.ignore();
            break;
        }
    }
}

void ManageCadet() {
    int menu;
    while (true) {
        system("cls");
        cout << "----- Manage Cadet -----" << endl;
        cout << "1. View Cadet" << endl;
        cout << "2. Add Cadet" << endl;
        cout << "3. Edit Cadet" << endl;
        cout << "4. Remove Cadet" << endl;
        cout << "5. Search Cadet" << endl;
        cout << "6. Manage Fitness" << endl;  // Add Fitness Management option
        cout << "7. Return to Menu" << endl;
        cout << "Choose option: ";
        cin >> menu;

        switch (menu) {
        case 1:
            ViewCadetDetails();
            break;
        case 2:
            AddCadet();
            break;
        case 3:
            EditCadet();
            break;
        case 4:
            RemoveCadet();
            break;
        case 5:
            SearchCadet();
            break;
        case 6:
            FitnessMenu();  // Call fitness management
            break;
        case 7:
            return;  // Return to the previous menu
        default:
            cout << "Invalid option. Press Enter to retry...";
            _getch();
            break;
        }
    }
}

void ManageCoach() {
    int menu;
    while (true) {
        system("cls");
        cout << "----- Manage Coach -----" << endl;
        cout << "1. View Coach" << endl;
        cout << "2. Add Coach" << endl;
        cout << "3. Edit Coach" << endl;
        cout << "4. Remove Coach" << endl;
        cout << "5. Search Coach" << endl;
        cout << "6. Manage Fitness" << endl;  // Add Fitness Management option
        cout << "7. Return to Menu" << endl;
        cout << "Choose option: ";
        cin >> menu;

        switch (menu) {
        case 1:
            ViewCoachDetails();
            break;
        case 2:
            AddCoach();
            break;
        case 3:
            EditCoach();
            break;
        case 4:
            RemoveCoach();
            break;
        case 5:
            SearchCoach();
            break;
        case 6:
            FitnessMenu();  // Call fitness management
            break;
        case 7:
            return;  // Return to the previous menu
        default:
            cout << "Invalid option. Press Enter to retry...";
            _getch();
            break;
        }
    }
}

void ManageAdmin() {
    int menu;
    while (true) {
        system("cls");
        cout << "----- Manage Admin -----" << endl;
        cout << "1. View Admin" << endl;
        cout << "2. Add Admin" << endl;
        cout << "3. Edit Admin" << endl;
        cout << "4. Remove Admin" << endl;
        cout << "5. Manage Fitness" << endl;
        cout << "6. Assign Coach to Batch and Squad" << endl;                    // Add Fitness Management option
        cout << "7. Return to Menu" << endl;
        cout << "Choose option: ";
        cin >> menu;

        switch (menu) {
        case 1:
            ViewAdminDetails();
            break;
        case 2:
            AddAdmin();
            break;
        case 3:
            EditAdmin();
            break;
        case 4:
            RemoveAdmin();
            break;
        case 5:
            FitnessMenu();  // Call fitness management
            break;
        case 6:
             AssignCoachToBatchAndSquad();
            break;
        case 7:
            return;  // Return to the previous menu
        default:
            cout << "Invalid option. Press Enter to retry...";
            _getch();
            break;
        }
    }
}


void FitnessMenu() {
    int menu;
    while (true) {
        system("cls");
        cout << "----- Fitness Menu -----" << endl;
        cout << "1. Manage Official Fitness" << endl;
        cout << "2. Manage Unofficial Fitness" << endl;
        cout << "3. Fitness Report" << endl;
        cout << "4. Return to Previous Menu" << endl;
        cout << "Choose option: ";
        cin >> menu;

        switch (menu) {
        case 1:
            ManageOfficialFitness();  // Call official fitness management
            break;
        case 2:
            ManageUnofficialFitness();  // Call unofficial fitness management
            break;
        case 3:
            ReportMenu();  // Placeholder for fitness report functionality
            break;
        case 4:
            return;  // Return to the previous menu (e.g., AdminMenu, CoachMenu, CadetMenu)
        default:
            cout << "Invalid option. Press Enter to retry...";
            _getch();
            break;
        }
    }
}

// Manage Official Fitness Records
void ManageOfficialFitness() {
    int menu;
    while (true) {
        system("cls");
        cout << "----- Manage Official Fitness -----" << endl;
        cout << "1. View Official Fitness" << endl;
        if (currentUserRole == "Admin" || currentUserRole == "Coach") {
            cout << "2. Add Official Fitness" << endl;
            cout << "3. Edit Official Fitness" << endl;
            cout << "4. Remove Official Fitness" << endl;
        }
        cout << "5. Search Official Fitness" << endl;
        cout << "6. Display Overall Fitness" << endl;
        cout << "7. Return to Menu" << endl;
        cout << "Choose option: ";
        cin >> menu;

        switch (menu) {
        case 1:
            ViewOfficialFitness();  // Admin and Coach view fitness records
            break;
        case 2:
            if (currentUserRole == "Admin" || currentUserRole == "Coach") {
                AddOfficialFitness();  // Admin and Cadet can add fitness records
            }
            break;
        case 3:
            if (currentUserRole == "Admin" || currentUserRole == "Coach") {
                EditOfficialFitness();  // Admin and Cadet can edit fitness records
            }
            break;
        case 4:
            if (currentUserRole == "Admin" || currentUserRole == "Coach") {
                RemoveOfficialFitness();  // Admin and Cadet can remove fitness records
            }
            break;
        case 5:
            SearchOfficialFitness();  // Admin, Coach, and Cadet can search fitness records
            break;
        case 6:
            DisplayOverallOfficialFitness();  // Display overall fitness
            break;
        case 7:
            return;  // Return to fitness menu
        default:
            cout << "Invalid option. Press Enter to retry...";
            _getch();
            break;
        }
    }
}

// Manage Unofficial Fitness Records
void ManageUnofficialFitness() {
    int menu;
    while (true) {
        system("cls");
        cout << "----- Manage Unofficial Fitness -----" << endl;
        cout << "1. View Unofficial Fitness" << endl;
        if (currentUserRole == "Admin" || currentUserRole == "Cadet") {
            cout << "2. Add Unofficial Fitness" << endl;
            cout << "3. Edit Unofficial Fitness" << endl;
            cout << "4. Remove Unofficial Fitness" << endl;
        }
        cout << "5. Search Unofficial Fitness" << endl;
        cout << "6. Display Unofficial Fitness" << endl;
        cout << "7. Return to Menu" << endl;
        cout << "Choose option: ";
        cin >> menu;

        switch (menu) {
        case 1:
            ViewUnofficialFitness();  // Admin, Coach, and Cadet view fitness records
            break;
        case 2:
            if (currentUserRole == "Admin" || currentUserRole == "Cadet") {
                AddUnofficialFitness();  // Admin and Cadet can add unofficial fitness records
            }
            break;
        case 3:
            if (currentUserRole == "Admin" || currentUserRole == "Cadet") {
                EditUnofficialFitness();  // Admin and Cadet can edit unofficial fitness records
            }
            break;
        case 4:
            if (currentUserRole == "Admin" || currentUserRole == "Cadet") {
                RemoveUnofficialFitness();  // Admin and Cadet can remove unofficial fitness records
            }
            break;
        case 5:
            SearchUnofficialFitness();  // Admin, Coach, and Cadet can search unofficial fitness records
            break;
        case 6:
            DisplayOverallUnofficialFitness();
            break;
        case 7:
            return;  // Return to fitness menu
        default:
            cout << "Invalid option. Press Enter to retry...";
            _getch();
            break;
        }
    }
}







// View Official Fitness details
void ViewOfficialFitness() {
    system("cls");
    string query;

    if (currentUserRole == "Admin") {
        int choice;
        cout << "1. View All Fitness Details\n2. View Fitness Details by Squad\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            query = "SELECT * FROM fitness_details";
        }
        else if (choice == 2) {
            string squad;
            cout << "Enter Squad: ";
            cin >> squad;
            query = "SELECT fd.* FROM fitness_details fd JOIN Cadet c ON fd.cadetID = c.cadetID WHERE c.squad = '" + squad + "'";
        }
        else {
            cout << "Invalid option. Returning to menu..." << endl;
            _getch();
            return;
        }
    }
    else if (currentUserRole == "Coach") {
        query = "SELECT fd.* FROM fitness_details fd JOIN Cadet c ON fd.cadetID = c.cadetID WHERE c.squad = '" + currentSquad + "'";
    }
    else if (currentUserRole == "Cadet") {
        query = "SELECT * FROM fitness_details WHERE cadetID = '" + currentUserID + "'";
    }
    else {
        cout << "Access Denied." << endl;
        _getch();
        return;
    }

    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        // Table header
        cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "| Record Date  | CadetID  | Pushup | Pullup | Situp | Height | Weight |   BMI   | Lunges | Squat | Beep Test | Distance |" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;

        // Table rows
        while ((row = mysql_fetch_row(res))) {
            cout <<"| " << left << setw(13) << row[0]  // Record Date
                << "| " << setw(9) << row[10]        // CadetID
                << "| " << setw(7) << row[1]         // Pushup
                << "| " << setw(7) << row[2]         // Pullup
                << "| " << setw(6) << row[3]         // Situp
                << "| " << setw(7) << row[4]         // Height
                << "| " << setw(7) << row[5]         // Weight
                << "| " << setw(8) << row[6]         // BMI
                << "| " << setw(7) << row[7]         // Lunges
                << "| " << setw(6) << row[8]         // Squat
                << "| " << setw(10) << row[9]         // Beep Test
                << "| " << setw(8) << row[11] << " |" << endl; // Distance
        }

        // Table footer
        cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;

        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    cout << "Press Enter to continue...";
    _getch();
}


// View Unofficial Fitness details
void ViewUnofficialFitness() {
    system("cls");
    string query;

    if (currentUserRole == "Admin") {
        int choice;
        cout << "1. View All Unofficial Fitness Details\n2. View by Squad\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            query = "SELECT * FROM unfitness";
        }
        else if (choice == 2) {
            string squad;
            cout << "Enter Squad: ";
            cin.ignore();
            getline(cin, squad);
            query = "SELECT uf.* FROM unfitness uf JOIN Cadet c ON uf.cadetID = c.cadetID WHERE c.squad = '" + squad + "'";
        }
        else {
            cout << "Invalid option. Returning to the menu..." << endl;
            _getch();
            return;
        }
    }
    else if (currentUserRole == "Coach") {
        query = "SELECT uf.* FROM unfitness uf JOIN Cadet c ON uf.cadetID = c.cadetID WHERE c.squad = '" + currentSquad + "'";
    }
    else if (currentUserRole == "Cadet") {
        query = "SELECT * FROM unfitness WHERE cadetID = '" + currentUserID + "'";
    }
    else {
        cout << "Access Denied. You don't have permission to view this information." << endl;
        _getch();
        return;
    }

    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        // Table header
        cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "| Record Date  | CadetID  | Pushup | Pullup | Situp | Height | Weight |   BMI   | Lunges | Squat | Beep Test | Distance |" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;

        // Table rows
        while ((row = mysql_fetch_row(res))) {
            cout << "| " << left << setw(13) << row[0]  // Record Date
                << "| " << setw(9) << row[10]        // CadetID
                << "| " << setw(7) << row[1]         // Pushup
                << "| " << setw(7) << row[2]         // Pullup
                << "| " << setw(6) << row[3]         // Situp
                << "| " << setw(7) << row[4]         // Height
                << "| " << setw(7) << row[5]         // Weight
                << "| " << setw(8) << row[6]         // BMI
                << "| " << setw(7) << row[7]         // Lunges
                << "| " << setw(6) << row[8]         // Squat
                << "| " << setw(10) << row[9]        // Beep Test
                << "| " << setw(8) << row[11] << " |" << endl; // Distance
        }

        // Table footer
        cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;

        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    cout << "Press Enter to continue...";
    _getch();
}




// Add Official Fitness Record
void AddOfficialFitness() {
    system("cls");
    string cadetID;
    int pushup, pullup, situp, lunges, squat;
    double height, weight, beepTest, distance, bmi;

    // Input Cadet ID
    cout << "Enter Cadet ID: ";
    cin >> cadetID;

    // Verify CadetID (function to check if the cadet exists)
    if (!VerifyCadetID(cadetID)) {
        cout << "Invalid Cadet ID. Operation aborted." << endl;
        _getch();
        return;
    }

    // Input Fitness Data with Validation
    cout << "Enter Pushup: ";
    while (true) {
        cin >> pushup;
        if (cin.fail() || pushup < 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Pushup must be a non-negative number. Try again: ";
        }
        else {
            break;
        }
    }

    cout << "Enter Pullup: ";
    while (true) {
        cin >> pullup;
        if (cin.fail() || pullup < 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Pullup must be a non-negative number. Try again: ";
        }
        else {
            break;
        }
    }

    cout << "Enter Situp: ";
    while (true) {
        cin >> situp;
        if (cin.fail() || situp < 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Situp must be a non-negative number. Try again: ";
        }
        else {
            break;
        }
    }

    cout << "Enter Lunges: ";
    while (true) {
        cin >> lunges;
        if (cin.fail() || lunges < 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Lunges must be a non-negative number. Try again: ";
        }
        else {
            break;
        }
    }

    cout << "Enter Squat: ";
    while (true) {
        cin >> squat;
        if (cin.fail() || squat < 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Squat must be a non-negative number. Try again: ";
        }
        else {
            break;
        }
    }

    cout << "Enter Height (cm): ";
    while (true) {
        cin >> height;
        if (cin.fail() || height <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Height must be a positive number. Try again: ";
        }
        else {
            break;
        }
    }

    cout << "Enter Weight (kg): ";
    while (true) {
        cin >> weight;
        if (cin.fail() || weight <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Weight must be a positive number. Try again: ";
        }
        else {
            break;
        }
    }

    cout << "Enter Beep Test (0-15): ";
    while (true) {
        cin >> beepTest;
        if (cin.fail() || beepTest < 0 || beepTest > 15) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Beep Test must be between 0 and 15. Try again: ";
        }
        else {
            break;
        }
    }

    cout << "Enter 2.4KM Time (minutes): ";
    while (true) {
        cin >> distance;
        if (cin.fail() || distance <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. 2.4KM Time must be a positive number. Try again: ";
        }
        else {
            break;
        }
    }

    // Convert height to meters and calculate BMI
    height /= 100.0;
    bmi = weight / (height * height);

    // Add the fitness record to the database
    string query = "INSERT INTO fitness_details (CadetID, pushup, pullup, situp, lunges, squat, height, weight, BMI, beepTest, distance, Date) "
        "VALUES ('" + cadetID + "', " + to_string(pushup) + ", " + to_string(pullup) + ", " + to_string(situp) + ", "
        + to_string(lunges) + ", " + to_string(squat) + ", " + to_string(height) + ", " + to_string(weight) + ", "
        + to_string(bmi) + ", " + to_string(beepTest) + ", " + to_string(distance) + ", CURDATE())";

    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate) {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    else {
        cout << "Fitness record added successfully. Press Enter to continue...";
    }
    _getch();
}

// Add Unofficial Fitness Recordu
void AddUnofficialFitness() {
    system("cls");
    string cadetID;
    int pushup, pullup, situp, lunges, squat;
    double height, weight, beepTest, distance, bmi;

    // Input Cadet ID
    cout << "Enter Cadet ID: ";
    cin >> cadetID;

    // Validation for Cadet Role
    if (currentUserRole == "Cadet" && cadetID != currentUserID) {
        cout << "Access Denied. You can only add fitness details for yourself." << endl;
        _getch();
        return;
    }

    // Verify Cadet ID
    if (!VerifyCadetID(cadetID)) {
        cout << "Invalid Cadet ID. Operation aborted." << endl;
        _getch();
        return;
    }

    // Input Fitness Data with Validation
    auto validateInt = [](const string& prompt, int& value) {
        cout << prompt;
        while (true) {
            cin >> value;
            if (cin.fail() || value < 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Must be a non-negative number. Try again: ";
            }
            else {
                break;
            }
        }
        };

    auto validateDouble = [](const string& prompt, double& value, double min, double max = DBL_MAX) {
        cout << prompt;
        while (true) {
            cin >> value;
            if (cin.fail() || value < min || value > max) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Value must be between " << min << " and " << max << ". Try again: ";
            }
            else {
                break;
            }
        }
        };

    validateInt("Enter Pushup: ", pushup);
    validateInt("Enter Pullup: ", pullup);
    validateInt("Enter Situp: ", situp);
    validateInt("Enter Lunges: ", lunges);
    validateInt("Enter Squat: ", squat);
    validateDouble("Enter Height (cm): ", height, 1.0);
    validateDouble("Enter Weight (kg): ", weight, 1.0);
    validateDouble("Enter Beep Test (0-15): ", beepTest, 0.0, 15.0);
    validateDouble("Enter 2.4KM Time (minutes): ", distance, 0.1);

    // Convert height to meters and calculate BMI
    height /= 100.0;
    bmi = weight / (height * height);

    // Insert into the database
    string query = "INSERT INTO unfitness (CadetID, pushup, pullup, situp, height, weight, BMI, lunges, squat, beepTest, distance, Date) VALUES ('" +
        cadetID + "', " + to_string(pushup) + ", " + to_string(pullup) + ", " + to_string(situp) + ", " +
        to_string(height) + ", " + to_string(weight) + ", " + to_string(bmi) + ", " + to_string(lunges) +
        ", " + to_string(squat) + ", " + to_string(beepTest) + ", " + to_string(distance) + ", CURDATE())";

    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate) {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    else {
        cout << "Unofficial Fitness record added successfully. Press Enter to continue...";
    }

    _getch();
}




void UpdateOfficialFitness() {
    system("cls");
    string cadetID;
    int pushup, pullup, situp, lunges, squat;
    double height, weight, beepTest, distance, bmi;

    // Input Cadet ID
    cout << "Enter Cadet ID to update: ";
    cin >> cadetID;

    // Verify Cadet ID based on the user's role
    if (currentUserRole == "Admin") {
        // Admin has full access to update any cadet's record
        if (!VerifyCadetID(cadetID)) {
            cout << "Invalid Cadet ID. Operation aborted." << endl;
            _getch();
            return;
        }
    }
    else if (currentUserRole == "Coach") {
        // Coach can only update records within their squad
        if (!IsCadetInSquad(cadetID, currentSquad)) {
            cout << "You do not have permission to update this cadet's record." << endl;
            _getch();
            return;
        }
    }
    else {
        cout << "Access Denied. You do not have permission to update official fitness details." << endl;
        _getch();
        return;
    }

    // Function to validate integer inputs
    auto validateInt = [](const string& prompt, int& value) {
        cout << prompt;
        while (true) {
            cin >> value;
            if (cin.fail() || value < 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Must be a non-negative number. Try again: ";
            }
            else {
                break;
            }
        }
        };

    // Function to validate double inputs
    auto validateDouble = [](const string& prompt, double& value, double min, double max = DBL_MAX) {
        cout << prompt;
        while (true) {
            cin >> value;
            if (cin.fail() || value < min || value > max) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Value must be between " << min << " and " << max << ". Try again: ";
            }
            else {
                break;
            }
        }
        };

    // Input and validation
    validateInt("Enter new Pushup: ", pushup);
    validateInt("Enter new Pullup: ", pullup);
    validateInt("Enter new Situp: ", situp);
    validateInt("Enter new Lunges: ", lunges);
    validateInt("Enter new Squat: ", squat);
    validateDouble("Enter new Height (cm): ", height, 1.0);
    validateDouble("Enter new Weight (kg): ", weight, 1.0);
    validateDouble("Enter new Beep Test (0-15): ", beepTest, 0.0, 15.0);
    validateDouble("Enter new 2.4KM Time (minutes): ", distance, 0.1);

    // Calculate BMI
    height /= 100.0;  // Convert to meters
    bmi = weight / (height * height);

    // Update the fitness record in the database
    string query = "UPDATE fitness_details SET "
        "pushup=" + to_string(pushup) + ", pullup=" + to_string(pullup) + ", situp=" + to_string(situp) + ", "
        "lunges=" + to_string(lunges) + ", squat=" + to_string(squat) + ", height=" + to_string(height) + ", "
        "weight=" + to_string(weight) + ", BMI=" + to_string(bmi) + ", beepTest=" + to_string(beepTest) + ", "
        "distance=" + to_string(distance) + ", Date=CURDATE() WHERE CadetID='" + cadetID + "'";

    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate) {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    else {
        cout << "Fitness record updated successfully. Press Enter to continue...";
    }
    _getch();
}


// Edit Official Fitness Record
void UpdateUnofficialFitness() {
    system("cls");

    if (currentUserRole != "Cadet") {
        cout << "Access Denied. Only cadets can update their unofficial fitness records." << endl;
        _getch();
        return;
    }

    string cadetID = currentUserID;
    int pushup, pullup, situp, lunges, squat;
    double height, weight, beepTest, distance, bmi;

    // Verify Cadet ID
    if (!VerifyCadetID(cadetID)) {
        cout << "Invalid Cadet ID. Operation aborted." << endl;
        _getch();
        return;
    }

    // Input Fitness Data with Validation
    auto validateInt = [](const string& prompt, int& value) {
        cout << prompt;
        while (true) {
            cin >> value;
            if (cin.fail() || value < 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Must be a non-negative number. Try again: ";
            }
            else {
                break;
            }
        }
        };

    auto validateDouble = [](const string& prompt, double& value, double min, double max = DBL_MAX) {
        cout << prompt;
        while (true) {
            cin >> value;
            if (cin.fail() || value < min || value > max) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input. Value must be between " << min << " and " << max << ". Try again: ";
            }
            else {
                break;
            }
        }
        };

    validateInt("Enter new Pushup: ", pushup);
    validateInt("Enter new Pullup: ", pullup);
    validateInt("Enter new Situp: ", situp);
    validateInt("Enter new Lunges: ", lunges);
    validateInt("Enter new Squat: ", squat);
    validateDouble("Enter new Height (cm): ", height, 1.0);
    validateDouble("Enter new Weight (kg): ", weight, 1.0);
    validateDouble("Enter new Beep Test (0-15): ", beepTest, 0.0, 15.0);
    validateDouble("Enter new 2.4KM Time (minutes): ", distance, 0.1);

    // Convert height to meters and calculate BMI
    height /= 100.0;
    bmi = weight / (height * height);

    // Update the fitness record in the database
    string query = "UPDATE unfitness SET "
        "pushup=" + to_string(pushup) + ", pullup=" + to_string(pullup) + ", situp=" + to_string(situp) + ", "
        "height=" + to_string(height) + ", weight=" + to_string(weight) + ", BMI=" + to_string(bmi) + ", "
        "lunges=" + to_string(lunges) + ", squat=" + to_string(squat) + ", beepTest=" + to_string(beepTest) + ", "
        "distance=" + to_string(distance) + ", Date=CURDATE() "
        "WHERE cadetID='" + cadetID + "'";

    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate) {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    else {
        cout << "Unofficial Fitness record updated successfully. Press Enter to continue...";
    }
    _getch();
}



void EditOfficialFitness() {
    system("cls");
    string cadetID;
    int recordID;

    // Input Cadet ID
    cout << "Enter Cadet ID: ";
    cin >> cadetID;

    // Role-Based Permission Check
    if (currentUserRole == "Coach") {
        if (!IsCadetInSquad(cadetID, currentSquad)) {
            cout << "Access Denied. You can only edit fitness details for cadets in your squad." << endl;
            _getch();
            return;
        }
    }
    else if (currentUserRole != "Admin") {
        cout << "Access Denied. Only Admin or Coach can edit official fitness details." << endl;
        _getch();
        return;
    }

    // Verify Cadet ID
    if (!VerifyCadetID(cadetID)) {
        cout << "Invalid Cadet ID. Please enter a valid ID." << endl;
        _getch();
        return;
    }

    // Input Record ID
    cout << "Enter Record ID to edit: ";
    cin >> recordID;

    // Fetch existing record for validation
    string query = "SELECT * FROM fitness_details WHERE RecordID = " + to_string(recordID) + " AND CadetID = '" + cadetID + "'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate || mysql_num_rows(mysql_store_result(conn)) == 0) {
        cout << "Record not found or you don't have permission to edit this record." << endl;
        _getch();
        return;
    }

    // Input new fitness data
    int pushup, pullup, situp, lunges, squat;
    double height, weight, beepTest, distance, bmi;

    cout << "Enter new Pushup: ";
    cin >> pushup;
    if (pushup < 0) { cout << "Pushup value cannot be negative. Operation aborted." << endl; _getch(); return; }

    cout << "Enter new Pullup: ";
    cin >> pullup;
    if (pullup < 0) { cout << "Pullup value cannot be negative. Operation aborted." << endl; _getch(); return; }

    cout << "Enter new Situp: ";
    cin >> situp;
    if (situp < 0) { cout << "Situp value cannot be negative. Operation aborted." << endl; _getch(); return; }

    cout << "Enter new Lunges: ";
    cin >> lunges;
    if (lunges < 0) { cout << "Lunges value cannot be negative. Operation aborted." << endl; _getch(); return; }

    cout << "Enter new Squat: ";
    cin >> squat;
    if (squat < 0) { cout << "Squat value cannot be negative. Operation aborted." << endl; _getch(); return; }

    cout << "Enter new Height (cm): ";
    cin >> height;
    if (height <= 0) { cout << "Height must be a positive value. Operation aborted." << endl; _getch(); return; }

    cout << "Enter new Weight (kg): ";
    cin >> weight;
    if (weight <= 0) { cout << "Weight must be a positive value. Operation aborted." << endl; _getch(); return; }

    cout << "Enter new Beep Test (0-15): ";
    cin >> beepTest;
    if (beepTest < 0 || beepTest > 15) { cout << "Beep Test must be between 0 and 15. Operation aborted." << endl; _getch(); return; }

    cout << "Enter new 2.4km Time (minutes): ";
    cin >> distance;
    if (distance <= 0) { cout << "2.4km Time must be a positive value. Operation aborted." << endl; _getch(); return; }

    // Calculate BMI
    height /= 100.0; // Convert height to meters
    bmi = weight / (height * height);

    // Update the fitness record in the database
    query = "UPDATE fitness_details SET "
        "pushup=" + to_string(pushup) + ", pullup=" + to_string(pullup) + ", situp=" + to_string(situp) + ", "
        "lunges=" + to_string(lunges) + ", squat=" + to_string(squat) + ", height=" + to_string(height) + ", "
        "weight=" + to_string(weight) + ", BMI=" + to_string(bmi) + ", beepTest=" + to_string(beepTest) + ", "
        "distance=" + to_string(distance) + ", Date=CURDATE() WHERE RecordID=" + to_string(recordID);

    q = query.c_str();
    qstate = mysql_query(conn, q);

    if (qstate) {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    else {
        cout << "Official Fitness record updated successfully. Press Enter to continue...";
    }
    _getch();
}

// Edit Unofficial Fitness Record
void EditUnofficialFitness() {
    system("cls");
    string cadetID;
    int recordID;

    // Input Cadet ID
    cout << "Enter Cadet ID: ";
    cin >> cadetID;

    if (currentUserRole == "Cadet" && cadetID != currentUserID) {
        cout << "Access Denied. You can only edit your own fitness details." << endl;
        _getch();
        return;
    }

    if (!VerifyCadetID(cadetID)) {
        cout << "Invalid Cadet ID. Operation aborted." << endl;
        _getch();
        return;
    }

    // Input Record ID
    cout << "Enter Record ID to edit: ";
    cin >> recordID;

    string query = "SELECT * FROM unfitness WHERE RecordID = " + to_string(recordID) + " AND CadetID = '" + cadetID + "'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate || mysql_num_rows(mysql_store_result(conn)) == 0) {
        cout << "Record not found or you don't have permission to edit this record." << endl;
        _getch();
        return;
    }

    // Update Unofficial Fitness Record
    UpdateUnofficialFitness();
}








// Remove Official Fitness Record
void RemoveOfficialFitness() {
    system("cls");
    string cadetID;

    cout << "Enter Cadet ID to remove official fitness records: ";
    cin >> cadetID;

    // Verify the role and cadet access
    if (currentUserRole == "Admin") {
        // Admin has full access to remove any fitness record
        if (!VerifyCadetID(cadetID)) {
            cout << "Invalid Cadet ID. Operation aborted." << endl;
            _getch();
            return;
        }
    }
    else if (currentUserRole == "Coach") {
        // Coach can only remove records for cadets in their squad
        if (!IsCadetInSquad(cadetID, currentSquad)) {
            cout << "You do not have permission to remove this cadet's fitness records." << endl;
            _getch();
            return;
        }
    }
    else {
        cout << "Access Denied. You do not have permission to remove official fitness records." << endl;
        _getch();
        return;
    }

    // Confirm the removal
    char confirmation;
    cout << "Are you sure you want to remove the official fitness records for Cadet ID " << cadetID << "? (Y/N): ";
    cin >> confirmation;

    if (toupper(confirmation) != 'Y') {
        cout << "Operation canceled. Press Enter to continue..." << endl;
        _getch();
        return;
    }

    // Remove the fitness records from the database
    string query = "DELETE FROM fitness_details WHERE CadetID='" + cadetID + "'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate) {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    else {
        cout << "Official fitness records for Cadet ID " << cadetID << " have been successfully removed. Press Enter to continue...";
    }

    _getch();
}



// Remove Unofficial Fitness Record
void RemoveUnofficialFitness() {
    system("cls");
    string cadetID;
    int recordID;

    // Input Cadet ID
    cout << "Enter Cadet ID: ";
    cin >> cadetID;

    // Validation for Cadet Role
    if (currentUserRole == "Cadet" && cadetID != currentUserID) {
        cout << "Access Denied. You can only remove your own fitness details." << endl;
        _getch();
        return;
    }

    // Verify Cadet ID
    if (!VerifyCadetID(cadetID)) {
        cout << "Invalid Cadet ID. Operation aborted." << endl;
        _getch();
        return;
    }

    // Input Record ID
    cout << "Enter Record ID to remove: ";
    cin >> recordID;

    // Delete the record
    string query = "DELETE FROM unfitness WHERE RecordID = " + to_string(recordID) + " AND CadetID = '" + cadetID + "'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (qstate) {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    else {
        cout << "Record successfully removed. Press Enter to continue...";
    }

    _getch();
}

// Search Official Fitness Record
void SearchOfficialFitness() {
    system("cls");
    string query;
    int option;

    cout << "----- Search Official Fitness -----" << endl;
    cout << "1. Search by Cadet ID" << endl;
    cout << "2. Search by Squad" << endl;
    cout << "Choose an option: ";
    cin >> option;

    if (option == 1) {
        string cadetID;
        cout << "Enter Cadet ID: ";
        cin >> cadetID;

        query = "SELECT CadetID, pushup, pullup, situp, lunges, squat, BMI, beepTest, distance FROM fitness_details WHERE CadetID='" + cadetID + "'";
    }
    else if (option == 2) {
        string squad;
        cout << "Enter Squad Name: ";
        cin >> squad;

        query = "SELECT CadetID, pushup, pullup, situp, lunges, squat, BMI, beepTest, distance FROM fitness_details WHERE squad='" + squad + "'";
    }
    else {
        cout << "Invalid option. Returning to menu...";
        _getch();
        return;
    }

    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "----- Fitness Records -----" << endl;
        while ((row = mysql_fetch_row(res))) {
            cout << "CadetID: " << row[0] << " | Pushup: " << row[1] << " | Pullup: " << row[2] << " | Situp: " << row[3]
                << " | Lunges: " << row[4] << " | Squat: " << row[5] << " | BMI: " << row[6] << " | Beep Test: " << row[7]
                << " | 2.4km: " << row[8] << endl;
        }
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    _getch();
}

// Search Unofficial Fitness Record
void SearchUnofficialFitness() {
    system("cls");
    string attribute, value;

    cout << "Enter the attribute to search by (e.g., Date, pushup, BMI): ";
    cin >> attribute;
    cout << "Enter the value to search for: ";
    cin.ignore();
    getline(cin, value);

    string query = "SELECT * FROM unfitness WHERE " + attribute + " LIKE '%" + value + "%'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        cout << "----- Unofficial Fitness Search Results -----" << endl;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << " | Date: " << row[1] << " | Pushup: " << row[2] << " | Pullup: " << row[3]
                << " | Situp: " << row[4] << " | Height: " << row[5] << " | Weight: " << row[6] << " | BMI: " << row[7] << endl;
        }
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    _getch();
}


void DisplayOverallOfficialFitness() {
    system("cls");
    string query;

    // Admin and Coach now have the same access
    if (currentUserRole == "Admin" || currentUserRole == "Coach") {
        query = "SELECT CadetID, pushup, pullup, situp, lunges, squat, BMI, beepTest, distance FROM fitness_details";
    }
    else if (currentUserRole == "Cadet") {
        query = "SELECT CadetID, pushup, pullup, situp, lunges, squat, BMI, beepTest, distance FROM fitness_details WHERE CadetID='" + currentUserID + "'";
    }

    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        // Header for the table
        cout << "----- Overall Fitness Details --------------------------------------+" << endl;
        cout << "| " << setw(10) << left << "CadetID"
            << "| " << setw(15) << left << "Overall Fitness"
            << "| " << setw(10) << left << "BMI"
            << "| " << setw(12) << left << "Beep Test"
            << "| " << setw(10) << left << "2.4km" << " |" << endl;

        // Horizontal line to separate header and data
        cout << "--------------------------------------------------------------------+" << endl;

        while ((row = mysql_fetch_row(res))) {
            double overall = CalculateOverallFitness(stoi(row[1]), stoi(row[2]), stoi(row[3]), stoi(row[4]), stoi(row[5]));
            // Display data in the table format with vertical lines
            cout << "| " << setw(10) << left << row[0]
                << "| " << setw(14) << left << overall << "%"
                << "| " << setw(10) << left << row[6]
                << "| " << setw(12) << left << row[7]
                << "| " << setw(10) << left << row[8] << " |" << endl;
            cout << "--------------------------------------------------------------------+" << endl;
        }
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    _getch();
}


void DisplayOverallUnofficialFitness() {
    system("cls");
    string query;

    // Query selection based on user role
    if (currentUserRole == "Admin") {
        query = "SELECT CadetID, pushup, pullup, situp, lunges, squat, BMI, beepTest, distance FROM unfitness";
    }
    else if (currentUserRole == "Coach") {
        query = "SELECT CadetID, pushup, pullup, situp, lunges, squat, BMI, beepTest, distance FROM unfitness WHERE squad='" + currentSquad + "'";
    }
    else if (currentUserRole == "Cadet") {
        query = "SELECT CadetID, pushup, pullup, situp, lunges, squat, BMI, beepTest, distance FROM unfitness WHERE CadetID='" + currentUserID + "'";
    }

    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        // Header for the table
        cout << "----- Overall Unofficial Fitness Details ---------------------------+" << endl;
        cout << "| " << setw(10) << left << "CadetID"
            << "| " << setw(15) << left << "Overall Fitness"
            << "| " << setw(10) << left << "BMI"
            << "| " << setw(12) << left << "Beep Test"
            << "| " << setw(10) << left << "2.4km" << " |" << endl;

        // Horizontal line to separate header and data
        cout << "--------------------------------------------------------------------+" << endl;

        while ((row = mysql_fetch_row(res))) {
            double overall = CalculateOverallFitness(stoi(row[1]), stoi(row[2]), stoi(row[3]), stoi(row[4]), stoi(row[5]));
            // Display data in the table format with vertical lines
            cout << "| " << setw(10) << left << row[0]
                << "| " << setw(14) << left << overall << "%"
                << "| " << setw(10) << left << row[6]
                << "| " << setw(12) << left << row[7]
                << "| " << setw(10) << left << row[8] << " |" << endl;
           cout << "--------------------------------------------------------------------+" << endl;

        }
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }
    _getch();
}


double CalculateOverallFitness(int pushup, int pullup, int situp, int lunges, int squat) {
    int maxScorePerWorkout = 50;
    double totalScore = pushup + pullup + situp + lunges + squat;
    double maxOverallScore = 5 * maxScorePerWorkout; // Since there are 5 workouts

    return (totalScore / maxOverallScore) * 100; // Return percentage
}

void CalculateOverallUnofficialFitness(string cadetID) {
    system("cls");

    // Query to fetch fitness records for the cadet
    string query = "SELECT * FROM unfitness WHERE CadetID = '" + cadetID + "'";
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        int totalWorkouts = 0;
        double overallPercentage = 0.0;

        cout << "----- Overall Unofficial Fitness Details -----" << endl;
        while ((row = mysql_fetch_row(res))) {
            // Fetch and calculate fitness data here
            // For each record, calculate the overall percentage
        }

        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    _getch();
}


void OfficialFitnessReport() {
    system("cls");
    if (currentUserRole == "Admin") {
        // Admin can view all cadet reports
        string query = "SELECT * FROM fitness_details";
        GenerateFitnessReport(query, "Official");
    }
    else if (currentUserRole == "Coach") {
        // Coach can view only their squad's fitness reports
        string query = "SELECT * FROM fitness_details WHERE squad = '" + currentSquad + "'";
        GenerateFitnessReport(query, "Official");
    }
    else if (currentUserRole == "Cadet") {
        // Cadet can view only their own fitness report
        string query = "SELECT * FROM fitness_details WHERE CadetID = '" + currentUserID + "'";
        GenerateFitnessReport(query, "Official");
    }
    else {
        cout << "Access Denied. You don't have permission to access this report." << endl;
        _getch();
    }
}


void UnofficialFitnessReport() {
    system("cls");
    if (currentUserRole == "Admin") {
        // Admin can view all cadet reports
        string query = "SELECT * FROM unfitness";
        GenerateFitnessReport(query, "Unofficial");
    }
    else if (currentUserRole == "Coach") {
        // Coach can view only their squad's fitness reports
        string query = "SELECT * FROM unfitness WHERE squad = '" + currentSquad + "'";
        GenerateFitnessReport(query, "Unofficial");
    }
    else if (currentUserRole == "Cadet") {
        // Cadet can view only their own fitness report
        string query = "SELECT * FROM unfitness WHERE CadetID = '" + currentUserID + "'";
        GenerateFitnessReport(query, "Unofficial");
    }
    else {
        cout << "Access Denied. You don't have permission to access this report." << endl;
        _getch();
    }
}

void GenerateFitnessReport(const string& query, const string& reportType) {
    const char* q = query.c_str();
    int qstate = mysql_query(conn, q);

    if (!qstate) {
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;

        // Report Header
        cout << "==============================================================================================================================" << endl;
        cout << "                              " << reportType << " Fitness Report" << endl;
        cout << "==============================================================================================================================" << endl;

        // Table Header
        cout << left << setw(10) << "CadetID"
            << setw(20) << "Date"
            << setw(10) << "Pushups"
            << setw(10) << "Situps"
            << setw(10) << "BMI"
            << setw(10) << "Lunges"
            << setw(10) << "Squat"
            << setw(10) << "Pullup"
            << setw(10) << "BeepTest"
            << setw(10) << "2.4KM"
            << setw(15) << "Overall %" << endl;
        cout << "------------------------------------------------------------------------------------------------------------------------------" << endl;

        // Table Rows
        while ((row = mysql_fetch_row(res))) {
            double overallPercentage = CalculateOverallFitness(
                stoi(row[2]), // Pushups
                stoi(row[3]), // Situps
                stof(row[7]), // Pullup
                stoi(row[4]), // Lunges
                stoi(row[5])  // Squat
            );

            cout << left << setw(10) << row[10]   // CadetID
                << setw(20) << row[0]           //date
                << setw(10) << row[1]           // Pushups
                << setw(10) << row[3]           // Situps
                << setw(10) << row[6]           // BMI
                << setw(10) << row[7]           // Lunges
                << setw(10) << row[8]           // Squat
                << setw(10) << row[2]           // Pullup
                << setw(10) << row[9]           // BeepTest
                << setw(10) << row[11]           // 2.4KM
                << setw(15) << overallPercentage << "%" << endl;

            // Feedback Section
            cout << "Feedback and Recommendations:" << endl;
            DisplayFeedbackAndRecommendations(
                stoi(row[2]), // Pushups
                stoi(row[3]), // Situps
                stof(row[7]), // Pullup
                stoi(row[4]), // Lunges
                stoi(row[5]), // Squat
                stof(row[6]), // BMI
                stof(row[8]), // BeepTest
                stof(row[9]), // 2.4KM
                overallPercentage
            );
            cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
        }

        mysql_free_result(res);
    }
    else {
        cout << "Query Execution Problem: " << mysql_error(conn) << endl;
    }

    cout << "Press Enter to continue...";
    _getch();
}



// Function to generate recommendations for the cadet
void DisplayFeedbackAndRecommendations(int pushup, int situp, int pullup, int lunges, int squat,
    float bmi, float beepTest, float distance, double overallPercentage) {
    // Feedback for Pushups
    if (pushup < 20) {
        cout << "Pushups: Fail - Try Knee Pushups (20 reps) and Incline Pushups (20 reps)." << endl;
    }
    else if (pushup >= 20 && pushup <= 50) {
        cout << "Pushups: Pass - Good! Keep consistent." << endl;
    }
    else {
        cout << "Pushups: Excellent - You are outstanding! Keep up the momentum." << endl;
    }

    // Feedback for Situps
    if (situp < 20) {
        cout << "Situps: Fail - Try Russian Twists and Crunches (20 reps each)." << endl;
    }
    else if (situp >= 20 && situp <= 50) {
        cout << "Situps: Pass - Good! Keep consistent." << endl;
    }
    else {
        cout << "Situps: Excellent - Outstanding work! Keep it up." << endl;
    }

    // Feedback for Pullup
    if (pullup < 20) {
        cout << "Pullup: Fail - Start with Australian pullup (20 reps), and gradually increase." << endl;
    }
    else if (pullup>= 20 && pullup <= 50) {
        cout << "Pullup: Pass - Good! Keep consistent." << endl;
    }
    else {
        cout << "Pullup: Excellent - You're doing great! Stay strong." << endl;
    }

    // Feedback for Lunges
    if (lunges < 20) {
        cout << "Lunges: Fail - Perform 20 forward lunges for each leg." << endl;
    }
    else if (lunges >= 20 && lunges <= 50) {
        cout << "Lunges: Pass - Good! Keep consistent." << endl;
    }
    else {
        cout << "Lunges: Excellent - Amazing! You're crushing it." << endl;
    }

    // Feedback for Squats
    if (squat < 20) {
        cout << "Squats: Fail - Perform 20 bodyweight squats and add resistance gradually." << endl;
    }
    else if (squat >= 20 && squat <= 50) {
        cout << "Squats: Pass - Good! Keep consistent." << endl;
    }
    else {
        cout << "Squats: Excellent - Fantastic! You're at peak performance." << endl;
    }

    // Feedback for BMI
    if (bmi < 18.5) {
        cout << "BMI: Fail - Underweight. You need to eat more, soldier." << endl;
    }
    else if (bmi >= 18.5 && bmi <= 24.9) {
        cout << "BMI: Pass - Normal. Keep consistent on your diet." << endl;
    }
    else {
        cout << "BMI: Fail - Overweight. You need to maintain a better diet." << endl;
    }

    // Feedback for Beep Test
    if (beepTest < 9) {
        cout << "Beep Test: Fail - You need improvement." << endl;
    }
    else {
        cout << "Beep Test: Pass - Great work! Keep consistent." << endl;
    }

    // Feedback for 2.4KM
    if (distance > 12) {
        cout << "2.4KM: Fail - Keep improving your speed and endurance." << endl;
    }
    else if (distance <= 12 && distance > 10) {
        cout << "2.4KM: Pass - Good pace! Keep consistent." << endl;
    }
    else {
        cout << "2.4KM: Excellent - Amazing! You are the real army soldier." << endl;
    }

    // Feedback for Overall Percentage
    if (overallPercentage < 50) {
        cout << "Overall Fitness: Fail - You need to improve more. Keep practicing!" << endl;
    }
    else if (overallPercentage >= 50 && overallPercentage < 80) {
        cout << "Overall Fitness: Pass - Great work! Keep consistent, soldier!" << endl;
    }
    else {
        cout << "Overall Fitness: Excellent - Outstanding performance! Keep up the momentum!" << endl;
    }
}



// Function to display feedback from the coach (for simplicity, hardcoded here)
void ReportMenu() {
    int menu;
    while (true) {
        system("cls");
        cout << "----- Fitness Report Menu -----" << endl;
        cout << "1. Official Fitness Report" << endl;
        cout << "2. Unofficial Fitness Report" << endl;
        cout << "3. Return to Previous Menu" << endl;
        cout << "Choose an option: ";
        cin >> menu;

        switch (menu) {
        case 1:
            OfficialFitnessReport();  // Handle official fitness report
            break;
        case 2:
            UnofficialFitnessReport();  // Handle unofficial fitness report
            break;
        case 3:
            return;  // Return to the main menu
        default:
            cout << "Invalid option. Press Enter to retry...";
            _getch();
            break;
        }
    }
}

double CalculateOverallFitness(string cadetID) {
    double officialTotalScore = 0, unofficialTotalScore = 0;
    int officialRecords = 0, unofficialRecords = 0;

    // Calculate official fitness total score
    string officialQuery = "SELECT * FROM fitness_details WHERE CadetID = '" + cadetID + "'";
    const char* qOfficial = officialQuery.c_str();
    int qStateOfficial = mysql_query(conn, qOfficial);

    if (!qStateOfficial) {
        MYSQL_RES* resOfficial = mysql_store_result(conn);
        officialRecords = mysql_num_rows(resOfficial);  // Count official fitness records

        if (officialRecords > 0) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(resOfficial))) {
                int pushup = atoi(row[2]);
                int pullup = atoi(row[3]);
                int situp = atoi(row[4]);
                int lunges = atoi(row[7]);
                int squat = atoi(row[8]);
                double beepTest = atof(row[9]);
                double distance = atof(row[10]);

                double score = pushup + pullup + situp + lunges + squat + beepTest + distance; // Adjust this scoring system as needed
                officialTotalScore += score;
            }
        }
    }
    else {
        cout << "Error retrieving official fitness data: " << mysql_error(conn) << endl;
    }

    // Calculate unofficial fitness total score
    string unofficialQuery = "SELECT * FROM unfitness WHERE CadetID = '" + cadetID + "'";
    const char* qUnofficial = unofficialQuery.c_str();
    int qStateUnofficial = mysql_query(conn, qUnofficial);

    if (!qStateUnofficial) {
        MYSQL_RES* resUnofficial = mysql_store_result(conn);
        unofficialRecords = mysql_num_rows(resUnofficial);  // Count unofficial fitness records

        if (unofficialRecords > 0) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(resUnofficial))) {
                int pushup = atoi(row[2]);
                int pullup = atoi(row[3]);
                int situp = atoi(row[4]);
                int lunges = atoi(row[7]);
                int squat = atoi(row[8]);
                double beepTest = atof(row[9]);
                double distance = atof(row[10]);

                double score = pushup + pullup + situp + lunges + squat + beepTest + distance; // Adjust this scoring system as needed
                unofficialTotalScore += score;
            }
        }
    }
    else {
        cout << "Error retrieving unofficial fitness data: " << mysql_error(conn) << endl;
    }

    double totalScore = officialTotalScore + unofficialTotalScore;
    double totalRecords = officialRecords + unofficialRecords;
    double maxTotalScore = totalRecords * 50;  // Assume the max score for each record is 50

    return (totalScore / maxTotalScore) * 100;
}


void CenterText(const string& text, int totalWidth) {
    int spaces = (totalWidth - text.length()) / 2;
    for (int i = 0; i < spaces; ++i) cout << " ";
    cout << text << endl;
}
void DisplayBanner() {
    int consoleWidth = 70; // Adjust based on your console size

    CenterText("========================================================================================================", consoleWidth);
    CenterText("    \n\t\t                       *****      *****      *****      *   *     ", consoleWidth);
    CenterText("                   \t\t       *   *      *   *        *        *   *     ", consoleWidth);
    CenterText("                   \t\t       *****      *   *        *        *   *     ", consoleWidth);
    CenterText("                   \t\t       *  *       *   *        *        *   *     ", consoleWidth);
    CenterText("                   \t\t       *   *      *****        *        *****     ", consoleWidth);
    CenterText("\n========================================================================================================", consoleWidth);
    cout << endl;
    CenterText("\t\t\t\t\1WELCOME TO THE PALAPES UTeM FITNESS SYSTEM", consoleWidth);
    CenterText("\t\t\t\tRESERVE OFFICER TRAINING UNIT", consoleWidth);
    CenterText("\t\t\STRONG AND LOYAL", consoleWidth);
    CenterText("\t\t\t\tREADY TO SERVE THE COUNTRY", consoleWidth);
    CenterText("\n========================================================================================================", consoleWidth);
    cout << endl;
}
void DisplayMenu() {
    int consoleWidth = 70; // Adjust for centering
    CenterText("\t\tMENU OPTIONS(ENTER ONLY NUMBERS):", consoleWidth);
    CenterText("\n\t\t\t\t1. Login          2. Register          3. Exit", consoleWidth);
    cout << endl;
    cout << string((consoleWidth - 22) / 2, ' ') << "\t\tChoose an option: ";
}


int main() {
    while (true) {
        system("cls"); // Clear the screen
        DisplayBanner();

        // Ensure the database connection is established at the start
        db_response::ConnectionFunction();

        DisplayMenu();

        int menu;
        cin >> menu;

        switch (menu) {
        case 1:
            Login();
            break;
        case 2:
            Register();
            break;
        case 3:
            cout << "Program Exiting..." << endl;
            db_response::Cleanup(); // Clean up the DB connection before exiting
            return 0;
        default:
            cout << "Invalid choice! Please select between 1 - 3. Press Enter to continue...";
            (void)_getch();
        }
    }
}
