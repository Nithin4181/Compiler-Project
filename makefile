driver:	driver.o lexer.o lookup.o parser.o
	gcc-5 -g -o exe driver.o lexer.o lookup.o parser.o

driver.o:	driver.c lexer.h parser.h
	gcc-5 -g -c driver.c

parser.o:	parser.c parser.h parserDef.h
	gcc-5 -g -c parser.c

lexer.o:	lexer.c	lexer.h lexerDef.h lookup.h
	gcc-5 -g -c lexer.c

lookup.o:	lookup.c lookup.h
	gcc-5 -g -c lookup.c

clean:
	rm -f *.o
	rm -f exe
	rm -f out.txt
	rm -f *.out