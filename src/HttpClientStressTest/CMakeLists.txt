set(HTTP_CLIENT_STRESS_TEST_NAME HttpClientStressTest)

add_executable(${HTTP_CLIENT_STRESS_TEST_NAME} HttpClientStressTest.cpp)

target_compile_definitions(${HTTP_CLIENT_STRESS_TEST_NAME} PUBLIC _WIN32_WINNT=0x0A00)

target_include_directories(${HTTP_CLIENT_STRESS_TEST_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(${HTTP_CLIENT_STRESS_TEST_NAME} PRIVATE
    HttpClientLib
    OpenSSL::SSL
    OpenSSL::Crypto
)
