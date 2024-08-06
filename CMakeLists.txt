cmake_minimum_required(VERSION 3.5)

project(
    strata-chart
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 Qt6)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)

# Объединение исходников
file(GLOB PROVIDERS_SRC "${PROJECT_SOURCE_DIR}/provider/*.*")
file(GLOB WIDGETS_SRC "${PROJECT_SOURCE_DIR}/widgets/*.*")
set(RESOURCES_SRC "resources/resources.qrc")

set(PROJECT_SOURCES "main.cpp" ${PROVIDERS_SRC} ${WIDGETS_SRC} ${RESOURCES_SRC})

# Таргет исполняемого файла
add_executable(strata-chart ${PROJECT_SOURCES})
target_include_directories(strata-chart PRIVATE ${PROJECT_SOURCE_DIR})

target_link_libraries(strata-chart PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                           Qt${QT_VERSION_MAJOR}::Charts)

set_target_properties(strata-chart PROPERTIES WIN32_EXECUTABLE TRUE)
