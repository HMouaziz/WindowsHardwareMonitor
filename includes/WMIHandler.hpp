//
// Created by hmoua on 10/12/2023.
//

#ifndef WINDOWSHARDWAREMONITOR_WMIHANDLER_HPP
#define WINDOWSHARDWAREMONITOR_WMIHANDLER_HPP

#include <windows.h>
#include <string>
#include <WbemCli.h>
#include <vector>
#include <map>
#include <functional>
#include <queue>
#include "../includes/HardwareMonitor.hpp"
#include "../includes/Logger.hpp"

struct WMIRequest {
    enum class Type { GetStaticProperty, GetAllStaticProperties, MonitorDynamicProperty,MonitorDynamicProperties };
    Type requestType;
    std::wstring className;
    std::wstring propertyName;
};


class WMIHandler {
    Logger logger;

    void setupCOM();
    bool initialiseWMIConnection(IWbemLocator** pLocator, IWbemServices** pServices);

    void processRequests(const WMIRequest& request);

    void handleGetStaticProperty(const WMIRequest& request, IWbemServices* pSvc);
    void handleGetAllStaticProperties(const WMIRequest& request, IWbemServices* pSvc);
    void handleMonitorDynamicProperty(const WMIRequest& request, IWbemServices* pSvc);
    void handleMonitorDynamicProperties(const WMIRequest& request, IWbemServices* pSvc);

public:
    WMIHandler();
    ~WMIHandler();
    void requestWMIProperty(const std::wstring& classname, std::wstring& propertyName);
    //add other request types
};


#endif //WINDOWSHARDWAREMONITOR_WMIHANDLER_HPP
