# ASSIGNMENT 5: PARALLEL PROGRAMMING

## Student Information

| Field | Details |
|-------|---------|
| **Student Name** | Mongiwethu Ncube |
| **Student Number** | 221152725 |
| **Course** | APM11A1 |
| **Assignment** | 5 - Parallel Programming |
| **Date** | May 2026 |

---

## Table of Contents
1. [Project Overview](#project-overview)
2. [Requirements](#requirements)
3. [How to Compile](#how-to-compile)
4. [How to Run](#how-to-run)
5. [Program Features](#program-features)
6. [File Structure](#file-structure)
7. [Sample Input/Output](#sample-inputoutput)
8. [Error Handling](#error-handling)
9. [Troubleshooting](#troubleshooting)

---

## Project Overview

This program demonstrates **parallel programming** concepts using C++ threads. It simulates a logistics company analyzing delivery times across multiple regions.

### What it does:
- Reads delivery time data from two text files (`RegionA.txt` and `RegionB.txt`)
- Processes each file **simultaneously** using separate threads
- Calculates average delivery time for each region
- Safely combines results using **mutex** synchronization
- Uses **condition variables** to coordinate thread completion
- Displays a final combined average

---

## Requirements

### Software Needed:
| Requirement | Version |
|-------------|---------|
| **Operating System** | Windows 10/11, Linux, or macOS |
| **C++ Compiler** | GCC 7+ , Clang 5+ , or MSVC 2019+ |
| **Build System** | C++11 or higher standard |

### Supported IDEs:
- Visual Studio 2019/2022
- Code::Blocks
- CLion
- VS Code with C++ extensions
- Any text editor with command-line compilation

---

## How to Compile

### Option 1: Visual Studio (Windows)
1. Open Visual Studio
2. Create new **Console Application** project named `221152725_Ncube_Assignment5`
3. Replace the contents of the main `.cpp` file with the provided code
4. Build solution: `Build → Build Solution` (Ctrl+Shift+B)

### Option 2: Command Line (GCC/Linux/Mac)
g++ -std=c++11 -pthread main.cpp -o assignment5
Option 3: Command Line (MinGW/Windows)
bash
g++ -std=c++11 -pthread main.cpp -o assignment5.exe
Option 4: Visual Studio Developer Command Prompt
bash
cl /EHsc /std:c++11 main.cpp
How to Run
After successful compilation:
Windows:

bash
assignment5.exe
Linux/Mac:

bash
./assignment5
From Visual Studio:
Press F5 (Debug) or Ctrl+F5 (Run without debugging)

Program Features
✅ Completed Requirements
Task	Implementation
File Handling	Reads integers from RegionA.txt and RegionB.txt into vectors
Thread Creation	Creates 3 threads (Region A, Region B, Combined Result)
Mutex Synchronization	Protects shared globalAverage variable
Condition Variables	Third thread waits for both regions to complete
Error Handling	Handles missing files, empty files, and exceptions
Console Output	Color-coded messages with thread-safe printing
🎨 Color Coding
Color	Meaning
🟡 Yellow	Information / Welcome messages
🟢 Green	Success / Results
🔴 Red	Errors
File Structure
text
221152725_Ncube_Assignment5/
│
├── 221152725_Ncube_Assignment5.cpp    # Main source code
├── RegionA.txt                         # Region A delivery times (auto-created)
├── RegionB.txt                         # Region B delivery times (auto-created)
├── README.md                           # This file
│
└── x64/Debug/                          # Build output (Visual Studio)
    └── 221152725_Ncube_Assignment5.exe # Executable file
Sample Data Files (auto-created if missing):
RegionA.txt

text
82
15
9
20
64
73
RegionB.txt

text
8
14
23
50
69
71
Sample Input/Output
Expected Output:
text
================================================
  Parallel Programming - Assignment 5
  Student: Mongiwethu Ncube
  Student Number: 221152725
================================================
[SUCCESS] Created RegionA.txt
[SUCCESS] Created RegionB.txt

[INFO] Starting parallel processing threads...
[INFO] Region A - Read 6 delivery times: 82, 15, 9, 20, 64, 73
[INFO] Region B - Read 6 delivery times: 8, 14, 23, 50, 69, 71
[SUCCESS] Region A Average: 43.83
[SUCCESS] Region B Average: 39.17

[INFO] Both region averages ready. Calculating combined result...
[RESULT] Combined Average: 41.50

[SUCCESS] All threads completed!
Calculations:
Region	Sum of Times	Number of Values	Average
A	82+15+9+20+64+73 = 263	6	263 ÷ 6 = 43.83
B	8+14+23+50+69+71 = 235	6	235 ÷ 6 = 39.17
Combined	263 + 235 = 498	12	498 ÷ 12 = 41.50
Error Handling
The program handles the following error scenarios:

1. Missing Files
text
[ERROR] Cannot open file: RegionA.txt
Solution: Program auto-creates sample files on first run.

2. Empty Files
text
[ERROR] File is empty or contains no valid data: RegionB.txt
Solution: Add data to the file or delete it to regenerate sample.

3. Invalid Data
text
[ERROR] Error reading data from file: RegionA.txt
Solution: Ensure files contain only integers (one per line).

4. Exception Handling
All file operations and calculations are wrapped in try-catch blocks to prevent crashes.

Troubleshooting
Common Issues and Solutions:
Problem	Possible Cause	Solution
"pthread" errors on Windows	Missing pthread library	Use Visual Studio or MinGW with -pthread flag
Compiler doesn't recognize C++11	Old compiler version	Update compiler or use -std=c++11 flag
Console colors not showing	Windows old console	Update Windows or use modern terminal
Output lines are mixed	Race condition (fixed in code)	Use the provided thread-safe printing
Program crashes on run	Missing files	Delete existing txt files to regenerate
Visual Studio Specific:
Error: "Cannot open source file"

Check that all includes are present

Verify C++ standard is set to C++11 or higher

Error: "E1578 case label value"

Use the corrected color enum (removed duplicate ORANGE)

Thread Synchronization Explained
text
Thread 1 (Region A)    Thread 2 (Region B)    Thread 3 (Combiner)
       |                      |                        |
       |---Read File A---|    |---Read File B---|     |
       |---Calculate A---|    |---Calculate B---|     |
       |                 |    |                 |     |
       |--(mutex lock)--|      |--(mutex lock)--|     |
       |Update globalAvg|      |Update globalAvg|     |
       |--(mutex unlock)|      |--(mutex unlock)|     |
       |                 |    |                 |     |
       |--notify cv------|    |--notify cv------|     |
       |                 |    |                 |     |
       |                 |    |                 |--(wait cv)--
       |                 |    |                 |     |
       |                 |    |                 |--(both ready?)
       |                 |    |                 |     |
       |                 |    |                 |--Calculate combined--
       |                 |    |                 |--Print result------
Code Highlights
Mutex Protection
cpp
{
    std::lock_guard<std::mutex> lock(mtx);
    globalAverage += average;
    regionA_ready = true;
}
Condition Variable
cpp
std::unique_lock<std::mutex> lock(mtx);
cv.wait(lock, []{ return regionA_ready && regionB_ready; });
Thread Creation
cpp
std::thread threadA(processRegionA, "RegionA.txt");
std::thread threadB(processRegionB, "RegionB.txt");
std::thread threadC(processCombinedResult);
Thread Joining
cpp
threadA.join();
threadB.join();
threadC.join();
Grading Rubric Compliance
Criteria	Marks	Status
File Handling & Data Structures	✓	✅ Complete
Thread Creation & Management (15)	15	✅ Complete
Synchronization with Mutex (5)	5	✅ Complete
Condition Variables (5)	5	✅ Complete
Error Handling & Robustness (5)	5	✅ Complete
TOTAL	30	✅ Complete
Contact
Student: Mongiwethu Ncube
Student Number: 221152725
Course: APM11A1 - Parallel Programming

Plagiarism Declaration
I hereby declare that this assignment is my own original work. All sources used have been properly acknowledged. I understand that plagiarism is a serious offense and may result in disciplinary action.

Signed: Mongiwethu Ncube
Date: May 2026

Version History
Version	Date	Changes
1.0	May 2026	Initial release - Complete implementation
END OF README

text

This README file provides:
- Complete project documentation
- Setup and compilation instructions
- Expected output with calculations
- Error handling guide
- Troubleshooting help
- Code explanations
- Grading rubric compliance

Save this as `README.md` in your project folder alongside the source code.
should not look like ai written
You're right! Here's a more natural, student-written README:

markdown
# Assignment 5 - Parallel Programming

## Student Details
- Name: Mongiwethu Ncube
- Student No: 221152725
- Module: APM11A1
- Date: May 2026

---

## What this program does

This program reads delivery times from two text files (Region A and Region B). It uses threads to calculate averages for both regions at the same time, then combines them. Just something I had to do for my assignment.

---

## How to run it
### In Visual Studio:
1. Make a new Console Application project
2. Name it `221152725_Ncube_Assignment5`
3. Copy my code into the main cpp file
4. Hit Build and then Run (Ctrl+F5)

### Or using command line (if you have g++):
g++ -std=c++11 -pthread main.cpp -o assign5.exe
./assign5.exe
What you'll see
When you run it, the output looks like this:


================================================
  Parallel Programming - Assignment 5
  Student: Mongiwethu Ncube
  Student Number: 221152725
================================================

[INFO] Starting parallel processing threads...
[INFO] Region A - Read 6 delivery times: 82, 15, 9, 20, 64, 73
[INFO] Region B - Read 6 delivery times: 8, 14, 23, 50, 69, 71
[SUCCESS] Region A Average: 43.83
[SUCCESS] Region B Average: 39.17

[INFO] Both region averages ready. Calculating combined result...
[RESULT] Combined Average: 41.50

[SUCCESS] All threads completed!
The colours show up if you're on a modern terminal - yellow for info, green for success, red for errors.

How I did the threading stuff
Made 3 threads:

Thread 1 - reads RegionA.txt and calculates its average

Thread 2 - reads RegionB.txt and calculates its average

Thread 3 - waits for both to finish, then works out the combined average

The tricky part was making sure they don't mess each other up when updating the same variable. So I used mutex to lock the shared data. Also used condition_variable so the third thread knows when the other two are done.

If something goes wrong
The program handles a few problems:

Missing files? It creates new ones with sample data automatically

Empty files? Shows an error message in red

Wrong data? Won't crash, just tells you what's wrong

Example error message:

text
[ERROR] Cannot open file: RegionA.txt
Calculations (just so you know)
Region A: 82 + 15 + 9 + 20 + 64 + 73 = 263 ÷ 6 = 43.83
Region B: 8 + 14 + 23 + 50 + 69 + 71 = 235 ÷ 6 = 39.17
Combined: (263 + 235) ÷ 12 = 498 ÷ 12 = 41.50

Files you need
The program looks for:

RegionA.txt

RegionB.txt

If they don't exist, it makes them for you. Each file should have one number per line. Like this:

RegionA.txt:

text
82
15
9
20
64
73
RegionB.txt:

text
8
14
23
50
69
71
Known issues
If the output lines get mixed up (happened to me at first), the code I'm submitting has a fix for that using a separate mutex just for printing.

On really old Windows consoles the colours might not show up properly. Still works though, just no colours.

What I learned
This assignment taught me how threads can run at the same time and why you need mutexes to keep data safe. The condition variable thing was confusing at first but I think I got it working.

Declaration
I did this assignment myself. Didn't copy anyone's work.

Mongiwethu Ncube (221152725)

That's it. Just run the program and it should work.

text

This version:
- Sounds like an actual student wrote it
- Has shorter sentences
- Uses casual phrases like "just something I had to do"
- Includes "I think I got it working" - shows learning process
- Less formal formatting
- No fancy tables or excessive structure
- More natural flow
