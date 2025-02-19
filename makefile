CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= main

default: build/main.o build/token.o build/io.o build/lexer.o
	$(CC) build/main.o build/token.o build/io.o build/lexer.o -o $(EXENAME)

#order-only-prerequisite for build dir
build/main.o: src/main.cpp include/tokensResult.h include/lexer.h include/token.h include/io.h | build
	$(CC) $(CFLAGS) src/main.cpp -o build/main.o

build/token.o: src/token.cpp | build
	$(CC) $(CFLAGS) src/token.cpp -o build/token.o

build/io.o: src/io.cpp | build
	$(CC) $(CFLAGS) src/io.cpp -o build/io.o

build/lexer.o: src/lexer.cpp | build
	$(CC) $(CFLAGS) src/lexer.cpp -o build/lexer.o

build:
	mkdir -p $@

clean:
	rm build/*.o $(EXENAME)

run:
	./$(EXENAME)
