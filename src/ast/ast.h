#ifndef AST_H
#define AST_H

#include "location.h"

typedef enum {
    /* Câu lệnh */
    NODE_PROGRAM, NODE_BLOCK, NODE_DECL, NODE_ASSIGN, NODE_PRINT,
    NODE_IF, NODE_DOWHILE, NODE_FOR,
    /* Biểu thức */
    NODE_BINOP, NODE_ID, NODE_INTLIT, NODE_BOOLLIT
} NodeKind;

typedef enum { TYPE_INT, TYPE_BOOL } VarType;

typedef enum { OP_ADD, OP_MUL, OP_GT, OP_GE, OP_EQ } BinOpKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Location loc;
    union {
        /* NODE_PROGRAM, NODE_BLOCK: danh sách câu lệnh */
        struct { Node **stmts; int count, cap; } block;
        /* NODE_DECL: Type name [= init] */
        struct { VarType type; char *name; Node *init; } decl;
        /* NODE_ASSIGN: name = value */
        struct { char *name; Node *value; } assign;
        /* NODE_PRINT */
        struct { Node *expr; } print;
        /* NODE_IF: else_blk có thể NULL */
        struct { Node *cond, *then_blk, *else_blk; } ifs;
        /* NODE_DOWHILE */
        struct { Node *body, *cond; } dowhile;
        /* NODE_FOR: init là Decl hoặc Assign */
        struct { Node *init, *cond, *update, *body; } fors;
        /* NODE_BINOP */
        struct { BinOpKind op; Node *left, *right; } binop;
        /* NODE_ID */
        struct { char *name; } id;
        /* NODE_INTLIT */
        struct { int value; } intlit;
        /* NODE_BOOLLIT */
        struct { int value; } boollit;
    } as;
};

/* ---- Hàm tạo node. loc là vị trí token đầu của cấu trúc ---- */
Node *ast_block(NodeKind kind, Location loc);          /* PROGRAM hoặc BLOCK, rỗng */
Node *ast_block_add(Node *block, Node *stmt);          /* thêm stmt, trả về block */
Node *ast_decl(Location loc, VarType type, char *name, Node *init);
Node *ast_assign(Location loc, char *name, Node *value);
Node *ast_print(Location loc, Node *expr);
Node *ast_if(Location loc, Node *cond, Node *then_blk, Node *else_blk);
Node *ast_dowhile(Location loc, Node *body, Node *cond);
Node *ast_for(Location loc, Node *init, Node *cond, Node *update, Node *body);
Node *ast_binop(Location loc, BinOpKind op, Node *left, Node *right);
Node *ast_id(Location loc, char *name);
Node *ast_intlit(Location loc, int value);
Node *ast_boollit(Location loc, int value);

/* ---- In và giải phóng ---- */
void ast_print_tree(const Node *root);
void ast_free(Node *n);

#endif