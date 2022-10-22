//============================================================================
// Name        : Project2.cpp
// Author      : Sulochana pradhan
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Binary search tree implementation of CS department course and prerequisites
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================
string stringToUpper(string str);

// define a structure to hold course information
struct Course {
	string courseId; // unique identifier
	string title;
	vector<string> prerequisites;
};

// Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course c) :
		Node() {
		course = c;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class CourseTree {

private:
	Node* root;

	void addNode(Node* node, Course course);
	void inOrderTraverse(Node* node);
	void destroyRecursive(Node* node);

public:
	CourseTree();
	virtual ~CourseTree();
	void InOrderTraverse();
	void Insert(Course course);
	Course Find(string courseId);
};

/**
 * Default constructor
 */
CourseTree::CourseTree() {
	//root is equal to nullptr
	root = nullptr;
}

/**
 * Destructor
 */
CourseTree::~CourseTree() {
	// recurse from root deleting every node
	destroyRecursive(root);
}

/**
 * Recursively destroy all tree pointers
 */
void CourseTree::destroyRecursive(Node* node) {
	if (node)
	{
		// Destroys left children
		destroyRecursive(node->left);
		// Destroys right children
		destroyRecursive(node->right);
		// Destroys node
		delete node;
	}
}

/**
 * Traverse the tree in order
 * This will automatically traverse the tree in sorted order.
 * There is no need to to do extra sorting.
 */
void CourseTree::InOrderTraverse() {
	cout << "Here is the sample schedule: " << endl << endl;
	// call inOrderTraverse fuction and pass root 
	inOrderTraverse(root);
}


/**
 * Insert a course
 */
void CourseTree::Insert(Course course) {

	// If there is not any roo, make new one
	if (root == nullptr) {
		root = new Node(course);
	}
	// Call addNode private function
	else {
		addNode(root, course);
	}
}


/**
 * Search for a course
 */
Course CourseTree::Find(string courseId) {

	Course course;
	// Set current to root
	Node* current = root;
	// Iterate while current is not null
	while (current != nullptr) {
		string currentCourseId = stringToUpper(current->course.courseId);
		// Return course if found
		if (current->course.courseId.compare(courseId) == 0) {
			return current->course;
		}
		// If course id is less than current, go to left child
		else if (courseId.compare(current->course.courseId) < 0) {
			current = current->left;
		}
		// Else, go to right child
		else {
			current = current->right;
		}
	}

	return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void CourseTree::addNode(Node* node, Course course) {
	// If current course id is smaller than course to insert, go to left subbree
	if (node->course.courseId.compare(course.courseId) > 0) {
		// if left child is null, add new course
		if (node->left == nullptr) {
			node->left = new Node(course);
		}
		// Recursively call addNode to insert at proper location
		else {
			this->addNode(node->left, course);
		}
	}
	// Else if current course id is greater than or equal to course to insert, go to right subtree
	else {
		// if right child is null, add new course
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		// Recursively call addNode to insert at proper location
		else {
			this->addNode(node->right, course);
		}
	}
}

/**
* Recursive function to traverse the tree in in order. This will automatically traverse in ascending order.
*/
void CourseTree::inOrderTraverse(Node* node) {
	// Loop until node is not null
	if (node != nullptr) {
		// Recursively call inorder for left subtree
		inOrderTraverse(node->left);
		Course course = node->course;
		// Display current course
		cout << course.courseId << ": " << course.title << endl;
		// Recursively call inorder for right subtree
		inOrderTraverse(node->right);
	}
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
	cout << course.courseId << ": " << course.title << endl;
	// Also display the prerequisites
	cout << "Prerequisites: ";
	for (unsigned i = 0; i < course.prerequisites.size(); i++) {
		cout << course.prerequisites.at(i);
		// Print comma 
		if (i < course.prerequisites.size() - 1) {
			cout << ", ";
		}
	}
	cout << endl;
	return;
}

/**
* Skips the utf-8 byte order mark if presents. It is added by application itself.
* Credit: https://stackoverflow.com/questions/10417613/c-reading-from-file-puts-three-weird-characters
*/
void skipBOM(std::ifstream& in)
{
	char test[3] = { 0 };
	in.read(test, 3);
	// Make sure to check certain characters
	if ((unsigned char)test[0] == 0xEF &&
		(unsigned char)test[1] == 0xBB &&
		(unsigned char)test[2] == 0xBF)
	{
		return;
	}
	in.seekg(0);
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the file to load
 * @param bst the pointer to the binary search tree of courses
 * @return bool success or failure based on the data.
 */
bool loadCourses(string filePath, CourseTree* bst) {
	cout << "Loading course file " << filePath << endl;
	std::string line, word;
	// Read file
	std::ifstream ifile(filePath.c_str());
	skipBOM(ifile);
	// To hold all coures
	vector<string> courses;
	// To hold all prerequisites
	vector<string> prerequisites;
	// Flag to indicate if file has all valid data
	bool success = true;
	if (ifile.is_open())
	{
		// Read each line of file one by one
		while (ifile.good())
		{
			getline(ifile, line);
			// If line is empty, exit the loop
			if (line == "") {
				break;
			}
			else {
				// Create stream to split the string
				stringstream ss(line);
				vector<string> row;
				// read every column data of a row and store it in word variable
				while (getline(ss, word, ',')) {

					// add all the column data of a row to a vector
					row.push_back(word);
				}
				// We should have at least course id and title
				if (row.size() < 2) {
					success = false;
					cout << "Invalid course found." << endl;
					break;
				}
				else {
					courses.push_back(row.at(0));
					// Get all prerequisities of courses
					vector<string> currentPrerequisites;
					for (unsigned int i = 2; i < row.size(); i++) {
						currentPrerequisites.push_back(row.at(i));
						prerequisites.push_back(row.at(i));
					}

					// Create course
					Course c;
					c.courseId = row.at(0);
					c.title = row.at(1);
					c.prerequisites = currentPrerequisites;
					// Insert into our course tree
					bst->Insert(c);
				}
			}

		}
		// release file resource
		ifile.close();

		// Check if data is valid by checking if there is each prerequisite available
		for (unsigned int i = 0; i < prerequisites.size(); i++) {
			if (std::find(courses.begin(), courses.end(), prerequisites.at(i)) == courses.end()) {
				success = false;
				cout << "Invalid data as some prerequisite doesn't have information." << endl;
				break;
			}
		}

	}
	cout << "Loaded " << courses.size() << " courses" << endl;
	return success;
}

/**
* Converts string to uppercase
* Credit: https://stackoverflow.com/questions/23418390/how-to-convert-a-c-string-to-uppercase
*/
string stringToUpper(string oString) {
	string uString = oString;
	for (unsigned int i = 0; i < oString.length(); i++) {
		uString[i] = toupper(oString[i]);
	}
	return uString;
}


/**
 * The one and only main() method
 * It expects optional file argument
 */
int main(int argc, char* argv[]) {

	// process command line arguments
	string csvPath, courseId;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		break;
	default:
		csvPath = "ABCU_Advising_Program_Input.txt";
	}

	// Define a binary search tree to hold all courses
	CourseTree* bst = new CourseTree();
	Course course;

	int choice = 0;
	while (choice != 4) {
		// Create main menu
		cout << endl;
		cout << "Menu:" << endl;
		cout << "  1. Load Courses" << endl;
		cout << "  2. Display All Courses" << endl;
		cout << "  3. Find Course" << endl;
		cout << "  4. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;
		// Make sure choice is number to avoid infinite loop
		if (!cin)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
			cout << "Invalid choice. ";
			continue;
		}
		switch (choice) {

		case 1:
			// Create new bst when user tries to load data again
			if (bst != nullptr) {
				delete bst;
			}
			bst = new CourseTree();
			loadCourses(csvPath, bst);
			break;

		case 2:
			// Prints the list of courses in sorted order
			bst->InOrderTraverse();
			break;

		case 3:
			cout << "Enter the course to find: ";
			cin >> courseId;
			// Try to find the course
			course = bst->Find(stringToUpper(courseId));
			if (!course.courseId.empty()) {
				// display course
				displayCourse(course);
			}
			else {
				cout << "Course Id " << courseId << " not found." << endl;
			}

			break;
		default:
			cout << choice << " is not a valid option." << endl << endl;
			break;
		}
	}

	cout << "Good bye." << endl;

	return 0;
}
