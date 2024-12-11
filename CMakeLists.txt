cmake_minimum_required(VERSION 3.16)

project(AutoparkingApp LANGUAGES CXX)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Устанавливаем путь к Qt6
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

# Подключаем необходимые компоненты Qt6 и SQLite3
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(SQLite3 REQUIRED)

# Указываем пути к заголовочным файлам и библиотекам xlsxwriter
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

# Источники проекта
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

# Заголовочные файлы проекта
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
)

# Обработка MOC для заголовочных файлов
qt6_wrap_cpp(MOC_SOURCES ${HEADERS})

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SOURCES})

# Линкуем необходимые библиотеки
target_link_libraries(${PROJECT_NAME} Qt6::Widgets SQLite::SQLite3 xlsxwriter)
