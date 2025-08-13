cmake_minimum_required(VERSION 3.27)

set(SAMPLE_HTTP_CLIENT 
	"SampleHttpClient"
)
set(SAMPLE_HTTP_CLIENT_SOURCE_FILES 
	"${CMAKE_CURRENT_SOURCE_DIR}/SampleHttpClient.cpp"
)

add_executable(${SAMPLE_HTTP_CLIENT} ${SAMPLE_HTTP_CLIENT_SOURCE_FILES})

project(${SAMPLE_HTTP_CLIENT} VERSION 1.0 LANGUAGES CXX)
target_compile_definitions(${SAMPLE_HTTP_CLIENT} PUBLIC _WIN32_WINNT=0x0A00) # Windows 10

set(BOOST_ROOT "D:/libs/boost_1_88_0")
set(Boost_LIBRARY_DIRS "D:/libs/boost_1_88_0/stage")
set(Boost_NO_WARN_NEW_VERSIONS ON)

set(OPENSSL_ROOT_DIR "C:/openssl-install")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")

find_package(Boost REQUIRED COMPONENTS system thread chrono url)
find_package(OpenSSL REQUIRED)

set(SAMPLE_HTTP_CLIENT_INCLUDES
	"${CMAKE_CURRENT_SOURCE_DIR}"
	${Boost_INCLUDE_DIRS}
)

target_include_directories(${SAMPLE_HTTP_CLIENT} PUBLIC 
	${SAMPLE_HTTP_CLIENT_INCLUDES}
	${OPENSSL_INCLUDE_DIR}
)

target_compile_features(${SAMPLE_HTTP_CLIENT} PUBLIC 
	cxx_std_20
)

target_link_libraries(${SAMPLE_HTTP_CLIENT} PUBLIC
	Boost::system
	Boost::thread
	Boost::chrono
	Boost::url
	OpenSSL::SSL
    OpenSSL::Crypto
)