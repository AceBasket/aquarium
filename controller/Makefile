CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -pthread


#This finds all c files and places then into SRC. It's equivalent would be
# SRC = src/main.c src/folder1/func1.c src/folder1/func2.c src/folder2/func3.c
SRC = ${shell find src -name *.c}
#This tells Make that somewhere below, going to convert all source into objects, so this is like:
# OBJ =  src/main.o src/folder1/func1.o src/folder1/func2.o src/folder2/func3.o
OBJ = ${SRC:%.c=build/%.o}

TST = ${shell find tst -name *.c}
OBJ_TST = ${TST:%.c=build/%.o}

BIN = build/controller
TEST = build/alltests


.PHONY: all build test clean

all: dir build

dir:
	mkdir -p build/src build/tst

build: ${BIN}

test: ${TEST}

clean:
	rm -f *.o *~ ${BIN} ${OBJ} ${TEST} ${OBJ_TST}


#################### EXECUTABLE ####################

#$(BIN) is dependent on objects
${BIN}: ${OBJ}
	${CC} ${CFLAGS} ${LDFLAGS} $^ -o $@

${TEST}: ${OBJ_TST} build/src/parser.o
	${CC} ${CFLAGS} $^ -o $@
    
# each object file is dependent on its source file, and whenever make needs to create an object file, to follow this rule:
build/%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@