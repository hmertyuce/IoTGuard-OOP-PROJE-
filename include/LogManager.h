#pragma once
#include "SecurityEvent.h"
#include <fstream>
#include <iostream>
#include <mutex>

class LogManager {
private:
    static std::mutex logMutex;
    static const std::string LOG_FILE;

public:
    static void saveLogToFile(const SecurityEvent& event) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::ofstream file(LOG_FILE, std::ios::app);
        if (file.is_open()) {
            file << event.toString() << "\n";
        }
    }

    static void writeSystemHeader(const std::string& header) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::ofstream file(LOG_FILE, std::ios::app);
        if (file.is_open()) {
            file << "\n=========================================\n";
            file << header << "\n";
            file << "=========================================\n";
        }
    }
};

// C++11 fallback instead of inline static
#ifdef LOG_MANAGER_IMPL
std::mutex LogManager::logMutex;
const std::string LogManager::LOG_FILE = "security_logs.txt";
#endif
