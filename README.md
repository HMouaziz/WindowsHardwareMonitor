# Hardware Monitor

This repository contains a Hardware Monitor application designed to monitor hardware properties using Windows Management Instrumentation (WMI). It includes logging capabilities to ensure that all actions and errors are recorded.

## Table of Contents

- [Hardware Monitor](#hardware-monitor)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Classes and Methods](#classes-and-methods)
    - [HardwareMonitor](#hardwaremonitor)
    - [Logger](#logger)
    - [WMIHandler](#wmihandler)
  - [Contributing](#contributing)
  - [License](#license)

## Features

- **Hardware Monitoring**: Monitors hardware properties such as CPU information using WMI.
- **Logging**: Logs all actions and errors to a log file with timestamps.
- **Thread Safety**: Ensures thread-safe logging.

## Installation

To compile and run the Hardware Monitor application, follow these steps:

1. **Clone the repository**:
    ```sh
    git clone https://github.com/HMouaziz/hardware-monitor.git
    cd hardware-monitor
    ```

2. **Install dependencies**:
    - Ensure you have a C++ compiler installed (e.g., GCC, Clang, or MSVC).
    - Install Windows SDK for WMI support.

3. **Build the application**:
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

## Usage

To run the Hardware Monitor application:

```sh
./HardwareMonitor
```

The application will log its actions to a log file located in the `logs` directory.

## Classes and Methods

### HardwareMonitor

This class is the entry point of the application.

- **run()**: Initializes the logging and handles the WMI requests for hardware properties.

### Logger

This class handles the logging functionality.

- **Logger()**: Constructor that initializes the log file with a unique name based on the current date and time.
- **log(const std::string &message, LogLevel level)**: Logs a message with the specified log level (DEBUG, INFO, WARNING, ERROR, CRITICAL).
- **getLevelString(LogLevel level)**: Returns the string representation of the log level.

### WMIHandler

This class handles the interactions with WMI.

- **WMIHandler()**: Constructor.
- **~WMIHandler()**: Destructor.
- **setupCOM()**: Initializes COM library.
- **initialiseWMIConnection(IWbemLocator **pLocator, IWbemServices **pServices)**: Initializes the WMI connection.
- **processRequests(const WMIRequest& request)**: Processes the WMI requests based on the request type.
- **handleGetStaticProperty(const WMIRequest &request, IWbemServices *pSvc)**: Handles static property requests.
- **handleGetAllStaticProperties(const WMIRequest &request, IWbemServices *pSvc)**: Handles requests for all static properties (not implemented).
- **handleMonitorDynamicProperty(const WMIRequest &request, IWbemServices *pSvc)**: Handles dynamic property monitoring requests (not implemented).
- **handleMonitorDynamicProperties(const WMIRequest &request, IWbemServices *pSvc)**: Handles requests for monitoring multiple dynamic properties (not implemented).
- **requestWMIProperty(const std::wstring &classname, std::wstring &propertyName)**: Sends a WMI request to get a specific property.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.

1. Fork the repository.
2. Create your feature branch (`git checkout -b feature/your-feature`).
3. Commit your changes (`git commit -am 'Add your feature'`).
4. Push to the branch (`git push origin feature/your-feature`).
5. Open a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---
