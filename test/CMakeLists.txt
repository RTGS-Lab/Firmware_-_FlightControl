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
add_executable(kestrel_tests 
    main.cpp 
    unit/Driver-_-Kestrel/kestrel_basic_test.cpp
)

# Link against GoogleTest
target_link_libraries(kestrel_tests gtest gtest_main)

# Include the necessary paths for header files
target_include_directories(kestrel_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib/Driver_-_Kestrel/src
)