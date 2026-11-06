cmake_minimum_required(VERSION 3.30)
project(ledt)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(vendor)
add_executable(ledt main.cpp
        Application.h
        Application.cpp
        Ui.cpp
        Ui.h
        Screen.cpp
        Screen.h
        ScreenMain.cpp
        ScreenMain.h
        Label.cpp
        Label.h
)
target_link_libraries(ledt PRIVATE vendor)