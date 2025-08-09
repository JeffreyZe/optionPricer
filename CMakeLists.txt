cmake_minimum_required(VERSION 3.10)
project(option_pricer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(pricer
    src/main.cpp
    src/Option.cpp
    src/BlackScholesPricer.cpp
    src/Utils.cpp
)
