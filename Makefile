CXXFLAGS = -Wall -Wextra -Weffc++ -Wfloat-equal -Wshadow\
	-Wpointer-arith -Wcast-align -Wstrict-overflow=5\
	-Wwrite-strings -Wswitch-default -Wswitch-enum -Wparentheses\
	-Woverloaded-virtual -pedantic

shell: main.o parse.o gram.o lexer.yy.o
	$(CXX) $(CXXFLAGS) main.o parse.o gram.o lexer.yy.o -o shell
 
parse.o: parse.cpp lexer.yy.hpp gram.hpp
 
gram.cpp: lemonfiles
 
gram.hpp: lemonfiles
 
.PHONY: lemonfiles
lemonfiles: lemon gram.y
	./lemon gram.y -s
	mv gram.c gram.cpp
	mv gram.h gram.hpp

lemon: lemon.c

lexer.o: lexer.yy.hpp lexer.yy.cpp
 
lexer.yy.cpp: flexfiles
 
lexer.yy.hpp: flexfiles
 
.PHONY: flexfiles
flexfiles: lexer.l
	flex --outfile=lexer.yy.cpp --header-file=lexer.yy.hpp lexer.l
 
.PHONY: clean
clean:
	rm -f *.o
	rm -f lemon
	rm -f lexer.yy.cpp lexer.yy.hpp
	rm -f gram.cpp gram.hpp gram.out
	rm -f shell
	rm -f test/*.o
	rm -f run_test

test: test/test.o parse.o gram.o lexer.yy.o
	$(CXX) $(CXXFLAGS) test/test.o parse.o gram.o lexer.yy.o -o run_test
