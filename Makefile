# Determine OS
OS := $(shell uname)

# Program variables
NAME=webserver
TESTS_NAME=$(NAME)-tests

# Source variables
SRC=src
BUILD:=build
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

$(BUILD)/config-parser.o:
	$(CC) $(CFLAGS) -c $(PARSER)/ConfigParser.cpp

$(BUILD)/parser-processor.o: 
	$(CC) $(CFLAGS) -c $(PARSER)/ParserProcessor.cpp 

# -- Server Files -- #

$(BUILD)/server.o:
	$(CC) $(CFLAGS) -c $(SERVER)/Server.cpp -I $(SERVER)

$(BUILD)/request-handler.o: 
	$(CC) $(CFLAGS) -c $(SERVER)/RequestHandler.cpp

$(BUILD)/file-request-handler.o: 
	$(CC) $(CFLAGS) -c $(SERVER)/FileRequestHandler.cpp -I $(SERVER)

$(BUILD)/echo-request-handler.o: 
	$(CC) $(CFLAGS) -c $(SERVER)/EchoRequestHandler.cpp -I $(SERVER)

$(BUILD)/mime-types.o:
	$(CC) $(CFLAGS) -c $(SERVER)/MimeTypes.cpp 

$(BUILD)/reply.o:
	$(CC) $(CFLAGS) -c $(SERVER)/reply.cpp 

# -- Server -- #

webserver: $(BUILD)/parser-processor.o $(BUILD)/config-parser.o \
		   $(BUILD)/server.o $(BUILD)/reply.o \
		   $(BUILD)/file-request-handler.o $(BUILD)/echo-request-handler.o \
		   $(BUILD)/request-handler.o
	$(CC) $(CFLAGS) $(BOOST_FLAGS) $(SRC)/webserver.cpp

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

