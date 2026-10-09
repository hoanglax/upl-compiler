#include <stdio.h>
#include "error.h"

extern FILE *yyin;
int yyparse(void);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file.upl>\n", argv[0]);
        return 2;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) { perror(argv[1]); return 2; }

    error_set_file(argv[1]);
    yyin = f;
    yyparse();
    fclose(f);

    if (error_count() > 0) {
        printf("Found %d error(s).\n", error_count());
        return 1;
    }
    printf("OK: no errors.\n");
    return 0;
}