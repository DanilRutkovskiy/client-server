cmake_minimum_required(VERSION 3.27)

project(SampleHttpClient VERSION 1.0 LANGUAGES CXX)

set(SAMPLE_HTTP_CLIENT SampleHttpClient)
set(SAMPLE_HTTP_CLIENT_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/SampleHttpClient.cpp"
)

add_executable(${SAMPLE_HTTP_CLIENT} ${SAMPLE_HTTP_CLIENT_SOURCE_FILES})

target_compile_definitions(${SAMPLE_HTTP_CLIENT} PUBLIC _WIN32_WINNT=0x0A00)

set(BOOST_ROOT "D:/libs/boost_1_88_0")
set(Boost_LIBRARY_DIRS "D:/libs/boost_1_88_0/stage/lib") # Ensure this points to .lib files
set(Boost_NO_WARN_NEW_VERSIONS ON)

set(OPENSSL_ROOT_DIR "C:/openssl-install")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")

find_package(Boost REQUIRED)

find_package(OpenSSL REQUIRED)

target_include_directories(${SAMPLE_HTTP_CLIENT} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

target_compile_features(${SAMPLE_HTTP_CLIENT} PUBLIC cxx_std_20)

file(GLOB BOOST_ALL_LIBS "${Boost_LIBRARY_DIRS}/*.lib")
target_link_libraries(${SAMPLE_HTTP_CLIENT} PUBLIC
    ${BOOST_ALL_LIBS}
    OpenSSL::SSL
    OpenSSL::Crypto
)
