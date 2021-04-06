
CC = g++
YACC = bison
LEX = flex

CFLAGS = -std=c++17 # -lreadline
YFLAGS = -d

output: lex.yy.c sql.tab.c
	$(CC) lex.yy.c sql.tab.c -o output

lex.yy.c: sql.tab.c sql.l
	$(LEX) sql.l

sql.tab.c: sql.y
	$(YACC) sql.y $(YFLAGS)

clean:
	rm sql.tab.h
	rm sql.tab.c
	rm lex.yy.c
	rm output
