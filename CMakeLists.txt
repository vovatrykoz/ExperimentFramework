cmake_minimum_required(VERSION 3.20)
project(ExperimentFramework)

add_executable(
    ExperimentRunner
    ${PROJECT_SOURCE_DIR}/source/main.cpp
)

set(UNIT_TESTS_DIR ${PROJECT_SOURCE_DIR}/tests)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    testRunner
    ${UNIT_TESTS_DIR}/IntermediateNodeTests.cpp
)
target_link_libraries(
    testRunner
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testRunner)