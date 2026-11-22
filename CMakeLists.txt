cmake_minimum_required(VERSION 3.16)
project(HireMatch VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Copy the sample text database beside the executable for the first run.
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

add_executable(HireMatch
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/models.h
    src/storage.h
    src/storage.cpp
    src/dsa.h
    src/matching.h
    src/matching.cpp
)

target_link_libraries(HireMatch PRIVATE Qt6::Widgets)
