#ifndef LOCATION_H
#define LOCATION_H

/* Vị trí trong mã nguồn, dùng chung cho lexer, parser, AST */
typedef struct {
    int line;
    int col;
} Location;

#endif