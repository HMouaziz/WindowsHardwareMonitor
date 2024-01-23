//
// Created by hmoua on 10/12/2023.
//

#include "../includes/WMIHandler.hpp"
#include <comdef.h>
#include <wbemidl.h>
#include <iostream>
#include <sstream>
#include <locale>
#include <codecvt>


WMIHandler::WMIHandler() = default;

WMIHandler::~WMIHandler() = default;


void WMIHandler::setupCOM() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        // Create log message
        std::stringstream logMessage;
        logMessage << "COM Initialization error: HRESULT 0x" << std::hex << hr;
        // Log message
        logger.log(logMessage.str(), Logger::LogLevel::CRITICAL);
        return;
    }
    // Initialize security settings for COM.
    hr = CoInitializeSecurity(nullptr,
                              -1,
                              nullptr,
                              nullptr,
                              RPC_C_AUTHN_LEVEL_DEFAULT,
                              RPC_C_IMP_LEVEL_IMPERSONATE,
                              nullptr,
                              EOAC_NONE,
                              nullptr);
    if (FAILED(hr)) {
        // Create log message
        std::stringstream logMessage;
        logMessage << "COM Security initialization error: HRESULT 0x" << std::hex << hr;
        // Log message
        logger.log(logMessage.str(), Logger::LogLevel::CRITICAL);
        return;
    }
}

bool WMIHandler::initialiseWMIConnection(IWbemLocator **pLocator, IWbemServices **pServices) {
    HRESULT hr = CoCreateInstance(CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER,
                                  IID_IWbemLocator, reinterpret_cast<void**>(pLocator));
    if (FAILED(hr)) {
        // Create log message
        std::stringstream logMessage;
        logMessage << "Failed to create IWbemLocator object: HRESULT 0x" << std::hex << hr;
        // Log message
        logger.log(logMessage.str(), Logger::LogLevel::CRITICAL);
        return false;
    }

    hr = (*pLocator)->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), nullptr, nullptr, nullptr,
                                    0, nullptr, nullptr, pServices);
    if (FAILED(hr)) {
        // Create log message
        std::stringstream logMessage;
        logMessage << "Failed to connect to ROOT\\CIMV2 namespace: HRESULT 0x" << std::hex << hr;
        // Log message
        logger.log(logMessage.str(), Logger::LogLevel::CRITICAL);
        (*pLocator)->Release();
        *pLocator = nullptr;
        return false;
    }

    // Set proxy security levels
    hr = CoSetProxyBlanket(*pServices,
                           RPC_C_AUTHN_WINNT,
                           RPC_C_AUTHZ_NONE,
                           nullptr,
                           RPC_C_AUTHN_LEVEL_CALL,
                           RPC_C_IMP_LEVEL_IMPERSONATE,
                           nullptr,
                           EOAC_NONE);

    if (FAILED(hr)) {
        // Create log message
        std::stringstream logMessage;
        logMessage << "Could not set proxy blanket: HRESULT 0x" << std::hex << hr;
        // Log message
        logger.log(logMessage.str(), Logger::LogLevel::CRITICAL);
        (*pServices)->Release();
        (*pLocator)->Release();
        *pServices = nullptr;
        *pLocator = nullptr;
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------

void WMIHandler::processRequests(const WMIRequest& request) {
    std::stringstream logMessage;
    logMessage << "Started processing requests."; // add request name
    logger.log(logMessage.str(), Logger::LogLevel::INFO);

    setupCOM();

    logMessage << "Inititalised COM."; // add request name
    logger.log(logMessage.str(), Logger::LogLevel::INFO);

    IWbemLocator* pLoc = nullptr;
    IWbemServices* pSvc = nullptr;
    if (initialiseWMIConnection(&pLoc, &pSvc)) {
        logMessage << "Connected to WMI Successfully."; // add request name
        logger.log(logMessage.str(), Logger::LogLevel::INFO);

        switch (request.requestType) {
            case WMIRequest::Type::GetStaticProperty:
                //retrieve property and invoke callback
                handleGetStaticProperty(request, pSvc);
                break;
            case WMIRequest::Type::GetAllStaticProperties:
                //retrieve properties and invoke callback
                handleGetAllStaticProperties(request, pSvc);
                break;
            case WMIRequest::Type::MonitorDynamicProperty:
                handleMonitorDynamicProperty(request, pSvc);
                break;
            case WMIRequest::Type::MonitorDynamicProperties:
                handleMonitorDynamicProperties(request, pSvc);
                break;
        }
    }
    if (pSvc) {
        pSvc->Release();
    }
    if (pLoc) {
        pLoc->Release();
    }

    CoUninitialize();

}

//----------------------------------------------------------------------------------------------------------------------

void WMIHandler::handleGetStaticProperty(const WMIRequest &request, IWbemServices *pSvc) {
    // create query string
    std::wstring query = L"SELECT " + request.propertyName + L" FROM " + request.className;
    std::stringstream logMessage;
    // Pointer for query results
    IEnumWbemClassObject* pEnumerator = nullptr;

    // Execute WQL query
    HRESULT hr = pSvc->ExecQuery(bstr_t("WQL"),
                                 bstr_t(query.c_str()),
                                 WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                                 nullptr,
                                 &pEnumerator);

    // Check for failure
    if (FAILED(hr)) {
        // create log message

        logMessage << "Query execution failed: HRESULT 0x"
                   << std::hex << hr;
        // Log message
        logger.log(logMessage.str(), Logger::LogLevel::CRITICAL);
        // Create error result to pass to callback
        std::map<std::wstring, std::wstring> errorResult = {
                {L"error", L"Query execution failed"}};
        return;
    }


    // Object for storing individual results
    IWbemClassObject* pclsObj = nullptr;
    ULONG uReturn = 0;

    // Get object in enum
    hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

    // If object exits process it
    if (uReturn != 0) {
        VARIANT vtProp;

        // Retrieve property from object
        hr = pclsObj->Get(request.propertyName.c_str(), 0, &vtProp, nullptr, nullptr);

        // If successful, prep result and send to callback
        if (SUCCEEDED(hr)) {
            // Prepare a map with the property name and value (Assumes it's a string)
            std::map<std::wstring, std::wstring> result = {
                    {request.propertyName, vtProp.bstrVal ? vtProp.bstrVal : L""}};


            for (const auto& pair : result) {
                std::wcout << pair.first << L": " << pair.second << std::endl;
            }
            logMessage << "Query Executed Successfully.";
            logger.log(logMessage.str(), Logger::LogLevel::INFO);
        }

        // Clear VARIANT and release WMI Object
        VariantClear(&vtProp);
        pclsObj->Release();
    }
    // Release enum
    pEnumerator->Release();
}

void WMIHandler::handleGetAllStaticProperties(const WMIRequest &request, IWbemServices *pSvc) {

}

void WMIHandler::handleMonitorDynamicProperty(const WMIRequest &request, IWbemServices *pSvc) {

}

void WMIHandler::handleMonitorDynamicProperties(const WMIRequest &request, IWbemServices *pSvc) {

}

//----------------------------------------------------------------------------------------------------------------------

void WMIHandler::requestWMIProperty(const std::wstring &classname, std::wstring &propertyName) {
    WMIRequest request;
    request.requestType = WMIRequest::Type::GetStaticProperty;
    request.className = classname;
    request.propertyName = propertyName;

    processRequests(request);
}

