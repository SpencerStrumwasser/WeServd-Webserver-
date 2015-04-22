#!/bin/bash

SRC=src
PARSER_SRC=src/parser

case `uname` in
  Linux) g++ ${PARSER_SRC}/config_parser.cc ${SRC}/webserver.cpp -std=c++0x -g -Wall -o webserver;;
  Darwin) clang++ ${PARSER_SRC}/config_parser.cc ${SRC}/webserver.cpp -std=c++11 -g -Wall -stdlib=libc++ -o webserver;;
  *) echo "Unknown operating system";;
esac
