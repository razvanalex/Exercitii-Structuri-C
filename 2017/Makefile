all: build

build: main.c
	gcc -Wall $^ -o main

run:
	./main

.PHONY: clean
clean:
	rm -f main

debug:
	valgrind --leak-check=full ./main
