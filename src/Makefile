
CC = g++
YACC = bison
LEX = flex

CFLAGS = -std=c++17
LINKERS = -lreadline
YFLAGS = -d

# output: lex.yy.c sql.tab.c
#	$(CC) lex.yy.c sql.tab.c -o output

output: lex.yy.c sql.tab.c
	$(CC) $(CFLAGS) main.c $(LINKERS) -o elkdb

lex.yy.c: sql.tab.c sql.l
	$(LEX) sql.l

sql.tab.c: sql.y
	$(YACC) sql.y $(YFLAGS)

clean:
	rm -f sql.tab.h
	rm -f sql.tab.c
	rm -f lex.yy.c
	rm -f output
