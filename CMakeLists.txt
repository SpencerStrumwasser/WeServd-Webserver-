cmake_minimum_required(VERSION 3.1)
project(WeServd)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/webserver.cpp)
add_executable(WeServd ${SOURCE_FILES} src/parser/ParserProcessor.cpp src/parser/ParserProcessor.h src/server/RequestHandler.cpp src/server/RequestHandler.h src/server/mime_types.h src/server/mime_types.cpp src/server/reply.h src/server/reply.cpp src/server/header.h src/server/header.cpp src/server/request.h src/server/request.cpp)