#include <stdio.h>
#include "token_name.h"
#include "parser.tab.h"

const char *token_name(int tok) {
    static char buf[8];
    switch (tok) {
        case T_BEGIN: return "T_BEGIN";
        case T_END:   return "T_END";
        case T_INT:   return "T_INT";
        case T_BOOL:  return "T_BOOL";
        case T_IF:    return "T_IF";
        case T_THEN:  return "T_THEN";
        case T_ELSE:  return "T_ELSE";
        case T_DO:    return "T_DO";
        case T_WHILE: return "T_WHILE";
        case T_FOR:   return "T_FOR";
        case T_PRINT: return "T_PRINT";
        case T_TRUE:  return "T_TRUE";
        case T_FALSE: return "T_FALSE";
        case T_GE:    return "T_GE";
        case T_EQ:    return "T_EQ";
        case T_ID:    return "T_ID";
        case T_NUM:   return "T_NUM";
        default:
            if (tok > 0 && tok < 256) {   /* ký tự đơn */
                snprintf(buf, sizeof buf, "'%c'", tok);
                return buf;
            }
            return "UNKNOWN";
    }
}