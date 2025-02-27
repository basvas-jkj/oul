cmake_minimum_required (VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_MODULE_STD 1)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GENERATOR "Ninja")

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")

project(oul)

# find packages
find_package(libzippp REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED algorithm filesystem process)

# helpers library
add_library(helpers "lib/helpers/Helpers.cpp")
set_property(TARGET helpers PROPERTY CXX_STANDARD 23)
target_include_directories(helpers PUBLIC "${VCPKG_INSTALLED_DIR}/x64-windows/include")
target_link_libraries(helpers PRIVATE libzippp::libzippp)

# OUL - main projetct
file(GLOB_RECURSE oul_source RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE oul_modules RELATIVE ${CMAKE_SOURCE_DIR} "src/*.ixx")
add_executable(oul ${oul_source})
target_sources(oul PUBLIC FILE_SET CXX_MODULES FILES ${oul_modules})
set_property(TARGET oul PROPERTY CXX_STANDARD 23)

target_link_libraries(oul PRIVATE helpers)
target_link_libraries(oul PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(oul PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(oul PRIVATE Boost::algorithm)
target_link_libraries(oul PRIVATE Boost::filesystem)
target_link_libraries(oul PRIVATE Boost::process)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/msg/english.json" "${CMAKE_CURRENT_BINARY_DIR}/english.json" COPYONLY)

if (MSVC)
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W1>)
else()
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic>)
endif()
