set(SAMPLE_HTTP_CLIENT_NAME SampleHttpClient)

add_executable(${SAMPLE_HTTP_CLIENT_NAME} SampleHttpClient.cpp)

target_compile_definitions(${SAMPLE_HTTP_CLIENT_NAME} PUBLIC _WIN32_WINNT=0x0A00)

target_include_directories(${SAMPLE_HTTP_CLIENT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${SAMPLE_HTTP_CLIENT_NAME} PUBLIC
    Common
    HttpClientLib
    ${BOOST_ALL_LIBS}
    OpenSSL::SSL
    OpenSSL::Crypto
)
