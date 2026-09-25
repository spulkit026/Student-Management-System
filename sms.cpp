#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <algorithm>
using namespace std;



string readLine(const string &prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

string readNonEmptyLine(const string &prompt) {
    string value;
    do {
        value = readLine(prompt);
        if (value.empty()) {
            cout << "This field cannot be empty. Please try again.\n";
        }
    } while (value.empty());
    return value;
}

string readMobileNumber(const string &prompt) {
    string value;
    while (true) {
        value = readLine(prompt);
        bool allDigits = !value.empty() &&
            all_of(value.begin(), value.end(), [](unsigned char c) { return isdigit(c); });
        if (allDigits && value.length() == 10) {
            return value;
        }
        cout << "Enter a valid 10-digit mobile number (digits only).\n";
    }
}


short readYear(const string &prompt) {
    short value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number between 1 and 4.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // consume trailing newline
        if (value < 1 || value > 4) {
            cout << "Enter a valid year (1-4).\n";
            continue;
        }
        return value;
    }
}

int readMenuChoice(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}

vector<string> splitCsvLine(const string &line) {
    vector<string> fields;
    stringstream ss(line);
    string field;
    while (getline(ss, field, ',')) {
        fields.push_back(field);
    }
    return fields;
}

class SMS {
    string name;
    string id;
    string mobile_number;
    string course;
    short year = 0;
    string state;

public:
    int no_of_students() {
        ifstream file("students.csv");
        if (!file) return 0;

        string line;
        int student_count = 0;

        getline(file, line); // skip header

        while (getline(file, line)) {
            if (!line.empty()) student_count++;
        }

        file.close();
        return student_count;
    }


    bool id_exists(const string &check_id) {
        ifstream file("students.csv");
        if (!file) return false;

        string line;
        getline(file, line); // skip header

        while (getline(file, line)) {
            if (line.empty()) continue;
            vector<string> fields = splitCsvLine(line);
            if (fields.size() > 1 && fields[1] == check_id) {
                return true;
            }
        }
        return false;
    }

    bool create() {
        name = readNonEmptyLine("Enter the name of the student: ");

        while (true) {
            id = readNonEmptyLine("Enter the ID of the student: ");
            if (id_exists(id)) {
                cout << "A student with ID \"" << id << "\" already exists. Please enter a different ID.\n";
                continue;
            }
            break;
        }

        mobile_number = readMobileNumber("Enter the mobile number of the student: ");
        course = readNonEmptyLine("Enter the course of the student: ");
        year = readYear("Enter the year of the student (1-4): ");
        state = readNonEmptyLine("Enter the state of the student: ");

        return true;
    }

    void display() {
        cout << "\nStudent Details\n";
        cout << "Name : " << name << endl;
        cout << "ID : " << id << endl;
        cout << "Mobile Number : " << mobile_number << endl;
        cout << "Course : " << course << endl;
        cout << "Year : " << year << endl;
        cout << "State : " << state << endl;
    }

    void save_to_csv() {
        ifstream checkFile("students.csv");
        bool fileExists = checkFile.good();
        checkFile.close();

        ofstream file("students.csv", ios::app);
        if (!file) {
            cout << "Unable to open CSV file." << endl;
            return;
        }

        if (!fileExists) {
            file << "Name,ID,MobileNumber,Course,Year,State\n";
        }

        file << name << ","
             << id << ","
             << mobile_number << ","
             << course << ","
             << year << ","
             << state << "\n";

        file.close();
        cout << "Student record saved successfully to students.csv" << endl;
    }

    // Fix: exact field match on ID instead of substring search.
    void delete_from_csv() {
        string delete_id = readNonEmptyLine("Enter the ID of the student to delete: ");

        ifstream file("students.csv");
        if (!file) {
            cout << "Error: File Not Found!\n";
            return;
        }

        ofstream temp("temp.csv");
        string line;
        bool found = false;

        getline(file, line); // header
        temp << line << "\n";

        while (getline(file, line)) {
            if (line.empty()) continue;
            vector<string> fields = splitCsvLine(line);
            if (fields.size() > 1 && fields[1] == delete_id) {
                found = true; // skip writing this line
            } else {
                temp << line << "\n";
            }
        }

        file.close();
        temp.close();

        if (found) {
            remove("students.csv");
            rename("temp.csv", "students.csv");
            cout << "Student record deleted successfully." << endl;
        } else {
            remove("temp.csv");
            cout << "Student record not found." << endl;
        }
    }

    void display_allrecord() {
        ifstream file("students.csv");
        if (!file) {
            cout << "Error: File Not Found!\n";
            return;
        }
        string line;
        cout << "\n===== Student Records =====\n";
        while (getline(file, line)) {
            if (!line.empty()) cout << line << endl;
        }
        file.close();
    }

