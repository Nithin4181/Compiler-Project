# Group No: 26
# Authors:
#     Nithin Benny Myppan - 2016A7PS0014P
#     Adhitya Mamallan    - 2016A7PS0028P
#     Swarup N            - 2016A7PS0080P
#     Naveen Unnikrishnan - 2016A7PS0111P

driver: driver.o lexer.o lookup.o parser.o NaryTree.o stack.o ast.o symbolTable.o
	gcc-5 -g -o compiler driver.o lexer.o lookup.o parser.o NaryTree.o stack.o ast.o symbolTable.o

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

ast.o:	ast.c ast.h astDef.h lexerDef.h
	gcc-5 -g -c ast.c

symbolTable.o:	symbolTable.c symbolTable.h symbolTableDef.h astDef.h lexerDef.h
	gcc-5 -g -c symbolTable.c

clean:
	rm -f *.o
	rm -f test
	rm -f out*
	rm -f *.out