cmake_minimum_required (VERSION 3.8)

project(oul LANGUAGES CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(libzippp REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED algorithm filesystem process)

# OUL - main projetct
add_library(helpers "lib/helpers/Helpers.cpp")
target_include_directories(helpers PUBLIC "C:/Users/vasst/dev/vcpkg/installed/x64-windows/include")
target_link_libraries(helpers PRIVATE libzippp::libzippp)

file(GLOB_RECURSE oul_source RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
add_executable(oul ${oul_source})
message(${oul_source})

target_link_libraries(oul PRIVATE helpers)
target_link_libraries(oul PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(oul PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(oul PRIVATE Boost::algorithm)
target_link_libraries(oul PRIVATE Boost::filesystem)
target_link_libraries(oul PRIVATE Boost::process)

# test libraries
enable_testing()

# CONFIG_TEST
add_library(config "src/config.cpp" "src/data_file/data_file.cpp")
target_include_directories(config PUBLIC "C:/Users/vasst/dev/vcpkg/installed/x64-windows/include")

add_executable(config_test "test/config_test.cpp")
add_test(config_test config_test)

target_link_libraries(config_test PRIVATE config)
target_link_libraries(config_test PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(config_test PRIVATE nlohmann_json::nlohmann_json)

if (MSVC)
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W2>)
else()
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic>)
endif()
