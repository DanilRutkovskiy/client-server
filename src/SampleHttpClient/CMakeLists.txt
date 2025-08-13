add_executable(SampleHttpClient SampleHttpClient.cpp)

target_compile_definitions(SampleHttpClient PUBLIC _WIN32_WINNT=0x0A00)

target_link_libraries(SampleHttpClient PUBLIC
    Common
    HttpClientLib
    ${BOOST_ALL_LIBS}
    OpenSSL::SSL
    OpenSSL::Crypto
)
