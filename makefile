driver:	driver.o lexer.o lookup.o parser.o NaryTree.o stack.o
	gcc-5 -g -o stage1exe driver.o lexer.o lookup.o parser.o NaryTree.o stack.o

driver.o:	driver.c lexer.h parser.h
	gcc-5 -g -c driver.c

parser.o:	parser.c parser.h parserDef.h lexer.h lexerDef.h
	gcc-5 -g -c parser.c

lexer.o:	lexer.c	lexer.h lexerDef.h lookup.h
	gcc-5 -g -c lexer.c

NaryTree.o:	NaryTree.c NaryTree.h parserDef.h lexerDef.h
	gcc-5 -g -c NaryTree.c

stack.o:	stack.c stack.h NaryTree.h parserDef.h lexerDef.h
	gcc-5 -g -c stack.c

lookup.o:	lookup.c lookup.h
	gcc-5 -g -c lookup.c

clean:
	rm -f *.o
	rm -f exe
	rm -f out*
	rm -f *.out