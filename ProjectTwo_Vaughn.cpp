//============================================================================
// Name        : ProjectTwo_Vaughn.cpp
// Author      : Victoria Vaughn
// Version     : 1.0
// Copyright   : Copyright � 2026
// Description : Project Two - ABCU Hash Table
//============================================================================

#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <string> // atoi

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================


// define a structure to hold bid information
struct CourseObject {
    string courseNumber; // unique identifier
    string courseTitle;
    vector<string> prerequisites;
};

//Helper function to convert into lowercase for sorting without case sensitivity
string ToLower(const string& str) {
    string out = str;
    transform(out.begin(), out.end(), out.begin(), ::tolower);
    return out;
}

vector<CourseObject> CourseTable; // Global vector to hold all courses read from the CSV file

//============================================================================
// ValidatePrerequisites Helper Function: Find course index by courseNumber
//============================================================================

int FindCourseIndex(const string& courseNumber) { // Returns the index of the course in CourseTable if found, otherwise returns -1
    for (size_t i = 0; i < CourseTable.size(); ++i) {
        if (ToLower(CourseTable[i].courseNumber) == ToLower(courseNumber)) { // Case-insensitive comparison
            return static_cast<int>(i);
        }
    }
    return -1;
}

//============================================================================
// ValidatePrerequisites function definition
//============================================================================

bool ValidatePrerequisites(const vector<CourseObject>& table) { // Validates that all prerequisites in the CourseTable exist in the table. Returns true if any prerequisite is invalid, otherwise false.
    bool failedValidation = false;

    for (const auto& course : table) { // Iterate through each course in the CourseTable
        for (const string& prereq : course.prerequisites) {
            if (FindCourseIndex(prereq) == -1) { // If the prerequisite course is not found in the CourseTable, print an error message and set failedValidation to true
                cout << "Error: Prerequisite not found (" << prereq << ") for course " 
                << course.courseNumber << endl;
                failedValidation = true;
            }
        }
    }
    return failedValidation; // Return true if any prerequisite is invalid, otherwise false
}

//============================================================================
// LoadCourses function definition
//============================================================================

int LoadCourses(const string& fileName) {
        ifstream file(fileName); // Open the file for reading

    if (!file.is_open()) { // Check if the file was opened successfully
        cout << "Error: Unable to open file." << endl;
        return 1; // Return 1 to indicate failure to open the file
    }

    string line;

    while (true) {
        if (!getline(file, line)) {
            break;
        }

        if (line.find_first_not_of(" \t\r\n") == string::npos) {
            continue;
        }

        // Split by comma
        stringstream ss(line);
        vector<string> tokens;
        string token;

        while (getline(ss, token, ',')) { // Split by comma
            // Trim whitespace
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            tokens.push_back(token);
        }

        if (tokens.size() < 2) { // Check if there are at least two tokens (course number and title)
            cout << "Error: Missing course number or title" << endl;
            continue;
        }

        string courseNumber = tokens[0];
        string courseTitle = tokens[1];

        vector<string> prerequisites; // Initialize an empty vector for prerequisites
        if (tokens.size() > 2) { // Check if there are prerequisites
            for (size_t i = 2; i < tokens.size(); i++)
                if (!tokens[i].empty()) {
                    prerequisites.push_back(tokens[i]);
                }
        }
        else {
            prerequisites = {}; //explicit empty vector if no prerequisites
        }

        CourseObject newCourse;
        newCourse.courseNumber = courseNumber;
        newCourse.courseTitle = courseTitle;
        newCourse.prerequisites = prerequisites;

        CourseTable.push_back(newCourse);
    }

    file.close(); // Close the file after reading

    bool failedValidation = ValidatePrerequisites(CourseTable);

    if (failedValidation) { // If validation fails, return 1 to indicate failure
        cout << "Critical Error: Course load unsuccessful due to invalid prerequisite course records." << endl;
        return 1;
    }

    return 0;
}

//============================================================================
// PrintCourseInformation function definition
//============================================================================

void PrintCourseInformation(const string& courseNumber) { // Prints the course information for a given course number, including its prerequisites
    int index = FindCourseIndex(courseNumber);

    if (index == -1) { // If course not found, print error message and return
        cout << "Error: Course not found (" << courseNumber << ")" << endl;
        return;
    }

    const CourseObject& course = CourseTable[index];

    cout << "Course Number: " << course.courseNumber << endl;
    cout << "Course Title: " << course.courseTitle << endl;

    if (!course.prerequisites.empty()) { //If statement added to check if the course has prerequisites before printing them
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1)
                cout << ", ";
        }
        cout << endl;
    } else { //Else statement added to handle courses with no prerequisites
        cout << "No Prerequisites" << endl;
    }
}

//============================================================================
// PrintSortCourses function definition
//============================================================================

void PrintSortCourses() { // Sorts the courses in alphabetical order by course number and prints them
    vector<string> sortedKeys;

    for (const auto& course : CourseTable) { // Collect all course numbers into a vector for sorting
        sortedKeys.push_back(course.courseNumber);
    }

    sort(sortedKeys.begin(), sortedKeys.end(), // Sort the course numbers in alphabetical order, ignoring case
        [](const string& a, const string& b) {
            return ToLower(a) < ToLower(b);
        });

    for (const string& key : sortedKeys) { // Print the sorted course numbers along with their corresponding course titles
        int index = FindCourseIndex(key);
        cout << key << "- " << CourseTable[index].courseTitle << endl;
    }
}

//============================================================================
// Main Program
//============================================================================

int main(int argc, char* argv[]) {
    bool fileLoaded = false; // Flag to track if the data structure has been loaded
    int userInput; // Variable to hold user input for menu selection
    string csvPath = "CS 300 ABCU_Advising_Program_Input.csv";

    if (argc > 0 && argv[0] != nullptr) {
        const string executablePath = argv[0];
        const size_t separator = executablePath.find_last_of("/\\");
        if (separator != string::npos) {
            csvPath = executablePath.substr(0, separator + 1) + csvPath;
        }
    }

    while (true) { // Main loop for user interaction
        cout << "Welcome to the Course Planner!" << endl;
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> userInput;

        switch (userInput) { // Switch statement to handle user input
            case 1: // Load Data Structure
                if (!fileLoaded) {
                    int status = LoadCourses(csvPath);
                     if (status == 0) {
                        fileLoaded = true;
                        cout << "Data structure loaded successfully." << endl;
                    } else {
                        cout << "Data structure failed to load." << endl;
                    }
                } else {
                    cout << "Data structure already loaded." << endl;
                }
                break;
            case 2: //Print Full Course List
                if (fileLoaded) {
                    cout << "Here is a sample schedule: " << endl;
                    PrintSortCourses();
                } else {
                    cout << "Please load the data structure first." << endl;
                }
                break;
            case 3: //Print User-defined Course Information
                if (fileLoaded) {
                    string courseNumber;
                    cout << "What course would you like to know about? ";
                    cin >> courseNumber;
                    PrintCourseInformation(courseNumber);
                } else {
                    cout << "Please load the data structure first." << endl;
                }
                break;
            case 9: //Exit
                cout << "Thank you for using the Course Planner! Exiting..." << endl;
                return 0;
            default: //Flags invalid input and prompts user to try again
                cout << userInput << " is not a valid option. Please try again." << endl;
        }
    }
}
