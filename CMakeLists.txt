cmake_minimum_required(VERSION 3.10)

project(CareerPathPredictionSystem VERSION 1.0 LANGUAGES C)

# Specify standard C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Set the include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Define all source files
set(SOURCES
    src/main.c
    src/auth.c
    src/student.c
    src/assessment.c
    src/prediction.c
    src/gap_analysis.c
    src/file_handler.c
    src/report.c
    src/ui.c
    src/colors.c
    src/input_handler.c
    src/analytics.c
)

# Build the executable
add_executable(career_system ${SOURCES})

# Ensure the executable output is in the root directory like the Makefile
set_target_properties(career_system PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
