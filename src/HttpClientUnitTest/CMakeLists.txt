set(HTTP_CLIENT_UNIT_TEST_NAME HttpClientUnitTest)

add_executable(${HTTP_CLIENT_UNIT_TEST_NAME}
    HttpClientUnitTest.cpp
)

target_compile_definitions(${HTTP_CLIENT_UNIT_TEST_NAME} PUBLIC _WIN32_WINNT=0x0A00)

target_include_directories(${HTTP_CLIENT_UNIT_TEST_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${HTTP_CLIENT_UNIT_TEST_NAME} PUBLIC
    HttpClientLib
    Catch2::Catch2WithMain
    ${BOOST_ALL_LIBS}
)


target_compile_features(${HTTP_CLIENT_UNIT_TEST_NAME} PUBLIC cxx_std_20)

catch_discover_tests(${HTTP_CLIENT_UNIT_TEST_NAME})
