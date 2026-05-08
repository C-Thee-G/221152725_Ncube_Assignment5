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
```bash
g++ -std=c++11 -pthread main.cpp -o assign5.exe
./assign5.exe
```

---

## What you'll see

When you run it, the output looks like this:

```
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
```

The colours show up if you're on a modern terminal - yellow for info, green for success, red for errors.

---

## How I did the threading stuff

Made 3 threads:
- **Thread 1** - reads RegionA.txt and calculates its average
- **Thread 2** - reads RegionB.txt and calculates its average  
- **Thread 3** - waits for both to finish, then works out the combined average

The tricky part was making sure they don't mess each other up when updating the same variable. So I used `mutex` to lock the shared data. Also used `condition_variable` so the third thread knows when the other two are done.

---

## If something goes wrong

The program handles a few problems:

- **Missing files?** It creates new ones with sample data automatically
- **Empty files?** Shows an error message in red
- **Wrong data?** Won't crash, just tells you what's wrong

Example error message:
```
[ERROR] Cannot open file: RegionA.txt
```

---

## Calculations (just so you know)

Region A: 82 + 15 + 9 + 20 + 64 + 73 = 263 ÷ 6 = 43.83
Region B: 8 + 14 + 23 + 50 + 69 + 71 = 235 ÷ 6 = 39.17
Combined: (263 + 235) ÷ 12 = 498 ÷ 12 = 41.50

---

## Files you need

The program looks for:
- `RegionA.txt` 
- `RegionB.txt`

If they don't exist, it makes them for you. Each file should have one number per line. Like this:

RegionA.txt:
```
82
15
9
20
64
73
```

RegionB.txt:
```
8
14
23
50
69
71
```

---

## Known issues

If the output lines get mixed up (happened to me at first), the code I'm submitting has a fix for that using a separate mutex just for printing.

On really old Windows consoles the colours might not show up properly. Still works though, just no colours.

---

## What I learned

This assignment taught me how threads can run at the same time and why you need mutexes to keep data safe. The condition variable thing was confusing at first but I think I got it working.

---

## Declaration

I did this assignment myself. Didn't copy anyone's work. 
- Mongiwethu Ncube (221152725)

---
re
- More natural flow
