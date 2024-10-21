cmake_minimum_required(VERSION 3.20)
project(ExperimentFramework)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include/)
set(PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/source)

add_library(
    ExperimentLibrary
    ${PROJECT_SOURCES}/logger/ConsoleLogger.cpp

    ${PROJECT_SOURCES}/nodes/IntermediateNode.cpp
    ${PROJECT_SOURCES}/nodes/PrimaryNode.cpp

    ${PROJECT_SOURCES}/queue/ThreadSafeQueue.cpp

    ${PROJECT_SOURCES}/receiver/InterThreadReceiver.cpp
    ${PROJECT_SOURCES}/receiver/UdpReceiver.cpp

    ${PROJECT_SOURCES}/transmitter/InterThreadTransmitter.cpp
    ${PROJECT_SOURCES}/transmitter/UdpTransmitter.cpp

    ${PROJECT_SOURCES}/InternalNodeConfigurator.cpp
)

add_executable(
    TransmitterRunner
    ${PROJECT_SOURCES}/transmitterMain.cpp
)

add_executable(
    ReceiverRunner
    ${PROJECT_SOURCES}/receiverMain.cpp
)

target_link_libraries(TransmitterRunner ExperimentLibrary)
target_link_libraries(ReceiverRunner ExperimentLibrary)

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
    ${UNIT_TESTS_DIR}/InterThreadTransmitterTest.cpp
    ${UNIT_TESTS_DIR}/PrimaryNodeTests.cpp
    ${UNIT_TESTS_DIR}/ThreadsafeQueueTests.cpp
)
target_link_libraries(
    testRunner
    ExperimentLibrary
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testRunner)