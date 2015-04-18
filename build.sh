#!/bin/bash

PARSER_SRC=src/parser

case `uname` in
  Linux) g++ ${PARSER_SRC}/config_parser.cc ${PARSER_SRC}/config_parser_main.cc -std=c++0x -g -Wall -o config_parser;;
  Darwin) clang++ ${PARSER_SRC}/config_parser.cc ${PARSER_SRC}/config_parser_main.cc -std=c++11 -g -Wall -stdlib=libc++ -o config_parser;;
  *) echo "Unknown operating system";;
esac
