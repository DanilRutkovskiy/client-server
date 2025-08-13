add_library(Common INTERFACE)

target_include_directories(Common INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(Common INTERFACE cxx_std_20)
