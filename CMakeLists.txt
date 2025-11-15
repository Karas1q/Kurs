cmake_minimum_required(VERSION 3.16)
project(YandexMapProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebEngineWidgets Sql)

set(SOURCES
    main.cpp
    mainwindow.cpp
    mapbridge.cpp
    regwindow.cpp
    menuwindow.cpp
    profilewindow.cpp
    adminwindow.cpp
    adminmenuwindow.cpp
    otzivwindow.cpp
    checkwindow.cpp
)

set(HEADERS
    mainwindow.h
    mapbridge.h
    regwindow.h
    menuwindow.h
    profilewindow.h
    adminwindow.h
    adminmenuwindow.h
    otzivwindow.h
    checkwindow.h
)

set(UIS
    mainwindow.ui
    regwindow.ui
)

set(RESOURCES
    resources.qrc
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UIS}
    ${RESOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebEngineWidgets
    Qt6::Sql
)

