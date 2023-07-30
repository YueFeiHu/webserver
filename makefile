CC:=gcc
SRC_DIR:=./src
INC_DIR:=./inc
BIN_DIR:=./bin
LIBS:=-lpthread

CFLAGS:= -std=c11 -g -fno-common -I${INC_DIR} 

.DEFAULT_GOAL := main

SRCS:=$(wildcard ${SRC_DIR}/*.c)
OBJS:=$(patsubst ${SRC_DIR}/%.c, ${BIN_DIR}/%.o, ${SRCS})

${BIN_DIR}/%.o : ${SRC_DIR}/%.c
	${CC} ${CFLAGS} -c -o $@ $< ${LIBS}

main : ${OBJS}
	${CC} -o $@ ${CFLAGS} $^ ${LIBS}

.PHONY: clean
clean:
	rm -rf ${BIN_DIR}/*.o main