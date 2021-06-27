cmake_minimum_required(VERSION 3.10)

project(ZHM5Randomizer CXX)

file (GLOB SRCFILES "src/*.h" "src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SRCFILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "DINPUT8")

target_link_libraries(${PROJECT_NAME} Wintrust)
target_link_libraries(${PROJECT_NAME} Crypt32)
