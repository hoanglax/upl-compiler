%{
#include "error.h"
#include "parser.tab.h"

/* Theo dõi cột: yylineno do Flex tự đếm, cột ta tự đếm */
static int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}

%option noyywrap yylineno

%%
"begin"     { return T_BEGIN; }
"end"       { return T_END; }
\n          { yycolumn = 1; }
[ \t\r]+    { /* bỏ khoảng trắng */ }
.           { error_report("lexical error", yylloc.first_line,
                           yylloc.first_column, "unexpected character"); }
%%