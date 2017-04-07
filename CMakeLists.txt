cmake_minimum_required(VERSION 3.6)
project(IPK2)

FIND_PACKAGE(OpenSSL COMPONENTS md5 REQUIRED)
INCLUDE_DIRECTORIES(OPENSSL_INCLUDE_DIR)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES ipk-client.cpp Connection.cpp Connection.h)
add_executable(ipk-client ${SOURCE_FILES})
target_link_libraries(ipk-client ${OPENSSL_LIBRARIES})
