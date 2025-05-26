cmake_minimum_required (VERSION 3.12)

project("Zh_Test" 
        VERSION 1.0.0 
        LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_CXX_STANDARD_REQUIRED on)


if (CMAKE_SYSTEM_NAME MATCHES "Windows")
add_executable( main_win
                main.cpp)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
add_executable( main_linux
                main.cpp)
ELSE ()
MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
endif()