cmake_minimum_required(VERSION 3.27)
project(HttpClientSuite VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../libs/boost_1_88_0")
set(Boost_NO_WARN_NEW_VERSIONS ON)

find_package(Boost REQUIRED)

set(OPENSSL_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libs/openssl")
find_package(OpenSSL REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
option(USE_CPM "Whether to use CPM." ON)

include(CTest)
enable_testing()

if(USE_CPM)
    message(STATUS "Using CPM")
    include(CPM)

    CPMAddPackage(
        NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        VERSION 3.5.3
    )

    if(Catch2_ADDED)
        message(STATUS "Catch2 successfully added at: ${Catch2_SOURCE_DIR}")
        list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
        include(Catch)
    else()
        message(FATAL_ERROR "Failed to fetch Catch2 via CPM")
    endif()
endif()

add_subdirectory(src)