add_executable(SampleHttpClient SampleHttpClient.cpp)

target_compile_definitions(SampleHttpClient PUBLIC _WIN32_WINNT=0x0A00)

file(GLOB BOOST_ALL_LIBS "${Boost_LIBRARY_DIRS}/*.lib")
target_link_libraries(SampleHttpClient PUBLIC
    HttpClientLib
    ${BOOST_ALL_LIBS}
    OpenSSL::SSL
    OpenSSL::Crypto
)
