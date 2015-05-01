#!/bin/bash

GTEST_DIR=lib/gtest-1.7.0
PARSER_DIR="src/parser"
PARSER_FILE="ConfigParser.cpp"
PARSER_HEADER="config_parser.h"
PARSER_PROCESSOR="ParserProcessor.cpp"
PARSER_TEST_DIR="tests/parser"
PARSER_TEST_FILE="ServerTest.cpp"
BUILD_DIR="build"

case `uname` in
  Linux)
    g++ -std=c++0x -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -o ${BUILD_DIR}/gtest-all.o
    ar -rv ${BUILD_DIR}/libgtest.a ${BUILD_DIR}/gtest-all.o
    g++ -std=c++0x -g -Wall -lboost_system -lboost_thread -isystem ${GTEST_DIR}/include -pthread ${PARSER_DIR}/${PARSER_FILE} ${PARSER_DIR}/${PARSER_PROCESSOR} ${PARSER_TEST_DIR}/${PARSER_TEST_FILE} \
    ${GTEST_DIR}/src/gtest_main.cc ${BUILD_DIR}/libgtest.a -o ServerTest ;;
  Darwin)
    clang++ -std=c++11 -stdlib=libc++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -o ${BUILD_DIR}/gtest-all.o
    ar -rv ${BUILD_DIR}/libgtest.a ${BUILD_DIR}/gtest-all.o
    clang++ -std=c++11 -stdlib=libc++ -isystem ${GTEST_DIR}/include -I${PARSER_DIR} -pthread ${PARSER_DIR}/${PARSER_FILE} \
    ${PARSER_TEST_DIR}/${PARSER_TEST_FILE} ${GTEST_DIR}/src/gtest_main.cc ${BUILD_DIR}/libgtest.a -o config_parser_test ;;
  *)
    echo "Unknown operating system." ;;
esac

