cmake_minimum_required(VERSION 3.27)
project(HttpClientSuite VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_ROOT "D:/libs/boost_1_88_0")
set(Boost_LIBRARY_DIRS "D:/libs/boost_1_88_0/stage/lib")
set(Boost_NO_WARN_NEW_VERSIONS ON)

set(OPENSSL_ROOT_DIR "C:/openssl-install")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

file(GLOB BOOST_ALL_LIBS "${Boost_LIBRARY_DIRS}/*.lib")

add_subdirectory(HttpClient)
add_subdirectory(SampleHttpClient)
add_subdirectory(Common)
add_subdirectory(HttpClientUnitTest)