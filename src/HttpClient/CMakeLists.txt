cmake_minimum_required(VERSION 3.27)

set(HTTP_CLIENT_LIB_NAME "HttpClientLib")

project(${HTTP_CLIENT_LIB_NAME} VERSION 1.0 LANGUAGES CXX)

set(BOOST_ROOT "D:/libs/boost_1_88_0")
set(Boost_LIBRARY_DIRS "D:/libs/boost_1_88_0/stage")
set(Boost_NO_WARN_NEW_VERSIONS ON)

find_package(Boost REQUIRED COMPONENTS system thread chrono url)

set(HTTP_CLIENT_LIB_INCLUDES
	"${CMAKE_CURRENT_SOURCE_DIR}"
	${Boost_INCLUDE_DIRS}
)

add_library(${HTTP_CLIENT_LIB_NAME} INTERFACE)

target_include_directories(${HTTP_CLIENT_LIB_NAME} INTERFACE 
	${HTTP_CLIENT_LIB_INCLUDES}
)

target_compile_features(${HTTP_CLIENT_LIB_NAME} INTERFACE 
	cxx_std_20
)

target_link_libraries(${HTTP_CLIENT_LIB_NAME} INTERFACE
	Boost::system
	Boost::thread
	Boost::chrono
	Boost::url
)