#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

static Node *new_node(NodeKind kind, Location loc) {
    Node *n = calloc(1, sizeof(Node));
    if (!n) { fprintf(stderr, "out of memory\n"); exit(2); }
    n->kind = kind;
    n->loc = loc;
    return n;
}

Node *ast_block(NodeKind kind, Location loc) {
    return new_node(kind, loc);   /* calloc: stmts=NULL, count=cap=0 */
}

Node *ast_block_add(Node *b, Node *stmt) {
    if (!stmt) return b;
    if (b->as.block.count == b->as.block.cap) {
        int ncap = b->as.block.cap ? b->as.block.cap * 2 : 4;
        Node **p = realloc(b->as.block.stmts, ncap * sizeof(Node *));
        if (!p) { fprintf(stderr, "out of memory\n"); exit(2); }
        b->as.block.stmts = p;
        b->as.block.cap = ncap;
    }
    b->as.block.stmts[b->as.block.count++] = stmt;
    return b;
}

Node *ast_decl(Location loc, VarType type, char *name, Node *init) {
    Node *n = new_node(NODE_DECL, loc);
    n->as.decl.type = type;
    n->as.decl.name = name;
    n->as.decl.init = init;
    return n;
}

Node *ast_assign(Location loc, char *name, Node *value) {
    Node *n = new_node(NODE_ASSIGN, loc);
    n->as.assign.name = name;
    n->as.assign.value = value;
    return n;
}

Node *ast_print(Location loc, Node *expr) {
    Node *n = new_node(NODE_PRINT, loc);
    n->as.print.expr = expr;
    return n;
}

Node *ast_if(Location loc, Node *cond, Node *then_blk, Node *else_blk) {
    Node *n = new_node(NODE_IF, loc);
    n->as.ifs.cond = cond;
    n->as.ifs.then_blk = then_blk;
    n->as.ifs.else_blk = else_blk;
    return n;
}

Node *ast_dowhile(Location loc, Node *body, Node *cond) {
    Node *n = new_node(NODE_DOWHILE, loc);
    n->as.dowhile.body = body;
    n->as.dowhile.cond = cond;
    return n;
}

Node *ast_for(Location loc, Node *init, Node *cond, Node *update, Node *body) {
    Node *n = new_node(NODE_FOR, loc);
    n->as.fors.init = init;
    n->as.fors.cond = cond;
    n->as.fors.update = update;
    n->as.fors.body = body;
    return n;
}

Node *ast_binop(Location loc, BinOpKind op, Node *left, Node *right) {
    Node *n = new_node(NODE_BINOP, loc);
    n->as.binop.op = op;
    n->as.binop.left = left;
    n->as.binop.right = right;
    return n;
}

Node *ast_id(Location loc, char *name) {
    Node *n = new_node(NODE_ID, loc);
    n->as.id.name = name;
    return n;
}

Node *ast_intlit(Location loc, int value) {
    Node *n = new_node(NODE_INTLIT, loc);
    n->as.intlit.value = value;
    return n;
}

Node *ast_boollit(Location loc, int value) {
    Node *n = new_node(NODE_BOOLLIT, loc);
    n->as.boollit.value = value;
    return n;
}

void ast_free(Node *n) {
    if (!n) return;
    switch (n->kind) {
        case NODE_PROGRAM:
        case NODE_BLOCK:
            for (int i = 0; i < n->as.block.count; i++) ast_free(n->as.block.stmts[i]);
            free(n->as.block.stmts);
            break;
        case NODE_DECL:
            free(n->as.decl.name);
            ast_free(n->as.decl.init);
            break;
        case NODE_ASSIGN:
            free(n->as.assign.name);
            ast_free(n->as.assign.value);
            break;
        case NODE_PRINT:
            ast_free(n->as.print.expr);
            break;
        case NODE_IF:
            ast_free(n->as.ifs.cond);
            ast_free(n->as.ifs.then_blk);
            ast_free(n->as.ifs.else_blk);
            break;
        case NODE_DOWHILE:
            ast_free(n->as.dowhile.body);
            ast_free(n->as.dowhile.cond);
            break;
        case NODE_FOR:
            ast_free(n->as.fors.init);
            ast_free(n->as.fors.cond);
            ast_free(n->as.fors.update);
            ast_free(n->as.fors.body);
            break;
        case NODE_BINOP:
            ast_free(n->as.binop.left);
            ast_free(n->as.binop.right);
            break;
        case NODE_ID:
            free(n->as.id.name);
            break;
        case NODE_INTLIT:
        case NODE_BOOLLIT:
            break;
    }
    free(n);
}