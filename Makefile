# Determine OS
OS := $(shell uname)

# Program variables
NAME=webserver
TESTS_NAME=$(NAME)-tests

# Source variables
SRC=src
BUILD=build
PARSER=$(SRC)/parser
SERVER=$(SRC)/server

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
	TEST_FLAGS=-g -std=c++0x 
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

# --------------- Source files --------------- #

# -- Parser Files -- #

config-parser.o:
	$(CC) -c $(PARSER)/ConfigParser.cpp $(CFLAGS) -o $(BUILD)/config-parser.o

parser-processor.o: 
	$(CC) -c $(PARSER)/ParserProcessor.cpp $(CFLAGS) \
	-o $(BUILD)/parser-processor.o

# -- Server Files -- #

file-request-handler.o: 
	$(CC) -c $(SERVER)/FileRequestHandler.cpp $(CFLAGS) -I $(SERVER) \
	-o $(BUILD)/file-request-handler.o

echo-request-handler.o:
	$(CC) -c $(SERVER)/EchoRequestHandler.cpp $(CFLAGS) \
	-o $(BUILD)/echo-request-handler.o

request-handler.o: 
	$(CC) -c $(SERVER)/RequestHandler.cpp $(CFLAGS) \
	-o $(BUILD)/request-handler.o

mime-types.o:
	$(CC) -c $(SERVER)/MimeTypes.cpp $(CFLAGS) -o $(BUILD)/mime-types.o

reply.o:
	$(CC) -c $(SERVER)/reply.cpp $(CFLAGS) -o $(BUILD)/reply.o

# -- Server -- #

webserver: parser-processor.o config-parser.o echo-request-handler.o \
		   file-request-handler.o request-handler.o reply.o
	$(CC) $(BUILD)/parser-processor.o $(BUILD)/config-parser.o \
	$(BUILD)/echo-request-handler.o $(BUILD)/file-request-handler.o \
	$(BUILD)/request-handler.o $(BUILD)/reply.o \
	$(SRC)/webserver.cpp $(CFLAGS) $(BOOST_FLAGS) -o $(NAME)

# Run the server using the default configuration
run: all
	./webserver default_config


# --------------- Test files --------------- #

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

parser-tests.o: gtest-main.o config-parser.o
	$(CC) $(TEST_FLAGS) -isystem $(GTEST)/include -I $(PARSER) \
	-c $(PARSER_TEST)/ConfigParserTests.cpp	-o $(BUILD)/parser-tests.o

parser-processor-tests.o: gtest-main.o parser-processor.o
	$(CC) $(TEST_FLAGS) -isystem $(GTEST)/include -I $(PARSER) \
	-c $(PARSER_TEST)/ParserProcessorTests.cpp \
	-o $(BUILD)/parser-processor-tests.o

all-tests: libgtest.a parser-tests.o parser-processor-tests.o
	$(CC) $(TEST_FLAGS) -isystem $(GTEST)/include $(BUILD)/config-parser.o \
	$(BUILD)/parser-processor.o \
	$(BUILD)/gtest-all.o $(BUILD)/parser-tests.o \
	$(BUILD)/parser-processor-tests.o \
	$(BUILD)/gtest-main.o -lpthread -o $(TESTS_NAME) 


# --------------- Cleaning --------------- #

# Not actual created files, phony files
.PHONY: clean clean-general clean-parser

clean: clean-parser clean-general

clean-general:
	rm -rf $(NAME) $(TESTS_NAME)

clean-parser: 
	rm -rf build/* $(CLEAN)