    // Fix: exact field match on ID instead of substring search.
    void update() {
        string update_id = readNonEmptyLine("Enter the ID of the student to update: ");

        ifstream file("students.csv");
        if (!file) {
            cout << "Error: File Not Found!\n";
            return;
        }

        string header;
        getline(file, header);

        vector<string> lines;
        string line;
        bool found = false;
        int target_index = -1;

        int idx = 0;
        while (getline(file, line)) {
            if (line.empty()) continue;
            vector<string> fields = splitCsvLine(line);
            if (fields.size() > 1 && fields[1] == update_id) {
                found = true;
                target_index = idx;
            }
            lines.push_back(line);
            idx++;
        }
        file.close();

        if (!found) {
            cout << "Student record not found." << endl;
            return;
        }

        vector<string> fields = splitCsvLine(lines[target_index]);
   

        int choice;
        do {
            cout << "\nCurrent Student Details:\n";
            cout << "1. Name           : " << fields[0] << endl;
            cout << "2. ID             : " << fields[1] << endl;
            cout << "3. Mobile Number  : " << fields[2] << endl;
            cout << "4. Course         : " << fields[3] << endl;
            cout << "5. Year           : " << fields[4] << endl;
            cout << "6. State          : " << fields[5] << endl;
            cout << "7. Done updating (save and exit)\n";
            choice = readMenuChoice("Enter the field number you want to update: ");

            switch (choice) {
            case 1:
                fields[0] = readNonEmptyLine("Enter new name: ");
                break;

            case 2: {
                string new_id;
                while (true) {
                    new_id = readNonEmptyLine("Enter new ID: ");
                    if (new_id != update_id && id_exists(new_id)) {
                        cout << "A student with ID \"" << new_id << "\" already exists.\n";
                        continue;
                    }
                    break;
                }
                fields[1] = new_id;
                break;
            }

            case 3:
                fields[2] = readMobileNumber("Enter new mobile number (10 digits): ");
                break;

            case 4:
                fields[3] = readNonEmptyLine("Enter new course: ");
                break;

            case 5:
                fields[4] = to_string(readYear("Enter new year (1-4): "));
                break;

            case 6:
                fields[5] = readNonEmptyLine("Enter new state: ");
                break;

            case 7:
                cout << "Saving changes and exiting update menu...\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
            }

        } while (choice != 7);

        string updated_line = fields[0] + "," + fields[1] + "," + fields[2] + "," +
                               fields[3] + "," + fields[4] + "," + fields[5];
        lines[target_index] = updated_line;

        ofstream temp("students.csv");
        temp << header << "\n";
        for (auto &l : lines) {
            temp << l << "\n";
        }
        temp.close();

        cout << "Student record updated successfully." << endl;
    }

    void search() {
        cout << "\nHow do you want to search?\n";
        cout << "1. Search with ID\n";
        cout << "2. Search with Name\n";
        int choice = readMenuChoice("Enter your choice: ");

        string query;
        if (choice == 1) {
            query = readNonEmptyLine("Enter the ID: ");
        } else if (choice == 2) {
            query = readNonEmptyLine("Enter the name: ");
        } else {
            cout << "Invalid choice!\n";
            return;
        }

        ifstream file("students.csv");
        if (!file) {
            cout << "Error: File Not Found!\n";
            return;
        }

        string line;
        getline(file, line); // skip header

        bool found = false;

        while (getline(file, line)) {
            if (line.empty()) continue;
            vector<string> f = splitCsvLine(line);
            if (f.size() < 6) continue;

            bool match = (choice == 1 && f[1] == query) ||
                         (choice == 2 && f[0] == query);

            if (match) {
                cout << "\n----- Student Found -----\n";
                cout << "Name          : " << f[0] << endl;
                cout << "ID            : " << f[1] << endl;
                cout << "Mobile Number : " << f[2] << endl;
                cout << "Course        : " << f[3] << endl;
                cout << "Year          : " << f[4] << endl;
                cout << "State         : " << f[5] << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "Student record not found.\n";
        }

        file.close();
    }
};

int main() {
    SMS S1;
    int choice;

    do {
        cout << "\n========== STUDENT MANAGEMENT SYSTEM ==========\n";
        cout << "1. Add Student\n";
        cout << "2. Display Last Entered Student\n";
        cout << "3. Update Student Record\n";
        cout << "4. Delete Student Record\n";
        cout << "5. Show Number of Students\n";
        cout << "6. Display All Student Records\n";
        cout << "7. Search Student\n";
        cout << "8. Exit\n";
        choice = readMenuChoice("Enter your choice: ");

        switch (choice) {
        case 1:
            if (S1.create()) {
                S1.save_to_csv();
            }
            break;

        case 2:
            S1.display();
            break;

        case 3:
            S1.update();
            break;

        case 4:
            S1.delete_from_csv();
            break;

        case 5:
            cout << "Number of students in students.csv: "
                 << S1.no_of_students() << endl;
            break;

        case 6:
            S1.display_allrecord();
            break;

        case 7:
            S1.search();
            break;

        case 8:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 8);

    return 0;
}