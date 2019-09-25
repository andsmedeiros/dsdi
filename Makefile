OPTS= -std=c++14 -Wall -Wextra -pedantic -O2

# CC=clang++-8 -stdlib=libc++ $(OPTS)
CC=g++ $(OPTS)

.PHONY: clean
clean:
	rm -rf bin

.PHONY: example
example: bin/example
	./bin/example

bin/example: example/main.cpp example/interfaces/* example/components/* src/*
	mkdir -p bin
	$(CC) example/main.cpp -Iexample -Isrc -o bin/example
