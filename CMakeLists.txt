cmake_minimum_required (VERSION 3.8)

project(oul LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_MODULE_STD 1) 

# find packages
find_package(libzippp REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED algorithm filesystem process)

# helpers library
add_library(helpers "lib/helpers/Helpers.cpp")
target_include_directories(helpers PUBLIC "${VCPKG_INSTALLED_DIR}/x64-windows/include")
target_link_libraries(helpers PRIVATE libzippp::libzippp)

# OUL - main projetct
file(GLOB_RECURSE oul_modules RELATIVE ${CMAKE_SOURCE_DIR} "src/*.ixx")
add_executable(oul "src/main.cpp" ${oul_modules})

target_link_libraries(oul PRIVATE helpers)
target_link_libraries(oul PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(oul PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(oul PRIVATE Boost::algorithm)
target_link_libraries(oul PRIVATE Boost::filesystem)
target_link_libraries(oul PRIVATE Boost::process)

if (MSVC)
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W1>)
else()
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic>)
endif()
