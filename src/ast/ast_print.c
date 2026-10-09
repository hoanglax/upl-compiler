#include <stdio.h>
#include "ast.h"

static const char *op_str(BinOpKind op) {
    switch (op) {
        case OP_ADD: return "+";
        case OP_MUL: return "*";
        case OP_GT:  return ">";
        case OP_GE:  return ">=";
        case OP_EQ:  return "==";
    }
    return "?";
}

static void indent(int depth) {
    for (int i = 0; i < depth; i++) printf("  ");
}

/* In một dòng nhãn (như "cond:", "then:") rồi in node con thụt vào một mức */
static void print_child(const char *label, const Node *n, int depth);

static void print_node(const Node *n, int depth) {
    if (!n) return;
    indent(depth);
    switch (n->kind) {
        case NODE_PROGRAM:
            printf("Program @%d:%d\n", n->loc.line, n->loc.col);
            for (int i = 0; i < n->as.block.count; i++)
                print_node(n->as.block.stmts[i], depth + 1);
            break;
        case NODE_BLOCK:
            printf("Block @%d:%d\n", n->loc.line, n->loc.col);
            for (int i = 0; i < n->as.block.count; i++)
                print_node(n->as.block.stmts[i], depth + 1);
            break;
        case NODE_DECL:
            printf("Decl %s %s @%d:%d\n",
                   n->as.decl.type == TYPE_INT ? "int" : "bool",
                   n->as.decl.name, n->loc.line, n->loc.col);
            if (n->as.decl.init) print_node(n->as.decl.init, depth + 1);
            break;
        case NODE_ASSIGN:
            printf("Assign %s @%d:%d\n", n->as.assign.name, n->loc.line, n->loc.col);
            print_node(n->as.assign.value, depth + 1);
            break;
        case NODE_PRINT:
            printf("Print @%d:%d\n", n->loc.line, n->loc.col);
            print_node(n->as.print.expr, depth + 1);
            break;
        case NODE_IF:
            printf("If @%d:%d\n", n->loc.line, n->loc.col);
            print_child("cond:", n->as.ifs.cond, depth + 1);
            print_child("then:", n->as.ifs.then_blk, depth + 1);
            if (n->as.ifs.else_blk)
                print_child("else:", n->as.ifs.else_blk, depth + 1);
            break;
        case NODE_DOWHILE:
            printf("DoWhile @%d:%d\n", n->loc.line, n->loc.col);
            print_child("body:", n->as.dowhile.body, depth + 1);
            print_child("cond:", n->as.dowhile.cond, depth + 1);
            break;
        case NODE_FOR:
            printf("For @%d:%d\n", n->loc.line, n->loc.col);
            print_child("init:", n->as.fors.init, depth + 1);
            print_child("cond:", n->as.fors.cond, depth + 1);
            print_child("update:", n->as.fors.update, depth + 1);
            print_child("body:", n->as.fors.body, depth + 1);
            break;
        case NODE_BINOP:
            printf("BinOp(%s) @%d:%d\n", op_str(n->as.binop.op),
                   n->loc.line, n->loc.col);
            print_node(n->as.binop.left, depth + 1);
            print_node(n->as.binop.right, depth + 1);
            break;
        case NODE_ID:
            printf("Id %s @%d:%d\n", n->as.id.name, n->loc.line, n->loc.col);
            break;
        case NODE_INTLIT:
            printf("IntLit %d @%d:%d\n", n->as.intlit.value, n->loc.line, n->loc.col);
            break;
        case NODE_BOOLLIT:
            printf("BoolLit %s @%d:%d\n", n->as.boollit.value ? "true" : "false",
                   n->loc.line, n->loc.col);
            break;
    }
}

static void print_child(const char *label, const Node *n, int depth) {
    indent(depth);
    printf("%s\n", label);
    print_node(n, depth + 1);
}

void ast_print_tree(const Node *root) {
    print_node(root, 0);
}