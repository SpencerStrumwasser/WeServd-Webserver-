#!/bin/bash

CFLAGS="-std=c++0x -g -Wall -lboost_system -lboost_thread-mt"
CFLAGS_MAC="-g -Wall -std=c++11 -stdlib=libc++ -lboost_system -lboost_thread-mt"
SRC=src
PARSER_SRC=src/parser

case `uname` in
  Linux) g++ ${PARSER_SRC}/config_parser.cc ${SRC}/webserver.cpp ${CFLAGS} -o webserver;;
  Darwin) clang++ ${PARSER_SRC}/config_parser.cc ${SRC}/webserver.cpp ${CFLAGS_MAC} -o webserver;;
  *) echo "Unknown operating system";;
esac
