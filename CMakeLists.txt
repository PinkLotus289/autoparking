cmake_minimum_required(VERSION 3.16)

project(AutoparkingApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")


find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(SQLite3 REQUIRED)


include_directories(/opt/homebrew/include ${PROJECT_SOURCE_DIR}/Exceptions)
link_directories(/opt/homebrew/lib)


set(SOURCES
    Source/main.cpp
    Source/Car.cpp
    Source/ParkingLot.cpp
    Source/ParkingSpot.cpp
    Source/MainWindow.cpp
    Source/LoginWindow.cpp
    Source/Truck.cpp
    Source/ExportManager.cpp
)


set(HEADERS
    Header/Car.hpp
    Header/DatabaseManager.hpp
    Header/ParkingLot.hpp
    Header/ParkingSpot.hpp
    Header/TerminalDisplay.hpp
    Header/MainWindow.hpp
    Header/LoginWindow.hpp
    Header/Truck.hpp
    Header/Vehicle.hpp
    Header/Queue.hpp
    Header/InputValidator.hpp
    Header/ExportManager.hpp
    Header/FilterCriteria.hpp
    Header/CustomExceptions.hpp
)


set(EXCEPTIONS
    Exceptions/ValidationError.hpp
    Exceptions/InvalidTimeFormatError.hpp
    Exceptions/InvalidLicensePlateError.hpp
    Exceptions/InputCancelledError.hpp
    Exceptions/DuplicateVehicleError.hpp
    Exceptions/InvalidSpotNumberError.hpp 
)


qt6_wrap_cpp(MOC_SOURCES ${HEADERS})

add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SOURCES})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets SQLite::SQLite3 xlsxwriter)
