#include <stdio.h>
#include "error.h"

static const char *g_file = "<stdin>";
static int g_count = 0;

void error_set_file(const char *filename) { g_file = filename; }

void error_report(const char *kind, int line, int col, const char *msg) {
    fprintf(stderr, "%s:%d:%d: %s: %s\n", g_file, line, col, kind, msg);
    g_count++;
}

int error_count(void) { return g_count; }