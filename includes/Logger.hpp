//
// Created by hmoua on 10/12/2023.
//

#ifndef WINDOWSHARDWAREMONITOR_LOGGER_HPP
#define WINDOWSHARDWAREMONITOR_LOGGER_HPP

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    Logger();
    enum class LogLevel {DEBUG, INFO, WARNING, LOG_ERROR, CRITICAL};
    void log(const std::string& message, LogLevel level);

private:
    std::ofstream logFile;
    std::mutex mtx;
    static std::string getLevelString(LogLevel level);
};

#endif //WINDOWSHARDWAREMONITOR_LOGGER_HPP
