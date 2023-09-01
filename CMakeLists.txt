cmake_minimum_required(VERSION 3.11)
project(EJP-S)

include(FetchContent)

FetchContent_Declare(
	libcurl
	GIT_REPOSITORY "https://github.com/curl/curl.git"
	GIT_TAG "curl-8_1_2"
	)
FetchContent_MakeAvailable(libcurl)

add_subdirectory(lib)
add_subdirectory(src)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/.ejp/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/.ejp/)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
endif()