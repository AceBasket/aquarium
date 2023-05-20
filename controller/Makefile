CC = gcc
CFLAGS += -Wall -Wextra -Werror -g -O0
LDFLAGS += -pthread -lm


#This finds all c files and places then into SRC. It's equivalent would be
# SRC = src/main.c src/folder1/func1.c src/folder1/func2.c src/folder2/func3.c
SRC = ${shell find src -name *.c}
#This tells Make that somewhere below, going to convert all source into objects, so this is like:
# OBJ =  src/main.o src/folder1/func1.o src/folder1/func2.o src/folder2/func3.o
OBJ = ${SRC:%.c=build/%.o}

TST = ${shell find tst -name *_test.c}
OBJ_TST = ${TST:%.c=build/%.o}

BIN = build/main.exe
TEST = ${OBJ_TST:%.o=%.tst}


.PHONY: all build test clean

.PRECIOUS: build/tst/%.tst ${BIN}

all: build

dir:
	mkdir -p build/tst build/src/threads/handler_functions build/src/communication build/src/aquarium build/src/parser

build: dir ${BIN}
	rm -f log_handle_fish*
	${BIN} 5 8000

valgrind: dir ${BIN}
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes ${BIN} 5 8000

test: dir ${TEST}
	@for test in ${TEST}; do \
		echo "Running $$test"; \
		$$test; \
	done

clean:
	rm -rf build/ *.log


#################### SOURCE FILES ####################

# $(BIN) is dependent on objects
${BIN}: ${OBJ}
	${CC} ${CFLAGS} $^ -o $@ ${LDFLAGS}

# each object file is dependent on its source file, and whenever make needs to create an object file, it will follow this rule:
build/%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@


#################### TEST FILES ####################

# each test object file is dependent on its source file, and whenever make needs to create an object file, it will follow this rule:
build/tst/%.o: tst/%.c
	@${CC} ${CFLAGS} -c $< -o $@

# each test executable file is dependent on its corresponding object file, and whenever make needs to create an executable file, it will follow this rule:
build/tst/%_test.tst: build/tst/%_test.o build/src/aquarium/aquarium.o build/src/aquarium/view.o build/src/aquarium/fish.o build/src/communication/socket_aquarium.o build/src/parser/client_inputs_parser.o build/src/parser/prompt_parser.o build/src/parser/parser.o build/src/utils.o
	@${CC} ${CFLAGS} $^ -o $@ -lm