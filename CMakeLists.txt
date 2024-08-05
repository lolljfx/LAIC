cmake_minimum_required(VERSION 3.15)
project(ssbesb)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")

# Enable OpenMP
find_package(OpenMP REQUIRED)

if(OpenMP_CXX_FOUND)
    message(STATUS "Found OpenMP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Add executable for preprocessor
add_executable(preprocessor preprocessor.cpp main_preprocessor.cpp)

# Link the OpenMP library to the preprocessor target
target_link_libraries(preprocessor OpenMP::OpenMP_CXX)

# Command to run preprocessor and create processed_main.cpp
add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/processed_main.cpp
        COMMAND preprocessor
        DEPENDS main.cpp preprocessor
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running preprocessor"
)

# Add executable for the main project using the processed file
add_executable(ssbesb ${CMAKE_CURRENT_SOURCE_DIR}/processed_main.cpp laic_impl.h)

# Set dependencies to ensure correct build order
add_dependencies(ssbesb preprocessor)
