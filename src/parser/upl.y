%{
#include <stdio.h>
#include "error.h"

int yylex(void);
void yyerror(const char *msg);
%}

%locations
%define parse.error verbose
%expect 0                      /* không cho phép bất kỳ xung đột nào */

%union {
    int   ival;
    char *sval;
}

%token T_BEGIN T_END T_INT T_BOOL
%token T_IF T_THEN T_ELSE T_DO T_WHILE T_FOR
%token T_PRINT T_TRUE T_FALSE
%token T_GE T_EQ
%token <sval> T_ID
%token <ival> T_NUM

%%

/* (1) */
program
    : T_BEGIN stmt_list T_END
    ;

/* (2)(3) */
stmt_list
    : stmt_list stmt
    | /* rỗng */
    ;

/* (4)-(10) */
stmt
    : decl ';'
    | assign ';'
    | print_stmt ';'
    | if_stmt
    | do_while
    | for_stmt
    | block
    ;

/* (11) */
block
    : '{' stmt_list '}'
    ;

/* (12)-(17) */
decl
    : type T_ID
    | type T_ID '=' expr
    ;

type
    : T_INT
    | T_BOOL
    ;

assign
    : T_ID '=' expr
    ;

print_stmt
    : T_PRINT '(' expr ')'
    ;

/* (18)-(23) */
if_stmt
    : T_IF '(' expr ')' T_THEN block
    | T_IF '(' expr ')' T_THEN block T_ELSE block
    ;

do_while
    : T_DO block T_WHILE '(' expr ')' ';'
    ;

for_stmt
    : T_FOR '(' for_init ';' expr ';' assign ')' block
    ;

for_init
    : decl
    | assign
    ;

/* (24)-(28) */
expr
    : arith
    | arith relop arith
    ;

relop
    : '>'
    | T_GE
    | T_EQ
    ;

/* (29)-(32) */
arith
    : arith '+' term
    | term
    ;

term
    : term '*' factor
    | factor
    ;

/* (33)-(37) */
factor
    : T_ID
    | T_NUM
    | T_TRUE
    | T_FALSE
    | '(' expr ')'
    ;

%%

void yyerror(const char *msg) {
    error_report("syntax error", yylloc.first_line, yylloc.first_column, msg);
}