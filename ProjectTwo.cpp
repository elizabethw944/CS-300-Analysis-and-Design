// Name: ProjectTwo.cpp 
// Author: Elizabeth Hardegen 
// 4/16/2026

#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <fstream> // For reading file
#include <sstream> // For parsing CSV lines
#include <set> // Stores valid course numbers


using namespace std;

// To store course information
struct Course {
    string courseNumber; // Stores course ID
    string courseTitle; // Stores course title
    vector<string> prerequisites; // Stores prerequisite course numbers
};

// Node structure for the binary search tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // Constructor with course
    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Class for binary search tree
class BinarySearchTree {
private:
    Node* root; // Root node of tree

    // Function to insert a node into the tree
    Node* insertNode(Node* root, Course course) {
        // If root is empty, create new node
        if (root == nullptr) {
            return new Node(course);
        }

        // Compare course numbers to decide where to insert
        if (course.courseNumber < root->course.courseNumber) {
            // Insert into left side
            root->left = insertNode(root->left, course);
        }
        else {
            // Insert into right side
            root->right = insertNode(root->right, course);
        }

        return root; // Return updated node
    }

    // Function to print courses in Alphanumeric order
    void printInOrder(Node* root) {
        // Only continue if current node is not empty
        if (root != nullptr) {
            printInOrder(root->left); // Visit left subtree
            cout << root->course.courseNumber << ", "
                << root->course.courseTitle << endl; // Print current course
            printInOrder(root->right); // Visit right
        }
    }

    // Helper Function to find course in tree, used to look up prerequisite titles
    Node* findCourse(Node* root, string courseNumber) {
        // if root is empty, course not found
        if (root == nullptr) {
            return nullptr;
        }

        // if course number matches, return this node
        if (courseNumber == root->course.courseNumber) {
            return root;
        }

        // if course number is smaller, search left
        if (courseNumber < root->course.courseNumber) {
            return findCourse(root->left, courseNumber);
        }
        // otherwise search right
        else {
            return findCourse(root->right, courseNumber);
        }
    }
     
    
    // Function to search for a course
    void searchTree(Node* root, string courseNumber) {
        // If root is empty, course not found
        if (root == nullptr) {
            cout << "Course not found." << endl;
            return;
        }

        // If course is found, print
        if (courseNumber == root->course.courseNumber) {
            cout << root->course.courseNumber << ", "
                << root->course.courseTitle << endl;

            cout << "Prerequisites: ";

            // Check if there are no prerequisites
            if (root->course.prerequisites.empty()) {
                cout << "None" << endl;
            }
            else {
                // loop through each prerequisite
                for (int i = 0; i < root->course.prerequisites.size(); i++) {
                    string prereqNumber = root->course.prerequisites[i];

                    // Find prerequisite course in tree
                    Node* prereqNode = findCourse(this->root, prereqNumber);

                    // If found, print number and title
                    if (prereqNode != nullptr) {
                        cout << prereqNode->course.courseNumber << ", "
                            << prereqNode->course.courseTitle;
                    }
                    else {
                        // if not found, just print number
                        cout << prereqNumber;
                    }

                    // Add separator between prerequisites
                    if (i < root->course.prerequisites.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;
            }
            return;
        }

        // If courseNumber is smaller, search left
        else if (courseNumber < root->course.courseNumber) {
            searchTree(root->left, courseNumber);
        }
        // Else search right
        else {
            searchTree(root->right, courseNumber);
        }
    }

public:
    // Constructor
    BinarySearchTree() {
        root = nullptr;
    }

    // Insert function
    void insert(Course course) {
        root = insertNode(root, course);
    }

    // Load data from file in to binary search tree
    void loadCoursesToTree(string fileName) {
        // Open file
        ifstream file(fileName);

        // Check if file opened correctly
        if (!file.is_open()) {
            cout << "Error: Unable to open file." << endl;
            return;
        }

        string line;
        set<string> courseNumbers;

        // First pass: collect all course numbers
        // Read each line in file
        while (getline(file, line)) {
            // Skip blank lines
            if (line.empty()) {
                continue;
            }

            stringstream ss(line);
            string value;
            vector<string> values;

            // Split line by comma
            while (getline(ss, value, ',')) {
                values.push_back(value);
            }

            // Check for valid course data 
            if (values.size() < 2) {
                cout << "Invalid file format." << endl;
                file.close();
                return;
            }

            // store the course number
            courseNumbers.insert(values[0]);
        }

        // clear file errors and go back to beginning
        file.clear();
        file.seekg(0);

        // Second pass: load courses and validate any prerequisites
        while (getline(file, line)) {
            // Skip blank lines
            if (line.empty()) {
                continue;
            }

            stringstream ss(line);
            string value;
            vector<string> values;

            // Split line by comma
            while (getline(ss, value, ',')) {
                values.push_back(value);
            }

            // Check for valid course data
            if (values.size() < 2) {
                cout << "Invalid file format." << endl;
                file.close();
                return;
            }

            // Create course object
            Course course;
            course.courseNumber = values[0];
            course.courseTitle = values[1];

            // Check and add prerequisites
            for (int i = 2; i < values.size(); i++) {
                // Only add prerequisites if it exists in file
                if (courseNumbers.find(values[i]) != courseNumbers.end()) {
                    course.prerequisites.push_back(values[i]);
                }

            }

            // Insert course into tree
            root = insertNode(root, course);
        }
        
        // Close File
        file.close();
    }

    // Function to print all courses
    void printInOrder() {
        printInOrder(root);
    }

    // Function to start search
    void searchTree(string courseNumber) {
        searchTree(root, courseNumber);
    }

    // Getter for the root
    Node* getRoot() {
        return root;
    }
};

// Function to display menu
void displayMenu() {
    cout << endl;
    cout << "Menu" << endl;
    cout << " 1. Load Data Structure" << endl;
    cout << " 2. Print Course List" << endl;
    cout << " 3. Display Course Information" << endl;
    cout << " 9. Exit" << endl;
    cout << endl;
}

int main() {
    // Create object for binary search tree
    BinarySearchTree courseTree;

    string fileName;
    // Ask user for file name
    cout << "Enter file name: ";
    getline(cin, fileName);

    string courseNumber;
    int choice = 0;
    bool dataLoaded = false;

    // show menu until user exits
    while (choice != 9) {
        displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        // Load data into tree
        if (choice == 1) {
            cout << "Loading data into Binary Search Tree..." << endl;
            courseTree.loadCoursesToTree(fileName);
            dataLoaded = true;
            cout << "Data loaded successfully." <<
                endl;
        }
        
        // Print course list
        else if (choice == 2) {
            if (dataLoaded) {
                cout << endl;
                cout << "Course List:" << endl;
                courseTree.printInOrder();
            }
            else {
                cout << "No data structure loaded." << endl;
            }
        }

        // Search and Print course
        else if (choice == 3) {
            if (dataLoaded) {
                cout << "Enter course number: ";
                cin >> courseNumber;
                cout << endl;
                courseTree.searchTree(courseNumber);
            }
            else {
                cout << "No data structure loaded." << endl;
            }
        }
        // Exit program
        else if (choice == 9) {
            cout << "Exiting Program." << endl;
        }

        // Invalid option
        else {
            cout << "Invalid option. Please try again." << endl;
        }

    }

    return 0;
}

