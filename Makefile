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
	TEST_FLAGS=-std=c++0x 
endif
ifeq ($(OS), Darwin)
	CC=clang++
	CFLAGS=-g -Wall -std=c++11 -stdlib=libc++ 
	BOOST_FLAGS=-lboost_system -lboost_thread-mt
	TEST_FLAGS=-std=c++11 -stdlib=libc++
	CLEAN=*.dSYM 
endif

all: directories webserver

# Create a build directory if not already created (-p: avoid error if present)
.PHONY: directories
directories:
	mkdir -p build

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


# Test files
test: directories all-tests
	./$(TESTS_NAME)

gtest-all.o:
	$(CC) $(TEST_FLAGS) -isystem $(GTEST)/include -I $(GTEST) -c \
	$(GTEST)/src/gtest-all.cc -o $(BUILD)/gtest-all.o

libgtest.a: gtest-all.o
	$(AR) $(BUILD)/libgtest.a $(BUILD)/gtest-all.o

gtest-main.o: gtest-all.o
	$(CC) $(TEST_FLAGS) -I $(GTEST)/include \
	-c $(GTEST)/src/gtest_main.cc -o $(BUILD)/gtest-main.o

parser-tests.o: gtest-main.o parser.o
	$(CC) $(TEST_FLAGS) -isystem $(GTEST)/include -I $(PARSER) \
	-c $(PARSER_TEST)/config_parser_test.cc -o $(BUILD)/parser-tests.o

all-tests: libgtest.a parser-tests.o
	$(CC) $(TEST_FLAGS) -isystem $(GTEST)/include $(BUILD)/parser.o \
	$(BUILD)/libgtest.a $(BUILD)/parser-tests.o $(BUILD)/gtest-main.o \
	-o $(TESTS_NAME) 


# Cleaning

# Not actual created files, phony files
.PHONY: clean clean-general clean-parser

clean: clean-parser clean-general

clean-general:
	rm -rf $(NAME) $(TESTS_NAME)

clean-parser: 
	rm -rf build/* $(CLEAN)

