cmake_minimum_required(VERSION 3.10)
project(OptionPricer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Main library
set(optionPricer_SOURCES
    src/Option.cpp
    src/BlackScholesPricer.cpp
    src/Utils.cpp
)

add_library(optionPricer ${optionPricer_SOURCES})
target_include_directories(optionPricer PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Main executable
set(pricer_SOURCES
    src/main.cpp
)
add_executable(pricer ${pricer_SOURCES})
target_link_libraries(pricer PRIVATE optionPricer)


# ---- GoogleTest setup ----
enable_testing()

# Locate GTest (installed via brew)
find_package(GTest REQUIRED)

# Test sources
set(TEST_SOURCES
    tests/test_black_scholes.cpp
)

# Register the test executable with CTest to enable running it via `ctest`
add_executable(runTests ${TEST_SOURCES})
target_include_directories(runTests PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(runTests PRIVATE optionPricer GTest::GTest GTest::Main)

add_test(NAME runTests COMMAND runTests)
