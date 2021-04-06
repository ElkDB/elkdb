
%{

#include <stdio.h>
#include <cstring>

int yylex();
void yyerror(const char* s);

#define YYDEBUG  1

%}

%start  query

%union  {
    int intval;
    char* strval;
    double realval;
}

%token  SELECT
%token  INSERT
%token  UPDATE
%token  UPSERT
%token  CREATE
%token  FROM
%token  INTO
%token  VALUES
%token  SET
%token  AS

%token  VARIABLE_NAME
%token  WHITESPACE
%token  OTHER

%token  EQ
%token  LT
%token  GT
%token  GE
%token  LE
%token  NE

%token  ADD
%token  SUB
%token  MUL
%token  DIV
%token  BOOLEAN

%token  SEMICOLON
%token  COLON
%token  COMMA
%token  LPAREN
%token  RPAREN
%token  LSBRACE
%token  RSBRACE

%token  INTEGER
%token  REAL
%token  STR

%type <strval> name attributeList VARIABLE_NAME STR MUL
%type <intval> INTEGER SEMICOLON WHITESPACE
%type <realval> REAL

%define parse.error verbose

%%

query:
        statement SEMICOLON
        { printf("Found Statement.\n"); }
        ;

statement:
          SELECT attributeList  FROM name
          { printf("Found SELECT '%s' FROM '%s';\n", $2, $4); }
        | INSERT INTO name VALUES LPAREN STR COMMA STR RPAREN
          { printf("Found INSERT '%s' FROM ('%s', '%s');\n", $3, $6, $8); }
        | UPDATE name SET name EQ STR
          { printf("Found UPDATE '%s' FROM '%s' = '%s'", $2, $4, $6); }
        | UPDATE name SET name EQ REAL
          { printf("Found UPDATE '%s' FROM '%s' = '%f'", $2, $4, $6); }
        | UPDATE name SET name EQ INTEGER
          { printf("Found UPDATE '%s' FROM '%s' = '%d'", $2, $4, $6); }
        ;

attributeList:
          name AS name
          { $$ = strcat(strcat($1, ":"), $3); }
        | name ',' attributeList
          { $$ = strcat(strcat($1, ","), $3); }
        | name AS name ',' attributeList
          { $$ = strcat(strcat($1, ","), $3); }
        | MUL
          { $$ = $1; }
        | name
          { $$ = $1; }
        ;

name:
          VARIABLE_NAME
          { $$ = $1; }
          ;

%%

void yyerror(const char* s) {
    printf("Error: %s\n", s);
}

int main(void) {
    printf("Enter Query: \n");
    return yyparse();
}
