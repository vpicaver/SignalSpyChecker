cmake_minimum_required(VERSION 3.23)

project(SignalSpyChecker LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Test REQUIRED)
find_package(Catch2 REQUIRED)

file(GLOB spyCheckerCPP *.cpp)
file(GLOB spyCheckerH *.h)
add_library(SignalSpyChecker STATIC
    ${spyCheckerCPP}
    ${spyCheckerH}
    )
target_link_libraries(SignalSpyChecker
    PUBLIC
    Catch2::Catch2
    Qt::Core
    Qt::Test
    )
target_include_directories(SignalSpyChecker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

