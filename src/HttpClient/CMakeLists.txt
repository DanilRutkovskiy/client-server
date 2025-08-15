set(HTTP_CLIENT_LIB_NAME HttpClientLib)

add_library(${HTTP_CLIENT_LIB_NAME} INTERFACE)

find_package(Boost REQUIRED system url)

target_include_directories(${HTTP_CLIENT_LIB_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIR}
)

target_link_libraries(${HTTP_CLIENT_LIB_NAME} INTERFACE
    Common
    Boost::system
    Boost::url
)
