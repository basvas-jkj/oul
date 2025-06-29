cmake_minimum_required (VERSION 3.30)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(oul)

# find packages
find_package(yaml-cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED filesystem process)

function(link_stdlib target_name)
    if (LINUX)
    target_link_options(target_name PRIVATE -lstdc++)
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
link_stdlib(oul)


add_custom_command(
        TARGET oul POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                "${CMAKE_SOURCE_DIR}/msg/english.json"
                "${CMAKE_BINARY_DIR}/english.json")

enable_testing()
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(file_iterator_test "test/file_iterator_test.cpp")
target_sources(file_iterator_test PUBLIC FILE_SET CXX_MODULES FILES "test/mock_filesystem.ixx" "src/server/file_iterator.ixx" "src/common.ixx" "src/usings.ixx")
set_property(TARGET file_iterator_test PROPERTY CXX_STANDARD 23)
target_link_libraries(file_iterator_test PRIVATE Boost::filesystem)
target_link_libraries(file_iterator_test PRIVATE Boost::unit_test_framework)
link_stdlib(file_iterator_test)

add_test(file_iterator_test "file_iterator_test")

if (MSVC)
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:/W4>)
else()
    target_compile_options(oul PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -pedantic>)
endif()
