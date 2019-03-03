driver:	driver.o lexer.o lookup.o
	gcc -o exe driver.o lexer.o lookup.o

driver.o:	driver.c lexer.h
	gcc -c driver.c

parser.o:	parser.c parser.h parserDef.h
	gcc -c parser.c

lexer.o:	lexer.c	lexer.h lexerDef.h lookup.h
	gcc -c lexer.c

lookup.o:	lookup.c lookup.h
	gcc -c lookup.c

clean:
	rm -f *.o
	rm -f exe
	rm -f out.txt
	rm -f *.out