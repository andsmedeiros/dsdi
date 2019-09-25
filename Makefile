OPTS= -std=c++14 -Wall -Wextra -pedantic -O2
# OPTS= -std=c++14 -Wall -Wextra -pedantic -O0 -g
# CC=clang++-8 -stdlib=libc++ $(OPTS)
CC=g++ $(OPTS)

.PHONY: clean
clean:
	rm -rf bin

.PHONY: test
test: bin/test
	./bin/test

bin/test: test/main.cpp test/interfaces/* test/components/* src/*
	mkdir -p bin
	$(CC) test/main.cpp -Itest -Isrc -o bin/test
