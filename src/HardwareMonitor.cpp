//
// Created by hmoua on 10/12/2023.
//

#include "../includes/HardwareMonitor.hpp"
#include "../includes/Logger.hpp"

int HardwareMonitor::run() {
    Logger logger;
    logger.log("Session start", Logger::LogLevel::INFO);

    auto *handler = new WMIHandler;

    std::wstring classname = L"Win32_processor";
    std::wstring propertyName =  L"Name";
    handler->requestWMIProperty(classname, propertyName);

    return 0;
}