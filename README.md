# Student Management System (C++)

A console-based Student Management System built in C++ that stores student records
in a CSV file. It supports adding, viewing, updating, deleting, and searching for
student records through a simple menu-driven interface.

## Features

- **Add Student** — Enter student details (name, ID, mobile number, course, year, state) with input validation.
- **Display Last Entered Student** — View the details of the most recently added student in the current session.
- **Update Student Record** — Look up a student by ID and edit any individual field.
- **Delete Student Record** — Remove a student record by ID.
- **Show Number of Students** — Get the total count of student records on file.
- **Display All Student Records** — Print every record stored in the CSV file.
- **Search Student** — Search by ID or by name.

## Data Validation

- Mobile numbers must be exactly 10 digits.
- Year must be between 1 and 4.
- No two students can share the same ID (duplicate IDs are rejected on add and on update).
- Text fields (name, course, state) cannot be left empty and may contain spaces.
- Invalid/non-numeric menu or year input is caught and re-prompted instead of crashing or looping silently.

## Tech Stack

- **Language:** C++ (C++17)
- **Storage:** Plain text CSV file (`students.csv`), created automatically on first use
- **Libraries used:** Standard C++ library only (`iostream`, `fstream`, `sstream`, `vector`, `algorithm`, `limits`) — no external dependencies

## Project Structure

```
.
├── sms.cpp          # Main source code
├── students.csv     # Auto-generated data file (created when you add the first student)
└── README.md        # This file
```

## How to Compile and Run

### Requirements
- A C++ compiler that supports C++17 (e.g., `g++`)

### Compile
```bash
g++ -std=c++17 -Wall -Wextra -o sms Student_Management_System.cpp
```

### Run
```bash
./sms
```
On Windows, compile with the same command using MinGW/G++ and run `sms.exe`.

## Sample Menu

```
========== STUDENT MANAGEMENT SYSTEM ==========
1. Add Student
2. Display Last Entered Student
3. Update Student Record
4. Delete Student Record
5. Show Number of Students
6. Display All Student Records
7. Search Student
8. Exit
Enter your choice:
```

## Sample CSV Output (`students.csv`)

```
Name,ID,MobileNumber,Course,Year,State
Alice,S1,9876543210,CSE,2,Delhi
Bob,S2,9876500000,ECE,3,Kerala
```

## Possible Future Improvements

- Persist and validate data using a proper database (e.g., SQLite) instead of a flat CSV file.
- Add file-based logging of actions (create/update/delete) for auditing.
- Support partial/fuzzy name search instead of exact match.
- Add unit tests for the CSV parsing and validation logic.
- Build a simple GUI or web front end on top of the existing logic.

## Author

Pulkit Sharma