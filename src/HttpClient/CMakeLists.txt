add_library(HttpClientLib INTERFACE
)

target_include_directories(HttpClientLib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(HttpClientLib INTERFACE
    ${Boost_LIBRARIES}
    Common
)
