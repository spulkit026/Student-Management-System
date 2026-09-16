#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
using namespace std;

class SMS {
    string name;
    string id;
    string mobile_number;
    string course;
    short year;
    string state;
    static int count;

public:
    int no_of_students() {

        ifstream file("students.csv");
        string line;
        int student_count = 0;

        getline(file, line);

        while (getline(file, line)) {
            student_count++;
        }

        file.close();
        return student_count;
    }

    void create() {
        cout << "Enter the name of the student: ";
        cin >> name;

        cout << "Enter the ID of the student: ";
        cin >> id;
        cout << "Enter the mobile number of the student: ";
        do{
        cin >> mobile_number;
        if(mobile_number.length()!=10){
        cout<<"Enter a 10-digit mobile number";}

       }
        while(mobile_number.length()!=10);

        cout << "Enter the course of the student: ";
        cin >> course;
        cout << "Enter the year of the student: ";
        do{
            cin >> year;
            if(year<1 or year>4)
                cout<<"Enter a valid year";
        }
        while(year<1 or year>4);

        cout << "Enter the state of the student: ";
        cin >> state;

        count++;
    }

    void display() {
        cout << "\nStudent Details\n";
        cout << "Name : " << name << endl;
        cout << "ID : " << id << endl;
        cout << "Mobile Number : " << mobile_number << endl;
        cout << "Course : " << course << endl;
        cout << "Year : " << year << endl;
        cout << "City : " << state << endl;
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

        // Write header only once
        if (!fileExists) {
            file << "Name,ID,MobileNumber,Course,Year,City\n";
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

   void delete_from_csv() {
    cout << "Enter the ID of the student to delete: ";
    string delete_id;
    cin >> delete_id;

    ifstream file("students.csv");
    ofstream temp("temp.csv");

    string line;
    bool found = false;

    getline(file, line);
    temp << line << endl;

    while (getline(file, line)) {
        if (line.find("," + delete_id + ",") == string::npos) {
            temp << line << endl;
        } else {
            found = true;
        }
    }

    file.close();
    temp.close();

    if (found) {
        remove("students.csv");
        rename("temp.csv", "students.csv");
        cout << "Student record deleted successfully." << endl;
        count--;
    } else {
        remove("temp.csv");
        cout << "Student record not found." << endl;
    }
}
void display_allrecord(){
        ifstream file("students.csv");
        if(!file){
            cout<<"Error: File Not Found!\n";
        }
        string line;
        cout << "\n===== Student Records =====\n";
        while (getline(file, line)) {
        cout << line << endl;
        }
        file.close();

    }

    // Updated: repeatedly asks which field to update until the user chooses to exit
    void update() {
        cout << "Enter the ID of the student to update: ";
        string update_id;
        cin >> update_id;

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
        string target_line;

        int idx = 0;
        while (getline(file, line)) {
            if (line.find("," + update_id + ",") != string::npos) {
                found = true;
                target_index = idx;
                target_line = line;
            }
            lines.push_back(line);
            idx++;
        }
        file.close();

        if (!found) {
            cout << "Student record not found." << endl;
            return;
        }

        // Parse the matched record into individual fields
        vector<string> fields;
        stringstream ss(target_line);
        string field;
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }
        // fields[0]=Name, 1=ID, 2=MobileNumber, 3=Course, 4=Year, 5=City

        int choice;
        do {
            cout << "\nCurrent Student Details:\n";
            cout << "1. Name           : " << fields[0] << endl;
            cout << "2. ID             : " << fields[1] << endl;
            cout << "3. Mobile Number  : " << fields[2] << endl;
            cout << "4. Course         : " << fields[3] << endl;
            cout << "5. Year           : " << fields[4] << endl;
            cout << "6. City           : " << fields[5] << endl;
            cout << "7. Done updating (save and exit)\n";
            cout << "Enter the field number you want to update: ";
            cin >> choice;

            switch (choice) {
            case 1:
                cout << "Enter new name: ";
                cin >> fields[0];
                break;

            case 2:
                cout << "Enter new ID: ";
                cin >> fields[1];
                break;

            case 3: {
                string new_mobile;
                do {
                    cout << "Enter new mobile number (10 digits): ";
                    cin >> new_mobile;
                    if (new_mobile.length() != 10)
                        cout << "Enter a 10-digit mobile number\n";
                } while (new_mobile.length() != 10);
                fields[2] = new_mobile;
                break;
            }

            case 4:
                cout << "Enter new course: ";
                cin >> fields[3];
                break;

            case 5: {
                short new_year;
                do {
                    cout << "Enter new year (1-4): ";
                    cin >> new_year;
                    if (new_year < 1 || new_year > 4)
                        cout << "Enter a valid year\n";
                } while (new_year < 1 || new_year > 4);
                fields[4] = to_string(new_year);
                break;
            }

            case 6:
                cout << "Enter new city: ";
                cin >> fields[5];
                break;

            case 7:
                cout << "Saving changes and exiting update menu...\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
            }

        } while (choice != 7);

        // Reconstruct the updated CSV line and write everything back
        string updated_line = fields[0] + "," + fields[1] + "," + fields[2] + "," +
                               fields[3] + "," + fields[4] + "," + fields[5];
        lines[target_index] = updated_line;

        ofstream temp("students.csv");
        temp << header << endl;
        for (auto &l : lines) {
            temp << l << endl;
        }
        temp.close();

        cout << "Student record updated successfully." << endl;
    }

void search() {
    cout << "\nHow do you want to search?\n";
    cout << "1. Search with ID\n";
    cout << "2. Search with Name\n";
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;
    cin.ignore(); // clears the newline before getline()

    string query;
    if (choice == 1) {
        cout << "Enter the ID: ";
    } 
    else if (choice == 2) {
        cout << "Enter the name: ";
    } 
    else {
        cout << "Invalid choice!\n";
        return;
    }

    getline(cin, query);

    ifstream file("students.csv");
    if (!file) {
        cout << "Error: File Not Found!\n";
        return;
    }

    string line;
    getline(file, line); // skip header row

    bool found = false;

    while (getline(file, line)) {
        stringstream ss(line);

        string csv_name, csv_id, csv_mobile, csv_course;
        string csv_year, csv_state;

        getline(ss, csv_name, ',');
        getline(ss, csv_id, ',');
        getline(ss, csv_mobile, ',');
        getline(ss, csv_course, ',');
        getline(ss, csv_year, ',');
        getline(ss, csv_state, ',');

        bool match = (choice == 1 && csv_id == query) ||
                     (choice == 2 && csv_name == query);

        if (match) {
            cout << "\n----- Student Found -----\n";
            cout << "Name          : " << csv_name << endl;
            cout << "ID            : " << csv_id << endl;
            cout << "Mobile Number : " << csv_mobile << endl;
            cout << "Course        : " << csv_course << endl;
            cout << "Year          : " << csv_year << endl;
            cout << "State          : " << csv_state << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "Student record not found.\n";
    }

    file.close();
}
};

int SMS::count = 0;

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
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            S1.create();
            S1.save_to_csv();
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