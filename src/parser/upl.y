%{
#include <stdio.h>
#include "error.h"

int yylex(void);
void yyerror(const char *msg);
%}

%locations
%define parse.error verbose

%token T_BEGIN T_END

%%
program
    : T_BEGIN T_END
    ;
%%

void yyerror(const char *msg) {
    error_report("syntax error", yylloc.first_line, yylloc.first_column, msg);
}