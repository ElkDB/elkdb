
%option case-insensitive

%{

#include "sql.tab.h"

void yyerror(const char* s);
%}

digit           [0-9]
alpha           [a-zA-Z]
identifier      [a-zA-Z][a-zA-Z0-9_]*

%%
#.*                                         { ; }
"--"[ \t].*                                 { ; }
[ \t\r\n]                                   { ; }

SELECT                                      { return SELECT; }
SET                                         { return SET; }
INSERT                                      { return INSERT; }
UPSERT                                      { return UPSERT; }
INTO                                        { return INTO; }
UPDATE                                      { return UPDATE; }
CREATE                                      { return CREATE; }
BUCKET                                      { return BUCKET; }
VALUES                                      { return VALUES; }
FROM                                        { return FROM; }

"SHOW BUCKETS"                              { return SHOW_BUCKETS; }
"SET INDENT"                                { return SET_INDENT; }

TRUE {
    yylval.intval = 1; return BOOLEAN;
}

UNKNOWN {
    yylval.intval = -1; return BOOLEAN;
}

FALSE {
    yylval.intval = 0; return BOOLEAN;
}

[a-zA-Z][a-zA-Z0-9_]*                       { yylval.strval = strdup(yytext); return VARIABLE_NAME; }


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
    yylval.strval = duplicate_segment(yytext + 1, yyleng - 2);
    // yylval.strval = strdup(yytext);
    return STR;
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

"\\"                                        { return FORWARD_SLASH; }
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
"\n"                                        { printf("New line"); }
.                                           { return yytext[0]; }
%%

int yywrap(void) {
    return 1;
}
