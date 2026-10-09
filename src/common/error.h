#ifndef ERROR_H
#define ERROR_H

/* Nơi DUY NHẤT định dạng và in lỗi. Lexer và parser đều gọi vào đây. */
void error_set_file(const char *filename);
void error_report(const char *kind, int line, int col, const char *msg);
int  error_count(void);

#endif