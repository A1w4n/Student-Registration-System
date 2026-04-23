#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <cstdio>
#include <math.h>
#include <chrono>
#include <thread>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <random>

using namespace std;

struct Date {
    string months ;
    int day;
    int year;
};

struct Location {
    string province;
    string municipality;
    string barangay;
};

struct Student {
    string srcode;
    string password;
    string fullName;
    string age;
    string sex;
    string civilStatus;
    string parents;
    string guardian;
    string studentContact;
    string familyContact;
    Date birthdate;
    Location address;
    Location birthplace;
};

struct Admin {
    string username;
    string code;
};

class SecurityUtils {
private:
    string generateSalt(int length = 16) {
        const string CHARACTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        
        random_device random_device;
        mt19937 generator(random_device());
        uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

        string salt = "";
        for (int i = 0; i < length; ++i) {
            salt += CHARACTERS[distribution(generator)];
        }
        return salt;
    }

    string sha256(const string& str) {
        std::hash<std::string> hasher;
        size_t hashValue = hasher(str);

        std::stringstream ss;
        ss << std::hex << std::setw(64) << std::setfill('0') << hashValue;

        string hash_output = ss.str();
        if (hash_output.length() > 64) {
            return hash_output.substr(0, 64);
        }
        return hash_output;
    }

public:
    string encrypt(const string& password) {
        string salt = generateSalt();
        string dataToHash = password + salt;
        string hashedOutput = sha256(dataToHash);

        return salt + ":" + hashedOutput; 
    }

    bool check(const string& enteredPassword, const string& storedSaltAndHash) {
        size_t delimiter_pos = storedSaltAndHash.find(':');

        if (delimiter_pos == string::npos) {
            return false;
        }

        string stored_salt = storedSaltAndHash.substr(0, delimiter_pos);
        string stored_hash = storedSaltAndHash.substr(delimiter_pos + 1);
        string dataToHash = enteredPassword + stored_salt;
        string entered_hash = sha256(dataToHash);
        
        return entered_hash == stored_hash;
    }
};

//Declaration of the functions
void clearscreen();
bool isStrongPassword(const string& password);
bool file_checker( int lineNumber);
void seeInfo(); 
void changeInfo(); 
void updateLocation(Student& user, string userInfo);
void updateDate();
void deleteAccount(); 
void changePassword ();
void data_rewrite(string filefor, string data, string report);
void showReport(string filename);
void grantDefaultPassword(const string& srcode);
void showReport_SystemErrors(string filename);
void showReport_DeleteRequests(string filename);
Student user;
Admin admin;
SecurityUtils crypto;

//Global declaration of variables
int x;
string password;
string srcode;

