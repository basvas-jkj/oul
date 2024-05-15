cmake_minimum_required (VERSION 3.8)

project(oul LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(oul "src/main.cpp" "src/config.cpp" "src/download.cpp")

find_package(libzippp REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Boost REQUIRED algorithm filesystem process)

target_link_libraries(oul PRIVATE libzippp::libzippp)
target_link_libraries(oul PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(oul PRIVATE Boost::algorithm)
target_link_libraries(oul PRIVATE Boost::filesystem)
target_link_libraries(oul PRIVATE Boost::process)

if (MSVC)
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W2>)
else()
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic>)
endif()
