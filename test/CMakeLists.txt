# FlightControl_Demo/test/CMakeLists.txt
# Add GoogleTest
add_subdirectory(external/googletest)

# Include FFF headers
include_directories(external/fff)

# Include mocks directory
include_directories(mocks)

# Include Google Test headers
include_directories(external/googletest/googletest/include)

# Specify test executable
add_executable(unit_tests 
    #main test runner
    main.cpp
    
    # mocks
    mocks/MockWire.cpp
    mocks/MockParticle.h
    
    # Kestrel tests
    unit/Driver-_-Kestrel/Driver-_-KestrelTest.cpp

    # FlightControl_Demo tests
    ${CMAKE_SOURCE_DIR}/src/FlightControl_Demo.cpp
    unit/FlightControl_Demo/FlightControl_DemoTest.cpp
    unit/FlightControl_Demo/FlightControl_DemoTestHarness.cpp
)

# Link against GoogleTest
target_link_libraries(unit_tests gtest gtest_main)

# Define the TESTING preprocessor macro for all test builds
target_compile_definitions(unit_tests PRIVATE TESTING)

# Find all driver directories and add them to the include path
file(GLOB DRIVER_DIRS ${CMAKE_SOURCE_DIR}/lib/*/src)

target_include_directories(unit_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${DRIVER_DIRS}  # Add all driver directories
)