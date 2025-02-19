CC=g++
CFLAGS= -c -g -Wall -std=c++17 -fpermissive
EXENAME= main

default: build/_math.o build/evalRpnNotation.o build/io.o build/lexer.o build/main.o build/shuntingYard.o build/taylorSeries.o build/token.o
	$(CC) build/_math.o build/evalRpnNotation.o build/io.o build/lexer.o build/main.o build/shuntingYard.o build/taylorSeries.o build/token.o -o $(EXENAME)

#order-only-prerequisite for build dir
build/main.o: src/main.cpp include/_math.h include/evalRpnNotation.h include/io.h include/lexer.h include/tokensResult.h include/shuntingYard.h include/taylorSeries.h include/token.h | build
	$(CC) $(CFLAGS) src/main.cpp -o build/main.o

build/_math.o: src/_math.cpp | build
	$(CC) $(CFLAGS) src/_math.cpp -o build/_math.o

build/evalRpnNotation.o: src/evalRpnNotation.cpp | build
	$(CC) $(CFLAGS) src/evalRpnNotation.cpp -o build/evalRpnNotation.o

build/io.o: src/io.cpp | build
	$(CC) $(CFLAGS) src/io.cpp -o build/io.o

build/lexer.o: src/lexer.cpp | build
	$(CC) $(CFLAGS) src/lexer.cpp -o build/lexer.o

build/shuntingYard.o: src/shuntingYard.cpp | build
	$(CC) $(CFLAGS) src/shuntingYard.cpp -o build/shuntingYard.o

build/taylorSeries.o: src/taylorSeries.cpp | build
	$(CC) $(CFLAGS) src/taylorSeries.cpp -o build/taylorSeries.o

build/token.o: src/token.cpp | build
	$(CC) $(CFLAGS) src/token.cpp -o build/token.o

build:
	mkdir -p $@

clean:
	rm build/*.o $(EXENAME)

run:
	./$(EXENAME)
