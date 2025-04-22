cmake_minimum_required (VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

project(oul)

# find packages
find_package(yaml-cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(unofficial-wildcards REQUIRED)
find_package(Boost REQUIRED algorithm filesystem process)

# OUL - main projetct
file(GLOB_RECURSE oul_source RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE oul_modules RELATIVE ${CMAKE_SOURCE_DIR} "src/*.ixx")
add_executable(oul ${oul_source})
target_sources(oul PUBLIC FILE_SET CXX_MODULES FILES ${oul_modules})
set_property(TARGET oul PROPERTY CXX_STANDARD 23)

target_link_libraries(oul PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(oul PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(oul PRIVATE unofficial::wildcards::wildcards)
target_link_libraries(oul PRIVATE Boost::algorithm)
target_link_libraries(oul PRIVATE Boost::filesystem)
target_link_libraries(oul PRIVATE Boost::process)

add_custom_command(
        TARGET oul POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/msg/english.json
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/english.json)

if (MSVC)
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W4>)
else()
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic>)
endif()