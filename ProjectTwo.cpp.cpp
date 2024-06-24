#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Class reps a course
class Course {
private:
    string id;
    string title;
    vector<string> prerequisites;

public:
    Course(const string& id, const string& title) : id(id), title(title) {}

    void addPrerequisite(const string& prereq) {
        prerequisites.push_back(prereq);
    }

    const string& getId() const {
        return id;
    }

    const string& getTitle() const {
        return title;
    }

    const vector<string>& getPrerequisites() const {
        return prerequisites;
    }
};

// Compares function for sorting courses by title
bool compareCourses(const Course& c1, const Course& c2) {
    return c1.getId() < c2.getId();
}

// Class reps a collection of courses
class CourseCatalog {
private:
    vector<Course> courses;

public:
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Error opening file: " + filename);
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string courseId, courseTitle, temp;
            getline(ss, courseId, ',');
            getline(ss, courseTitle, ',');
            Course course(courseId, courseTitle);
            while (getline(ss, temp, ',')) {
                course.addPrerequisite(temp);
            }
            courses.push_back(course);
        }
        file.close();
    }

    void printAll() const {
        for (const auto& course : courses) {
            cout << course.getId() << ", " << course.getTitle() << endl;
        }
    }

    void printCourse(const string& courseId) const {
        auto it = find_if(courses.begin(), courses.end(), [&courseId](const Course& c) {
            return c.getId() == courseId;
        });
        if (it != courses.end()) {
            cout << "Course Title: " << it->getTitle() << endl;
            cout << "Prerequisites: ";
            const auto& prerequisites = it->getPrerequisites();
            for (size_t i = 0; i < prerequisites.size(); ++i) {
                cout << prerequisites[i];
                if (i < prerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        } else {
            cout << "Course with ID '" << courseId << "' not found." << endl;
        }
    }

    void printComputerScienceCourses() const {
        vector<Course> computerScienceCourses;
        for (const auto& course : courses) {
            if (course.getId().substr(0, 2) == "CS" || course.getId().substr(0, 4) == "MATH") {
                computerScienceCourses.push_back(course);
            }
        }
        sort(computerScienceCourses.begin(), computerScienceCourses.end(), compareCourses);
        cout << "Computer Science Course List:" << endl;
        for (const auto& course : computerScienceCourses) {
            cout << course.getId() << ", " << course.getTitle() << endl;
        }
    }
};

// Function to print menu options
void printMenu() {
    cout << "\nMenu:" << endl;
    cout << " 1. Load Data Structure from File." << endl;
    cout << " 2. Print Alphanumeric List of Computer Science Courses." << endl;
    cout << " 3. Print Course Information and Prerequisites." << endl;
    cout << " 9. Exit Program." << endl;
}

int main() {
    CourseCatalog catalog;
    bool loaded = false;
    int choice;

    do {
        printMenu();
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string filename;
            cout << "Enter the name of the data file to load: ";
            cin.ignore();
            getline(cin, filename);
            try {
                catalog.loadFromFile(filename);
                loaded = true;
                cout << "Data loaded successfully." << endl;
            } catch (const exception& e) {
                cout << "Error loading data: " << e.what() << endl;
            }
            break;
        }
        case 2:
            if (!loaded) {
                cout << "Please load data (Option 1) before printing course lists." << endl;
            } else {
                catalog.printComputerScienceCourses();
            }
            break;
        case 3: {
            if (!loaded) {
                cout << "Please load data (Option 1) before printing course info." << endl;
            } else {
                string courseId;
                cout << "Enter the course ID to show info: ";
                cin >> courseId;
                catalog.printCourse(courseId);
            }
            break;
        }
        case 9:
            cout << "Exiting the program!" << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a valid option (1, 2, 3, or 9)." << endl;
            break;
        }
    } while (choice != 9);

    return 0;
}
