# Created by Basvas j.k.j <basvas@seznam.cz>
# Unlicensed

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_VERSION VERSION_LESS "3.30.0")
	message(FATAL_ERROR "This version of  CMake (${CMAKE_VERSION}) doesn't support 'import std'.")
elseif(DEFINED CMAKE_EXPERIMENTAL_CXX_IMPORT_STD)
	message("Use custom value of CMAKE_EXPERIMENTAL_CXX_IMPORT_STD: ${CMAKE_EXPERIMENTAL_CXX_IMPORT_STD}.")
elseif(CMAKE_VERSION VERSION_LESS_EQUAL "3.31.7")
	set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "0e5b6991-d74f-4b3d-a41c-cf096e0b2508")
elseif(CMAKE_VERSION VERSION_LESS_EQUAL "3.31.11")
	set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
elseif(CMAKE_VERSION VERSION_LESS_EQUAL "4.0.2")
	set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457")
elseif(CMAKE_VERSION VERSION_LESS_EQUAL "4.2.3")
	set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
elseif(CMAKE_VERSION VERSION_LESS_EQUAL "4.3.0")
	set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
else()
	set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "451f2fe2-a8a2-47c3-bc32-94786d8fc91b")
	message(WARNING "Unknown version of CMake. (${CMAKE_VERSION})")
	message(STATUS "Use default value of CMAKE_EXPERIMENTAL_CXX_IMPORT_STD. (${CMAKE_EXPERIMENTAL_CXX_IMPORT_STD})")
endif()

set(IS_VS OFF)
set(IS_NINJA OFF)

if(CMAKE_GENERATOR STREQUAL "Visual Studio 17 2022" OR CMAKE_GENERATOR STREQUAL "Visual Studio 18 2026")
	set(IS_VS ON)
	set(CXX_SCAN_FOR_MODULES OFF)
elseif(CMAKE_GENERATOR STREQUAL "Ninja" OR CMAKE_GENERATOR STREQUAL "Ninja Multi-Config")
	set(IS_NINJA ON)
	set(CMAKE_CXX_MODULE_STD ON)
	set(CXX_SCAN_FOR_MODULES OFF)
else()
	message(FATAL_ERROR "${CMAKE_GENERATOR} doesn't support C++ modules.'")
endif()

function("target_modules" target visibility)
	target_sources(${target} ${visibility} FILE_SET CXX_MODULES TYPE CXX_MODULES FILES ${ARGN})
endfunction()
function("target_headers" target visibility)
	target_sources(${target} ${visibility} FILE_SET HEADERS TYPE HEADERS FILES ${ARGN})
endfunction()

function("system_header_units" target visibility)
	if(IS_VS)
		return()
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		message(FATAL_ERROR "${CMAKE_GENERATOR} with ${CMAKE_CXX_COMPILER_ID} is not supported currently.")
	endif()

	foreach(header IN LISTS ARGN)
		__init_header_pcm("${header}" pcm_path)
		__init_target_name("${header}" header_unit_target)
		__init_reference("${header}" "${pcm_path}" REFERENCE)
		
		__add_system_unit("${header_unit_target}" "${header}" "${pcm_path}")
		add_dependencies(${target} "${header_unit_target}")
		target_compile_options(${target} ${visibility} ${REFERENCE})
	endforeach()
endfunction()
function("user_header_units" target visibility)
	if(IS_VS)
		return()
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		message(FATAL_ERROR "${CMAKE_GENERATOR} with ${CMAKE_CXX_COMPILER_ID} is not supported currently.")
	endif()

	foreach(header IN LISTS ARGN)
		__init_header_pcm("${header}" pcm_path)
		__init_target_name("${header}" header_unit_target)
		__init_reference("${header}" "${pcm_path}" REFERENCE)
		
		__add_user_unit("${header_unit_target}" "${header}" "${pcm_path}")
		add_dependencies(${target} "${header_unit_target}")
		target_compile_options(${target} ${visibility} ${REFERENCE})
	endforeach()
endfunction()
function("vcpkg_header_units" target visibility)
	if(IS_VS)
		return()
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		message(FATAL_ERROR "${CMAKE_GENERATOR} with ${CMAKE_CXX_COMPILER_ID} is not supported currently.")
	endif()

	foreach(header IN LISTS ARGN)
		__init_header_pcm("${header}" pcm_path)
		__init_target_name("${header}" header_unit_target)
		__init_reference("${header}" "${pcm_path}" REFERENCE)
		
		__add_vcpkg_unit("${header_unit_target}" "${header}" "${pcm_path}")
		add_dependencies(${target} "${header_unit_target}")
		target_compile_options(${target} ${visibility} ${REFERENCE})
	endforeach()
