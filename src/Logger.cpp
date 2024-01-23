//
// Created by hmoua on 10/12/2023.
//

#include "../includes/Logger.hpp"

#include <ctime>
#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>

Logger::Logger() {
    // Create a unique filename based on the current date and time
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = {};
    errno_t err = localtime_s(&now_tm, &now_time_t);
    if (err) {
        std::cerr << "Logger Time Conversion Error" << std::endl;
    }
    std::stringstream filenameStream;
    filenameStream << "logs/log_" << std::put_time(&now_tm, "%Y-%m-%d_%H-%M-%S") << ".log";

    // Create logs directory if it does not exist
    std::filesystem::create_directories("logs");
    // Open the file
    logFile.open(filenameStream.str(), std::ios::out | std::ios::app);
}

void Logger::log(const std::string &message, LogLevel level) {
    std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety

    // Get the current time and format it
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = {};
    errno_t err = localtime_s(&now_tm, &now_time_t);
    if (err) {
        std::cerr << "Logger Time Conversion Error" << std::endl;
    }
    std::stringstream timeStream;
    timeStream << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");

    // Construct the log message
    std::string formattedMessage = "[" + timeStream.str() + "][" + getLevelString(level) + "] " + message;

    // Log to file and console
    if (logFile.is_open()) {
        logFile << formattedMessage << std::endl;
    }
    std::cout << formattedMessage << std::endl;
}

std::string Logger::getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::LOG_ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

