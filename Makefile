CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude -g

SRC_DIRS = src src/algorithms src/memory src/scheduler src/utils

SRC = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))

TESTS = $(wildcard tests/*.c)

all: clean build

build:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o bin/main

run: build
	./bin/main

test: clean
	mkdir -p bin
	$(CC) $(CFLAGS) $(TESTS) $(filter-out src/main.c, $(SRC)) -o bin/tests
	./bin/tests

clean:
	rm -rf bin/*