endfunction()

function(__add_system_unit header_unit_target header pcm_path)
	if(IS_VS OR TARGET "${header_unit_target}")
		return()
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		message(FATAL_ERROR "${CMAKE_GENERATOR} with ${CMAKE_CXX_COMPILER_ID} is not supported currently.")
	endif()

	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(PARAMS
		"-std=c++${CMAKE_CXX_STANDARD}"
		"-xc++-system-header"
		"--compile" "${header}"
		"-o" "${pcm_path}"
	)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(PARAMS
		"-std=c++${CMAKE_CXX_STANDARD}"
		"-fmodules"
		"-xc++-system-header"
		"--compile" "${header}"
	)
	endif()

	add_custom_command(
			OUTPUT ${pcm_path}
			COMMAND ${CMAKE_CXX_COMPILER} ${PARAMS}
			COMMAND ${CMAKE_COMMAND} -E touch ${pcm_path}
			VERBATIM
	)
	add_custom_target("${header_unit_target}" DEPENDS ${pcm_path})
endfunction()
function(__add_user_unit header_unit_target header pcm_path)
	if(IS_VS OR TARGET "${header_unit_target}")
		return()
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		message(FATAL_ERROR "${CMAKE_GENERATOR} with ${CMAKE_CXX_COMPILER_ID} is not supported currently.")
	endif()

	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(PARAMS
		"-std=c++${CMAKE_CXX_STANDARD}"
		"-xc++-user-header"
		"--compile" "${CMAKE_CURRENT_SOURCE_DIR}/${header}"
		"-o" "${pcm_path}"
	)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(PARAMS
		"-std=c++${CMAKE_CXX_STANDARD}"
		"-fmodules"
		"-xc++-user-header"
		"--compile" "${header}"
	)
	endif()
	add_custom_command(
			DEPENDS "${header}"
			OUTPUT ${pcm_path}
			COMMAND ${CMAKE_CXX_COMPILER} ${PARAMS}
			COMMAND ${CMAKE_COMMAND} -E touch ${pcm_path}
			VERBATIM
	)
	add_custom_target("${header_unit_target}" DEPENDS ${pcm_path})
endfunction()
function(__add_vcpkg_unit header_unit_target header pcm_path)
	if(IS_VS OR TARGET "${header_unit_target}")
		return()
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		message(FATAL_ERROR "${CMAKE_GENERATOR} with ${CMAKE_CXX_COMPILER_ID} is not supported currently.")
	endif()
	
	set(VCPKG_INCLUDE_PATH "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(PARAMS
		"-std=c++${CMAKE_CXX_STANDARD}"
		"-xc++-user-header"
		"--compile" "${VCPKG_INCLUDE_PATH}/${header}"
		"-I" "${VCPKG_INCLUDE_PATH}"
		"-o" "${pcm_path}"
	)
	elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(PARAMS
		"-std=c++${CMAKE_CXX_STANDARD}"
		"-fmodules"
		"-xc++-user-header"
		"--compile" "${header}"
	)
	endif()
	add_custom_command(
			DEPENDS "${VCPKG_INCLUDE_PATH}/${header}"
			OUTPUT ${pcm_path}
			COMMAND ${CMAKE_CXX_COMPILER} ${PARAMS}
			COMMAND ${CMAKE_COMMAND} -E touch ${pcm_path}
			VERBATIM
	)
	add_custom_target("${header_unit_target}" DEPENDS ${pcm_path})
endfunction()

function(__init_header_pcm header pcm)
	set(${pcm} "${CMAKE_CURRENT_BINARY_DIR}/pcm_units/${header}.pcm" PARENT_SCOPE)
endfunction()
function(__init_target_name file_name target_name)
	string(MD5 file_hash "${CMAKE_CURRENT_LIST_FILE}")
	string(REGEX REPLACE "[^A-Za-z0-9_+\\-\\.]" "_" sanitised ${file_name})
	set(${target_name} "${sanitised}_${file_hash}" PARENT_SCOPE)
endfunction()
function(__init_reference header pcm reference)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		set(${reference} "-fmodule-file=${pcm}" PARENT_SCOPE)
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		#g++ -fmodules  unit.hpp main.cpp
		set(${reference} "-fmodules" PARENT_SCOPE)
	else()
		message(FATAL_ERROR "${CMAKE_GENERATOR} with ${CMAKE_CXX_COMPILER_ID} is not supported currently.")
	endif()
endfunction()