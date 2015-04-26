# Determine OS
OS := $(shell uname)

# Program variables
NAME=webserver
TESTS_NAME=$(NAME)-tests

# Source variables
SRC=src
BUILD=build
PARSER=$(SRC)/parser

# Test variables
AR=ar -rv 
GTEST=lib/gtest-1.7.0
TEST=tests
PARSER_TEST=$(TEST)/parser

# Set variables according to OS, (Windows unsupported)
ifeq ($(OS), Linux)
	CC=g++
	CFLAGS=-g -Wall -std=c++0x
	BOOST_FLAGS=-lboost_system -lboost_thread
	TEST_FLAGS=-std=c++0x -isystem
endif
ifeq ($(OS), Darwin)
	CC=clang++
	CFLAGS=-g -Wall -std=c++11 -stdlib=libc++ 
	BOOST_FLAGS=-lboost_system -lboost_thread-mt
	TEST_FLAGS=-std=c++11 -stdlib=libc++ -isystem
	CLEAN=*.dSYM 
endif

all: webserver

# Source files
parser.o:
	$(CC) -c $(PARSER)/ConfigParser.cpp $(CFLAGS) -o $(BUILD)/parser.o

parser_processor.o: 
	$(CC) -c $(PARSER)/ParserProcessor.cpp $(CFLAGS) \
	-o $(BUILD)/parser_processor.o

webserver: parser_processor.o parser.o
	$(CC) $(BUILD)/parser_processor.o  $(BUILD)/parser.o $(SRC)/webserver.cpp \
	$(CFLAGS) $(BOOST_FLAGS) -o $(NAME)

test-server: all
	./webserver default_config

# Tests
gtest:
	$(CC) $(TEST_FLAGS) $(GTEST)/include -I $(GTEST) -c \
	$(GTEST)/src/gtest-all.cc -o $(BUILD)/gtest-all.o

archive: gtest
	$(AR) $(BUILD)/libgtest.a $(BUILD)/gtest-all.o

gtest-main.o: gtest
	$(CC) $(TEST_FLAGS) $(BUILD)/gtest-all.o -I $(GTEST)/include \
	-c $(GTEST)/src/gtest_main.cc -o $(BUILD)/gtest-main.o

parser-tests.o: gtest-main.o parser.o
	$(CC) $(TEST_FLAGS) $(BUILD)/parser.o -I $(GTEST)/include -I $(PARSER) \
	-c $(PARSER_TEST)/config_parser_test.cc -o $(BUILD)/parser-tests.o

all-tests: archive parser-tests.o 
	$(CC) $(TEST_FLAGS) $(GTEST)/include $(BUILD)/parser.o $(BUILD)/libgtest.a \
	$(BUILD)/parser-tests.o $(BUILD)/gtest-main.o -o $(TESTS_NAME)

test: all-tests
	./$(TESTS_NAME)

# Cleaning
clean: clean-parser clean-general

clean-general:
	rm -rf webserver

clean-parser: 
	rm -rf build/* config_parser_test $(CLEAN)

