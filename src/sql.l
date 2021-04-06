
%option case-insensitive

%{

#include "sql.tab.h"

void yyerror(const char* s);
%}

digit           [0-9]
alpha           [a-zA-Z]
identifier      [a-zA-Z][a-zA-Z0-9_]*

%%
#.* { ; }
"--"[ \t].* { ; }
[ \t\r\n] { ; }

SELECT                                      { printf("Found SELECT\n"); return SELECT; }
SET                                         { printf("Found SET\n"); return SET; }
INSERT                                      { printf("Found INSERT\n"); return INSERT; }
UPSERT                                      { printf("Found UPSERT\n"); return UPSERT; }
INTO                                        { printf("Found INTO\n"); return INTO; }
UPDATE                                      { printf("Found UPDATE\n"); return UPDATE; }
CREATE                                      { printf("Found CREATE\n"); return CREATE; }
VALUES                                      { printf("Found VALUES\n"); return VALUES; }
FROM                                        { printf("Found FROM\n"); return FROM; }

TRUE {
    yylval.intval = 1; return BOOLEAN;
}

UNKNOWN {
    yylval.intval = -1; return BOOLEAN;
}

FALSE {
    yylval.intval = 0; return BOOLEAN;
}

[a-zA-Z][a-zA-Z0-9_]*                       { yylval.strval = strdup(yytext); printf("Found variable: %s\n", yylval.strval); return VARIABLE_NAME; }


-?[0-9]+    {
    yylval.intval = atoi(yytext); return INTEGER;
}

-?[0-9]+"."[0-9]*                   |
-?"."[0-9]+                         |
-?[0-9]+E[-+]?[0-9]+                |
-?[0-9]+"."[0-9]*E[-+]?[0-9]+       |
-?"."[0-9]+E[-+]?[0-9]+             {
    yylval.realval = atof(yytext); return REAL;
}

'(\\.|''|[^'\n])*'                  |
\"(\\.|\"\"|[^"\n])*\" {
    yylval.strval = strdup(yytext); return STR;
}

'(\\.|[^'\n])*$ {
    char* buffer;
    sprintf(buffer, "Unterminated string %s", yytext);
    yyerror(buffer);
}

\"(\\.|[^"\n])*$ {
    char* buffer;
    sprintf(buffer, "Unterminated string %s", yytext);
    yyerror(buffer);
}

"="                                         { return EQ; }
">="                                        { return GE; }
"<="                                        { return LE; }
"<>"                                        { return NE; }
"<"                                         { return LT; }
">"                                         { return GT; }

";"                                         { return SEMICOLON; }
":"                                         { return COLON; }
"+"                                         { return ADD; }
"-"                                         { return SUB; }
"*"                                         { yylval.strval = strdup(yytext); return MUL; }
"/"                                         { return DIV; }
"("                                         { return LPAREN; }
")"                                         { return RPAREN; }
"["                                         { return LSBRACE; }
"]"                                         { return RSBRACE; }
","                                         { return COMMA; }

.                                           { return yytext[0]; }
%%

int yywrap(void) {
    return 1;
}
