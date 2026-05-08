//============================================================================
// Student Name: Mongiwethu Ncube
// Student Number: 221152725
// Assignment: 5 - Parallel Programming
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <numeric>
#include <string>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

//--------------------------------------------------------------------------
// Console Colors
//--------------------------------------------------------------------------
enum Color {
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    RESET = 0
};

void setColor(Color color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int winColor;
    switch (color) {
    case RED: winColor = 12; break;
    case GREEN: winColor = 10; break;
    case YELLOW: winColor = 14; break;
    default: winColor = 7; break;
    }
    SetConsoleTextAttribute(hConsole, winColor);
#else
    std::cout << "\033[" << static_cast<int>(color) << "m";
#endif
}

void resetColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
#else
    std::cout << "\033[0m";
#endif
}

//--------------------------------------------------------------------------
// Global shared data with synchronization
//--------------------------------------------------------------------------
std::mutex mtx;                           // Mutex for shared data
std::mutex consoleMtx;                    // Mutex for console output (to prevent messy printing)
std::condition_variable cv;               // Condition variable for signaling
double globalAverage = 0.0;               // Shared variable (sum of averages)
bool regionA_ready = false;               // Flag for Region A completion
bool regionB_ready = false;               // Flag for Region B completion

//--------------------------------------------------------------------------
// Thread-safe console printing function
//--------------------------------------------------------------------------
void safePrint(const std::string& message, Color color = YELLOW) {
    std::lock_guard<std::mutex> lock(consoleMtx);
    setColor(color);
    std::cout << message;
    resetColor();
}

void safePrintLine(const std::string& message, Color color = YELLOW) {
    std::lock_guard<std::mutex> lock(consoleMtx);
    setColor(color);
    std::cout << message << std::endl;
    resetColor();
}

//--------------------------------------------------------------------------
// Function to read integers from a file into a vector
//--------------------------------------------------------------------------
bool readDeliveryTimes(const std::string& filename, std::vector<int>& data) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        safePrintLine("[ERROR] Cannot open file: " + filename, RED);
        return false;
    }

    int value;
    while (file >> value) {
        data.push_back(value);
    }

    if (data.empty()) {
        safePrintLine("[ERROR] File is empty or contains no valid data: " + filename, RED);
        return false;
    }

    file.close();
    return true;
}

//--------------------------------------------------------------------------
// Function to calculate average
//--------------------------------------------------------------------------
double calculateAverage(const std::vector<int>& data) {
    if (data.empty()) return 0.0;
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

//--------------------------------------------------------------------------
// Thread function for Region A
//--------------------------------------------------------------------------
void processRegionA(const std::string& filename) {
    std::vector<int> deliveryTimes;

    try {
        if (readDeliveryTimes(filename, deliveryTimes)) {
            double average = calculateAverage(deliveryTimes);

            // Build the info message
            std::string infoMsg = "[INFO] Region A - Read " + std::to_string(deliveryTimes.size()) + " delivery times: ";
            for (size_t i = 0; i < deliveryTimes.size(); ++i) {
                infoMsg += std::to_string(deliveryTimes[i]);
                if (i < deliveryTimes.size() - 1) infoMsg += ", ";
            }
            safePrintLine(infoMsg, YELLOW);

            // Update shared data with mutex
            {
                std::lock_guard<std::mutex> lock(mtx);
                globalAverage += average;
                regionA_ready = true;
            }

            // Display result
            std::string resultMsg = "[SUCCESS] Region A Average: " +
                std::to_string(average).substr(0, std::to_string(average).find(".") + 3);
            safePrintLine(resultMsg, GREEN);
        }
    }
    catch (const std::exception& e) {
        safePrintLine("[ERROR] Exception in Region A: " + std::string(e.what()), RED);
        std::lock_guard<std::mutex> lock(mtx);
        regionA_ready = true;
    }

    cv.notify_one();
}
//--------------------------------------------------------------------------
// Thread function for Region B
//--------------------------------------------------------------------------
void processRegionB(const std::string& filename) {
    std::vector<int> deliveryTimes;

    try {
        if (readDeliveryTimes(filename, deliveryTimes)) {
            double average = calculateAverage(deliveryTimes);

            // Build the info message
            std::string infoMsg = "[INFO] Region B - Read " + std::to_string(deliveryTimes.size()) + " delivery times: ";
            for (size_t i = 0; i < deliveryTimes.size(); ++i) {
                infoMsg += std::to_string(deliveryTimes[i]);
                if (i < deliveryTimes.size() - 1) infoMsg += ", ";
            }
            safePrintLine(infoMsg, YELLOW);

            // Update shared data with mutex
            {
                std::lock_guard<std::mutex> lock(mtx);
                globalAverage += average;
                regionB_ready = true;
            }

            // Display result
            std::string resultMsg = "[SUCCESS] Region B Average: " +
                std::to_string(average).substr(0, std::to_string(average).find(".") + 3);
            safePrintLine(resultMsg, GREEN);
        }
    }
    catch (const std::exception& e) {
        safePrintLine("[ERROR] Exception in Region B: " + std::string(e.what()), RED);
        std::lock_guard<std::mutex> lock(mtx);
        regionB_ready = true;
    }

    cv.notify_one();
}

//--------------------------------------------------------------------------
// Thread that waits for both regions
//--------------------------------------------------------------------------
void processCombinedResult() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return regionA_ready && regionB_ready; });

    double combinedAverage = globalAverage / 2.0;

    safePrintLine("\n[INFO] Both region averages ready. Calculating combined result...", YELLOW);

    std::string resultMsg = "[RESULT] Combined Average: " +
        std::to_string(combinedAverage).substr(0, std::to_string(combinedAverage).find(".") + 3);
    safePrintLine(resultMsg, GREEN);
}

//--------------------------------------------------------------------------
// Create sample files if they don't exist
//--------------------------------------------------------------------------
void createSampleFiles() {
    std::ifstream testA("RegionA.txt");
    std::ifstream testB("RegionB.txt");

    if (!testA.good()) {
        std::ofstream fileA("RegionA.txt");
        fileA << "82\n15\n9\n20\n64\n73\n";
        fileA.close();
        safePrintLine("[SUCCESS] Created RegionA.txt", GREEN);
    }
    if (!testB.good()) {
        std::ofstream fileB("RegionB.txt");
        fileB << "8\n14\n23\n50\n69\n71\n";
        fileB.close();
        safePrintLine("[SUCCESS] Created RegionB.txt", GREEN);
    }
    testA.close();
    testB.close();
}

//============================================================================
// Main
//============================================================================
int main() {
    // Display header
    setColor(YELLOW);
    std::cout << "================================================" << std::endl;
    std::cout << "  Parallel Programming - Assignment 5" << std::endl;
    std::cout << "  Student: Mongiwethu Ncube" << std::endl;
    std::cout << "  Student Number: 221152725" << std::endl;
    std::cout << "================================================" << std::endl;
    resetColor();

    createSampleFiles();
    std::cout << std::endl;

    // Reset state
    {
        std::lock_guard<std::mutex> lock(mtx);
        globalAverage = 0.0;
        regionA_ready = false;
        regionB_ready = false;
    }

    safePrintLine("[INFO] Starting parallel processing threads...", YELLOW);

    // Create and run threads
    std::thread threadA(processRegionA, "RegionA.txt");
    std::thread threadB(processRegionB, "RegionB.txt");
    std::thread threadC(processCombinedResult);

    // Wait for all threads to complete
    threadA.join();
    threadB.join();
    threadC.join();

    safePrintLine("\n[SUCCESS] All threads completed!", GREEN);

    return 0;
}