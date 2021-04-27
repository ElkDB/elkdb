
%{

#include <stdio.h>
#include <cstring>

int yylex();
void yyerror(const char* s);

#define YYDEBUG  1

%}

%start  _start

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
%token  BUCKET
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

%token  FORWARD_SLASH
%token  SEMICOLON
%token  COLON
%token  COMMA
%token  LPAREN
%token  RPAREN
%token  LSBRACE
%token  RSBRACE

%token  SHOW_BUCKETS
%token  SET_INDENT

%token  INTEGER
%token  REAL
%token  STR

%type <strval> name keys VARIABLE_NAME STR MUL
%type <intval> INTEGER SEMICOLON WHITESPACE
%type <realval> REAL

// %destructor { printf("freed\n"); free($$); } <strval>

%define parse.error verbose

%%

_start:
      special_commands
    | query
    ;

special_commands:
      FORWARD_SLASH SHOW_BUCKETS
      {
        bucket_manager->listBuckets();
        /*view_all_buckets();*/
      }
    |
      FORWARD_SLASH SHOW_BUCKETS error
      { }
    | FORWARD_SLASH SET_INDENT INTEGER
      {
        output_manager->setIndent((unsigned)$3);
      }
    | FORWARD_SLASH SET_INDENT INTEGER error
      {}
    ;

query:
      statement SEMICOLON
      { }
    ;

statement:
      SELECT keys FROM name
      {
        /* printf("Found SELECT '%s' FROM '%s';\n", $2, $4); */
        //select_bucket($2, $4);
        bucket_manager->selectBucket($2, $4);
      }
    |
      CREATE BUCKET name
      {
        bucket_manager->createBucket($3);
      }
    |
      INSERT INTO name VALUES LPAREN STR COMMA STR RPAREN
      {
        bucket_manager->insertIntoBucket($3, $6, $8);
      }
    |
      UPDATE name SET name EQ STR
      { printf("Found UPDATE '%s' FROM '%s' = '%s'", $2, $4, $6); }
    |
      UPDATE name SET name EQ REAL
      { printf("Found UPDATE '%s' FROM '%s' = '%f'", $2, $4, $6); }
    |
      UPDATE name SET name EQ INTEGER
      { printf("Found UPDATE '%s' FROM '%s' = '%d'", $2, $4, $6); }
    ;

keys:
      name AS name
      { $$ = strcat(strcat($1, ":"), $3); }
    |
      name ',' keys
      { $$ = strcat(strcat($1, ","), $3); }
    |
      MUL
      { $$ = $1; }
    |
      name
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
