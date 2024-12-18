ROOT_DIR=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

BIN_DIR=$(ROOT_DIR)/bin
BUILD_DIR=$(ROOT_DIR)/build
INCLUDE_DIR=$(ROOT_DIR)/include
SRC_DIR=$(ROOT_DIR)/src

CC=gcc
CFLAGS=-Wall -std=c99
LFlags=-lncurses
INC=-I$(ROOT_DIR) #THIS IS SETTING THE INCLUDE PATH TO THE PROJECT ROOT DIRECTORY

# GOOD TO NOTE:
# patsubst pattern -> $(patsubst %.pattern, %.replacement, input)
O_FILES=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(SRC_DIR)/*.c))
H_FILES=$(wildcard $(INCLUDE_DIR)/*.h)

all: $(O_FILES) $(BIN_DIR)/main $(patsubst %, $(BUILD_DIR)/%, $(wildcard $(LIB_FILES)/*.o))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(INC) $(CFLAGS) -c -o $@ $< 

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(INC) $(CFLAGS) -c -o $@ $< $(TFLAGS)

$(BIN_DIR)/main: $(O_FILES)
	$(CC) $(O_FILES) -o $(BIN_DIR)/main $(LFlags)

setup:
	mkdir $(ROOT_DIR)/bin $(ROOT_DIR)/build 

teardown:
	rm -rf $(ROOT_DIR)/bin $(ROOT_DIR)/build 

clean:
	rm -f $(BUILD_DIR)/*
	rm -f $(BIN_DIR)/*
	clear

run:
	$(BIN_DIR)/main

run_tests:
	$(BIN_DIR)/unit_tests

attempt:
	make clean && \
	make && \
	clear && \
	make run

attempt_tests:
	make clean && \
	make && \
	clear && \
	make run_tests


