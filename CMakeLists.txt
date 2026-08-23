cmake_minimum_required(VERSION 3.10)

project(CareerPathPredictionSystem VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    src/main.c
    src/auth.c
    src/student.c
    src/assessment.c
    src/prediction.c
    src/gap_analysis.c
    src/db_handler.c
    src/report.c
    src/ui.c
    src/colors.c
    src/input_handler.c
    src/analytics.c
    src/sha256.c
    src/sqlite3.c
)

add_executable(career_system ${SOURCES})

if (WIN32)
    target_link_libraries(career_system PRIVATE ws2_32 advapi32)
else()
    target_link_libraries(career_system PRIVATE pthread dl)
endif()

set_target_properties(career_system PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
