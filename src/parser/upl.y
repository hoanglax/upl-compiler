%code requires {
#include "ast.h"
}

%{
#include <stdio.h>
#include "error.h"
#include "ast.h"

int yylex(void);
void yyerror(const char *msg);

Node *ast_root = NULL;   /* gốc AST, main.c đọc sau khi yyparse xong */

/* Tạo Location từ vị trí Bison (first_line, first_column) */
#define MKLOC(l, c) ((Location){ (l), (c) })
%}

%locations
%define parse.error verbose
%expect 0

%union {
    int       ival;
    char     *sval;
    Node     *node;
    VarType   vtype;
    BinOpKind op;
}

%token T_BEGIN T_END T_INT T_BOOL
%token T_IF T_THEN T_ELSE T_DO T_WHILE T_FOR
%token T_PRINT T_TRUE T_FALSE
%token T_GE T_EQ
%token <sval> T_ID
%token <ival> T_NUM

%type <node>  program stmt_list stmt block decl assign print_stmt
%type <node>  if_stmt do_while for_stmt for_init
%type <node>  expr arith term factor
%type <vtype> type
%type <op>    relop

%%

program
    : T_BEGIN stmt_list T_END
      {
          $2->kind = NODE_PROGRAM;      /* stmt_list là Block, đổi thành Program */
          $2->loc = MKLOC(@1.first_line, @1.first_column);
          ast_root = $2;
          $$ = $2;
      }
    ;

stmt_list
    : stmt_list stmt   { $$ = ast_block_add($1, $2); }
    | /* rỗng */       { $$ = ast_block(NODE_BLOCK, MKLOC(@$.first_line, @$.first_column)); }
    ;

stmt
    : decl ';'         { $$ = $1; }
    | assign ';'       { $$ = $1; }
    | print_stmt ';'   { $$ = $1; }
    | if_stmt          { $$ = $1; }
    | do_while         { $$ = $1; }
    | for_stmt         { $$ = $1; }
    | block            { $$ = $1; }
    ;

block
    : '{' stmt_list '}'
      {
          $2->loc = MKLOC(@1.first_line, @1.first_column);             /* vị trí dấu { */
          $$ = $2;
      }
    ;

decl
    : type T_ID                { $$ = ast_decl(MKLOC(@1.first_line, @1.first_column), $1, $2, NULL); }
    | type T_ID '=' expr       { $$ = ast_decl(MKLOC(@1.first_line, @1.first_column), $1, $2, $4); }
    ;

type
    : T_INT     { $$ = TYPE_INT; }
    | T_BOOL    { $$ = TYPE_BOOL; }
    ;

assign
    : T_ID '=' expr            { $$ = ast_assign(MKLOC(@1.first_line, @1.first_column), $1, $3); }
    ;

print_stmt
    : T_PRINT '(' expr ')'     { $$ = ast_print(MKLOC(@1.first_line, @1.first_column), $3); }
    ;

if_stmt
    : T_IF '(' expr ')' T_THEN block
      { $$ = ast_if(MKLOC(@1.first_line, @1.first_column), $3, $6, NULL); }
    | T_IF '(' expr ')' T_THEN block T_ELSE block
      { $$ = ast_if(MKLOC(@1.first_line, @1.first_column), $3, $6, $8); }
    ;

do_while
    : T_DO block T_WHILE '(' expr ')' ';'
      { $$ = ast_dowhile(MKLOC(@1.first_line, @1.first_column), $2, $5); }
    ;

for_stmt
    : T_FOR '(' for_init ';' expr ';' assign ')' block
      { $$ = ast_for(MKLOC(@1.first_line, @1.first_column), $3, $5, $7, $9); }
    ;

for_init
    : decl      { $$ = $1; }
    | assign    { $$ = $1; }
    ;

expr
    : arith                { $$ = $1; }
    | arith relop arith    { $$ = ast_binop(MKLOC(@2.first_line, @2.first_column), $2, $1, $3); }
    ;

relop
    : '>'     { $$ = OP_GT; }
    | T_GE    { $$ = OP_GE; }
    | T_EQ    { $$ = OP_EQ; }
    ;

arith
    : arith '+' term       { $$ = ast_binop(MKLOC(@2.first_line, @2.first_column), OP_ADD, $1, $3); }
    | term                 { $$ = $1; }
    ;

term
    : term '*' factor      { $$ = ast_binop(MKLOC(@2.first_line, @2.first_column), OP_MUL, $1, $3); }
    | factor               { $$ = $1; }
    ;

factor
    : T_ID                 { $$ = ast_id(MKLOC(@1.first_line, @1.first_column), $1); }
    | T_NUM                { $$ = ast_intlit(MKLOC(@1.first_line, @1.first_column), $1); }
    | T_TRUE               { $$ = ast_boollit(MKLOC(@1.first_line, @1.first_column), 1); }
    | T_FALSE              { $$ = ast_boollit(MKLOC(@1.first_line, @1.first_column), 0); }
    | '(' expr ')'         { $$ = $2; }
    ;

%%

void yyerror(const char *msg) {
    error_report("syntax error", yylloc.first_line, yylloc.first_column, msg);
}