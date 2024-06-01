CC = gcc
CFLAGS = -O3 -fopenmp -Wall -std=c17
OBJECTS = main.o pgm.o mandelbrot.o
HEADERS = ${wildcard *.h}

all: main

main: ${OBJECTS} ${HEADERS}
	${CC} ${CFLAGS} ${OBJECTS} -o $@ -lm

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $^

.PHONY: clean main

clean:
	rm -rf *.o
