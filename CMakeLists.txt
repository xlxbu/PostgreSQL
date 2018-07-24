cmake_minimum_required(VERSION 3.10)
project(PostgreSQL)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")

#set(PQXX /usr/local/include/pqxx)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

add_executable(PostgreSQL src/main.cpp)

target_link_libraries(PostgreSQL ${PQXX_LIB} ${PQ_LIB})