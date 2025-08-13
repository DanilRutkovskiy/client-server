add_executable(HttpClientUnitTest
    HttpClientUnitTest.cpp
)

target_compile_definitions(HttpClientUnitTest PUBLIC _WIN32_WINNT=0x0A00)

target_include_directories(HttpClientUnitTest PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    "D:/libs/thirdparty/include"
)

target_link_libraries(HttpClientUnitTest PRIVATE
    HttpClientLib
    ${BOOST_ALL_LIBS}
)

target_compile_features(HttpClientUnitTest PUBLIC cxx_std_20)
