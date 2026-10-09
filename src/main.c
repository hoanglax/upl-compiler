#include <stdio.h>
#include <string.h>
#include "error.h"
#include "token_name.h"
#include "parser.tab.h"

extern FILE *yyin;
extern char *yytext;
int yylex(void);
int yyparse(void);

/* Chế độ --tokens: chỉ chạy lexer, in từng token */
static void dump_tokens(void) {
    int tok;
    printf("%-8s %-10s %s\n", "LINE:COL", "TOKEN", "LEXEME");
    while ((tok = yylex()) != 0) {
        printf("%d:%-5d %-10s '%s'\n", yylloc.first_line, yylloc.first_column, token_name(tok), yytext);
    }
}

int main(int argc, char **argv) {
    int tokens_mode = 0;
    const char *path = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--tokens") == 0) tokens_mode = 1;
        else path = argv[i];
    }
    if (!path) {
        fprintf(stderr, "Usage: %s [--tokens] <file.upl>\n", argv[0]);
        return 2;
    }

    FILE *f = fopen(path, "r");
    if (!f) { perror(path); return 2; }

    error_set_file(path);
    yyin = f;

    if (tokens_mode) dump_tokens();
    else             yyparse();
    fclose(f);

    if (error_count() > 0) {
        printf("Found %d error(s).\n", error_count());
        return 1;
    }
    printf(tokens_mode ? "Lexing OK.\n" : "OK: no errors.\n");
    return 0;
}