int main () {
    char homeResponse;
    char response;
    char menuResponse;
    char reportCategory;
    char adminHomeResponse;
    char adminResponse;
    int  confirmationResponse;

    do {
        clearscreen();
        cout << "================ SECURE STUDENT REGISTRATION SYSTEM ==============" << endl;
        cout << "\nWelcome to your Secure Student Registration System" << endl;
        cout << "Select what to you want to do : " << endl;
        cout << "\n[1] Log-in " << "\n[2] Create Account " << "\n[3] Exit " << endl;
        cout << "\n\n\n[!] Report issue" << endl;
        cin >> homeResponse;
        cin.ignore(numeric_limits<streamsize> ::max(), '\n');
        clearscreen();

        switch(homeResponse) {
        case '1': { //Login phase
            x = 4;
            cout << "================ LOG-IN ==============" << endl;

            cout << "\nEnter your Student Registered code (SR-code) : ";
            getline(cin, srcode);
            if ( file_checker (0) == true) {
                user.srcode = srcode;

                string filename = srcode + "_data.txt";
                vector < string > fileLines;
                string line;

                ifstream inFile (user.srcode + "_data.txt" );
                if (inFile.is_open()) {
                    while (getline (inFile, line)) {
                        fileLines.push_back(line);
                    }
                }
                inFile.close();

                if (!fileLines[20].empty()) {
                    cout << "\nRequest default password granted." << endl;
                    cout << "Your password is now set to default password : Password123" << endl;
                    cout << "After log-in, please change your password immediately, Thank you!! \n" << endl;
                }

                do {
                    cout << "Enter your Password : ";
                    cin >> password;

                    if ( file_checker(1) == true) { 
                        user.password = crypto.encrypt(password);

                        clearscreen();
                        do {
                            clearscreen();
                            cout << "================ REGISTRATION MENU ==============" << endl;
                            cout << "\nWelcome to Your Registration Menu"<< endl;
                            cout << "Select what you will to accomplish in this menu : " << endl;
                            cout << "\n[1] See your information \n[2] Change your information \n[3] Delete this account \n[4] Change your password  \n[5] Back to home menu" << endl;
                            cin >> menuResponse;
                            cin.ignore(numeric_limits<streamsize> :: max(), '\n');

                            switch (menuResponse) {
                            case '1':
                                clearscreen();
                                cout << "================ USER INFORMATION DISPLAY ==============" << endl;

                                seeInfo();

                                cout << "\nGo to? : [1] Home Menu | [2] Back " << endl;
                                cin >> response;
                                clearscreen();
                                break;
                            case '2':
                                clearscreen();
                                cout << "================ CHANGE INFORMATION ==============" << endl;
                                cout << "\nTo change your data, just simply input the new data accordingly " << endl;
                                cout << "If want to skip and didn't want to overwwrite the current data, type [SKIP] !!" << endl;

                                changeInfo();

                                cout << "\nGo to? : [1] Home Menu | [2] Back " << endl;
                                cin >> response;
                                clearscreen();
                                break;
                            case '3':
                                clearscreen();
                                cout << "================ DELETE ACCOUNT ==============" << endl;

                                deleteAccount();

                                cout << "\nGo to? : [1] Home Menu | [2] Back " << endl;
                                cin >> response;
                                clearscreen();
                                break;
                            case '4':
                                clearscreen();
                                cout << "================ CHANGE PASSWORD ==============" << endl;

                                changePassword();

                                cout << "\nGo to? : [1] Home Menu | [2] Back " << endl;
                                cin >> response;
                                clearscreen();
                                break;
                            case '5' : 
								response = '1' ;     
								break;
                            }
                        } while (response == '2' );
                        x=0;
                        break;
                    } else {
                        x--;
                        cout << "\nPASSWORD IS INCORRECT!!, TRY AGAIN!!" << endl;
                        cout << "Your remaining attempt : " << x << endl;

                        if (x == 0 ) {
                            cout << "\n!!! MAXIMUN ATTEMPT REACHED !!!" << endl;
                            cout << "\nPRESS [1] TO GO BACK TO HOME MENU " << endl;
                            cin >> response;
                        }
                    }
                } while( x > 0);

            } else {
                cout << "\n!!1 USERNAME NOT FOUND !!!" << endl;
                cout << "Please check your username or create account if you have none " << endl;
                cout << "\nPRESS [1] TO GO BACK TO HOME MENU " << endl;
                cin >> response;
            }
            break;
        }//For closing of the Log-in phase

        case '2': {// Account creation phase
            clearscreen();
            cout << "================ CREATE ACCOUNT ==============" << endl;
            cout << "\nTo create an account you must first enter the items that are being asked. " << endl;

            cout << "\nSR-Code : ";
            getline(cin, srcode);

            cout << "\n--- New Password Requirments ---" << endl;
            cout << "~ Your password must contain at least 8 characters." << endl;
            cout << "~ It must have at least one capital letter." << endl;
            cout << "~ It must have at least one small character." << endl;
            cout << "~ It must have at least one number." << endl;

            do {
                cout << "\nEnter your new password : ";
                cin >> password;

                if (!isStrongPassword(password)) {
                    cout << "\nWeak password! Please try again.\n";
                }
            } while (!isStrongPassword(password));

            user.srcode = srcode;
            user.password = crypto.encrypt(password);

            int null = 0;
            ofstream outFile (user.srcode +"_data.txt");
            if (outFile.is_open() ) {
                outFile << user.srcode << '\n'; 
                outFile << user.password << '\n' ;
                outFile << user.fullName << '\n' ;
                outFile << user.age << '\n' ;
                outFile << user.sex << '\n' ;
                outFile << user.address.province << '\n';
                outFile << user.address.municipality << '\n' ;
                outFile << user.address.barangay << '\n';
                outFile << user.birthdate.months << '\n' ;
                outFile << user.birthdate.day << '\n' ;
                outFile << user.birthdate.year << '\n' ; 
                outFile << user.birthplace.province << '\n' ;
                outFile << user.birthplace.municipality << '\n';
                outFile << user.birthplace.barangay << '\n' ;
                outFile << user.civilStatus << '\n' ;
                outFile << user.parents << '\n' ;
                outFile << user.guardian << '\n' ;
                outFile << user.studentContact << '\n';
                outFile << user.familyContact<< '\n';
                outFile << null << '\n';
                outFile << '\n' ;
            } else {
                cout << "ERROR : File not found! " << endl;
            }
            outFile.close();

            cout << "\nAccount creation is successful!!" << endl;
            cout << "Please fill up your additional information in the 'Change Info' tab"<< endl;
            cout << "\nPRESS [1] TO EXIT  " << endl;
            cin >> response;
            clearscreen();
            break;
        }//For closing of the Account Creation phase 

        case '3' : { // Exit
            cout << "NOTICE : EXITING SYSTEM..." << endl;
            cout << "Thank you !!!" << endl;
            response = '0';
            break;
        }//For closing of Exit phase

        case '!' : { //Report system
            clearscreen();

            string report;
            string filename;

            cout << "================ REPORT ISSUE ==============" << endl;
            cout << "\nWhat is the category of report : " << endl;
            cout << "[1] Forgot password  \n[2] Error in the system \n[3] Back" << endl;
            cin >> reportCategory;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (reportCategory) {
            case '1' : {
                cout << "\nCASE : Forgot password" << endl;
                cout << "Enter your SR - Code : ";
                getline(cin, srcode);

                if(file_checker(0) == true ) {
                    data_rewrite( "forgot_password", srcode, "none"  );
                } else {
                    cerr << "\nERROR : SR-code not found!! " << endl;
                }

                cout << "Please wait for the response of admin. Thank you!!" << endl;
                cout << "\nPRESS [1] TO EXIT !!... " << endl;
                cin >> response;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            case '2' : {
                cout << "\nCASE : System Error" << endl;
                cout << "Enter your SR-code first : ";
                getline(cin, srcode);
                cout << "\nPlease explain the error : " << endl;
                getline(cin, report);

                data_rewrite("report", srcode, report);

                cout << "Please wait for the response of admin. Thank you!!" << endl;
                cout << "PRESS [1] TO EXIT !!..." << endl;
                cin >> response;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            case '3': {
            	response = '1';
                        break;
                }
            }
            break;
        }// For closing of Report system

        case '@' : {// Admin system
            x=0;
            int index = 0;

            cout << "Enter the username: ";
            getline(cin, admin.username);
            cout << "Enter passcode: ";
            getline(cin, admin.code);

            clearscreen();
            if (admin.username == "admin2025ssrsHMO" && admin.code == "HMOssrs123access" ) {
                do {
                    clearscreen();
                    cout << "================ WELCOME ADMIN!! ==============" << endl;
                    cout << "\nWhat to you want to accomplish : \n[1] See forgot password request \n[2] See reports \n[3] Delete account request \n[4] Log-out " << endl;
                    cin >> adminHomeResponse;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

                    switch(adminHomeResponse) {
                    case '1' : {
                        clearscreen();
                        cout << "================ FORGOT PASSWORD REQUESTS ==============" << endl;
                        
                        showReport("forgot_passwordFile.txt");

                        cout << "\nContinue? : [1] yes, back to admin menu | [2] no, log out " << endl;
                        cin >>  adminResponse; // Use x as the loop control (1 to continue, 2 to log out)
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    case '2' : {
                        clearscreen();
                        cout << "================ SPECIFIED REPORTS ==============" << endl;
                        
                        showReport_SystemErrors("reportFile.txt");

                        cout << "\nContinue? : [1] yes, back to admin menu | [2] no, log out " << endl;
                        cin >>  adminResponse;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    case '3' : {
                        clearscreen();
                        cout << "================ DELETE ACCOUNT REQUESTS ==============" << endl;
                        
                        showReport_DeleteRequests("delete_accFile.txt");

                        cout << "\nContinue? : [1] yes, back to admin menu | [2] no, log out " << endl;
                        cin >>  adminResponse;
                        break;
                    }
                    case '4' : {
                        response = '1';
                        break;
                        }
                    default: {
                        cout << "Invalid choice. Logging out." << endl;
                        response = '1';
                        break;
                    }
                    }
                } while( adminResponse == '1' );
                response = '1';
            } else {
                cout << "Access denied..." << endl;
                cout << "\nNOTICE! : Redirecting back to Home Menu " << endl;
                cout << "DO NOT PRESS ANYTHING" << endl;
                this_thread::sleep_for(chrono::seconds(2));
                clearscreen();
                response = '1';
            }
            break;
        }//For closing of admin system
        
        default : {
            cout << "Please enter only the appropriate choices !!" << endl;
            cout << "\nNOTICE! : Redirecting back to Home Menu " << endl;
            cout << "DO NOT PRESS ANYTHING" << endl;
            this_thread::sleep_for(chrono::seconds(2));
            response = '1';
        }
        break;
        }
    }
    while(response == '1');

    return 0;
}


void clearscreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

bool isStrongPassword(const string& password) {
    bool hasUpper = false;
    bool hasDigit = false;

    if (password.length() < 8)
        return false;

    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (isdigit(c)) hasDigit = true;
    }
    return hasUpper && hasDigit;
}

bool file_checker( int lineNumber) {
    string filename = srcode + "_data.txt";
    vector < string > fileLines;
    string line;
    bool srcode_match = false;
    bool password_match = false;

    ifstream inFile (filename);
    if ( inFile.is_open()) {
        while (getline (inFile, line)) {
            fileLines.push_back(line);
        }
        inFile.close();

        if (lineNumber < fileLines.size()) {
            if (lineNumber == 0) {
                if (srcode == fileLines[lineNumber] ) {
                    srcode_match = true;
                }
            } else if (lineNumber == 1) {
                string storedPassword = fileLines[lineNumber];
                if ( crypto.check(password, storedPassword ) ) {
                    password_match = true;
                }
            }
        }
    }
    return srcode_match || password_match;
}

void seeInfo() {
    string filename = user.srcode + "_data.txt";
    vector < string > fileLines;
    string line;

    ifstream inFile (filename);
    if (inFile.is_open()) {
        while (getline (inFile, line)) {
            fileLines.push_back(line);
        }

        user.srcode = fileLines[0];
        user.fullName = fileLines[2];
        user.age = fileLines[3];
        user.sex = fileLines[4];
        user.address.province = fileLines[5];
        user.address.municipality = fileLines[6];
        user.address.barangay = fileLines[7];
        user.birthdate.months = fileLines[8];

        try {
            user.birthdate.day = stoi( fileLines[9]);
            user.birthdate.year = stoi( fileLines[10]);
        } catch (const std::invalid_argument& e) {
            cerr << "Invalid argument: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            cerr << "Out of range: " << e.what() << std::endl;
        }

        user.birthplace.province = fileLines[11];
        user.birthplace.municipality = fileLines[12];
        user.birthplace.barangay = fileLines[13];
        user.civilStatus = fileLines[14];
        user.parents = fileLines[15];
        user.guardian = fileLines[16];
        user.studentContact = fileLines[17];
        user.familyContact = fileLines[18];

    }
    inFile.close();

    cout << "Student Registration code : " << user.srcode << '\n' << endl;
    cout << "Name : " << user.fullName << endl;
    cout << "Age : " << user.age << endl;
    cout << "Sex : " << user.sex << endl;
    cout << "Address : " << user.address.province << ","  << user.address.municipality << "," << user.address.barangay << endl;
    cout << "Birthday : " << user.birthdate.months << " " << user.birthdate.day << "," << user.birthdate.year  << endl;
    cout << "Place of Birth : " << user.birthplace.province <<"," << user.birthplace.municipality << "," << user.birthplace.barangay << endl;
    cout << "Civil Status : " << user.civilStatus << endl;
    cout << "Parents : " << user.parents << endl;
    cout << "Guardian : " << user.guardian << endl;
    cout << "Your contact number : " << user.studentContact << endl;
    cout << "Parents's / Guardian's contact number : " << user.familyContact << endl;
}

void changeInfo() {
    string filename = user.srcode + "_data.txt";
    vector < string > fileLines;
    string line;
    string new_srcode;

    ifstream inFile (filename);
    if ( inFile.is_open()) {
        while (getline (inFile, line)) {
            fileLines.push_back(line);
        }
    } else {
        cout << "ERROR : File not Found!" << endl;
    }
    inFile.close();

    if (fileLines.size() >= 2 ) {

        cout << "\nEnter your Name ( Current : " << fileLines[2] << " )" << endl;
        getline(cin, user.fullName);
        if (user.fullName == "Skip" || user.fullName == "SKIP" || user.fullName == "skip") {
            user.fullName = fileLines[2];
        } else {
            fileLines[2] = user.fullName;
        }

        cout << "\nEnter your updated age ( Current : " << fileLines[3] << " )" << endl;
        getline(cin, user.age);
        if (user.age == "SKIP" || user.age == "skip" || user.age == "Skip") {
            user.age = fileLines[3];
        } else {
            fileLines[3] = user.age;
        }

        if (fileLines[19] == "0"  ) {
            cout << "\nEnter your sex at birth( Current : " << fileLines[4] << " )" << endl;
            cout << "[M] male || [F] female" << endl;
            getline (cin, user.sex);

            if (user.sex == "M" || user.sex == "m" ) {
                fileLines[4] = "Male";
                fileLines[19] = "LOCKED!!";
            } else if (user.sex == "F" || user.sex == "f" ) {
                fileLines[4] = "Female";
                fileLines[19] = "LOCKED!!";
            } else if (user.sex == "SKIP" || user.sex == "skip" || user.sex == "Skip" ) {
                user.sex = fileLines[4];
            } else {
                cout << "ERROR !! , Unable to save that response" << endl;
            }
        } else {
            cout << "\nSex : " << fileLines[4] << endl;
        }

        cout << "\nNOTICE! : Jumping now to updating address part..." << endl;
        cout << "DO NOT PRESS ANYTHING " << endl;
        this_thread::sleep_for(chrono::seconds(2));

        clearscreen();
        cout << "Enter your updated address ( Current : " << fileLines[5] << "," << fileLines[6] << "," << fileLines[7] << " )" <<  endl;
        updateLocation(user, "address" );
        fileLines[5] = user.address.province;
        fileLines[6] = user.address.municipality;
        fileLines[7] = user.address.barangay;

        cout << "\nNOTICE! : Jumping now to updating birthdate part..." << endl;
        cout << "DO NOT PRESS ANYTHING " << endl;
        this_thread::sleep_for(chrono::seconds(2));

        clearscreen();
        cout << "\nEnter your updated birthday ( Current : " << fileLines[8] << " " << fileLines[9] << "," << fileLines[10] << " )" << endl;
        updateDate();
        fileLines[8] = user.birthdate.months;
        fileLines[9] = to_string (user.birthdate.day);
        fileLines[10] = to_string (user.birthdate.year);

        cout << "\nNOTICE! : Jumping now to updating address of birth part..." << endl;
        cout << "DO NOT PRESS ANYTHING " << endl;
        this_thread::sleep_for(chrono::seconds(2));

        clearscreen();
        cout << "\nEnter your updated place of birth ( Current : " << fileLines[11] << "," << fileLines[12] << "," <<fileLines[13] << " )"  << endl;
        updateLocation(user, "birthplace" );
        fileLines[11] = user.birthplace.province;
        fileLines[12] = user.birthplace.municipality;
        fileLines[13] = user.birthplace.barangay;

        cout << "================ CHANGE INFORMATION ==============" << endl;
        cout << "\nTo change your data, just simply input the new data accordingly " << endl;
        cout << "If want to skip and didn't want to overwwrite the current data, type [SKIP] !!" << endl;

        cout << "\nName : " << fileLines[2] << endl;
        cout << "Age : " << fileLines[3] << endl;
        cout << "Sex : " << fileLines[4] << endl;
        cout << "Address ; " << fileLines[5] << "," << fileLines[6] << "," << fileLines[7] <<  endl;
        cout << "Birthday ; " << fileLines[8] << " " << fileLines[9] << "," << fileLines[10] << endl;
        cout << "Place of birth : " << fileLines[11] << "," << fileLines[12] << "," <<fileLines[13]  << endl;

        cout << "\nEnter your updated civil status ( Current : " << fileLines[14] << " )" << endl;
        cout << "[1] single \n[2] married \n[3] divorced \n[4] widowed " << endl;
        getline(cin, user.civilStatus);
        if (user.civilStatus == "SKIP" || user.civilStatus == "skip" || user.civilStatus == "Skip" ) {
            user.civilStatus = fileLines[14];
        } else if (user.civilStatus == "1") {
            user.civilStatus = "single";
        } else if (user.civilStatus == "2") {
            user.civilStatus = "married";
        } else if (user.civilStatus == "3") {
            user.civilStatus = "divorced";
        } else if (user.civilStatus == "4") {
            user.civilStatus = "widowed";
        } else {
            cout << "ERROR!! : invalid input..." << endl;
        }
        fileLines[14] = user.civilStatus;

        cout << "\nEnter your Parents fullname ( Current : " << fileLines[15] << " )" << endl;
        getline(cin, user.parents);
        if (user.parents == "SKIP" || user.parents == "skip" || user.parents == "Skip" ) {
            user.parents = fileLines[15];
        } else {
            fileLines[15] = user.parents;
        }

        cout << "\nEnter your guardian fullname ( Current : " << fileLines[16] << " )" << endl;
        getline(cin, user.guardian);
        if (user.guardian == "SKIP" || user.guardian == "skip" || user.guardian == "Skip" ) {
            user.guardian = fileLines[16];
        } else {
            fileLines[16] = user.guardian;
        }

        cout << "\nEnter your new contact number ( Current : " << fileLines[17] << " )" << endl;
        getline(cin, user.studentContact);
        if (user.studentContact == "SKIP" || user.studentContact == "skip" || user.studentContact == "Skip" ) {
            user.studentContact = fileLines[17];
        } else {
            fileLines[17] = user.studentContact;
        }

        cout << "\nEnter your parent's / guardian's new contact number ( Current : " << fileLines[18] << " )" << endl;
        getline(cin, user.familyContact);
        if (user.familyContact == "SKIP" || user.familyContact == "skip" || user.familyContact == "Skip" ) {
            user.familyContact = fileLines[18];
        } else {
            fileLines[18] = user.familyContact;
        }
    }

    ofstream outFile (filename, ios::trunc);
    if (outFile.is_open()) {
        for (const string& modifiedLine : fileLines) {
            outFile << modifiedLine << "\n";
        }
    }
    outFile.close();

    cout << "\nINFORMATION SAVED SUCCESFULLY!!" << endl;
}

void updateLocation(Student& user, string userInfo) {
    // List of CALABARZON provinces
    vector<string> provinces = {"Cavite", "Laguna", "Batangas", "Rizal", "Quezon", "Others..." };

    // Municipalities for each province
    vector<vector<string>> municipalities = {
        // Cavite
        {"Bacoor", "Cavite City", "Dasmariñas", "General Trias", "Imus", "Tagaytay", "Trece Martires", "Alfonso", "Amadeo", "Carmona", "Add_location"},
        // Laguna
        {"Biñan", "Calamba", "San Pablo", "Santa Rosa", "Cabuyao", "Los Baños", "Bay", "Calauan", "Liliw", "Nagcarlan", "Add_location..."},
        // Batangas
        {"Agoncillo", "Alitagtag", "Balayan", "Balete", "Bauan", "Calaca", "Calatagan", "Cuenca", "Ibaan", "Laurel", "Lemery", "Lian", "Lipa", "Lobo", "Mabini", "Malvar", "Mataasnakahoy", "Nasugbu", "Padre Garcia", "Rosario", "San Jose", "San Juan", "San Luis", "San Nicolas", "San Pascual", "Santa Teresita", "Santo Tomas", "Taal", "Talisay", "Tanauan", "Taysan", "Tingloy", "Tuy", "Add_location..."},
        // Rizal
        {"Antipolo", "Angono", "Baras", "Binangonan", "Cainta", "Cardona", "Jala-Jala", "Morong", "Pililla", "Rodriguez", "Add_location..."},
        // Quezon
        {"Lucena", "Tayabas", "Sariaya", "Candelaria", "Lucban", "Tiaong", "Sampaloc", "San Antonio", "Padre Burgos", "General Nakar", "Add_location..."},
        // othes
        {"Add_location" }
    };

    // Barangays for each municipality/city
    vector<vector<vector<string>>> barangays = {
        // 1. Cavite barangays
        {
            // Bacoor
            {"Molino V", "Molino IV", "Molino III", "Molino VI", "Molino VII", "Mambog V", "Mambog I", "Mambog IV", "Molino I", "Molino II", "Add_location..."},
            // Cavite City
            {"Barangay 10A (Pob.)", "Barangay 10B (Pob.)", "Barangay 39 (Pob.)", "Barangay 41 (Pob.)", "Barangay 42 (Pob.)", "Barangay 43 (Pob.)", "Barangay 44 (Pob.)", "Barangay 45 (Pob.)", "Barangay 46 (Pob.)", "Barangay 47 (Pob.)", "Add_location..."},
            // Dasmarinas
            {"Paliparan I", "San Jose", "Salawag", "San Agustin III", "Paliparan II", "San Agustin I", "San Agustin II", "Sampaloc IV", "Burol Main", "Paliparan III", "Add_location..."},
            // General Trias
            {"Manggahan", "Bacao II", "Javalera", "Buenavista I", "San Francisco", "Panungyanan", "Buenavista II", "Arnaldo", "Buenavista III", "Tapia", "Others..."},
            // Imus
            {"Bayan Luma 9", "Toclong 1-A", "Malagasang 1-G", "Malagasang 2-A", "Malagasang 2-F", "Malagasang 2-G", "Malagasang 2-H", "Malagasang 2-I", "Malagasang 2-J", "Malagasang 2-K", "Add_location..."},
            // Tagaytay
            {"Kaybagal Central", "Maharlika East", "Maharlika West", "Mendez Crossing East", "Mendez Crossing West", "Neogan", "Poblacion", "Sungay West", "Sungay East", "Asisan", "Add_location..."},
            // Trece Martires
            {"San Agustin", "San Jose", "De Ocampo", "Luciano (Poblacion)", "Inocencio", "Aguado", "Lallana", "Pulo", "Cabezas", "Conchu", "Add_location..."},
            // Alfonso
            {"Marahan I", "Marahan II", "Kaytitinga", "Sinaliw Malaki", "Sinaliw Na Munti", "Upli", "Palay-Palay", "Matagbak I", "Matagbak II", "Buck Estate", "Add_location..."},
            // Amadeo
            {"Banaybanay", "Pangil", "Dagatan", "Loma", "Maitim I", "Maitim II", "Malagasi", "Pulo", "Salaban", "Sampaloc I", "Add_location..."},
            // Carmona
            {"Maduya", "Pulo", "Milagrosa", "Lantic", "San Jose", "Cabilang Baybay", "Tulong", "Poblacion 1", "Poblacion 2", "Poblacion 3", "Add_location..."}
        },
        // 2. Laguna barangays
        {
            // Binan
            {"Langkiwa", "Malaban", "San Antonio", "San Vicente", "Casile", "Gatid", "Mabuhay", "Canlalay", "Platero", "San Jose", "Add_location..."},
            // Calamba
            {"Canlubang", "Pulo", "Mayapa", "Real", "Pansol", "Makiling", "Punta", "Barandal", "San Cristobal", "Paciano Rizal", "Add_location..."},
            // San Pablo
            {"San Jose", "San Francisco", "Santa Maria", "San Roque", "San Antonio", "Santa Isabel", "San Lucas I", "San Lucas II", "San Rafael", "San Gabriel", "Add_location..."},
            // Santa Rosa
            {"Balibago", "Macabling", "Malitlit", "Pulong Santa Cruz", "Santo Domingo", "Aplaya", "Caingin", "Dila", "Labas", "Pook", "Add_location..."},
            // Cabuyao
            {"Banaybanay", "Mamatid", "Pulo", "Bigaa", "Butong", "Baclaran", "Caniogan", "Gulod", "Marinig", "Niugan", "Add_location..."},
            // Los Banos
            {"Batong Malake", "Mayondon", "Bayog", "San Antonio", "Pangaw", "Lalakay", "Malinta", "Anos", "Poblacion", "Putho-Tuntungin", "Add_location..."},
            // Bay
            {"Pacita", "Dila", "Masaya", "Maitim", "Calo", "San Antonio", "Bitin", "Puypuy", "San Isidro", "Poblacion", "Add_location..."},
            // Calauan
            {"Dayap", "Balayhangin", "Bangyas", "Imok", "Guelew", "Lamot 1", "Lamot 2", "Mabacan", "Masiit", "Poblacion 1", "Add_location..."},
            // Liliw
            {"Bubukal", "Calumpang", "Kanlurang Talaongan", "Lubo", "Luya", "Luy-os", "Malabo-Kalantukan", "Masico", "Palayan", "Pangao", "Add_location..."},
            // Nagcarlan
            {"Laub", "Pansol", "Sibulan", "Sta. Catalina", "Sta. Maria", "Sta. Teresita", "Sto. Niño", "Talaungan", "Tipacan", "Abo", "Add_location..."}
        },
        // 3. Batangas barangays
        {
            // Agoncillo
            {"Pook", "Panhulan", "Banyaga", "Pansipit", "Poblacion", "Balangon", "Coral Na Munti", "Bilibinwang", "Gulod", "Talisay", "Add_location..."},
            // Alitagtag
            {"Muzon", "Poblacion East", "Poblacion West", "Balagbag", "San Jose", "Concordia", "Lalap", "Pangao", "Dominador", "Pinagkurusan", "Add_location..."},
            // Balayan
            {"Barangay 12 (Pob.)", "Barangay 13 (Pob.)", "Barangay 14 (Pob.)", "Barangay 15 (Pob.)", "Barangay 16 (Pob.)", "Barangay 17 (Pob.)", "Barangay 18 (Pob.)", "Barangay 19 (Pob.)", "Barangay 20 (Pob.)", "Barangay 21 (Pob.)", "Add_location..."},
            // Balete
            {"Sampalocan", "Poblacion", "Malabanan", "Malitlit", "Magapi", "Looc", "Calawit", "Alangilan", "Balete", "Paligawan", "Add_location..."},
            // Bauan
            {"Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Poblacion 8", "Poblacion 9", "Poblacion 10", "Add_location..."},
            // Calaca
            {"Dila", "Talisay", "Camastilisan", "Calantas", "Bantog", "Niyugan", "Tugtug", "Salong", "Baclaran", "Taklang-Awa", "Add_location..."},
            // Calatagan
            {"Balibago", "Quilitisan", "Gulod", "Talisay", "Baha", "Balsahan", "Balitoc", "Bungahan", "Lucsuhin", "Poblacion 1", "Add_location..."},
            // Cuenca
            {"Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Don Juan", "Don Pedro", "Dita", "Labac", "Dacutan", "Add_location..."},
            // Ibaan
            {"Talaibon", "Quilo", "Malainin", "Palindan", "Poblacion", "San Jose", "Bungahan", "Calamias", "Catandala", "Dayapan", "Add_location..."},
            // Laurel
            {"Balakilong", "Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Poblacion 8", "Poblacion 9", "Add_location..."},
            // Lemery
            {"Payapa Ilaya", "Payapa Ibaba", "Dita", "Aplaya", "Tigbe", "Mataas Na Lupa", "Palanas", "Tala", "Poblacion 1", "Poblacion 2", "Add_location..."},
            // Lian
            {"Matabungkay", "Malacot", "Lian (Poblacion)", "Bungahan", "Malaruhatan", "Balibago", "Binubusan", "Cumba", "Bagong Lipunan", "Baldeo", "Add_location..."},
            // Lipa
            {"Pinagkawitan", "Sampaguita", "Sambat", "Mataas Na Lupa", "Tipacan", "Cumba", "Pag-Olingin", "Lumbang", "Talaibon", "Bolbok", "Add_location..."},
            // Lobo
            {"Balatbat", "Malabrigo", "Malapad Na Parang", "Balibago", "Poblacion", "Olo-Olo", "Sawang", "Sublo", "Mabilog Na Bundok", "Calo", "Add_location..."},
            // Mabini
            {"Anilao Proper", "Poblacion", "San Juan", "Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Add_location..."},
            // Malvar
            {"Poblacion", "Bagong Pook", "Bilucao", "Bulihan", "Luta del Norte", "Luta del Sur", "San Andres", "San Isidro", "San Juan", "San Pioquinto", "Add_location..."},
            // Mataasnakahoy
            {"Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Poblacion 8", "Poblacion 9", "Poblacion 10", "Add_location..."},
            // Nasugbu
            {"Bilaran", "Aga", "Wawa", "Bucana", "Cale", "Calayo", "Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Add_location..."},
            // Padre Garcia
            {"Poblacion", "Banaba", "Malaquitan", "Bucal", "Bancoro", "San Miguel", "San Felipe", "San Rafael", "Quilo-Quilo", "Quilo-Quilo Norte", "Add_location..."},
            // Rosario
            {"Quilib", "Poblacion A", "Poblacion B", "Poblacion C", "Poblacion D", "Poblacion E", "Poblacion F", "Poblacion G", "Poblacion H", "Poblacion I", "Add_location..."},
            // San Jose
            {"Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Poblacion 8", "Poblacion 9", "Poblacion 10", "Add_location..."},
            // San Juan
            {"Pinagbayanan", "Calicanto", "Hugom", "Laiya Ibaba", "Laiya Ilaya", "Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Add_location..."},
            // San Luis
            {"Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Poblacion 8", "Poblacion 9", "Poblacion 10", "Add_location..."},
            // San Nicolas
            {"Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Poblacion 8", "Poblacion 9", "Poblacion 10", "Add_location..."},
            // San Pascual
            {"Bayanan", "Poblacion", "San Antonio", "San Mateo", "San Miguel", "San Rafael", "San Roque", "San Simon", "San Vicente", "San Ysidro", "Add_location..."},
            // Santa Teresita
            {"Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Poblacion 8", "Poblacion 9", "Poblacion 10", "Add_location..."},
            // Santo Tomas
            {"San Roque", "San Felix", "San Antonio", "San Pedro", "San Miguel", "San Vicente", "San Juan", "San Rafael", "San Bartolome", "San Blas", "Add_location..."},
            // Taal
            {"Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Poblacion 8", "Poblacion 9", "Poblacion 10", "Add_location..."},
            // Talisay
            {"Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Poblacion 8", "Poblacion 9", "Poblacion 10", "Add_location..."},
            // Tanauan
            {"Darasa", "Sambat", "Baloc", "Talaga", "Malaking Pulo", "Luyos", "Bagumbayan", "Maugat", "Ulango", "Poblacion 1", "Add_location..."},
            // Taysan
            {"Poblacion West", "Poblacion East", "Poblacion North", "Poblacion South", "Tilambo", "Tugtug", "San Isidro", "San Luis", "San Jose", "San Rafael", "Add_location..."},
            // Tingloy
            {"Poblacion 1", "Poblacion 2", "Poblacion 3", "Poblacion 4", "Poblacion 5", "Poblacion 6", "Poblacion 7", "Poblacion 8", "Poblacion 9", "Poblacion 10", "Add_location..."},
            // Tuy
            {"Barangay 1", "Akle", "Bolbok", "Bayudbod", "Luna", "Putol", "Guinhawa"," Mataywanac", "Magahis", "Toong", "Add_location..."}
        },
        // 4. Rizal barangays
        {
            // Antipolo
            {"Cupang", "San Isidro", "San Jose", "Mayamot", "Mambugan", "Dalig", "Santa Cruz", "Calawis", "Inarawan", "Bagong Nayon", "Add_location..."},
            // Angono
            {"San Vicente", "Mahabang Parang", "San Isidro", "Bagumbayan", "San Roque", "Santo Niño", "Tagpos", "Poblacion Ibaba", "Poblacion Itaas", "Kalayaan", "Add_location..."},
            // Baras
            {"San Jose", "San Salvador", "Rizal", "Santiago", "Evangelista", "Pinugay", "Barangay I (Pob.)", "Barangay II (Pob.)", "Barangay III (Pob.)", "Barangay IV (Pob.)", "Add_location..."},
            // Binangonan
            {"Darangan", "Mahabang Parang", "Bilibiran", "Calumpang", "Batingan", "Ithan", "Pila-Pila", "Wawa", "Palangoy", "Pag-asa", "Add_location..."},
            // Cainta
            {"San Isidro", "San Andres (Pob.)", "Santo Niño", "San Juan", "San Roque", "Santa Rosa", "Cainta (Pob.)", "Add_location..."},
            // Cardona
            {"Dalig", "Pila-Pila", "Sampad", "Poblacion", "San Roque", "Iglesia", "Looc", "Del Remedio", "Calahan", "Kalinawan", "Add_location..."},
            // Jala-Jala
            {"Bagumbong", "Bagong Silang", "Bayugo", "Lubo", "Pag-asa", "Pulo", "First District (Pob.)", "Second District (Pob.)", "Third District (Pob.)", "Fourth District (Pob.)", "Add_location..."},
            // Morong
            {"Lagundi", "Bombongan", "Caniogan-Calero-Lanang (CCL)", "San Pedro (Pob.)", "Maybancal", "San Guillermo", "Linga", "San Jose (Pob.)", "Add_location..."},
            // Pililla
            {"Malaya", "Niogan", "Quisao", "Hulo (Pob.)", "Ilog (Pob.)", "Bagong Nayon (Pob.)", "Wawa (Pob.)", "Hulo", "Niogan", "Add_location..."},
            // Rodriguez
            {"San Jose", "San Isidro", "Manggahan", "Burgos", "Balite", "Geronimo", "Macabud", "Mascap", "Maysawa", "San Rafael", "Add_location..."}
        },
        // 5. Quezon barangays
        {
            // Lucena
            {"Ilayang Dupay", "Ibabang Dupay", "Bantigue", "Dalahican", "Mayao Crossing", "Isabang", "Domoit", "Cotta", "Ibabang Iyam", "Ilayang Iyam", "Add_location..."},
            // Tayabas
            {"Angustias Zone I", "Angustias Zone II", "Angustias Zone III", "Angustias Zone IV", "Angeles Zone I (Pob.)", "Angeles Zone II", "Angeles Zone III", "Angeles Zone IV", "San Vicente", "Alitao", "Add_location..."},
            // Sariaya
            {"Balubal", "Concepcion Banahaw", "Concepcion Pinagbakuran", "Gibanga", "Guinayangan", "Janagdong I", "Janagdong II", "Mamala I", "Mamala II", "Manggalang I", "Add_location..."},
            // Candelaria
            {"Malabanban Norte", "Malabanban Sur", "Mangilag Norte", "Mangilag Sur", "Masalukot I", "Masalukot II", "Masalukot III", "Masalukot IV", "Masalukot V", "Masin Norte", "Add_location..."},
            // Lucban
            {"Abang", "Ayunan", "Barangay 1 (Pob.)", "Barangay 2 (Pob.)", "Barangay 3 (Pob.)", "Barangay 4 (Pob.)", "Barangay 5 (Pob.)", "Barangay 6 (Pob.)", "Barangay 7 (Pob.)", "Barangay 8 (Pob.)", "Add_location..."},
            // Tiaong
            {"Lalig", "Lusacan", "Poblacion 3", "Poblacion 4", "San Antonio", "San Isidro", "San Juan", "San Pedro", "Tagbakin", "Talisay", "Add_location..."},
            // Sampaloc
            {"Banot", "Bayongon (Pob.)", "Bilucao", "Caldong", "Ibabang Owain (Pob.)", "Ilayang Owain (Pob.)", "Mamala", "San Bueno", "San Isidro (Pob.)", "San Roque (Pob.)", "Add_location..."},
            // San Antonio
            {"Poblacion", "Buliran", "Callejon", "Corazon", "Del Valle", "Loob", "Magsaysay", "Matipunso", "Niing", "Pulo", "Add_location..."},
            // Padre Burgos
            {"Poblacion", "San Roque", "Sipa", "Marquez", "Rizal", "Hinguiwin", "Kinagunan Ibaba", "Kinagunan Ilaya", "Lapu-lapu", "Lipunan", "Add_location..."},
            // General Nakar
            {"Poblacion", "Anoling", "Banglos", "Batangan", "Canaway", "Kinalin", "Lavides", "Mabog", "Maligaya", "Mapu", "Add_location..."}
        },
        //6
        {
            {" Add location "}
        }
    };

    // Display province 
    cout << "\nSelect your Province in CALABARZON (1-5), if province is not included in he list enter [6] :" << endl;
    for (size_t i = 0; i < provinces.size(); ++i) {
        cout << (i + 1) << "-" << provinces[i] << endl;
    }

    int provChoice = -1;
    int input;
    while (provChoice < 1 || provChoice > static_cast<int>(provinces.size())) {
        cout << "Enter the number of your province : ";
        cin >> input;
        provChoice = input;

        cin.ignore(numeric_limits<streamsize> :: max(), '\n');
    }

    if (provChoice == provinces.size()) { 
        cout << "Specify your province name that is not included in the list : ";
        if (userInfo == "address") {
            getline(cin, user.address.province);
        } else {
            getline(cin, user.birthplace.province);
        }
    } else {
        if (userInfo == "address") {
            user.address.province = provinces[provChoice - 1];
        } else {
            user.birthplace.province = provinces[provChoice - 1];
        }
    }
    clearscreen();

    // Display municipality 
    if (userInfo == "address" ) {
        cout << "Province : " << user.address.province << endl;
    } else if (userInfo == "birthplace" ) {
        cout << "Province : " << user.birthplace.province << endl;
    }
    cout << "\nSelect your Municipality : " <<  endl;
    const vector<string>& selectedMunicipalities = municipalities[provChoice - 1];
    for (size_t i = 0; i < selectedMunicipalities.size(); ++i) {
        cout << (i + 1) << "-" << selectedMunicipalities[i] << endl;
    }

    int munChoice = -1;
    while (munChoice < 1 || munChoice > static_cast<int>(selectedMunicipalities.size())) {
        cout << "Enter the number for your municipality: ";
        string input;
        getline(cin, input);
        try {
            munChoice = stoi(input);
        } catch (...) {
            cout << "Invalid input. Please enter a number." << endl;
        }
    }
    
    if (munChoice == selectedMunicipalities.size()) { // If user selected "Others..."
        cout << "Specify your province name that is not included in the list : ";
        if (userInfo == "address") {
            getline(cin, user.address.municipality);
        } else {
            getline(cin, user.birthplace.municipality);
        }
    } else {
        if (userInfo == "address") {
            user.address.municipality = selectedMunicipalities[munChoice - 1];
        } else {
            user.birthplace.municipality = selectedMunicipalities[munChoice - 1];
        }
    }

    clearscreen();
    
    // Display barangay 
    if (userInfo == "address" ) {
        cout << "Province : " << user.address.province << endl;
        cout << "Municipality : "<< user.address.municipality << endl;
    } else if (userInfo == "birthplace" ) {
        cout << "Province : " << user.birthplace.province << endl;
        cout << "Municipality : " << user.birthplace.municipality << endl;
    }
    cout << "\nSelect your Barangay : " << endl;
    const vector<string> & selectedBarangays = barangays[provChoice - 1][munChoice - 1];
    for (size_t i = 0; i < selectedBarangays.size(); ++i) {
        cout << (i + 1) << "-" << selectedBarangays[i] << endl;
    }

    int barChoice = -1;
    while (barChoice < 1 || barChoice > static_cast<int>(selectedBarangays.size())) {
        cout << "Enter the number for your barangay: ";
        string input;
        getline(cin, input);
        try {
            barChoice = stoi(input);
        } catch (...) {
            cout << "Invalid input. Please enter a number." << endl;
        }
    }

    if (barChoice == selectedBarangays.size()) { // If user selected "Others..."
        cout << "Specify your province name that is not included in the list : ";
        if (userInfo == "address") {
            getline(cin, user.address.barangay);
        } else {
            getline(cin, user.birthplace.barangay);
        }
    } else {
        if (userInfo == "address") {
            user.address.barangay = selectedBarangays[barChoice - 1];
        } else {
            user.birthplace.barangay = selectedBarangays[barChoice - 1];
        }
    }
    clearscreen();
}

void updateDate () {
    int x=0;
    int index;
    string months[12] = { "January", "February", "March", "April","May","June", "July","August", "September", "October", "November", "December" };

    cout << "Enter the month : " << endl;
    for (int i = 0; i < 12 ; i++) {
        cout << "[" << x++ << "]" << months[i] << '\n';
    }
    cin >> index;
    user.birthdate.months =  months[index];

    do {
        cout << "Enter the day : " ;
        cin >> user.birthdate.day;

        if (user.birthdate.day > 31 || user.birthdate.day < 0) {
            cout << "ERROR !! Plese enter a correct day" << endl;
        }
    } while(user.birthdate.day > 31 || user.birthdate.day < 0);

    cout << "Enter the year : ";
    cin >> user.birthdate.year;

    cin.ignore(numeric_limits<streamsize> :: max(), '\n');
    clearscreen();
}

void deleteAccount() {
    x = 4;
    string password_checker;

    cout << "\nTo delete your account, you must first enter your password to confirm your identity " << endl;
    do {
        cout << "\nEnter your password: ";
        cin >> password;

        if ( file_checker(1) == true ) {

            data_rewrite( "delete_acc", user.srcode, "request"  );

            cout << "\nThank You !!" << endl;
            x = 0;
            break;
        } else {
            x--;
            if ( x > 0 ) {
                cout << "\nERROR : Incorrect password. Try again" << endl;
                cout <<  "Remaining attempt : " << x << endl;
            } else {
                cout << "!!! MAXIMUM ATTEMPT REACHED . RETURNING TO MENU !!!" << endl;
            }
        }
    }
    while (x > 0);
}

void changePassword() {
    string filename = user.srcode + "_data.txt";

    cout << "\nWelcome to Change Password area!" << endl;
    cout << "To change, you must first enter your current password to connfirm your identity." <<endl;

    do {
        cout << "\nEnter your Current Password: ";
        cin >> password;
        cin.ignore(numeric_limits<streamsize> ::max(), '\n');

        if (file_checker(1) == true) {
            string new_password;
            do {
                cout << "\n--- New Password Requirments ---" << endl;
                cout << "~ Your password must contain at least 8 characters." << endl;
                cout << "~ It must have at least one capital letter." << endl;
                cout << "~ It must have at least one small character." << endl;
                cout << "~ It must have at least one number." << endl;

                cout << "\nEnter your NEW Password: ";
                getline(cin, new_password);
                if (isStrongPassword(new_password)) {
                    break;
                } else {
                    cout << "\n!!! NEW PASSWORD DOES NOT MEET COMPLEXITY REQUIREMENTS. TRY AGAIN." << endl;
                }
            } while (true);

            ifstream inFile(filename);
            vector<string> fileLines;
            string line;

            while (getline(inFile, line)) {
                fileLines.push_back(line);
            }
            inFile.close();

            if (fileLines.size() > 1) {
                fileLines[1] = crypto.encrypt(new_password);
                fileLines[20] = '\n';
            } else {
                cout << "\nERROR: Could not find existing password line in file to update." << endl;
                return;
            }

            ofstream outFile(filename, ios::trunc);
            if  (outFile.is_open()) {
                for (const string& modifiedLine : fileLines) {
                    outFile << modifiedLine << "\n";
                }
                outFile.close();
                cout << "\nPASSWORD CHANGED SUCCESSFULLY!!" << endl;
            } else {
                cerr << "\nERROR: Could not open file for writing. Password was not saved." << endl;
            }
            break;
        } else {
            x--;
            if (x > 0) {
                cout << "\nPASSWORD IS INCORRECT! TRY AGAIN!!" << endl;
                cout << "YOur remaining attempt(s): " << x << endl;
            } else {
                cout << "\n!!! TOO MANY FAILED ATTEMPTS. RETURNING TO MAIN MENU. !!!" << endl;
                return;
            }
        }
    } while (x > 0);
}

void grantDefaultPassword(const string& srcode) {
    const string defaultPass = "Password123";
    string filename = srcode + "_data.txt";
    string encryptedPass = crypto.encrypt(defaultPass);

    ifstream inFile(filename);
    vector<string> fileLines;
    string line;

    if (!inFile.is_open()) {
        cerr << "\nERROR: Could not open student data file: " << filename << endl;
        return;
    }

    while (getline(inFile, line)) {
        fileLines.push_back(line);
    }
    inFile.close();

    if (fileLines.size() > 1) {
        fileLines[1] = encryptedPass;
        fileLines[20] = "approved!!" ;

        ofstream outFile(filename, ios::trunc);
        if  (outFile.is_open()) {
            for (const string& modifiedLine : fileLines) {
                outFile << modifiedLine << "\n";
            }
            outFile.close();
            cout << "\n--- SUCCESS ---" << endl;
            cout << "Password for SR-Code " << srcode << " changed to default: " << defaultPass << endl;
            cout << "NOTE: The user must log in and change this default password immediately." << endl;
        } else {
            cerr << "\nERROR: Could not open file for writing. Password was not saved." << endl;
        }
    } else {
        cerr << "\nERROR: Student data file is corrupted or too short for password update." << endl;
    }
}

void showReport(string filename) {
    ifstream inFile(filename);
    vector<string> lines;
    string line;
    int maxDisplay = 5;

    if (!inFile.is_open()) {
        cout << "NOTICE: The report file '" << filename << "' is currently empty or does not exist." << endl;
        return;
    }

    while (getline(inFile, line)) {
        stringstream ss;
        ss << line;
        string trimmed_line;
        if (ss >> trimmed_line) { 
            lines.push_back(line);
        }
    }
    inFile.close();

    if (lines.empty()) {
        cout << "\nNOTICE: There are no pending requests in the '" << filename << "' file." << endl;
        return;
    }

    cout << "Showing up to " << maxDisplay << " requests (Total: " << lines.size() << "):" << endl;

    vector<string> srCodesToDisplay;
    for (size_t i = 0; i < min((size_t)maxDisplay, lines.size()); ++i) {
        string currentLine = lines[i];

        size_t startPos = currentLine.find("] ");
        size_t endPos = currentLine.find(" = ");
        string srcodeExtracted = "UNKNOWN_SRCODE";

        if (startPos != string::npos && endPos != string::npos && endPos > startPos + 2) {
            srcodeExtracted = currentLine.substr(startPos + 2, endPos - (startPos + 2));
        } else {
            srcodeExtracted = currentLine;
        }

        srCodesToDisplay.push_back(srcodeExtracted);
        cout << "[" << i + 1 << "] SR-Code: " << srcodeExtracted << endl;
        cout << "    Request: " << currentLine << endl;
    }

    cout << "\n================ ADMIN ACTION ==============" << endl;
    cout << "Enter the **number** of the SR-Code to grant a default password to (1-" << min((int)lines.size(), maxDisplay) << "):" << endl;
    cout << "Enter [0] to cancel and go back: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice >= 1 && choice <= min((int)lines.size(), maxDisplay)) {
        string targetSrCode = srCodesToDisplay[choice - 1];
        grantDefaultPassword(targetSrCode);

        lines.erase(lines.begin() + (choice - 1));

        ofstream outFile(filename, ios::trunc);
        if (outFile.is_open()) {
            for (const string& remainingLine : lines) {
                outFile << remainingLine << "\n";
            }
            outFile.close();
            cout << "\nNOTICE: Remaining requests have been saved. The processed request was removed." << endl;
        } else {
            cerr << "\nERROR: Failed to rewrite the report file! The request may show up again." << endl;
        }
    } else if (choice == 0) {
        cout << "\nAction cancelled. Returning to Admin Menu." << endl;
    } else {
        cout << "\nInvalid choice. Returning to Admin Menu." << endl;
    }
}

void data_rewrite(string filefor, string data, string report) {
    string filename = filefor + "File.txt";

    ofstream outFile (filename, ios::app);
    if (outFile.is_open()) {
        outFile << "[" << filefor << "] " << data << " = " << report <<  '\n';
        cout << "\nNOTICE : Request sent!" << endl;
        if(filefor == "delete_acc") {
            cout << "Your request to delete this account has been logged in for admin approval " << endl;
        }
    } else {
        cerr << "ERROR : Unable to open the file " << endl;
    }
}

bool processNextSystemReport(string filename) {
    ifstream inFile(filename);
    vector<string> lines;
    string line;
    int maxDisplay = 5;

    if (!inFile.is_open()) {
        cout << "NOTICE! : The system report file '" << filename << "' is currently empty or does not exist." << endl;
        return false;
    }

    while (getline(inFile, line)) {
        stringstream ss;
        ss << line;
        string trimmed_line;
        if (ss >> trimmed_line) {
            lines.push_back(line);
        }
    }
    inFile.close();

    if (lines.empty()) {
        cout << "NOTICE: There are no pending system error reports remaining in the file." << endl;
        return false;
    }

    cout << "\nTotal pending reports: " << lines.size() << endl;

    vector<string> srCodeReports;
    for (size_t i = 0; i < min((size_t)maxDisplay, lines.size()); ++i) {
        string currentLine = lines[i];

        size_t contentPos = currentLine.find(" = ");
        string srCodeExtracted = "N/A";
        string reportDetails = "No details found.";

        if (contentPos != string::npos) {
            reportDetails = currentLine.substr(contentPos + 3);
            size_t startPos = currentLine.find("] ");
            if (startPos != string::npos && startPos < contentPos) {
                srCodeExtracted = currentLine.substr(startPos + 2, contentPos - (startPos + 2));
            }
        }
        srCodeReports.push_back(srCodeExtracted); 

        cout << "[" << i + 1 << "] From : " << srCodeExtracted << endl;
        cout << "  Report: " << reportDetails << endl;
        cout << "------------------------------------------------------------------" << endl;
    }

    cout << "\n================ ADMIN ACTION (Checklist) ==============" << endl;
    cout << "Enter the number of the report to mark as *FIXED/DONE* and remove (1-" << min((int)lines.size(), maxDisplay) << "):" << endl;
    cout << "Enter [0] to cancel and go back: ";

    int choice;
    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nInvalid input. Action cancelled." << endl;
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice >= 1 && choice <= min((int)lines.size(), maxDisplay)) {
        string resolvedReport = lines[choice - 1];
        lines.erase(lines.begin() + (choice - 1));

        ofstream outFile(filename, ios::trunc);
        if (outFile.is_open()) {
            for (const string& remainingLine : lines) {
                outFile << remainingLine << "\n";
            }
            outFile.close();
            cout << "\n\n--- REPORT RESOLVED ---" << endl;
            cout << "The report was marked as fixed and removed from the list." << endl;
            cout << "Processed entry: " << resolvedReport << endl;
            cout << "-----------------------------" << endl;
            return true; 
        } else {
            cerr << "\nERROR! : Failed to rewrite the report file! The report was not removed." << endl;
            return false;
        }
    } else if (choice == 0) {
        cout << "\nAction cancelled. Returning to Admin Menu." << endl;
        return false;
    } else {
        cout << "\nInvalid choice. Please enter a valid number from the list or 0." << endl;
        return false;
    }
}

void showReport_SystemErrors(string filename) {
    int continue_process = 1;

    while (continue_process == 1) {
        bool reportProcessed = processNextSystemReport(filename);

        if (reportProcessed) {
            cout << "\nProcess another report? [1] Yes | [2] No (Go back to Admin Menu): ";
            int loopChoice;
            if (!(cin >> loopChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue_process = 2;
            } else {
                continue_process = loopChoice;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearscreen();
        } else {
            continue_process = 2;
        }
    }
}

void showReport_DeleteRequests(string filename) {
    vector<string> lines;
    string line;
    string targetSrCode;
    int choice;
    int index = 1;
    int actionChoice = 0;
    int continue_process = 1;

    do {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            while (getline(inFile, line)) {
                if (!line.empty()) {
                    lines.push_back(line);
                }
            }
            inFile.close();
        } else {
            cout << "NOTICE: Request file not found or cannot be opened." << endl;
            ofstream temp(filename);
        }

        cout << "\nPending Requests (" << lines.size() << "):" << endl;
        for (const string& req : lines) {
            cout << "[" << index++ << "] " << req << endl;
        }

        cout << "Enter the number of the request to process (or 0 to cancel): ";
        cin >> choice;

        if (choice > 0 && choice <= lines.size()) {
            string targetLine = lines[choice - 1];
            size_t start = targetLine.find(']') + 2;

            size_t end = targetLine.find('=', start) - 1;

            if (start != string::npos && end != string::npos && end > start) {
                targetSrCode = targetLine.substr(start, end - start);
            }

            if (targetSrCode.empty()) {
                cerr << "\nERROR: Failed to parse SR-Code from request line. Treating as processed and removing." << endl;
                lines.erase(lines.begin() + (choice - 1));
                actionChoice = 3; 
            } else {
                clearscreen();
                cout << "\nProcessing Request: " << targetLine << endl;
                cout << "Target SR-CODE: " << targetSrCode << endl; // Displaying the target variable
                cout << "\nDo you approve the permanent deletion of the file for " << targetSrCode << "?" << endl;
                cout << "[1] Approve & Delete User File | [2] Ignore Request & Keep in List | [3] Remove Request from List: ";
                cin >> actionChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if( actionChoice == 1 ) {
                    string userFilename = targetSrCode + "_data.txt";
                    
                    if (remove(userFilename.c_str()) == 0) {
                        cout << "\nSUCCESS: User file '" << userFilename << "' has been permanently DELETED." << endl;
                    } else  {
                        cout << "\nNOTICE: Could not find or delete user file '" << userFilename << "'. File may not exist or permission denied." << endl;
                    }
                    
                    lines.erase(lines.begin() + (choice - 1));
                } else if ( actionChoice == 2 ) {
                    cout << "\nNOTICE: Request for " << targetSrCode << " IGNORED. It will remain in the list." << endl;
                } else if ( actionChoice == 3 ) {
                    cout << "\nNOTICE: Request for " << targetSrCode << " has been REMOVED from the list without file deletion." << endl;
                    lines.erase(lines.begin() + (choice - 1));
                } else {
                    cout << "\nInvalid action choice. Returning to list." << endl;
                }
            }

            if (actionChoice == 1 || actionChoice == 3) {
                ofstream outFile(filename, ios::trunc); 
                if (outFile.is_open()) {
                    for (const string& remainingLine : lines) {
                        outFile << remainingLine << "\n";
                    }
                    outFile.close();

                    cout << "\nNOTICE: Remaining delete requests have been saved." << endl;
                    cout << "Redirecting back,..." << endl;
                    cout << "DO NOT PRESS ANYTHING" << endl;
                    this_thread::sleep_for(chrono::seconds(2));
                    clearscreen();
                    continue_process = 1;
                } else {
                    cerr << "\nERROR: Failed to rewrite the report file! The request may show up again." << endl;
                }
            }
        } else if (choice == 0) {
            cout << "\nAction cancelled. Returning to Admin Menu." << endl;
            continue_process = 2;
        } else {
            cout << "\nInvalid choice. Returning to Admin Menu." << endl;
            continue_process = 2;
        }

    }
    while (continue_process == 1 );
}


