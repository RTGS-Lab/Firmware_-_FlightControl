# FlightControl_Demo/test/CMakeLists.txt
# Add GoogleTest
add_subdirectory(external/googletest)

# Define the TESTING preprocessor macro for all test builds
add_compile_definitions(TESTING)

# IMPORTANT: Make sure mocks are included first to override system headers
include_directories(BEFORE mocks)

# Include FFF headers
include_directories(external/fff)

# Include Google Test headers
include_directories(external/googletest/googletest/include)

# Create a mocks library
add_library(mocks STATIC
    mocks/MockWire.cpp
    mocks/MockArduino.cpp
    mocks/MockPCAL9535A.cpp
    mocks/MockMCP79412.cpp
    mocks/MockSPI.cpp
    mocks/MockPCA9634.cpp
    mocks/MockSHT4x.cpp
    mocks/MockVEML3328.cpp
    mocks/MockPAC1934.cpp
    mocks/MockMXC6655.cpp
    mocks/MockBMA456.cpp
    mocks/MockGNSS.cpp
)

# Specify test executable
add_executable(kestrel_tests 
    # Main test runner
    main.cpp
    
    # Kestrel tests
    unit/Driver-_-Kestrel/Driver-_-KestrelTest.cpp
    unit/Driver-_-Kestrel/KestrelFunctionTests.cpp
)

# Link against mocks and GoogleTest
target_link_libraries(kestrel_tests mocks gtest gtest_main)

# Find all driver directories and add them to the include path
file(GLOB DRIVER_DIRS ${CMAKE_SOURCE_DIR}/lib/*/src)

# Add header include directories - mock directory MUST be first
target_include_directories(mocks BEFORE PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks  # Mocks FIRST
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${DRIVER_DIRS}  # Add all driver directories
)

target_include_directories(kestrel_tests BEFORE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks  # Mocks FIRST
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${DRIVER_DIRS}  # Add all driver directories
)