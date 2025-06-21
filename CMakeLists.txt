cmake_minimum_required(VERSION 3.16)
project(proLeague VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

# Enable automoc, autorcc and autouic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set UI files directory
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/Pages/MenuPage.cpp
    src/Pages/HomePage.cpp
    src/Pages/LoginPage.cpp
    src/Pages/UserMenuPage.cpp
    src/Pages/AdminMenuPage.cpp
    src/datamanager.cpp
)

# Header files
set(HEADERS
    include/mainwindow.h
    include/Pages/BasePage.hpp
    include/Pages/MenuPage.hpp
    include/Pages/HomePage.hpp
    include/Pages/LoginPage.hpp
    include/Pages/UserMenuPage.hpp
    include/Pages/AdminMenuPage.hpp
    include/datamanager.h
    include/streamoperators.h
)

# UI files
set(UI_FILES
    ui/mainwindow.ui
)

# Resource files
set(RESOURCE_FILES
    resources/resources.qrc
)

# Create executable
qt_add_executable(proLeague
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

# Set include directories
target_include_directories(proLeague PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Link Qt libraries
target_link_libraries(proLeague PRIVATE
    Qt6::Core
    Qt6::Widgets
) 