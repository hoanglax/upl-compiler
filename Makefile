CC     = gcc
CFLAGS = -Wall -Wextra -g -Isrc/common -Isrc/ast -Ibuild
BUILD  = build
TARGET = $(BUILD)/upl

SRCS = src/main.c src/common/error.c src/common/token_name.c \
       src/ast/ast.c src/ast/ast_print.c
OBJS = $(patsubst src/%.c,$(BUILD)/%.o,$(SRCS)) \
       $(BUILD)/parser.tab.o $(BUILD)/lex.yy.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/parser.tab.c $(BUILD)/parser.tab.h: src/parser/upl.y
	@mkdir -p $(BUILD)
	bison -d -v -o $(BUILD)/parser.tab.c $<

$(BUILD)/lex.yy.c: src/lexer/upl.l $(BUILD)/parser.tab.h
	flex -o $@ $<

$(BUILD)/parser.tab.o: $(BUILD)/parser.tab.c
	$(CC) $(CFLAGS) -Wno-unused-function -c $< -o $@

$(BUILD)/lex.yy.o: $(BUILD)/lex.yy.c
	$(CC) $(CFLAGS) -Wno-unused-function -c $< -o $@

# Các file .c thường cần parser.tab.h (mã token) -> sinh header trước
$(BUILD)/%.o: src/%.c | $(BUILD)/parser.tab.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)

test: all
	@bash tests/run_tests.sh

.PHONY: all clean test