cmake_minimum_required (VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(oul)

# find packages
find_package(yaml-cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED filesystem process)

function(linux_link_library target_name library_name)
    if (LINUX)
    target_link_options(${target_name} PRIVATE ${library_name})
    else()
    # Windows doesn't require this.
    endif()
endfunction()

# OUL - main projetct
file(GLOB_RECURSE oul_source RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE oul_modules RELATIVE ${CMAKE_SOURCE_DIR} "src/*.ixx")
add_executable(oul ${oul_source})
target_sources(oul PUBLIC FILE_SET CXX_MODULES FILES ${oul_modules})
set_property(TARGET oul PROPERTY CXX_STANDARD 23)

target_link_libraries(oul PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(oul PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(oul PRIVATE Boost::filesystem)
target_link_libraries(oul PRIVATE Boost::process)
linux_link_library(oul -lstdc++)

add_custom_target(english_messages
        COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_SOURCE_DIR}/msg/english.json"
                "${CMAKE_BINARY_DIR}/english.json")
add_custom_target(czech_messages
        COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_SOURCE_DIR}/msg/czech.json"
                "${CMAKE_BINARY_DIR}/czech.json")
add_dependencies(oul english_messages)
add_dependencies(oul czech_messages)

# test projects

enable_testing()
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

function(declare_test TEST_NAME)
    file(GLOB_RECURSE test_source RELATIVE ${CMAKE_SOURCE_DIR} "test/${TEST_NAME}/*.cpp")
    file(GLOB_RECURSE test_modules RELATIVE ${CMAKE_SOURCE_DIR} "test/${TEST_NAME}/*.ixx")

    add_executable(${TEST_NAME} ${test_source})
    target_sources(${TEST_NAME} PUBLIC FILE_SET CXX_MODULES FILES ${test_modules})
    set_property(TARGET ${TEST_NAME} PROPERTY CXX_STANDARD 23)

    target_link_libraries(${TEST_NAME} PRIVATE Boost::unit_test_framework)
    linux_link_library(${TEST_NAME} -lstdc++)
    linux_link_library(${TEST_NAME} -lm)

    add_test(${TEST_NAME} "${TEST_NAME}")
endfunction()

declare_test("config_test")
declare_test("file_iterator_test")

target_sources(config_test PUBLIC "src/config/data_file.cpp")
target_sources(config_test PUBLIC FILE_SET CXX_MODULES FILES "src/message.ixx" "src/config/data_file.ixx" "src/config/config.ixx" "src/config/item.ixx" "src/support.ixx")
target_sources(file_iterator_test PUBLIC FILE_SET CXX_MODULES FILES "src/server/file_iterator.ixx" "src/support.ixx")

target_link_libraries(config_test PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(config_test PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(config_test PRIVATE Boost::filesystem)
target_link_libraries(file_iterator_test PRIVATE Boost::filesystem)

if (MSVC)
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W4>)
else()
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic>)
endif()
