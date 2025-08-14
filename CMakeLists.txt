cmake_minimum_required(VERSION 3.27)
project(HttpClientSuite VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT "D:/libs/boost_1_88_0")
set(Boost_LIBRARY_DIRS "D:/libs/boost_1_88_0/stage/lib")
set(Boost_NO_WARN_NEW_VERSIONS ON)
find_package(Boost REQUIRED)

set(OPENSSL_ROOT_DIR "C:/openssl-install")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
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

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
file(GLOB BOOST_ALL_LIBS "${Boost_LIBRARY_DIRS}/*.lib")

add_subdirectory(src)
